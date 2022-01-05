#ifndef UNCERTAINTYENVELOPE_H
#define UNCERTAINTYENVELOPE_H

#include "Uncertainty.h"

class UncertaintyEnvelope : public Uncertainty {
private:
    std::vector<HistogramManager*> envelopeHists;
    std::string process;

public:
    UncertaintyEnvelope(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms);
    //~UncertaintyEnvelope();

    void finalizeEnvelope(bool nonPrompt);

    //void newSample(std::string& uniqueName) {};
    void newProcess(std::string& newProcess, TFile* outfile);

    void fillEnvelope(std::vector<double>& fillVec, std::vector<double> weight, bool nonPrompt);
    void fillEnvelopeSingles(std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, bool nonPrompt);
    void fillEnvelope2Ds(std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, bool nonPrompt);

    void writeCurrentHistogramsProcess(TFile* outfile);
    void writeCurrentHistograms() {};
    void writeNonpromptHistograms();

};

#endif