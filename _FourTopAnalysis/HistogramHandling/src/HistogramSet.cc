#include "../interface/HistogramSet.h"

void HistogramSet::newSample(std::string& uniqueSampleName) {
    for (auto it : processHist) {
        it->newSample(uniqueSampleName);
    }
}

void HistogramSet::writeHistograms() {
    for (unsigned i; i < processHistName.size(); i++) {
        // TODO: folder stuff
        processHist[i]->writeHistograms();
    }
}
