#ifndef HISTOGRAMCONTAINER_H
#define HISTOGRAMCONTAINER_H

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "../../additionalTools/interface/histHelper.h"


class HistogramContainer {
    private:
        std::vector<std::shared_ptr<TH1D>>* oneDims = nullptr;
        std::vector<std::shared_ptr<TH2D>>* twoDims = nullptr;
        
        std::vector<HistInfo>* oneDimInfo;
        std::vector<HistInfo_2D>* twoDimInfo = nullptr;

    public:
        HistogramContainer(std::vector<HistInfo>* histInfo);
        ~HistogramContainer();

        std::vector<HistInfo>* getHistInfo() {return oneDimInfo;}
        std::vector<std::shared_ptr<TH1D>>* getHistograms() {return oneDims;}

        std::vector<HistInfo_2D>* get2DHistInfo() {return twoDimInfo;}
        std::vector<std::shared_ptr<TH2D>>* get2DHistograms() {return twoDims;}

        //void updateHistInfo(std::vector<HistInfo>* extraInfo);
        //void set2DHistInfo(std::vector<HistInfo_2D>* new2DInfo);

        void fillHistograms(std::vector<double>& fillVec, double eventWeight);
        void fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double eventWeight);

        void fillSingleHistogram(std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void newSample(std::string& uniqueSampleName);
        void writeHistograms();
};

#endif