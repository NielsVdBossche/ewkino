#ifndef HISTOGRAMMANAGER_h
#define HISTOGRAMMANAGER_h

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"

class HistogramManager {
    private:
        std::vector<std::shared_ptr<TH1D>> nonpromptHists, currentSampleHists;
        std::vector<HistInfo*> histInfo;

        std::string channel;
    public:
        HistogramManager();
        ~HistogramManager();
};

#endif
