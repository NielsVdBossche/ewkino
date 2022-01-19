#ifndef HISTOGRAMSET_H
#define HISTOGRAMSET_H

#include "HistogramContainer.h"

class HistogramSet {
    private:
        std::string additionalFlags;

        std::vector<HistogramContainer*> processHist;
        std::vector<std::string> processHistName;
    public:
        HistogramSet(std::string addFlag);
        HistogramSet(HistogramSet* copy, std::string& name);

        ~HistogramSet();

        std::vector<std::shared_ptr<TH1D>>* getHistograms(unsigned subProc) {return processHist[subProc]->getHistograms();};
        std::vector<std::string>& getProcessNames() {return processHistName;}

        void addSubProcesses(std::vector<std::string>& processNames);
        void changeSubProcessTitle(unsigned index, std::string& newTitle);

        void fillHistograms(unsigned subProcess, std::vector<double>& fillVec, double eventWeight);
        void fill2DHistograms(unsigned subProcess, std::vector<std::pair<double, double>>& fillVec, double eventWeight);

        void fillSingleHistograms(unsigned subProcess, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(unsigned subProcess, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void newSample(std::string& uniqueSampleName);
        void writeHistograms(unsigned subProc);
};

#endif