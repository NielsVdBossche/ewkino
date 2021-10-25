#ifndef UNCERTAINTY_4T_H
#define UNCERTAINTY_4T_H

#include <map>
#include <vector>
#include <memory>

#include <TH1.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"

#include "../../FourTopEventHandling/interface/EventSelection4T.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"

#include "../../../TreeReader/interface/TreeReader.h"

// Shape uncertainties
// JEC and JER are year dependant
enum shapeUncertaintyIdentifier {
    jecDown,
    jecUp,
    jerDown,
    jerUp
};

// potential things to add: btag, met, leptons, scale, pileup, prefire, 

// Next should be flat uncertainty (map or somthing)

// Lastly something to keep track of correlations

// Functions to build datacards as well

class UncertaintyWrapper {
    private:
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

        TreeReader* treereader;
        EventSelection4T* selection;
    public:
        UncertaintyWrapper(EventSelection4T* selection, TreeReader* reader);

        void initDL(std::vector<HistInfo>* dlInfo);
        void initML(std::vector<HistInfo>* mlInfo);
        void initCRZ(std::vector<HistInfo>* crzInfo);
        void initCRW(std::vector<HistInfo>* crwInfo);
        void initCRO(std::vector<HistInfo>* croInfo);

        std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* uncertaintyHistogramInit(std::vector<HistInfo>* info);

        // all other required functions
};


#endif