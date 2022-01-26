#include "../interface/HistogramContainer.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

HistogramContainer::HistogramContainer(std::vector<HistInfo>* oneDim, std::vector<HistInfo_2D>* twoDim) : oneDimInfo(oneDim), twoDimInfo(twoDim) {

}

void HistogramContainer::fillHistograms(std::vector<double>& fillVec, double eventWeight) {
    // fill hists
    for (size_t dist = 0; dist < fillVec.size(); dist++) {
        histogram::fillValue(oneDims->at(dist).get(), fillVec[dist], eventWeight);
    }
}

void HistogramContainer::fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double eventWeight) {
    // fill hists
    for (size_t dist = 0; dist < fillVec.size(); dist++) {
        histogram::fillValues(twoDims->at(dist).get(), fillVec[dist].first, fillVec[dist].second, eventWeight);
    }
}


void HistogramContainer::fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double eventWeight) {
    for (auto it : fillVec) {
        histogram::fillValue(oneDims->at(it.first).get(), it.second, eventWeight);
    }
}

void HistogramContainer::fillSingle2DHistograms(std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
    for (auto it : fillVec) {
        histogram::fillValues(twoDims->at(it.first).get(), it.second.first, it.second.second, eventWeight);
    }
}


void HistogramContainer::newSample(std::string& uniqueSampleName) {
    oneDims = new std::vector<std::shared_ptr<TH1D>>(oneDimInfo->size());

    for( size_t dist = 0; dist < oneDimInfo->size(); ++dist ){
        oneDims->at(dist) = oneDimInfo->at(dist).makeHist( oneDimInfo->at(dist).name() + "_" + uniqueSampleName);
    }
    if (twoDimInfo) {
        twoDims = new std::vector<std::shared_ptr<TH2D>>(twoDimInfo->size());

        for( size_t dist = 0; dist < twoDimInfo->size(); ++dist ){
            twoDims->at(dist) = twoDimInfo->at(dist).makeHist_2D( twoDimInfo->at(dist).name() + "_" + uniqueSampleName);
        }
    }
}

void HistogramContainer::writeHistograms() {
    for( size_t dist = 0; dist < oneDimInfo->size(); ++dist ) {
        oneDims->at(dist)->Write(TString(oneDimInfo->at(dist).name()), TObject::kOverwrite);
    }
    if (twoDimInfo) {
        for( size_t dist = 0; dist < twoDimInfo->size(); ++dist ) {
            twoDims->at(dist)->Write(TString(twoDimInfo->at(dist).name()), TObject::kOverwrite);
        }
    }
}
