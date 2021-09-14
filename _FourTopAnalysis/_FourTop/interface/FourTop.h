#ifndef fourtop_h
#define fourtop_h

#include "../../globalSettings.h"

#include <TFile.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../TreeReader/interface/TreeReader.h"
#include "../../../Event/interface/Event.h"

#include "../../histogramSetups/histogramSetup.h"
#include "../../additionalTools/interface/histHelper.h"

#include "../../FourTopSelection/interface/EventSelection4T.h"


class FourTop {
    private:
        std::vector<HistInfo>* histInfoVec_Global; // Histograms shared between DL and ML (if any?)
        std::vector<HistInfo>* histInfoVec_DL; // Histogram for DL kinematics
        std::vector<HistInfo>* histInfoVec_ML; // Histogram for ML kinematics
        std::vector<HistInfo>* histInfoVec_CRZ;
        std::vector<HistInfo>* histInfoVec_CRW;

        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_DL;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_ML;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_CRZ;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_CRW;

        TreeReader* treeReader;
        std::vector<Sample>* sampleVec;

        // General settings for analysis run


        // IO-management
        TFile* outfile;
        
        Event* currentEvent = nullptr;
    public:
        // Loading settings for analysis, preparing trees, ...
        FourTop(std::vector< std::string > argvString);
        ~FourTop();

        // Prepare 
        void createHistInfoVec();

        // Event selection components
        
        // Main loop function
        void analyze();

        void crzHandling(size_t sampleIndex);
        void crwHandling(size_t sampleIndex);
};


#endif
