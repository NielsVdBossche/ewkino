#include "../interface/Uncertainty.h"

Uncertainty::Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms) : id(id) {
    name = translateUnc[id];
    std::string upFlag = name + "_Up";
    std::string downFlag = name + "_Down";

    bareHists = histograms;

    upHists = new HistogramSet(histograms, upFlag);
    downHists = new HistogramSet(histograms, downFlag);
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

void Uncertainty::fillHistograms(std::vector<double>& fillVec, double weightUp, double weightDown, unsigned subProc) {
    upHists->fillHistograms(subProc, fillVec, weightUp);
    downHists->fillHistograms(subProc, fillVec, weightDown);
}

void Uncertainty::fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, unsigned subProc) {
        upHists->fillSingleHistograms(subProc, fillVec, weightUp);
        downHists->fillSingleHistograms(subProc, fillVec, weightDown);
}

void Uncertainty::fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, unsigned subProc) {
    upHists->fill2DHistograms(subProc, fillVec, weightUp);
    downHists->fill2DHistograms(subProc, fillVec, weightDown);
}

void Uncertainty::writeHistograms() {
    gDirectory->cd("Up");
    upHists->writeHistograms();
    gDirectory->cd("../Down");
    downHists->writeHistograms();
    gDirectory->cd("..");

    if (upSubMap != nullptr) writeSubHistograms();
}

void Uncertainty::writeSubHistograms() {
    gDirectory->cd("..");

    for (auto it : *upSubMap) {
        std::string uncName = name + "_" + it.first;
        HistogramSet* localUpHists = upSubMap->at(it.first);
        HistogramSet* localDownHists = downSubMap->at(it.first);

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


void Uncertainty::fillUpOrDownHistograms(std::vector<double>& fillVec, double weight, bool up, unsigned subProc) {
    if (up) {
        upHists->fillHistograms(subProc, fillVec, weight);
    } else {
        downHists->fillHistograms(subProc, fillVec, weight);
    } 
}

void Uncertainty::fillUpOrDownSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weight, bool up, unsigned subProc) {
    if (up) {
        upHists->fillSingleHistograms(subProc, fillVec, weight);
    } else {
        downHists->fillSingleHistograms(subProc, fillVec, weight);
    }
}

void Uncertainty::fillUpOrDown2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weight, bool up, unsigned subProc) {
    if (up) {
        upHists->fill2DHistograms(subProc, fillVec, weight);
    } else {
        downHists->fill2DHistograms(subProc, fillVec, weight);
    } 
}

void Uncertainty::addSubUncertainties(std::vector<std::string>& subUnc) {
    upSubMap = new std::map<std::string, HistogramSet*>();
    downSubMap = new std::map<std::string, HistogramSet*>();
    for (unsigned i=0; i < subUnc.size(); i++) {
        std::string upFlag = name + "_" + subUnc[i] + "_Up";
        std::string downFlag = name + "_" + subUnc[i] + "_Down";

        (*upSubMap)[subUnc[i]] = new HistogramSet(bareHists, upFlag);
        (*downSubMap)[subUnc[i]] = new HistogramSet(bareHists, downFlag);
    }
}

void Uncertainty::fillSubHistograms(std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown, unsigned subProc) {
    HistogramSet* localUpHists = upSubMap->at(subUnc);
    HistogramSet* localDownHists = downSubMap->at(subUnc);
    
    localUpHists->fillHistograms(subProc, fillVec, weightUp);
    localDownHists->fillHistograms(subProc, fillVec, weightDown);
}

void Uncertainty::fillSubSingleHistograms(std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, unsigned subProc) {
    HistogramSet* localUpHists = upSubMap->at(subUnc);
    HistogramSet* localDownHists = downSubMap->at(subUnc);
    
    localUpHists->fillSingleHistograms(subProc, fillVec, weightUp);
    localDownHists->fillSingleHistograms(subProc, fillVec, weightDown);
}

void Uncertainty::fillSub2DHistograms(std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, unsigned subProc) {
    HistogramSet* localUpHists = upSubMap->at(subUnc);
    HistogramSet* localDownHists = downSubMap->at(subUnc);
    
    localUpHists->fill2DHistograms(subProc, fillVec, weightUp);
    localDownHists->fill2DHistograms(subProc, fillVec, weightDown);
}
