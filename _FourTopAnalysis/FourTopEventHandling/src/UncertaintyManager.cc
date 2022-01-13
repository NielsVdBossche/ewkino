#include "../interface/UncertaintyManager.h"

UncertaintyWrapper::UncertaintyWrapper(HistogramManager* histograms) {
    // init uncertainties for all channels
    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            std::cout << "envelope " << id << std::endl;
            uncHistMap[shapeUncId(id)] = new UncertaintyEnvelope(translateUnc, shapeUncId(id), histograms);
        } else {
            std::cout << "current uncertainty " << id << std::endl;
            uncHistMap[shapeUncId(id)] = new Uncertainty(translateUnc, shapeUncId(id), histograms);
        }
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
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            id++;
            continue;
        }
        uncHistMap[shapeUncId(id)]->newSample(uniqueSampleName);
        id++;
    }
}

void UncertaintyWrapper::newProcess(std::string& uniqueProcessName, TFile* outfile) {
    unsigned id = 0;
    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[shapeUncId(id)]);
            unc->newProcess(uniqueProcessName, outfile);
        }
        id++;
    }
}


void UncertaintyWrapper::writeCurrentHistograms() {
    unsigned id = 0;

    while (id != shapeUncId::end) {
        if (id == shapeUncId::qcdScale || id == shapeUncId::pdfShapeVar) {
            id++;
            continue;
        }
        std::string uncName = translateUnc[shapeUncId(id)];
        if (! gDirectory->GetDirectory(uncName.c_str())) {
            gDirectory->mkdir(uncName.c_str());
            gDirectory->cd(uncName.c_str());
            gDirectory->mkdir("Up");
            gDirectory->mkdir("Down");
        } else {
            gDirectory->cd(uncName.c_str());
        }
        
        uncHistMap[shapeUncId(id)]->writeCurrentHistograms();
        gDirectory->cd("..");
        id++;
    }
}

void UncertaintyWrapper::writeNonpromptHistograms() {
    unsigned id = 0;

    while (id != shapeUncId::end) {
        std::string uncName = translateUnc[shapeUncId(id)];

        if (! gDirectory->GetDirectory(uncName.c_str())) {
            gDirectory->mkdir(uncName.c_str());
            gDirectory->cd(uncName.c_str());
            gDirectory->mkdir("Up");
            gDirectory->mkdir("Down");
        } else {
            gDirectory->cd(uncName.c_str());
        }

        uncHistMap[shapeUncId(id)]->writeNonpromptHistograms();
        
        gDirectory->cd("..");
        id++;
    }
}

void UncertaintyWrapper::fillUpOrDownUncertainty(shapeUncId id, std::vector<double>& fillVec, double weight, bool up, bool nonPrompt) {
    uncHistMap[id]->fillUpOrDownHistograms(fillVec, weight, up, nonPrompt);
}

void UncertaintyWrapper::fillUpOrDownSingleHistograms(shapeUncId id, std::vector<std::pair<int, double>>& fillVec, double weight, bool up, bool nonPrompt) {
    uncHistMap[id]->fillUpOrDownSingleHistograms(fillVec, weight, up, nonPrompt);
}

void UncertaintyWrapper::fillUpOrDown2DHistograms(shapeUncId id, std::vector<std::pair<double, double>>& fillVec, double weight, bool up, bool nonPrompt) {
    uncHistMap[id]->fillUpOrDown2DHistograms(fillVec, weight, up, nonPrompt);
}

void UncertaintyWrapper::fillEnvelope(shapeUncId id, std::vector<double>& fillVec, std::vector<double> weight, bool nonPrompt) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelope(fillVec, weight, nonPrompt);
}
void UncertaintyWrapper::fillEnvelopeSingles(shapeUncId id, std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, bool nonPrompt) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelopeSingles(fillVec, weight, nonPrompt );
}
void UncertaintyWrapper::fillEnvelope2Ds(shapeUncId id, std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, bool nonPrompt) {
    UncertaintyEnvelope* unc = static_cast<UncertaintyEnvelope*>(uncHistMap[id]);
    unc->fillEnvelope2Ds(fillVec, weight, nonPrompt );
}

void UncertaintyWrapper::addSubUncertainties(shapeUncId id, std::vector<std::string>& subUnc) {
    uncHistMap[id]->addSubUncertainties(subUnc);
}

void UncertaintyWrapper::fillSubUncertainty(shapeUncId id, std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    uncHistMap[id]->fillSubHistograms(subUnc, fillVec, weightUp, weightDown, nonPrompt);
}

void UncertaintyWrapper::fillSubSingleHistograms(shapeUncId id, std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    uncHistMap[id]->fillSubSingleHistograms(subUnc, fillVec, weightUp, weightDown, nonPrompt);
}

void UncertaintyWrapper::fillSub2DHistograms(shapeUncId id, std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt) {
    uncHistMap[id]->fillSub2DHistograms(subUnc, fillVec, weightUp, weightDown, nonPrompt);
}