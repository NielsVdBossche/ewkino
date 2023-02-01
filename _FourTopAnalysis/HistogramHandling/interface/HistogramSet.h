#ifndef HISTOGRAMSET_H
#define HISTOGRAMSET_H

#include "../../globalSettings.h"

#include "HistogramContainer.h"

class HistogramSet {
    private:
        std::string additionalFlags;

        std::vector<HistogramContainer*> processHist;
        std::vector<std::string> processHistName;
    public:
        HistogramSet(std::vector<std::string>& initialProcesses, std::string addFlag, std::vector<HistInfo>* oneDimInfo, std::vector<HistInfo_2D>* twoDimInfo);
        HistogramSet(HistogramSet* copy, std::string& name);

        ~HistogramSet();

        HistogramContainer* getHistogramContainer(unsigned subProc) {return processHist[subProc];}
        std::vector<TH1D*>* getHistograms(unsigned subProc) {return processHist[subProc]->getHistograms();};
        std::vector<TH2D*>* get2DHistograms(unsigned subProc) {return processHist[subProc]->get2DHistograms();};
        std::vector<std::string>& getProcessNames() {return processHistName;}

        //void addProcesses(std::vector<std::string>& processNames);
        void changeProcess(unsigned index, std::string& newTitle);
        void addProcess(std::string& newProc);

        void fillHistograms(unsigned subProcess, std::vector<double>& fillVec, double eventWeight);
        void fill2DHistograms(unsigned subProcess, std::vector<std::pair<double, double>>& fillVec, double eventWeight);

        void fillSingleHistograms(unsigned subProcess, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(unsigned subProcess, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void newSample(std::string& uniqueSampleName);
        void newSample(std::string& uniqueSampleName, unsigned subProc);

        void writeHistograms(unsigned subProc);

        void flushOldHistograms();
};

#endif