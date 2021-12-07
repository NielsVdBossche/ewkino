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
