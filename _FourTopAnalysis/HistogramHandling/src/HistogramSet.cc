#include "../interface/HistogramSet.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

HistogramSet::HistogramSet(std::vector<std::string>& initialProcesses, std::string addFlag, std::vector<HistInfo>* oneDimInfo, std::vector<HistInfo_2D>* twoDimInfo) : 
    additionalFlags(addFlag), 
    processHistName(initialProcesses) 
    {
    for (auto it : processHistName) {
        HistogramContainer* newHists = new HistogramContainer(oneDimInfo, twoDimInfo);
        processHist.push_back(newHists);
    }
}

HistogramSet::HistogramSet(HistogramSet* copy, std::string& name) {
    additionalFlags = copy->additionalFlags + "_" + name;
    processHistName = copy->getProcessNames();

    std::vector<HistInfo>* oneDimInfo = copy->getHistogramContainer(0)->getHistInfo();
    std::vector<HistInfo_2D>* twoDimInfo = copy->getHistogramContainer(0)->get2DHistInfo();

    for (auto it : processHistName) {
        HistogramContainer* newHists = new HistogramContainer(oneDimInfo, twoDimInfo);
        processHist.push_back(newHists);
    }
}

void HistogramSet::changeProcess(unsigned index, std::string& newTitle) {
    processHistName[index] = newTitle;
}

void HistogramSet::newSample(std::string& uniqueSampleName) {
    for (unsigned i=0; i<processHist.size(); i++) {
        std::string uniqueName = additionalFlags + "_" + uniqueSampleName + "_" + processHistName[i];
        processHist[i]->newSample(uniqueName);
    }
}

void HistogramSet::newSample(std::string& uniqueSampleName, unsigned subProc) {
    std::string uniqueName = additionalFlags + "_" + uniqueSampleName + "_" + processHistName[subProc];
    processHist[subProc]->newSample(uniqueName);
}


void HistogramSet::writeHistograms(unsigned subProc) {
    processHist[subProc]->writeHistograms();
}

void HistogramSet::fillHistograms(unsigned subProcess, std::vector<double>& fillVec, double eventWeight) {
    processHist[subProcess]->fillHistograms(fillVec, eventWeight);
}

void HistogramSet::fill2DHistograms(unsigned subProcess, std::vector<std::pair<double, double>>& fillVec, double eventWeight) {
    processHist[subProcess]->fill2DHistograms(fillVec, eventWeight);
}

void HistogramSet::fillSingleHistograms(unsigned subProcess, std::vector<std::pair<int, double>>& fillVec, double eventWeight) {
    processHist[subProcess]->fillSingleHistograms(fillVec, eventWeight);
}

void HistogramSet::fillSingle2DHistograms(unsigned subProcess, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight) {
    processHist[subProcess]->fillSingle2DHistograms(fillVec, eventWeight);
}
