#include "../interface/Channel.h"

Channel::Channel(std::string channel, std::vector<HistInfo>* histInfo) : channel(channel) {
    oneDimInfo = new std::vector<HistInfo>(hardCopyInfoVector(histInfo));
}

Channel::~Channel() {

}

std::vector<HistInfo> Channel::hardCopyInfoVector(std::vector<HistInfo>* infoVec) {
    std::vector<HistInfo> newInfoVec;

    for (unsigned i = 0; i < infoVec->size(); i++) {
        std::string newName = infoVec->at(i).name() + "_" + channel;
        HistInfo hardCopy(infoVec->at(i));
        hardCopy.setName(newName);
        newInfoVec.push_back(hardCopy);
    }

    return newInfoVec;
}


void Channel::addSubChannels(std::vector<std::string>& newSubChannels) {
    if (newSubChannels.size() == 0) return;
    subChannels = new std::map<std::string, Channel*>;

    for (unsigned i=0; i<newSubChannels.size(); i++) {
        (*subChannels)[newSubChannels[i]] = new Channel(newSubChannels[i], oneDimInfo);
    }
}

void Channel::updateHistInfo(std::vector<HistInfo>* extraInfo) {
    std::vector<HistInfo> extraInfoCopy = hardCopyInfoVector(extraInfo);

    oneDimInfo->insert(oneDimInfo->end(), extraInfoCopy.begin(), extraInfoCopy.end());
}

void Channel::set2DHistInfo(std::vector<HistInfo_2D>* new2DInfo) {
    
}

void Channel::initializeHistogramStack() {
    // build uncertainty class objects, histogramsets etc etc
    nominalHistograms = new HistogramSet("");
    
    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            std::cout << "envelope " << id << std::endl;
            uncHistMap[shapeUncId(id)] = new UncertaintyEnvelope(translateUnc, shapeUncId(id), nominalHistograms);
        } else {
            std::cout << "current uncertainty " << id << std::endl;
            uncHistMap[shapeUncId(id)] = new Uncertainty(translateUnc, shapeUncId(id), nominalHistograms);
        }
        id++;
    }
}

void Channel::subDivisions(std::vector<std::string>& divs) {

}

void Channel::changeProcessTitle(unsigned index, std::string& newTitle) {

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

void Channel::fillSingle2DHistograms(unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
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

void Channel::fillSubchannelSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
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

void Channel::fillAllSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
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

void Channel::addSubUncertainties(shapeUncId id, std::vector<std::string>& subUnc) {
    uncHistMap[id]->addSubUncertainties(subUnc);
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

void Channel::newSample(std::string& uniqueSampleName) {
    nominalHistograms->newSample(uniqueSampleName);

    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            id++;
            continue;
        }
        uncHistMap[shapeUncId(id)]->newSample(uniqueSampleName);
        id++;
    }

    if (subChannels) {
        for (auto it : *subChannels) {
            it.second->newSample(uniqueSampleName);
        }
    }
}

void Channel::writeNominalHistograms(unsigned subProc) {
    nominalHistograms->writeHistograms(subProc);
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
}

void Channel::writeUncertaintyEnvelopeHistograms(unsigned subProc) {

}
