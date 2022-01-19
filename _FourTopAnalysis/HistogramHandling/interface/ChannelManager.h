#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include "Channel.h"
#include "HistogramConfigurations.h"
#include "../../FourTopEventHandling/interface/EventFourT.h"

class ChannelManager {
    private:
        std::map<eventClass, Channel*> mapping;
        std::vector<std::string> processHistName;
    public:
        ChannelManager(std::map<eventClass, std::string> namingScheme);
        ~ChannelManager();

        Channel* getChannel(eventClass ev) {return mapping[ev];}
        Channel* operator[](eventClass ev) {return mapping[ev];}

        void newSample(std::string& sampleName);
        void newProcess(std::string& processName, TFile* outfile);

        void initProcessNames(std::vector<std::string>& processNames);
        void changePrimaryProcess(std::string& newPrimProc);

        void writeNominalHistograms(std::string& uniqueSampleName);
        void writeUncertaintyHistograms(std::string& uniqueSampleName);
        void writeUncertaintyEnvelopeHistograms(unsigned subProc);

        // series of functions passing through
};

#endif