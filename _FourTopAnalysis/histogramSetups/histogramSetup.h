#ifndef histogramSetup_h
#define histogramSetup_h

#include "../globalSettings.h"

#include "../../Tools/interface/HistInfo.h"
#include "../../Tools/interface/histogramTools.h"

#include "../../Event/interface/Event.h"

// Might move this functionality back to FourTop class itself

namespace fourTopHists {
    std::vector<HistInfo> bdtInfoDL();
    std::vector<HistInfo> bdtInfoML();

    std::vector<HistInfo> bdtOutputDL();
    std::vector<HistInfo> bdtOutputML();

    std::vector<HistInfo> lepInfoDL();
    std::vector<HistInfo> lepInfoML();

    std::vector<HistInfo> jetInfoDL();
    std::vector<HistInfo> jetInfoML();

    // others

    // create filling vector?
    std::vector<double> fillLepInfoDL(Event* event);
    std::vector<double> fillLepInfoML(Event* event);

}

#endif
