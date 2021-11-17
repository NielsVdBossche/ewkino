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
        Double_t bTagPtLead, bTagPtSub, bTagPtThird, bTagPtFourth;
        Double_t ptJetTwo, ptJetThree, ptLepFour;
        Double_t massBestTop, massBestTopW, massSecTop, massSecTopW;
        Double_t m2ll, mtLeadLepMET, mtSubLeadLepMET;
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
