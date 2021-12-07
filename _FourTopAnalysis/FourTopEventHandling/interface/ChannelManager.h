#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include "UncertaintyManager.h"
#include "EventFourT.h"
#include "../../additionalTools/interface/HistogramManager.h"

class ChannelManager
{
    private:
        std::map<eventClass, UncertaintyWrapper*> channelMap;
    public:
        ChannelManager();
        ~ChannelManager();
        
        UncertaintyWrapper* getChannelUncertainties(eventClass channel) {return channelMap[channel];}
        //void fillUncertainty();

        void newSample(std::string& uniqueName);
        void addChannel(eventClass channel, HistogramManager* histograms);
        void writeCurrentHistograms();
        void writeNonpromptHistograms();
};

#endif