#ifndef fourtop_h
#define fourtop_h

#include "../../globalSettings.h"

#include <TFile.h>
#include <TMVA/Reader.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../TreeReader/interface/TreeReader.h"
#include "../../../Event/interface/Event.h"

#include "../../histogramSetups/histogramSetup.h"
#include "../../additionalTools/interface/histHelper.h"
#include "../../additionalTools/interface/calculators.h"

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

        // Is this used?
        std::vector<Sample>* sampleVec;
        int ttgOverlapCheck; // 0: neither, 1: ttbar, 2: ttgamma

        // General settings for analysis run


        // IO-management
        TFile* outfile;
        
        Event* currentEvent = nullptr;

        // BDT Variables
        Double_t n_jets_f, n_bjets_f, deltaRBjets, n_b_loose;
        Double_t n_b_tight, dRleps, highestTops, secHighestTop, aziAngle, ht;
        Double_t massToPt;
        Double_t min_dr_lep_b, sec_min_dr_lep_b;
        Double_t nrTightLeps;
        Double_t ptJetOne, ptJetFour, ptJetFive, ptJetSix;
        Double_t ptLepOne, ptLepTwo, ptLepThree;

        TMVA::Reader* bdt_DL, bdt_ML;

        TTree* outputTree;
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
