#ifndef UNCERTAINTYENVELOPE_H
#define UNCERTAINTYENVELOPE_H

#include "../../globalSettings.h"

#include "Uncertainty.h"

class UncertaintyEnvelope : public Uncertainty {
private:
    std::vector<HistogramSet*> envelopeHists;
    std::vector<std::string> processes;
public:
    UncertaintyEnvelope(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms);
    //UncertaintyEnvelope(std::string& uncName, shapeUncId uncID, HistogramSet* histograms);
    ~UncertaintyEnvelope();

    void finalizeEnvelope(unsigned subProc);

    //void newSample(std::string& uniqueName) {};
    virtual void changeProcess(unsigned index, std::string& newProcess);
    virtual void addProcess(std::string& newProc);

    void fillEnvelope(std::vector<double>& fillVec, std::vector<double> weight, unsigned subProc);
    void fillEnvelopeSingles(std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight, unsigned subProc);
    void fillEnvelope2Ds(std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight, unsigned subProc);

    void writeHistogramsEnvelope(unsigned processNb);
};

#endif