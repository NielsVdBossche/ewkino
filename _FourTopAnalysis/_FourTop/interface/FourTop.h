#ifndef fourtop_h
#define fourtop_h

#include "../../globalSettings.h"

#include <TFile.h>
#include <TMVA/Reader.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "../../../TreeReader/interface/TreeReader.h"
#include "../../../Event/interface/Event.h"

#include "../../histogramSetups/histogramSetup.h"
#include "../../additionalTools/interface/histHelper.h"
#include "../../additionalTools/interface/calculators.h"
#include "../../additionalTools/interface/HistogramManager.h"

#include "../../FourTopEventHandling/interface/EventFourT.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"

class FourTop {
    private:
        std::vector<HistInfo>* histInfoVec_DL = nullptr; // Histogram for DL kinematics
        std::vector<HistInfo>* histInfoVec_3L = nullptr; // Histogram for ML kinematics
        std::vector<HistInfo>* histInfoVec_4L = nullptr; 
        std::vector<HistInfo>* histInfoVec_CRZ = nullptr;
        std::vector<HistInfo>* histInfoVec_CRW = nullptr;
        std::vector<HistInfo>* histInfoVec_Other = nullptr;
        std::vector<HistInfo>* histInfoVec_mva_DL = nullptr;
        std::vector<HistInfo>* histInfoVec_mva_3L = nullptr;
        std::vector<HistInfo>* histInfoVec_mva_4L = nullptr;

        std::vector<HistInfo_2D>* histInfoVec2D_mva_DL = nullptr;
        std::vector<HistInfo_2D>* histInfoVec2D_mva_3L = nullptr;
        std::vector<HistInfo_2D>* histInfoVec2D_mva_4L = nullptr;

        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_DL;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_3L;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_4L;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_CRZ;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_CRW;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_Other;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_mva_DL;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_mva_3L;
        std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_mva_4L;

        std::vector< std::vector<std::shared_ptr<TH2D>>>* hists2D_mva_DL;
        std::vector< std::vector<std::shared_ptr<TH2D>>>* hists2D_mva_3L;
        std::vector< std::vector<std::shared_ptr<TH2D>>>* hists2D_mva_4L;

        TreeReader* treeReader;

        // Is this used?
        //std::vector<Sample>* sampleVec;
        int ttgOverlapCheck; // 0: neither, 1: ttbar, 2: ttgamma
        bool infuseNonPrompt = false; // Boolean to allow 1 loose lepton for ttbar 

        // General settings for analysis run


        // IO-management
        TFile* outfile;
        
        Event* currentEvent = nullptr;
        EventFourT* selection;

        // BDT Variables
        Double_t n_jets_f, n_bjets_f, deltaRBjets, n_b_loose;
        Double_t n_b_tight, dRleps, aziAngle, ht;
        Double_t massToPt;
        Double_t min_dr_lep_b, sec_min_dr_lep_b;
        Double_t ptJetOne, ptJetFour, ptJetFive, ptJetSix;
        Double_t ptLepOne, ptLepTwo, ptLepThree;
        Double_t bTagLead, bTagSub, bTagThird, bTagFourth;
        Double_t ptJetTwo, ptJetThree, ptLepFour;
        MVAHandler_4T *mva_DL = nullptr, *mva_ML = nullptr;
    public:
        // Loading settings for analysis, preparing trees, ...
        FourTop(std::vector<std::string>& argvString, int mode = 0);
        ~FourTop();

        // Prepare 
        void createHistInfoVec();
        void createMVAHandlers();

        // Event selection components
        
        // Main loop functions
        void analyze();
        void analyzeLean();
        void createMVATrainingSamples();

        void linkMVAVariables(TTree* tree, bool isML);
        void fillMVAVariables(bool isML);

        bool eventPassesTriggers();
};


#endif
