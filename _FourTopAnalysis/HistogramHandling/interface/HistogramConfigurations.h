#ifndef HISTOGRAMCONFIGURATIONS_H
#define HISTOGRAMCONFIGURATIONS_H

#include <vector>
#include "../../globalSettings.h"
#include "../../../Tools/interface/HistInfo.h"

class EventFourT;

namespace HistogramConfig {
    std::vector<HistInfo>* getMinimalHists(const eventClass evClass);
    std::vector<HistInfo>* getNominalHists(const eventClass evClass);
    std::vector<HistInfo>* getAllBDTVarsHists(const eventClass evClass);
    
    std::vector<double> fillMinimalHists(const eventClass evClass, EventFourT* event);
    std::vector<double> fillNominalHists(const eventClass evClass, EventFourT* event);
    std::vector<double> fillAllBDTVarsHists(const eventClass evClass, EventFourT* event);
}


#endif