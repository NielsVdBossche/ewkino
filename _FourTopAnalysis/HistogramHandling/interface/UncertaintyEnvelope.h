#ifndef UNCERTAINTYENVELOPE_H
#define UNCERTAINTYENVELOPE_H

#include "Uncertainty.h"

class UncertaintyEnvelope : public Uncertainty {
private:
    std::vector<HistogramSet*> envelopeHists;
    std::string process;

public:
    UncertaintyEnvelope(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms);
    //~UncertaintyEnvelope();

    void finalizeEnvelope(unsigned subProc);

    //void newSample(std::string& uniqueName) {};
    void newProcess(std::string& newProcess, TFile* outfile);

    void fillEnvelope(std::vector<double>& fillVec, std::vector<double> weight, unsigned subProc);
    void fillEnvelopeSingles(std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, unsigned subProc);
    void fillEnvelope2Ds(std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, unsigned subProc);

    void writeCurrentHistogramsProcess(TFile* outfile);
    void writeCurrentHistograms() {};
};

#endif