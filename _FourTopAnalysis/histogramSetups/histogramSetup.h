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
    std::vector<HistInfo>* allHistsDL();
    std::vector<HistInfo>* allHistsML();


    // others
    std::vector<HistInfo>* crzInfo();
    std::vector<HistInfo>* crwInfo();

    // create filling vector?
    std::vector<double> fillLepInfoDL(Event* event);
    std::vector<double> fillLepInfoML(Event* event);

    std::vector<double> fillJetInfoDL(Event* event);
    std::vector<double> fillJetInfoML(Event* event);

    std::vector<double> fillAllHistsDL(Event* event);
    std::vector<double> fillAllHistsML(Event* event);


    std::vector<HistInfo>* allHists(std::string flag, bool multilep, bool fourLep=false);
    std::vector<HistInfo>* infoLean(std::string flag, bool multilep);

    std::vector<double> fillAllHists(bool multilep, EventFourT* selec, bool fourLep=false);
    std::vector<double> fillAllLean(bool multilep, EventFourT* selec);


}

#endif
