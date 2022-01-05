#include "../interface/ChannelManager.h"

ChannelManager::ChannelManager() {

}

void ChannelManager::addChannel(eventClass channel, HistogramManager* histograms) {
    UncertaintyWrapper* wrapper = new UncertaintyWrapper(histograms);
    channelMap[channel] = wrapper;
}

void ChannelManager::newSample(std::string& uniqueName) {
    unsigned currClass = eventClass::crz;

    while (currClass <= eventClass::fourlep) {
        channelMap[eventClass(currClass)]->newSample(uniqueName);
        currClass++;
    }
}

void ChannelManager::newProcess(std::string& processName, TFile* outfile) {
    unsigned currClass = eventClass::crz;

    while (currClass <= eventClass::fourlep) {
        channelMap[eventClass(currClass)]->newProcess(processName, outfile);
        currClass++;
    }
}


void ChannelManager::writeCurrentHistograms() {
    unsigned currClass = eventClass::crz;

    while (currClass <= eventClass::fourlep) {
        channelMap[eventClass(currClass)]->writeCurrentHistograms();
        currClass++;
    }
}

void ChannelManager::writeNonpromptHistograms() {
    unsigned currClass = eventClass::crz;

    while (currClass <= eventClass::fourlep) {
        channelMap[eventClass(currClass)]->writeNonpromptHistograms();
        currClass++;
    }
}

void ChannelManager::fillUpHistograms(eventClass ev, shapeUncId id, std::vector<double>& fill, std::vector<std::pair<int, double>>& singles, std::vector<std::pair<double, double>>& fill2d, double weight, bool nonprompt) {
    if (ev == eventClass::fail) return;
    channelMap[ev]->fillUpOrDownUncertainty(id, fill, weight, true, nonprompt);
    channelMap[ev]->fillUpOrDownSingleHistograms(id, singles, weight, true, nonprompt);
    channelMap[ev]->fillUpOrDown2DHistograms(id, fill2d, weight, true, nonprompt);
}

void ChannelManager::fillDownHistograms(eventClass ev, shapeUncId id, std::vector<double>& fill, std::vector<std::pair<int, double>>& singles, std::vector<std::pair<double, double>>& fill2d, double weight, bool nonprompt) {
    if (ev == eventClass::fail) return;
    channelMap[ev]->fillUpOrDownUncertainty(id, fill, weight, false, nonprompt);
    channelMap[ev]->fillUpOrDownSingleHistograms(id, singles, weight, false, nonprompt);
    channelMap[ev]->fillUpOrDown2DHistograms(id, fill2d, weight, false, nonprompt);
}

void ChannelManager::addSubUncertainties(shapeUncId id, std::vector<std::string>& subUnc) {
    unsigned currClass = eventClass::crz;

    while (currClass <= eventClass::fourlep) {
        channelMap[eventClass(currClass)]->addSubUncertainties(id, subUnc);
        currClass++;
    }
}
