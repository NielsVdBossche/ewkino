#include "../interface/Uncertainty.h"

Uncertainty::Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms) {
    name = translateUnc[id];
    std::string upFlag = name + "_Up";
    std::string downFlag = name + "_Down";

    upHists = new HistogramManager(histograms, upFlag);
    downHists = new HistogramManager(histograms, downFlag);

    if (id == shapeUncId::qcdScale) {
        envelope = true;
        createEnvelope(histograms);
        // generateEnvelope -> if envelope is present, fill histograms for each
    }
}

void Uncertainty::newSample(std::string& uniqueName) {
    upHists->newSample(uniqueName);
    downHists->newSample(uniqueName);

    if (envelope) {
        for (unsigned i=0; i < envelopeHists.size(); i++) {
            envelopeHists[i]->newSample(uniqueName);
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
    if (envelope) finalizeEnvelope(false);

    gDirectory->cd("Up");
    upHists->writeCurrentHistograms();
    gDirectory->cd("../Down");
    downHists->writeCurrentHistograms();
    gDirectory->cd("..");
}

void Uncertainty::writeNonpromptHistograms() {
    if (envelope) finalizeEnvelope(true);

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

void Uncertainty::createEnvelope(HistogramManager* histograms) {
    int variations = 0;
    if (id == shapeUncId::qcdScale) {
        variations = 6;
    }

    for (int i=0; i < variations; i++) {
        std::string otherName = name + std::to_string(i);
        envelopeHists.push_back(new HistogramManager(histograms, otherName));
    }
}

void Uncertainty::fillEnvelope(std::vector<double>& fillVec, std::vector<double> weight, bool nonPrompt) {
    std::cout << envelopeHists.size() << " " << weight.size() << std::endl;
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fillHistograms(fillVec, weight[i], nonPrompt);
    }
}

void Uncertainty::fillEnvelopeSingles(std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, bool nonPrompt) {
    for (unsigned i=0; i < weight.size(); i++) {
        for (auto it : fillVec) {
            envelopeHists[i]->fillSingleHistogram(it.first, it.second, weight[i], nonPrompt);
        }
    }
}

void Uncertainty::fillEnvelope2Ds(std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, bool nonPrompt) {
    for (unsigned i=0; i < weight.size(); i++) {
        envelopeHists[i]->fill2DHistograms(fillVec, weight[i], nonPrompt);
    }
}

void Uncertainty::finalizeEnvelope(bool nonPrompt) {
    // nominal histograms should be fixed
    std::vector<std::shared_ptr<TH1D>>* upHistograms = upHists->getHistograms(nonPrompt);
    std::vector<std::shared_ptr<TH1D>>* downHistograms = downHists->getHistograms(nonPrompt);

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
