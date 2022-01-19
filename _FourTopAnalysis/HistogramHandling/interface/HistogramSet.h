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
        ~HistogramSet();

        void addSubProcesses(std::vector<std::string>& processNames);
        void changeSubProcessTitle(unsigned index, std::string& newTitle);

        void fillHistograms(unsigned subProcess, std::vector<double>& fillVec, double eventWeight);
        void fill2DHistograms(unsigned subProcess, std::vector<std::pair<double, double>>& fillVec, double eventWeight);

        void fillSingleHistograms(unsigned subProcess, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(unsigned subProcess, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void newSample(std::string& uniqueSampleName);
        void writeHistograms();
};

#endif