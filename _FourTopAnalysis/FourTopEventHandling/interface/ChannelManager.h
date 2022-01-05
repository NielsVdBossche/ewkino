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
        std::map<shapeUncId, std::string> getTranslateUncMap() {return channelMap[eventClass::crz]->getTranslateUncMap();}

        void fillUpHistograms(eventClass, shapeUncId, std::vector<double>&, std::vector<std::pair<int, double>>&, std::vector<std::pair<double, double>>&, double, bool);
        void fillDownHistograms(eventClass, shapeUncId, std::vector<double>&, std::vector<std::pair<int, double>>&, std::vector<std::pair<double, double>>&, double, bool);

        //void fillUncertainty();

        void newSample(std::string& uniqueName);
        void newProcess(std::string& processName, TFile* outfile);
        void addChannel(eventClass channel, HistogramManager* histograms);
        void writeCurrentHistograms();
        void writeNonpromptHistograms();
};

#endif