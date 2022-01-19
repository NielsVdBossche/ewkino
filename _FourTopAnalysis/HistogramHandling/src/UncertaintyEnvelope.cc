#include "../interface/UncertaintyEnvelope.h"

UncertaintyEnvelope::UncertaintyEnvelope(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms) :
    Uncertainty(translateUnc, id, histograms) 
    {

    process = "";

    int variations = 0;
    if (id == shapeUncId::qcdScale) {
        variations = 6;
    } else if (id == shapeUncId::pdfShapeVar) {
        variations = 100;
    }

    for (int i=0; i < variations; i++) {
        std::string otherName = getName() + std::to_string(i);
        envelopeHists.push_back(new HistogramSet(histograms, otherName));
    }
}

void UncertaintyEnvelope::newProcess(std::string& newProcess, TFile* outfile) {
    if (process == newProcess) return;

    if (process != "") writeCurrentHistogramsProcess(outfile);

    //std::cout << "New process!" << std::endl;

    getUpHists()->newSample(newProcess);
    getDownHists()->newSample(newProcess);
    process = newProcess;
    for (unsigned i=0; i < envelopeHists.size(); i++) {
        envelopeHists[i]->newSample(newProcess);
    }
}

void UncertaintyEnvelope::fillEnvelope(std::vector<double>& fillVec, std::vector<double> weight, unsigned subProc) {
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fillHistograms(subProc, fillVec, weight[i]);
    }
}

void UncertaintyEnvelope::fillEnvelopeSingles(std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, unsigned subProc) {
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fillSingleHistograms(subProc, fillVec, weight[i]);
    }
}

void UncertaintyEnvelope::fillEnvelope2Ds(std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, unsigned subProc) {
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fill2DHistograms(subProc, fillVec, weight[i]);
    }
}

void UncertaintyEnvelope::finalizeEnvelope(unsigned subProc) {
    // nominal histograms should be fixed
    std::vector<std::shared_ptr<TH1D>>* upHistograms = getUpHists()->getHistograms(subProc);
    std::vector<std::shared_ptr<TH1D>>* downHistograms = getDownHists()->getHistograms(subProc);

    for (unsigned i = 0; i < upHistograms->size(); i++) {
        // loop all individual histograms
        for (auto hists : envelopeHists) {

            // loop all possible up and down variations
            std::vector<std::shared_ptr<TH1D>>* currentVariation = hists->getHistograms(subProc);
            std::shared_ptr<TH1D> currentHist = currentVariation->at(i);

            for(int j=1; j < currentHist->GetNbinsX()+1; j++){

                // for each up and down variation, we fix the content
                double bincontentCurrent = currentHist->GetBinContent(j);

                if (bincontentCurrent > upHistograms->at(i)->GetBinContent(j) ){
                    upHistograms->at(i)->SetBinContent(j, bincontentCurrent);
                } else if (bincontentCurrent < downHistograms->at(i)->GetBinContent(j) ){
                    downHistograms->at(i)->SetBinContent(j, bincontentCurrent);
                }
            }
        }
    }
}

void UncertaintyEnvelope::writeCurrentHistogramsProcess(TFile* outfile) {
    /*
    finalizeEnvelope(false);
    outfile->cd("Uncertainties");
    gDirectory->cd(process.c_str());
    if (! gDirectory->GetDirectory(getName().c_str())) {
        gDirectory->mkdir(getName().c_str());
        gDirectory->cd(getName().c_str());
        gDirectory->mkdir("Up");
        gDirectory->mkdir("Down");
    } else {
        gDirectory->cd(getName().c_str());
    }

    Uncertainty::writeHistograms();
    */
}


