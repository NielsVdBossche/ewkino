#include "../interface/Channel.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

Channel::Channel(std::string& channel, std::vector<HistInfo>* histInfo) : ChannelName(channel) {
    oneDimInfo = new std::vector<HistInfo>(hardCopyInfoVector(histInfo));
}

Channel::Channel(std::string& channel, std::string& subChannel, std::vector<HistInfo>* histInfo) : ChannelName(channel), SubChannelName(subChannel) {
    oneDimInfo = new std::vector<HistInfo>(hardCopyInfoVector(histInfo));
}

Channel::~Channel() {

}

std::vector<HistInfo> Channel::hardCopyInfoVector(std::vector<HistInfo>* infoVec) {
    std::vector<HistInfo> newInfoVec;

    for (unsigned i = 0; i < infoVec->size(); i++) {
        std::string newName = infoVec->at(i).name() + SubChannelName;
        HistInfo hardCopy(infoVec->at(i));
        hardCopy.setName(newName);
        newInfoVec.push_back(hardCopy);
    }

    return newInfoVec;
}

std::vector<HistInfo_2D> Channel::hardCopy2DInfoVector(std::vector<HistInfo_2D>* infoVec) {
    std::vector<HistInfo_2D> newInfoVec;

    for (unsigned i = 0; i < infoVec->size(); i++) {
        std::string newName = infoVec->at(i).name() + SubChannelName;
        HistInfo_2D hardCopy(infoVec->at(i));
        hardCopy.setName(newName);
        newInfoVec.push_back(hardCopy);
    }

    return newInfoVec;
}


void Channel::addSubChannels(std::vector<std::string>& newSubChannels) {
    if (newSubChannels.size() == 0) return;
    subChannels = new std::map<std::string, Channel*>;

    for (unsigned i=0; i<newSubChannels.size(); i++) {
        (*subChannels)[newSubChannels[i]] = new Channel(ChannelName, newSubChannels[i], oneDimInfo);
        (*subChannels)[newSubChannels[i]]->set2DHistInfo(twoDimInfo);
    }
}

void Channel::addSubUncertainties(shapeUncId uncID, std::vector<std::string>& subUnc) {
    uncHistMap[uncID]->addSubUncertainties(subUnc);

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->addSubUncertainties(uncID, subUnc);
        }
    }
}

void Channel::SetPrintAllUncertaintyVariations(bool setting) {
    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->SetPrintAllUncertaintyVariations(setting);
        }
    }

    for (auto it : uncHistMap) {
        it.second->SetPrintAllVariations(setting);
    }
}

void Channel::updateHistInfo(std::vector<HistInfo>* extraInfo) {
    std::vector<HistInfo> extraInfoCopy = hardCopyInfoVector(extraInfo);

    oneDimInfo->insert(oneDimInfo->end(), extraInfoCopy.begin(), extraInfoCopy.end());
}

void Channel::set2DHistInfo(std::vector<HistInfo_2D>* new2DInfo) {
    twoDimInfo = new std::vector<HistInfo_2D>(hardCopy2DInfoVector(new2DInfo));
}

void Channel::initializeHistogramStack(std::vector<std::string>& divsInitial, bool uncertainties) {
    // build uncertainty class objects, histogramsets etc etc
    nominalHistograms = new HistogramSet(divsInitial, "", oneDimInfo, twoDimInfo);

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->initializeHistogramStack(divsInitial, uncertainties);
        }
    }
    
    if (! uncertainties) return;

    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            //std::cout << "envelope " << id << std::endl;
            
            uncHistMap[shapeUncId(id)] = new UncertaintyEnvelope(translateUnc, shapeUncId(id), nominalHistograms);
        } else {
            //std::cout << "current uncertainty " << id << std::endl;
            uncHistMap[shapeUncId(id)] = new Uncertainty(translateUnc, shapeUncId(id), nominalHistograms);
        }
        id++;
    }
}

