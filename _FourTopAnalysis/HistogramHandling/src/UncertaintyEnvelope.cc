#include "../interface/UncertaintyEnvelope.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

UncertaintyEnvelope::UncertaintyEnvelope(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms) :
    Uncertainty(translateUnc, id, histograms) 
    {

    processes = {""};

    int variations = 0;
    if (id == shapeUncId::qcdScale) {
        variations = 6;
    } else if (id == shapeUncId::pdfShapeVar) {
        variations = 100;
        SetPrintAllVariations(true);
    }

    for (int i=0; i < variations; i++) {
        std::string otherName = getName() + "_" + std::to_string(i);
        envelopeHists.push_back(new HistogramSet(histograms, otherName));
    }
}

//UncertaintyEnvelope::UncertaintyEnvelope(std::string& uncName, shapeUncId uncID, HistogramSet* histograms) :
//    Uncertainty(uncName, uncID, histograms) 
//    {
//
//    processes = {""};
//
//    int variations = 0;
//    if (uncID == shapeUncId::qcdScale) {
//        variations = 6;
//    } else if (uncID == shapeUncId::pdfShapeVar) {
//        variations = 100;
//        SetPrintAllVariations(true);
//    }
//
//    for (int i=0; i < variations; i++) {
//        std::string otherName = getName() + "_" + std::to_string(i);
//        envelopeHists.push_back(new HistogramSet(histograms, otherName));
//    }
//}

UncertaintyEnvelope::~UncertaintyEnvelope() {
    for (auto& it : envelopeHists) {
        delete it;
    }
}

void UncertaintyEnvelope::changeProcess(unsigned index, std::string& newProcess) {
    if (processes[index] == newProcess) return;
    std::string process = processes[index];
    std::string empty = "";

    if (process != "") {
        writeHistogramsEnvelope(index);
    } else if (index == 0) {
        for (unsigned j=1; j < getUpHists()->getProcessNames().size(); j++){
            getUpHists()->newSample(empty, j);
            getDownHists()->newSample(empty, j);
            for (unsigned i=0; i < envelopeHists.size(); i++) {
                envelopeHists[i]->newSample(empty, j);
            }
        }
    }
    //std::cout << "New process!" << std::endl;



    getUpHists()->changeProcess(index, newProcess);
    getUpHists()->newSample(empty, index);
    getDownHists()->changeProcess(index, newProcess);
    getDownHists()->newSample(empty, index);

    processes[index] = newProcess;
    for (unsigned i=0; i < envelopeHists.size(); i++) {
        envelopeHists[i]->changeProcess(index, newProcess);
        envelopeHists[i]->newSample(empty, index);
    }
}

void UncertaintyEnvelope::addProcess(std::string& newProc) {
    std::string empty = "";
    for (auto env : envelopeHists) {
        env->addProcess(empty);
    }
    getUpHists()->addProcess(empty);
    getDownHists()->addProcess(empty);
    processes.push_back(empty);
    changeProcess(processes.size()-1, newProc);
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


    if (getID() == shapeUncId::qcdScale) {
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
    } else if (getID() == shapeUncId::pdfShapeVar) {
        std::vector<std::shared_ptr<TH1D>>* upHistograms = getUpHists()->getHistograms(subProc);
        std::vector<std::shared_ptr<TH1D>>* downHistograms = getDownHists()->getHistograms(subProc);
        for (unsigned i = 0; i < upHistograms->size(); i++) {
            // loop all individual histograms
            TH1D* tempHistogram = new TH1D(*(upHistograms->at(i).get()));
            tempHistogram->Reset();

            for (auto hists : envelopeHists) {
                // loop all possible up and down variations
                std::shared_ptr<TH1D> currentHist = hists->getHistograms(subProc)->at(i);

                for(int j=1; j < tempHistogram->GetNbinsX()+1; j++){
                    // for each up and down variation, we fix the content
                    double bincontentCurrent = currentHist->GetBinContent(j) - upHistograms->at(i)->GetBinContent(j);
                    tempHistogram->SetBinContent(j, tempHistogram->GetBinContent(j) + bincontentCurrent * bincontentCurrent);
                }
            }

            //for(int j=1; j < tempHistogram->GetNbinsX()+1; j++){
            //    tempHistogram->SetBinContent(j, sqrt(tempHistogram->GetBinContent(j)));
            //}
            upHistograms->at(i)->Reset();
            upHistograms->at(i)->Add(tempHistogram);
            downHistograms->at(i)->Reset();
            downHistograms->at(i)->Add(tempHistogram, -1.);
        }
    }
}

void UncertaintyEnvelope::writeHistogramsEnvelope(unsigned processNb) {
    finalizeEnvelope(processNb);
    //outfile->cd("Uncertainties");
    std::string process = processes[processNb];
    gDirectory->cd(process.c_str());
    if (! gDirectory->GetDirectory(getName().c_str())) {
        gDirectory->mkdir(getName().c_str());
        gDirectory->cd(getName().c_str());
        gDirectory->mkdir("Up");
        gDirectory->mkdir("Down");
    } else {
        gDirectory->cd(getName().c_str());
    }

    if (GetPrintAllVariations() || getID() == shapeUncId::qcdScale) {
        for (auto hists : envelopeHists) {
            // loop all possible up and down variations
            std::vector<std::shared_ptr<TH1D>>* currentVariation = hists->getHistograms(processNb);
            for (auto hist : *currentVariation) {
                hist->Write(hist->GetName(), TObject::kOverwrite);
            }
        }
    }
    Uncertainty::writeHistograms(processNb);


    gDirectory->cd("../../");
}


