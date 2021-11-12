#ifndef HISTOGRAMMANAGER_h
#define HISTOGRAMMANAGER_h

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "histHelper.h"

class HistogramManager {
    private:
        std::vector<std::shared_ptr<TH1D>> *nonpromptHists, *currentSampleHists;
        std::vector<HistInfo*>* histInfo;

        std::string channel;
    public:
        HistogramManager(std::string& channel, std::vector<HistInfo*> histInfo);
        ~HistogramManager();

        std::string getChannel() const {return channel;}
        std::vector<HistInfo*>* getHistInfo() {return histInfo;}
        std::vector<std::shared_ptr<TH1D>>* getHistograms(bool nonPrompt);
        
        void setChannel(std::string& newChannel) {channel = newChannel;}
        void setHistInfo(std::vector<HistInfo*>* newHistInfo) {histInfo = new std::vector<HistInfo*>(*newHistInfo);}
        
        void fillHistograms(std::vector<double>& fillValues, double eventWeight, bool nonPrompt);

        void newSample(std::string& uniqueSampleName);

        // check how to manage
        void writeCurrentHistograms(); // histograms should be made in file directly -> objects should then not be deleted manually
        void writeNonpromptHistograms();
};

#endif