void Channel::initializeAdditionalHistogramStack(std::string& newProcess, bool uncertainties) {
    // build uncertainty class objects, histogramsets etc etc
    nominalHistograms->addProcess(newProcess);
        
    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->initializeAdditionalHistogramStack(newProcess, uncertainties);
        }
    }
    
    if (! uncertainties) return;

    unsigned id = 0;

    while (id != shapeUncId::end) {
        uncHistMap[shapeUncId(id)]->addProcess(newProcess);
        id++;
    }
}


void Channel::changeProcess(unsigned index, std::string& newTitle, bool uncertainties) {
    nominalHistograms->changeProcess(index, newTitle);

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->changeProcess(index, newTitle, uncertainties);
        }
    }

    if (! uncertainties) return;

    for (auto it : uncHistMap) {
        it.second->changeProcess(index, newTitle);
    }
}

void Channel::fillHistograms(unsigned subProc, std::vector<double>& fillVec, double eventWeight) {
    nominalHistograms->fillHistograms(subProc, fillVec, eventWeight);
}

void Channel::fillSingleHistograms(unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight) {
    nominalHistograms->fillSingleHistograms(subProc, fillVec, eventWeight);
}

void Channel::fill2DHistograms(unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight) {
    nominalHistograms->fill2DHistograms(subProc, fillVec, eventWeight);
}

void Channel::fillSingle2DHistograms(unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
    nominalHistograms->fillSingle2DHistograms(subProc, fillVec, eventWeight);
}

