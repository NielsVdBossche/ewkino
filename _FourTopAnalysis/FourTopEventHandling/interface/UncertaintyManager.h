#ifndef UNCERTAINTY_4T_H
#define UNCERTAINTY_4T_H

#include <map>
#include <vector>
#include <memory>

#include <TH1.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"

#include "../../FourTopEventHandling/interface/EventFourT.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"
#include "../../additionalTools/interface/histHelper.h"

#include "../../../TreeReader/interface/TreeReader.h"

// Shape uncertainties
// JEC and JER are year dependant
enum shapeUncertaintyIdentifier {
    jecDown,
    jecUp,
    jerDown,
    jerUp,
    end  /// always at the end for easier loops
};

// potential things to add: btag, met, leptons, scale, pileup, prefire, 

// Next should be flat uncertainty (map or somthing)

// Lastly something to keep track of correlations

// Functions to build datacards as well

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
        
        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncDown_DL;
        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncUp_DL;

        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncDown_ML;
        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncUp_ML;

        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncDown_CRZ;
        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncUp_CRZ;

        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncDown_CRW;
        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncUp_CRW;

        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncDown_CRO;
        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* histogramsUncUp_CRO;

        std::vector<HistInfo>* histogramsUnc_info_DL;

        std::vector<HistInfo>* histogramsUnc_info_ML;

        std::vector<HistInfo>* histogramsUnc_info_CRZ;

        std::vector<HistInfo>* histogramsUnc_info_CRW;

        std::vector<HistInfo>* histogramsUnc_info_CRO;

        EventFourT* selection;
        TreeReader* treeReader;
    public:
        UncertaintyWrapper(EventFourT* selection, TreeReader* reader);

        void initDL(std::vector<HistInfo>* dlInfo);
        void initML(std::vector<HistInfo>* mlInfo);
        void initCRZ(std::vector<HistInfo>* crzInfo);
        void initCRW(std::vector<HistInfo>* crwInfo);
        void initCRO(std::vector<HistInfo>* croInfo);

        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* uncertaintyHistogramInit(std::vector<HistInfo>* info, bool up);

        // all other required functions
};


#endif