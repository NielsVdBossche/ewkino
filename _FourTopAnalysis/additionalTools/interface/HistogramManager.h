#ifndef HISTOGRAMMANAGER_h
#define HISTOGRAMMANAGER_h

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "histHelper.h"

/*
Class HistogramManager:
    Takes as input: channel, a vector of HistInfo*

    The channel will be used as the flag in the future for the individual channels in the final filename

    To begin filling histograms for a new sample, first run newSample function with the sample name
    Use fillHistograms during filling to fill either the sample or nonprompt histograms

    Once a sample is finished, write the histograms to the outputfile using the writeCurrentHistograms() methods

    Once all samples are ran, the nonprompt contribution can also be written to file using the writeNonpromptHistograms() method

    Further simple getters and setters are available
*/
class HistogramManager {
    private:
        std::vector<std::shared_ptr<TH1D>> *nonpromptHists, *currentSampleHists;
        std::vector<HistInfo>* histInfo;

        std::string channel;
    public:
        HistogramManager(std::string& channel, std::vector<HistInfo>* histInfo);
        ~HistogramManager();

        std::string getChannel() const {return channel;}
        std::vector<HistInfo>* getHistInfo() {return histInfo;}
        std::vector<std::shared_ptr<TH1D>>* getHistograms(bool nonPrompt);
        
        void setChannel(std::string& newChannel) {channel = newChannel;}
        void setHistInfo(std::vector<HistInfo>* newHistInfo) {histInfo = new std::vector<HistInfo>(*newHistInfo);}
        
        void fillHistograms(std::vector<double>& fillValues, double eventWeight, bool nonPrompt);

        void newSample(std::string& uniqueSampleName);

        // check how to manage
        void writeCurrentHistograms(); // histograms should be made in file directly -> objects should then not be deleted manually
        void writeNonpromptHistograms();
};

#endif
