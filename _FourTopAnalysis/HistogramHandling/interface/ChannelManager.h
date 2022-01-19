#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include "Channel.h"
#include "../../FourTopEventHandling/interface/EventFourT.h"

class ChannelManager {
    private:
        std::map<eventClass, Channel*> mapping;
    public:
        ChannelManager(std::map<eventClass, std::string> namingScheme);
        ~ChannelManager();

        Channel* getChannel(eventClass ev) {return mapping[ev];}
        Channel* operator[](eventClass ev) {return mapping[ev];}

        void newSample(std::string& sampleName);
        void newProcess(std::string& processName, TFile* outfile);

        void writeHistograms();
        // series of functions passing through
};

#endif