#ifndef HISTOGRAMCONTAINER_H
#define HISTOGRAMCONTAINER_H

#include "../../globalSettings.h"


#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "../../additionalTools/interface/histHelper.h"

#include <map>


class HistogramContainer {
    private:
        // is this used?
        std::vector<std::vector<std::shared_ptr<TH1D>>*> oldHists;
        std::vector<std::vector<std::shared_ptr<TH2D>>*> oldHists2D;

        // should be deleted
        std::vector<std::shared_ptr<TH1D>>* oneDims = nullptr;
        std::vector<std::shared_ptr<TH2D>>* twoDims = nullptr;
        
        // managed by channel - no hard copy
        std::vector<HistInfo>* oneDimInfo;
        std::vector<HistInfo_2D>* twoDimInfo = nullptr;

    public:
        HistogramContainer(std::vector<HistInfo>* oneDim, std::vector<HistInfo_2D>* twoDim);
        ~HistogramContainer();

        std::vector<HistInfo>* getHistInfo() {return oneDimInfo;}
        std::vector<std::shared_ptr<TH1D>>* getHistograms() {return oneDims;}

        std::vector<HistInfo_2D>* get2DHistInfo() {return twoDimInfo;}
        std::vector<std::shared_ptr<TH2D>>* get2DHistograms() {return twoDims;}

        void fillHistograms(std::vector<double>& fillVec, double eventWeight);
        void fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double eventWeight);

        void fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void newSample(std::string& uniqueSampleName);
        void writeHistograms();

        void flushOldHistograms();
};

#endif