#include "../interface/HistogramSet.h"

void HistogramSet::newSample(std::string& uniqueSampleName) {
    for (unsigned i=0; i<processHist.size(); i++) {
        std::string uniqueName = uniqueSampleName + "_" + processHistName[i];
        processHist[i]->newSample(uniqueName);
    }
}

void HistogramSet::writeHistograms(unsigned subProc) {
    processHist[subProc]->writeHistograms();
}
