#include "../interface/Uncertainty.h"

Uncertainty::Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms) {
    std::string addFlag = translateUnc[id];
    std::string upFlag = addFlag + "_Up";
    std::string downFlag = addFlag + "_Down";

    upHists = new HistogramManager(histograms, upFlag);
    downHists = new HistogramManager(histograms, downFlag);
}

void Uncertainty::newSample(std::string& uniqueName) {
    upHists->newSample(uniqueName);
    downHists->newSample(uniqueName);
}

void Uncertainty::fillHistograms(std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    upHists->fillHistograms(fillVec, weightUp, nonPrompt);
    downHists->fillHistograms(fillVec, weightDown, nonPrompt);
}

void Uncertainty::fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    for (auto it : fillVec) {
        upHists->fillSingleHistogram(it.first, it.second, weightUp, nonPrompt);
        downHists->fillSingleHistogram(it.first, it.second, weightDown, nonPrompt);
    }

}

void Uncertainty::fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    upHists->fill2DHistograms(fillVec, weightUp, nonPrompt);
    downHists->fill2DHistograms(fillVec, weightDown, nonPrompt);
}

void Uncertainty::writeCurrentHistograms() {

    gDirectory->cd("up");
    upHists->writeCurrentHistograms();
    gDirectory->cd("../down");
    downHists->writeCurrentHistograms();
    gDirectory->cd("..");
}

void Uncertainty::writeNonpromptHistograms() {
    gDirectory->cd("up");
    upHists->writeNonpromptHistograms();    
    gDirectory->cd("../down");
    downHists->writeNonpromptHistograms();
    gDirectory->cd("..");
}
