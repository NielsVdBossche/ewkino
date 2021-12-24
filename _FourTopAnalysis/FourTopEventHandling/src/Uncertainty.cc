#include "../interface/Uncertainty.h"

Uncertainty::Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms) : id(id) {
    name = translateUnc[id];
    std::string upFlag = name + "_Up";
    std::string downFlag = name + "_Down";

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
    gDirectory->cd("Up");
    upHists->writeCurrentHistograms();
    gDirectory->cd("../Down");
    downHists->writeCurrentHistograms();
    gDirectory->cd("..");
}

void Uncertainty::writeNonpromptHistograms() {
    gDirectory->cd("Up");
    upHists->writeNonpromptHistograms();    
    gDirectory->cd("../Down");
    downHists->writeNonpromptHistograms();
    gDirectory->cd("..");
}

void Uncertainty::fillUpOrDownHistograms(std::vector<double>& fillVec, double weight, bool up, bool nonPrompt) {
    if (up) {
        upHists->fillHistograms(fillVec, weight, nonPrompt);
    } else {
        downHists->fillHistograms(fillVec, weight, nonPrompt);
    } 
}

void Uncertainty::fillUpOrDownSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weight, bool up, bool nonPrompt) {
    for (auto it : fillVec) {
        if (up) {
            upHists->fillSingleHistogram(it.first, it.second, weight, nonPrompt);
        } else {
            downHists->fillSingleHistogram(it.first, it.second, weight, nonPrompt);
        }
    }
}

void Uncertainty::fillUpOrDown2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weight, bool up, bool nonPrompt) {
    if (up) {
        upHists->fill2DHistograms(fillVec, weight, nonPrompt);
    } else {
        downHists->fill2DHistograms(fillVec, weight, nonPrompt);
    } 
}
