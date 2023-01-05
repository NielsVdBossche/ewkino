#ifndef histogramSetup_h
#define histogramSetup_h

#include "../globalSettings.h"

#include "../../Tools/interface/HistInfo.h"
#include "../../Tools/interface/histogramTools.h"

#include "../additionalTools/interface/calculators.h"

#include "../../Event/interface/Event.h"
#include "../FourTopEventHandling/interface/EventFourT.h"
#include "../FourTopEventHandling/interface/MVAHandler.h"

#include "../TopQuarkReconstruction/interface/TopReconstructionNew.h"

#include "../../Tools/interface/mt2.h"

// Might move this functionality back to FourTop class itself

namespace fourTopHists {
    std::vector<double> fillAllHists(EventFourT* selec);
    std::vector<double> fillAllLean(eventClass evClass, EventFourT* selec);

    std::vector<HistInfo>* testHists(eventClass);
    std::vector<double> fillTestHists(eventClass, EventFourT* selec);
}

#endif
