#include "../interface/ChannelManager.h"

ChannelManager::ChannelManager(std::map<eventClass, std::string> namingScheme) {
    // ask for outputfile as well?

    for (auto it : namingScheme) {
        std::vector<HistInfo>* histInfoVec = HistogramConfig::getHistInfo(it.first);
        mapping[it.first] = new Channel(it.second, histInfoVec);
    }
}

ChannelManager::~ChannelManager() {

}

void ChannelManager::newSample(std::string& sampleName) {
    for (auto it : mapping) {
        it.second->newSample(sampleName);
    }
}

void ChannelManager::newProcess(std::string& processName, TFile* outfile) {

}

void ChannelManager::initProcessNames(std::vector<std::string>& processNames) {

}

void ChannelManager::changePrimaryProcess(std::string& newPrimProc) {
    // make sure the process exists in the outputfile both for uncertainties as for nominal case
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
    gDirectory->cd(processHistName[subProc].c_str());
    for (auto it : mapping) {
        it.second->writeUncertaintyEnvelopeHistograms(subProc);
    }
}

