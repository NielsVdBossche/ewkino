#include "../interface/ChannelManager.h"

ChannelManager::ChannelManager(std::map<eventClass, std::string> namingScheme) {

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

void ChannelManager::writeHistograms() {
    for (auto it : mapping) {
        it.second->writeHistograms();
    }
}
