#include "../interface/UncertaintyEnvelope.h"

UncertaintyEnvelope::UncertaintyEnvelope(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms) :
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
        envelopeHists.push_back(new HistogramManager(histograms, otherName));
    }
}

void UncertaintyEnvelope::newProcess(std::string& newProcess, TFile* outfile) {
    if (process == newProcess) return;

    if (process != "") writeCurrentHistogramsProcess(outfile);

    getUpHists()->newSample(newProcess);
    getDownHists()->newSample(newProcess);
    process = newProcess;
    for (unsigned i=0; i < envelopeHists.size(); i++) {
        envelopeHists[i]->newSample(newProcess);
    }
}

void UncertaintyEnvelope::fillEnvelope(std::vector<double>& fillVec, std::vector<double> weight, bool nonPrompt) {
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fillHistograms(fillVec, weight[i], nonPrompt);
    }
}

void UncertaintyEnvelope::fillEnvelopeSingles(std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, bool nonPrompt) {
    for (unsigned i=0; i < weight.size(); i++) {
        for (auto it : fillVec) {
            envelopeHists[i]->fillSingleHistogram(it.first, it.second, weight[i], nonPrompt);
        }
    }
}

void UncertaintyEnvelope::fillEnvelope2Ds(std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, bool nonPrompt) {
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fill2DHistograms(fillVec, weight[i], nonPrompt);
    }
}

void UncertaintyEnvelope::finalizeEnvelope(bool nonPrompt) {
    // nominal histograms should be fixed
    if (getID() == shapeUncId::pdfShapeVar) return;
    std::vector<std::shared_ptr<TH1D>>* upHistograms = getUpHists()->getHistograms(nonPrompt);
    std::vector<std::shared_ptr<TH1D>>* downHistograms = getDownHists()->getHistograms(nonPrompt);

    for (unsigned i = 1; i < upHistograms->size(); i++) {
        // loop all individual histograms
        for (auto hists : envelopeHists) {
            // loop all possible up and down variations
            std::vector<std::shared_ptr<TH1D>>* currentVariation = hists->getHistograms(nonPrompt);
            std::shared_ptr<TH1D> currentHist = currentVariation->at(i);

            for(int j=1; j < currentHist->GetNbinsX()+1; ++i){
                // for each up and down variation, we fix the content

                if (currentHist->GetBinContent(j) > upHistograms->at(j)->GetBinContent(j) ){
                    upHistograms->at(j)->SetBinContent(j, currentHist->GetBinContent(j));
                }
                if (currentHist->GetBinContent(j) < downHistograms->at(j)->GetBinContent(j) ){
                    downHistograms->at(j)->SetBinContent(j, currentHist->GetBinContent(j));
                }
            }
        }
    }
}

void UncertaintyEnvelope::writeCurrentHistogramsProcess(TFile* outfile) {
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

    Uncertainty::writeCurrentHistograms();
}

void UncertaintyEnvelope::writeNonpromptHistograms() {
    finalizeEnvelope(true);
    Uncertainty::writeNonpromptHistograms();
}