void Channel::fillSubchannelHistogram(std::vector<std::string>& subs, unsigned subProc, std::vector<double>& fillVec, double eventWeight) {
    for (auto it : subs) {
        subChannels->at(it)->fillHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillSubchannelSingleHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight) {
    for (auto it : subs) {
        subChannels->at(it)->fillSingleHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillSubchannel2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight) {
    for (auto it : subs) {
        subChannels->at(it)->fill2DHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillSubchannelSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
    for (auto it : subs) {
        subChannels->at(it)->fillSingle2DHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillAllHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<double>& fillVec, double eventWeight) {
    nominalHistograms->fillHistograms(subProc, fillVec, eventWeight);
    for (auto it : subs) {
        subChannels->at(it)->fillHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillAllSingleHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight) {
    nominalHistograms->fillSingleHistograms(subProc, fillVec, eventWeight);
    for (auto it : subs) {
        subChannels->at(it)->fillSingleHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillAll2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight) {
    nominalHistograms->fill2DHistograms(subProc, fillVec, eventWeight);
    for (auto it : subs) {
        subChannels->at(it)->fill2DHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillAllSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
    nominalHistograms->fillSingle2DHistograms(subProc, fillVec, eventWeight);
    for (auto it : subs) {
        subChannels->at(it)->fillSingle2DHistograms(subProc, fillVec, eventWeight);
    }
}

void Channel::fillUncertainties(shapeUncId id, unsigned subProc, std::vector<double>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillHistograms(fillVec, weightUp, weightDown, subProc);
}

void Channel::fillSingleUncertainties(shapeUncId id, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSingleHistograms(fillVec, weightUp, weightDown, subProc);
}

void Channel::fill2DUncertainties(shapeUncId id, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fill2DHistograms(fillVec, weightUp, weightDown, subProc);
}

void Channel::fillAllUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<double>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillHistograms(fillVec, weightUp, weightDown, subProc);
    for (auto it : subs) {
        subChannels->at(it)->fillUncertainties(id, subProc, fillVec, weightUp, weightDown);
    }
}

void Channel::fillAllSingleUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSingleHistograms(fillVec, weightUp, weightDown, subProc);

    for (auto it : subs) {
        subChannels->at(it)->fillSingleUncertainties(id, subProc, fillVec, weightUp, weightDown);
    }
}

void Channel::fillAll2DUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fill2DHistograms(fillVec, weightUp, weightDown, subProc);
    for (auto it : subs) {
        subChannels->at(it)->fill2DUncertainties(id, subProc, fillVec, weightUp, weightDown);
    }
}

void Channel::fillEnvelope(shapeUncId id,  unsigned subProc, std::vector<double>& fillVec, std::vector<double> weight) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelope(fillVec, weight, subProc);
}

void Channel::fillEnvelopeSingles(shapeUncId id,  unsigned subProc, std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelopeSingles(fillVec, weight, subProc );
}

void Channel::fillEnvelope2Ds(shapeUncId id,  unsigned subProc, std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelope2Ds(fillVec, weight, subProc );
}

void Channel::fillSubUncertainty(shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSubHistograms(subUnc, fillVec, weightUp, weightDown, subProc);
}

void Channel::fillSingleSubUncertainty(shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSubSingleHistograms(subUnc, fillVec, weightUp, weightDown, subProc);
}

void Channel::fill2DSubUncertainty(shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSub2DHistograms(subUnc, fillVec, weightUp, weightDown, subProc);
}

void Channel::fillAllEnvelope(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::vector<double>& fillVec, std::vector<double> weight) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelope(fillVec, weight, subProc);

    for (auto it : subs) {
        subChannels->at(it)->fillEnvelope(id, subProc, fillVec, weight);
    }
}

void Channel::fillAllEnvelopeSingles(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelopeSingles(fillVec, weight, subProc );

    for (auto it : subs) {
        subChannels->at(it)->fillEnvelopeSingles(id, subProc, fillVec, weight);
    }
}

void Channel::fillAllEnvelope2Ds(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelope2Ds(fillVec, weight, subProc );

    for (auto it : subs) {
        subChannels->at(it)->fillEnvelope2Ds(id, subProc, fillVec, weight);
    }
}

void Channel::fillAllSubUncertainty(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSubHistograms(subUnc, fillVec, weightUp, weightDown, subProc);
    for (auto it : subs) {
        subChannels->at(it)->fillSubUncertainty(id, subProc, subUnc, fillVec, weightUp, weightDown);
    }
}

void Channel::fillAllSingleSubUncertainty(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSubSingleHistograms(subUnc, fillVec, weightUp, weightDown, subProc);
    for (auto it : subs) {
        subChannels->at(it)->fillSingleSubUncertainty(id, subProc, subUnc, fillVec, weightUp, weightDown);
    }
}

void Channel::fillAll2DSubUncertainty(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown) {
    uncHistMap[id]->fillSub2DHistograms(subUnc, fillVec, weightUp, weightDown, subProc);
    for (auto it : subs) {
        subChannels->at(it)->fill2DSubUncertainty(id, subProc, subUnc, fillVec, weightUp, weightDown);
    }
}

void Channel::newSample(std::string& uniqueSampleName, bool uncertainties) {
    nominalHistograms->newSample(uniqueSampleName);

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->newSample(uniqueSampleName, uncertainties);
        }
    }

    if (! uncertainties) return;
    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            id++;
            continue;
        }
        uncHistMap[shapeUncId(id)]->newSample(uniqueSampleName);
        id++;
    }
}

void Channel::writeNominalHistograms(unsigned subProc) {
    nominalHistograms->writeHistograms(subProc);

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->writeNominalHistograms(subProc);
        }
    }
}

void Channel::writeUncertaintyHistograms(unsigned subProc) {
    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            id++;
            continue;
        }
        std::string uncName = translateUnc[shapeUncId(id)];
        if (! gDirectory->GetDirectory(uncName.c_str())) {
            gDirectory->mkdir(uncName.c_str());
            gDirectory->cd(uncName.c_str());
            gDirectory->mkdir("Up");
            gDirectory->mkdir("Down");
        } else {
            gDirectory->cd(uncName.c_str());
        }
        
        uncHistMap[shapeUncId(id)]->writeHistograms(subProc);
        gDirectory->cd("..");
        id++;
    }

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->writeUncertaintyHistograms(subProc);
        }
    }
}

