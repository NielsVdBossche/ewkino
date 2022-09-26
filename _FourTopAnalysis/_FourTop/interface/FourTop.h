#ifndef fourtop_h
#define fourtop_h

#include "../../globalSettings.h"
#include <fstream>
#include <iostream>

#include <TFile.h>
#include <TMVA/Reader.h>

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "../../../Tools/interface/SampleCrossSections.h"
#include "../../../Tools/interface/analysisTools.h"
#include "../../../TreeReader/interface/TreeReader.h"
#include "../../../Event/interface/Event.h"

#include "../../histogramSetups/histogramSetup.h"
#include "../../additionalTools/interface/histHelper.h"
#include "../../additionalTools/interface/calculators.h"
#include "../../additionalTools/interface/HistogramManager.h"

#include "../../FourTopEventHandling/interface/EventFourT.h"
#include "../../FourTopEventHandling/interface/EventFourTLoose.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"
#include "../../HistogramHandling/interface/ChannelManager.h"


#include "../../../weights/interface/CombinedReweighter.h"
#include "../../../weights/interface/ConcreteReweighterFactory.h"
#include "../../../weights/interface/ReweighterBTagShape.h"
#include "../../SampleSF/interface/CombinedSampleReweighter.h"

class FourTop {
    private:
        TreeReader* treeReader;

        // Is this used?
        //std::vector<Sample>* sampleVec;
        int ttgOverlapCheck; // 0: neither, 1: ttbar, 2: ttgamma
        int zgOverlapCheck; // 0: neither, 1: DY, 2: ZG

        bool infuseNonPrompt = false; // Boolean to allow 1 loose lepton for ttbar
        bool leanEventSelection = true;
        bool onlyCR = false;
        bool bdtOutput = true;
        bool printEventTags = false;
        bool testRun = false;
        bool useUncertainties = true;
        std::string plotString = "Minimal";
        eventClass considerRegion = eventClass::fail;
        selectionType st = selectionType::MCAll;

        // General settings for analysis run
        std::string yearString = "Combi";
        std::string searchRegion = "All";
        // IO-management
        TFile* outfile;

        Event* currentEvent = nullptr;
        EventFourT* selection;

        // BDT Variables
        Double_t n_mu, chargeSum;
        Double_t n_jets_f, n_bjets_f, deltaRBjets, n_b_loose, met;
        Double_t n_b_tight, dRleps, aziAngle, ht;
        Double_t massToPt;
        Double_t min_dr_lep_b, sec_min_dr_lep_b;
        Double_t bTagLead, bTagSub, bTagThird, bTagFourth;
        Double_t bTagPtLead, bTagPtSub, bTagPtThird, bTagPtFourth;
        Double_t massBestTop, massBestTopW, massSecTop, massSecTopW;
        Double_t m2ll, mtLeadLepMET, mtSubLeadLepMET;
        Double_t m2bb, m2lblb, mvaWeight, anWeight, mcWeight, nonWeight;

        Double_t ptJetOne, phiJetOne, etaJetOne, btagJetOne, 
                    ptJetTwo, phiJetTwo, etaJetTwo, btagJetTwo, 
                    ptJetThree, phiJetThree, etaJetThree, btagJetThree, 
                    ptJetFour, phiJetFour, etaJetFour, btagJetFour, ptJetFive, 
                    phiJetFive, etaJetFive, btagJetFive, ptJetSix, phiJetSix, 
                    etaJetSix, btagJetSix, ptJetSeven, phiJetSeven, etaJetSeven, 
                    btagJetSeven, ptJetEight, phiJetEight, etaJetEight, btagJetEight, 
                    ptJetNine, phiJetNine, etaJetNine, btagJetNine, ptJetTen, 
                    phiJetTen, etaJetTen, btagJetTen, ptLepOne, phiLepOne, 
                    etaLepOne, flavLepOne, ptLepTwo, phiLepTwo, etaLepTwo, flavLepTwo,                    
                    ptLepThree, phiLepThree, etaLepThree, flavLepThree;
                    
        MVAHandler_4T *mva_DL = nullptr, *mva_ML = nullptr;


        TH2D* MisIDRates = nullptr;
        TH2D* FakeRatesElectron = nullptr;
        TH2D* FakeRatesMuon = nullptr;

    public:
        // Loading settings for analysis, preparing trees, ...
        FourTop(std::string outputName, std::vector<std::string>& argvString, int mode = 0);
        ~FourTop();

        // Prepare
        void createMVAHandlers();
        void addBTaggingNormFactors(ReweighterBTagShape* reweighter, std::string dir);
        void generateBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normDirectory, std::string& var);

        // Event selection components
        void setYearString(std::string year) {yearString = year;}
        std::string getYearString() {return yearString;}

        // Main loop functions
        void analyze(std::string method);
        void testRuns();
        
        std::map<eventClass, int> FillHistogramManager(ChannelManager* mgrAll);
        ChannelManager* GenerateChannelManager();

        void cutFlow(std::string& sortingMode);
        void createMVATrainingSamples();

        void linkMVAVariables(TTree* tree, bool isML);
        void fillMVAVariables(bool isML);
        void fillMVAVariablesNormalized(bool is4L);

        bool eventPassesTriggers();
        bool FillRegion(eventClass nominalClass, selectionType st);

        void initDdChargeMisID(double* corr);
        void initFakerate();
        double ChmisIDWeight();
        double FakeRateWeight();

        std::vector<std::string> GetSubClasses(eventClass currClass);

        CombinedSampleReweighter* createSampleReweighter(std::string dir);
};


#endif
