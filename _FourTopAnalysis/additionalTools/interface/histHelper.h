#ifndef histhelper_h
#define histhelper_h

#include <vector>
#include <memory>
#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/Sample.h"
#include "../../../Tools/interface/histogramTools.h"

namespace histHelper {
    std::vector< std::vector< std::shared_ptr<TH1D>>>* initHistograms(std::vector<HistInfo>* histInfoVec, std::vector<Sample>& sampleVec);
    void histFiller(std::vector<double>& fillVec, std::vector<std::shared_ptr<TH1D>>* hists, double weight);
}

#endif