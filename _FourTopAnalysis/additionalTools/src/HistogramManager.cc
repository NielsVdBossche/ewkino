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

void HistogramManager::fillHistograms(std::vector<double>& fillValues, double eventWeight, bool nonPrompt) {
    std::vector<std::shared_ptr<TH1D>>* histsToFill = getHistograms(nonPrompt);

    // fill hists
    histHelper::histFiller(fillValues, histsToFill, eventWeight);
}

void HistogramManager::newSample(std::string& uniqueSampleName) {
    // make new hists from stuff
    currentSampleHists = new std::vector<std::shared_ptr<TH1D>>(histInfo->size());
    for( size_t dist = 0; dist < histInfo->size(); ++dist ){
        currentSampleHists->at(dist) = histInfo->at(dist).makeHist( histInfo->at(dist).name() + "_" + uniqueSampleName);
    }
}

void HistogramManager::writeCurrentHistograms() {
    for( size_t dist = 0; dist < histInfo->size(); ++dist ) {
        currentSampleHists->at(dist)->Write(TString(histInfo->at(dist).name()), TObject::kOverwrite);
    }
}

void HistogramManager::writeNonpromptHistograms() {
    for( size_t dist = 0; dist < histInfo->size(); ++dist ) {
        nonpromptHists->at(dist)->Write(TString(histInfo->at(dist).name()), TObject::kOverwrite);
    }
}
