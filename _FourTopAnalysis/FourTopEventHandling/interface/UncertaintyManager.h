#ifndef UNCERTAINTYMANAGER_4T_H
#define UNCERTAINTYMANAGER_4T_H

#include <map>
#include <vector>
#include <memory>

#include <TH1.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"

#include "MVAHandler.h"
#include "Uncertainty.h"

#include "../../additionalTools/interface/HistogramManager.h"
//#include "../../additionalTools/interface/histHelper.h"

class UncertaintyWrapper {
    private:
        // how do we improve this one? 
        // map makes sense tho, other way around? But that just requires more maps
        // other possibilities: 
        // per channel an object keeping track -> this just splits up the variables
        // per uncertainty an object in a linked list, so going through uncertainties just lets these call the required changes
        // this requires a good implementation but might be the best option to do it.
        // per channel then one LL? This can then be put in a vector of LL headers per channel...
        // Yeah try something like that.
        // keep histinfo then somewhere centralized i guess...

        // Main question is then what to do to apply uncertainty
        // think about that... maybe a function in a case switch for each uncertainty... each requires their own implementation so
        // this function is then applied by the uncertainty object i guess.
        // OR a base object for the LL with a dedicated subclass which is actually used in the linking, but this might get complicated fast
        
        std::map<shapeUncId, Uncertainty*> uncHistMap;
        std::map<shapeUncId, std::string> translateUnc = { {muonIDSys, "muonIDSyst"}, {muonIDStat, "muonIDStat"}, {EleIDSys, "electronIDSyst"}, {EleIDStat, "electronIDStat"},
                                                           {pileup, "pileup"}, {electronReco, "electronReco"}, {isrShape, "isrShape"}, {fsrShape, "fsrShape"}, {isrNorm, "isrNorm"}, 
                                                           {fsrNorm, "fsrNorm"} };

    public:
        UncertaintyWrapper(HistogramManager* histograms);

        Uncertainty* getUncertainty(shapeUncId id) {return uncHistMap[id];}
        std::map<shapeUncId, std::string> getTranslateUncMap() {return translateUnc;}

        void fillUncertainty(shapeUncId id, std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt);
        void fillSingleHistograms(shapeUncId id, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt);
        void fill2DHistograms(shapeUncId id, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt);

        void fillUpOrDownUncertainty(shapeUncId id, std::vector<double>& fillVec, double weightUp, bool up, bool nonPrompt);
        void fillUpOrDownSingleHistograms(shapeUncId id, std::vector<std::pair<int, double>>& fillVec, double weightUp, bool up, bool nonPrompt);
        void fillUpOrDown2DHistograms(shapeUncId id, std::vector<std::pair<double, double>>& fillVec, double weightUp, bool up, bool nonPrompt);

        void newSample(std::string& uniqueSampleName);
        void writeCurrentHistograms();
        void writeNonpromptHistograms();
        // all other required functions
};


#endif