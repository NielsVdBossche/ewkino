#include "../interface/UncertaintyManager.h"

UncertaintyWrapper::UncertaintyWrapper(HistogramManager* histograms) {
    // init uncertainties for all channels
    unsigned id = 0;

    while (id != shapeUncId::end) {
        uncHistMap[shapeUncId(id)] = new Uncertainty(translateUnc, shapeUncId(id), histograms);
        id++;
    }
}

void UncertaintyWrapper::fillUncertainty(shapeUncId id, std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    uncHistMap[id]->fillHistograms(fillVec, weightUp, weightDown, nonPrompt);
}

void UncertaintyWrapper::fillSingleHistograms(shapeUncId id, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    uncHistMap[id]->fillSingleHistograms(fillVec, weightUp, weightDown, nonPrompt);
}

void UncertaintyWrapper::fill2DHistograms(shapeUncId id, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    uncHistMap[id]->fill2DHistograms(fillVec, weightUp, weightDown, nonPrompt);
}

void UncertaintyWrapper::newSample(std::string& uniqueSampleName) {
    unsigned id = 0;

    while (id != shapeUncId::end) {
        uncHistMap[shapeUncId(id)]->newSample(uniqueSampleName);
        id++;
    }
}

void UncertaintyWrapper::writeCurrentHistograms() {
    unsigned id = 0;

    while (id != shapeUncId::end) {
        std::string uncName = translateUnc[shapeUncId(id)];
        if (! gDirectory->GetDirectory(uncName.c_str())) {
            gDirectory->mkdir(uncName.c_str());
            gDirectory->cd(uncName.c_str());
            gDirectory->mkdir("up");
            gDirectory->mkdir("down");
        } else {
            gDirectory->cd(uncName.c_str());
        }
        //gDirectory->mkdir(uncName.c_str());
        //gDirectory->cd(uncName.c_str());
        //gDirectory->mkdir("up");
        //gDirectory->mkdir("down");
        
        uncHistMap[shapeUncId(id)]->writeCurrentHistograms();
        gDirectory->cd("..");
        id++;
    }
}

void UncertaintyWrapper::writeNonpromptHistograms() {
    unsigned id = 0;

    while (id != shapeUncId::end) {
        std::string uncName = translateUnc[shapeUncId(id)];
        gDirectory->mkdir(uncName.c_str());
        gDirectory->cd(uncName.c_str());
        gDirectory->mkdir("up");
        gDirectory->mkdir("down");

        uncHistMap[shapeUncId(id)]->writeNonpromptHistograms();
        
        gDirectory->cd("..");
        id++;
    }
}
