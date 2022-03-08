#include "../interface/ChannelManager.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

ChannelManager::ChannelManager(TFile* outputFile) : outfile(outputFile) {
    // ask for outputfile as well?

    for (auto it : namingScheme) {
        if (it.first == eventClass::fail) continue;
        std::vector<HistInfo>* histInfoVec = HistogramConfig::getHistInfo(it.first);
        mapping[it.first] = new Channel(it.second, histInfoVec);
    }
}


ChannelManager::ChannelManager(TFile* outputFile, std::map<eventClass, std::string> names) : outfile(outputFile) {
    // ask for outputfile as well?

    for (auto it : names) {
        std::vector<HistInfo>* histInfoVec = HistogramConfig::getHistInfo(it.first);
        mapping[it.first] = new Channel(it.second, histInfoVec);
    }
}

ChannelManager::ChannelManager(TFile* outputFile, std::vector<HistInfo>* (&histInfoGenerator)(const eventClass)) : outfile(outputFile) {
    for (auto it : namingScheme) {
        if (it.first == eventClass::fail) continue;
        std::vector<HistInfo>* histInfoVec = histInfoGenerator(it.first);
        mapping[it.first] = new Channel(it.second, histInfoVec);
    }
}

ChannelManager::~ChannelManager() {

}

void ChannelManager::newSample(std::string& sampleName) {
    for (auto it : mapping) {
        it.second->newSample(sampleName, useUncertainties);
    }
}

void ChannelManager::addSubUncertainties(shapeUncId uncID, std::vector<std::string>& subUncNames) {
    if (! useUncertainties) return;
    
    for (auto it : mapping) {
        it.second->addSubUncertainties(uncID, subUncNames);
    }
}

void ChannelManager::initHistogramStacks(std::vector<std::string>& initialProcessNames, bool uncertainties) {
    processHistName = initialProcessNames;
    useUncertainties = uncertainties;
    for (auto it : mapping) {
        it.second->initializeHistogramStack(processHistName, uncertainties);
    }

    for (auto it : initialProcessNames) {
        if (it == "") continue;

        outfile->cd();
        outfile->cd("Nominal");
        if (! gDirectory->GetDirectory(it.c_str())) gDirectory->mkdir(it.c_str());

        if (! uncertainties) continue;

        outfile->cd();
        outfile->cd("Uncertainties");
        if (! gDirectory->GetDirectory(it.c_str())) gDirectory->mkdir(it.c_str());
    }
}

void ChannelManager::changePrimaryProcess(std::string& newPrimProc) {
    changeProcess(0, newPrimProc);
}

void ChannelManager::changeProcess(unsigned procNumber, std::string& newProc) {
    // make sure the process exists in the outputfile both for uncertainties as for nominal case
    outfile->cd("Uncertainties");
    processHistName[procNumber] = newProc;
    for (auto it : mapping) {
        it.second->changeProcess(procNumber, newProc);
    }

    // folder checking:
    outfile->cd();
    outfile->cd("Nominal");
    if (! gDirectory->GetDirectory(newProc.c_str())) gDirectory->mkdir(newProc.c_str());
    outfile->cd();
    outfile->cd("Uncertainties");
    if (! gDirectory->GetDirectory(newProc.c_str())) gDirectory->mkdir(newProc.c_str());
}

void ChannelManager::writeNominalHistograms(std::string& uniqueSampleName) {
    for (unsigned i=0; i<processHistName.size(); i++) {
        gDirectory->cd(processHistName[i].c_str());
        if (! gDirectory->GetDirectory(uniqueSampleName.c_str())) {
            gDirectory->mkdir(uniqueSampleName.c_str());
            gDirectory->cd(uniqueSampleName.c_str());
        } else {
            gDirectory->cd(uniqueSampleName.c_str());
        }
        for (auto it : mapping) {
            it.second->writeNominalHistograms(i);
        }
        
        gDirectory->cd("../../");
    }
}

void ChannelManager::writeUncertaintyHistograms(std::string& uniqueSampleName) {
    if (! useUncertainties) return;

    for (unsigned i=0; i<processHistName.size(); i++) {
        gDirectory->cd(processHistName[i].c_str());
        if (! gDirectory->GetDirectory(uniqueSampleName.c_str())) {
            gDirectory->mkdir(uniqueSampleName.c_str());
            gDirectory->cd(uniqueSampleName.c_str());
        } else {
            gDirectory->cd(uniqueSampleName.c_str());
        }
        for (auto it : mapping) {
            it.second->writeUncertaintyHistograms(i);
        }
        
        gDirectory->cd("../../");
    }
}

void ChannelManager::writeUncertaintyEnvelopeHistograms(unsigned subProc) {
    if (! useUncertainties) return;

    gDirectory->cd(processHistName[subProc].c_str());
    for (auto it : mapping) {
        //it.second->writeUncertaintyEnvelopeHistograms(subProc);
    }
}


void ChannelManager::fillUpHistograms(eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    if (evClass == eventClass::fail) return;
    mapping[evClass]->fillUpHistograms(id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
}

void ChannelManager::fillDownHistograms(eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    if (evClass == eventClass::fail) return;
    mapping[evClass]->fillDownHistograms(id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
}

void ChannelManager::fillAllUpHistograms(std::vector<std::string>& subs, eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    if (evClass == eventClass::fail) return;
    mapping[evClass]->fillAllUpHistograms(subs, id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
}

void ChannelManager::fillAllDownHistograms(std::vector<std::string>& subs, eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight) {
    if (evClass == eventClass::fail) return;
    mapping[evClass]->fillAllDownHistograms(subs, id, procNumber, fillVec, singleHist, twoDimFillVec, weight);
}
