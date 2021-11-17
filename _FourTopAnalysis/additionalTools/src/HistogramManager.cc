#include "../interface/HistogramManager.h"

HistogramManager::HistogramManager(std::string& channel, std::vector<HistInfo>* histInfo) : channel(channel), histInfo(histInfo) {
    nonpromptHists = new std::vector<std::shared_ptr<TH1D>>(histInfo->size());
    for( size_t dist = 0; dist < histInfo->size(); ++dist ){
        nonpromptHists->at(dist) = histInfo->at(dist).makeHist( histInfo->at(dist).name() + "_nonprompt");
    }
}

HistogramManager::~HistogramManager() {
    delete histInfo;

}

std::vector<std::shared_ptr<TH1D>>* HistogramManager::getHistograms(bool nonPrompt) {
    if (nonPrompt) return nonpromptHists;
    else return currentSampleHists;
}

std::vector<std::shared_ptr<TH2D>>* HistogramManager::get2DHistograms(bool nonPrompt) {
    if (nonPrompt) return nonpromptHists2D;
    else return currentSampleHists2D;
}

void HistogramManager::extendHistInfo(std::vector<HistInfo>* extraHistInfo) {
    histInfo->insert(histInfo->end(), extraHistInfo->begin(), extraHistInfo->end());
    
    for( size_t dist = 0; dist < extraHistInfo->size(); ++dist ){
        nonpromptHists->push_back(extraHistInfo->at(dist).makeHist( extraHistInfo->at(dist).name() + "_nonprompt"));
    }
}

void HistogramManager::set2DHistInfo(std::vector<HistInfo_2D>* newHistInfo) {
    histInfo2D = new std::vector<HistInfo_2D>(*newHistInfo);
    nonpromptHists2D = new std::vector<std::shared_ptr<TH2D>>(histInfo->size());
    for( size_t dist = 0; dist < histInfo2D->size(); ++dist ){
        nonpromptHists2D->at(dist) = histInfo2D->at(dist).makeHist_2D( histInfo2D->at(dist).name() + "_nonprompt");
    }
}


void HistogramManager::fillHistograms(std::vector<double>& fillValues, double eventWeight, bool nonPrompt) {
    std::vector<std::shared_ptr<TH1D>>* histsToFill = getHistograms(nonPrompt);

    // fill hists
    for (size_t dist = 0; dist < fillValues.size(); dist++) {
        histogram::fillValue(histsToFill->at(dist).get(), fillValues[dist], eventWeight);
    }
}

void HistogramManager::fill2DHistograms(std::vector<std::pair<double, double>>& fillValues, double eventWeight, bool nonPrompt) {
    std::vector<std::shared_ptr<TH2D>>* histsToFill = get2DHistograms(nonPrompt);

    // fill hists
    for (size_t dist = 0; dist < fillValues.size(); dist++) {
        histogram::fillValues(histsToFill->at(dist).get(), fillValues[dist].first, fillValues[dist].second, eventWeight);
    }
}

void HistogramManager::fillSingleHistogram(size_t index, double value, double eventWeight, double nonPrompt) {
    std::vector<std::shared_ptr<TH1D>>* histsToFill = getHistograms(nonPrompt);

    // fill hists
    histogram::fillValue(histsToFill->at(index).get(), value, eventWeight);

}

void HistogramManager::fillSingle2DHistograms(size_t index, std::pair<double, double>& values, double eventWeight, bool nonPrompt) {
    std::vector<std::shared_ptr<TH2D>>* histsToFill = get2DHistograms(nonPrompt);

    // fill hists
    histogram::fillValues(histsToFill->at(index).get(), values.first, values.second, eventWeight);

}

void HistogramManager::newSample(std::string& uniqueSampleName) {
    // make new hists from stuff
    currentSampleHists = new std::vector<std::shared_ptr<TH1D>>(histInfo->size());
    for( size_t dist = 0; dist < histInfo->size(); ++dist ){
        currentSampleHists->at(dist) = histInfo->at(dist).makeHist( histInfo->at(dist).name() + "_" + uniqueSampleName);
    }
    if (histInfo2D) {
        currentSampleHists2D = new std::vector<std::shared_ptr<TH2D>>(histInfo2D->size());

        for( size_t dist = 0; dist < histInfo2D->size(); ++dist ){
            currentSampleHists2D->at(dist) = histInfo2D->at(dist).makeHist_2D( histInfo2D->at(dist).name() + "_" + uniqueSampleName);
        }
    }
}

void HistogramManager::writeCurrentHistograms() {
    for( size_t dist = 0; dist < histInfo->size(); ++dist ) {
        currentSampleHists->at(dist)->Write(TString(histInfo->at(dist).name()), TObject::kOverwrite);
    }
    if (histInfo2D) {
        for( size_t dist = 0; dist < histInfo2D->size(); ++dist ) {
            currentSampleHists2D->at(dist)->Write(TString(histInfo2D->at(dist).name()), TObject::kOverwrite);
        }
    }
}

void HistogramManager::writeNonpromptHistograms() {
    for( size_t dist = 0; dist < histInfo->size(); ++dist ) {
        nonpromptHists->at(dist)->Write(TString(histInfo->at(dist).name()), TObject::kOverwrite);
    }
    if (histInfo2D) {
        for( size_t dist = 0; dist < histInfo2D->size(); ++dist ) {
            nonpromptHists2D->at(dist)->Write(TString(histInfo2D->at(dist).name()), TObject::kOverwrite);
        }
    }
}