//void Channel::writeUncertaintyEnvelopeHistograms(unsigned subProc) {
//
//}

void Channel::fillUpHistograms(shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillUpOrDownHistograms(fillVec, weight, true, procNumber);
    uncHistMap[id]->fillUpOrDownSingleHistograms(singleHist, weight, true, procNumber);
    uncHistMap[id]->fillUpOrDown2DHistograms(twoDimFillVec, weight, true, procNumber);
}

void Channel::fillDownHistograms(shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillUpOrDownHistograms(fillVec, weight, false, procNumber);
    uncHistMap[id]->fillUpOrDownSingleHistograms(singleHist, weight, false, procNumber);
    uncHistMap[id]->fillUpOrDown2DHistograms(twoDimFillVec, weight, false, procNumber);
}

void Channel::fillSubUpHistograms(std::string& subUnc,shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillSubUpOrDownHistograms(subUnc, fillVec, weight, true, procNumber);
    uncHistMap[id]->fillSubUpOrDownSingleHistograms(subUnc, singleHist, weight, true, procNumber);
    uncHistMap[id]->fillSubUpOrDown2DHistograms(subUnc, twoDimFillVec, weight, true, procNumber);
}

void Channel::fillSubDownHistograms(std::string& subUnc,shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillSubUpOrDownHistograms(subUnc, fillVec, weight, false, procNumber);
    uncHistMap[id]->fillSubUpOrDownSingleHistograms(subUnc, singleHist, weight, false, procNumber);
    uncHistMap[id]->fillSubUpOrDown2DHistograms(subUnc, twoDimFillVec, weight, false, procNumber);
}

void Channel::fillAllUpHistograms(std::vector<std::string>& subs, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillUpOrDownHistograms(fillVec, weight, true, procNumber);
    uncHistMap[id]->fillUpOrDownSingleHistograms(singleHist, weight, true, procNumber);
    uncHistMap[id]->fillUpOrDown2DHistograms(twoDimFillVec, weight, true, procNumber);

    for (auto it : subs) {
        subChannels->at(it)->fillUpHistograms(id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
    }
}

void Channel::fillAllDownHistograms(std::vector<std::string>& subs, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillUpOrDownHistograms(fillVec, weight, false, procNumber);
    uncHistMap[id]->fillUpOrDownSingleHistograms(singleHist, weight, false, procNumber);
    uncHistMap[id]->fillUpOrDown2DHistograms(twoDimFillVec, weight, false, procNumber);

    for (auto it : subs) {
        subChannels->at(it)->fillDownHistograms(id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
    }
}


void Channel::fillAllSubUpHistograms(std::string& subUnc, std::vector<std::string>& subs, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillSubUpOrDownHistograms(subUnc, fillVec, weight, true, procNumber);
    uncHistMap[id]->fillSubUpOrDownSingleHistograms(subUnc, singleHist, weight, true, procNumber);
    uncHistMap[id]->fillSubUpOrDown2DHistograms(subUnc, twoDimFillVec, weight, true, procNumber);

    for (auto it : subs) {
        subChannels->at(it)->fillSubUpHistograms(subUnc, id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
    }
}

void Channel::fillAllSubDownHistograms(std::string& subUnc, std::vector<std::string>& subs, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    uncHistMap[id]->fillSubUpOrDownHistograms(subUnc, fillVec, weight, false, procNumber);
    uncHistMap[id]->fillSubUpOrDownSingleHistograms(subUnc, singleHist, weight, false, procNumber);
    uncHistMap[id]->fillSubUpOrDown2DHistograms(subUnc, twoDimFillVec, weight, false, procNumber);

    for (auto it : subs) {
        subChannels->at(it)->fillSubDownHistograms(subUnc, id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
    }
}
