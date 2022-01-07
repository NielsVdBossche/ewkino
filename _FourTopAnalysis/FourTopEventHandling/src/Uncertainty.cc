#include "../interface/Uncertainty.h"

Uncertainty::Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms) : id(id) {
    name = translateUnc[id];
    std::string upFlag = name + "_Up";
    std::string downFlag = name + "_Down";

    bareHists = histograms;

    upHists = new HistogramManager(histograms, upFlag);
    downHists = new HistogramManager(histograms, downFlag);
}

void Uncertainty::newSample(std::string& uniqueName) {
    upHists->newSample(uniqueName);
    downHists->newSample(uniqueName);

    if (upSubMap != nullptr) {
        for (auto it : *upSubMap) {
            it.second->newSample(uniqueName);
        }
        for (auto it : *downSubMap) {
            it.second->newSample(uniqueName);
        }
    }
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

    if (upSubMap != nullptr) writeSubHistograms(false);
}

void Uncertainty::writeNonpromptHistograms() {
    gDirectory->cd("Up");
    upHists->writeNonpromptHistograms();    
    gDirectory->cd("../Down");
    downHists->writeNonpromptHistograms();
    gDirectory->cd("..");

    if (upSubMap != nullptr) writeSubHistograms(true);
}

void Uncertainty::writeSubHistograms(bool nonPrompt) {
    gDirectory->cd("..");

    for (auto it : *upSubMap) {
        std::string uncName = name + it.first;
        HistogramManager* localUpHists = upSubMap->at(it.first);
        HistogramManager* localDownHists = downSubMap->at(it.first);

        if (! gDirectory->GetDirectory(uncName.c_str())) {
            gDirectory->mkdir(uncName.c_str());
            gDirectory->cd(uncName.c_str());
            gDirectory->mkdir("Up");
            gDirectory->mkdir("Down");
        } else {
            gDirectory->cd(uncName.c_str());
        }

        if (nonPrompt) {
            gDirectory->cd("Up");
            localUpHists->writeNonpromptHistograms();    
            gDirectory->cd("../Down");
            localDownHists->writeNonpromptHistograms();
            gDirectory->cd("..");
        } else {
            gDirectory->cd("Up");
            localUpHists->writeCurrentHistograms();
            gDirectory->cd("../Down");
            localDownHists->writeCurrentHistograms();
            gDirectory->cd("..");
        }
        gDirectory->cd("..");
    }

    gDirectory->cd(name.c_str());
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

void Uncertainty::addSubUncertainties(std::vector<std::string>& subUnc) {
    upSubMap = new std::map<std::string, HistogramManager*>();
    downSubMap = new std::map<std::string, HistogramManager*>();
    for (unsigned i=0; i < subUnc.size(); i++) {
        std::string upFlag = name + "_" + subUnc[i] + "_Up";
        std::string downFlag = name + "_" + subUnc[i] + "_Down";

        (*upSubMap)[subUnc[i]] = new HistogramManager(bareHists, upFlag);
        (*downSubMap)[subUnc[i]] = new HistogramManager(bareHists, downFlag);
    }
}

void Uncertainty::fillSubHistograms(std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    HistogramManager* localUpHists = upSubMap->at(subUnc);
    HistogramManager* localDownHists = downSubMap->at(subUnc);

    std::cout << "filling subhistograms" << std::endl;
    
    localUpHists->fillHistograms(fillVec, weightUp, nonPrompt);
    localDownHists->fillHistograms(fillVec, weightDown, nonPrompt);
}

void Uncertainty::fillSubSingleHistograms(std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    HistogramManager* localUpHists = upSubMap->at(subUnc);
    HistogramManager* localDownHists = downSubMap->at(subUnc);
    
    for (auto it : fillVec) {
        localUpHists->fillSingleHistogram(it.first, it.second, weightUp, nonPrompt);
        localDownHists->fillSingleHistogram(it.first, it.second, weightDown, nonPrompt);
    }
}

void Uncertainty::fillSub2DHistograms(std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    HistogramManager* localUpHists = upSubMap->at(subUnc);
    HistogramManager* localDownHists = downSubMap->at(subUnc);
    
    localUpHists->fill2DHistograms(fillVec, weightUp, nonPrompt);
    localDownHists->fill2DHistograms(fillVec, weightDown, nonPrompt);
}
