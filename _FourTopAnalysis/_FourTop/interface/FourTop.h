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
#include "../../../TreeReader/interface/NanoReader.h"
#include "../../../Event/interface/Event.h"

#include "../../histogramSetups/histogramSetup.h"
#include "../../additionalTools/interface/histHelper.h"
#include "../../additionalTools/interface/calculators.h"
#include "../../additionalTools/interface/HistogramManager.h"

#include "../../FourTopEventHandling/interface/EventFourT.h"
#include "../../FourTopEventHandling/interface/EventFourTLoose.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"
#include "../../HistogramHandling/interface/ChannelManager.h"

#include "../../OutputTreeHandler/interface/OutputTree.h"

#include "../../../weights/interface/CombinedReweighter.h"
#include "../../../weights/interface/ConcreteReweighterFactory.h"
#include "../../../weights/interface/ReweighterBTagShape.h"
#include "../../SampleSF/interface/CombinedSampleReweighter.h"

class FourTop {
    private:
        BaseReader* treeReader;

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
        bool useNpNmDistributions = false;
        bool overarchClasses = false;

        std::string plotString = "Minimal";
        eventClass considerRegion = eventClass::fail;
        selectionType st = selectionType::MCAll;

        // General settings for analysis run
        std::string yearString = "Combi";
        std::string searchRegion = "All";
        // IO-management
        TFile* outfile = nullptr;
        std::string outputSubfolder;
        // metadata
        std::string outputFileTags;
        std::string branchString;
        std::string commithash;
        std::string timestampExport;
        std::string anTypeStr = "";
        TH1F* intLuminosityMC;


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

        // Matrix of EFT
        std::vector<std::vector<double>>* inverted_matrix;
        std::map<int, std::vector<int>>* combinations;
        std::vector<std::vector<double>>* param_points;
    public:
        // Loading settings for analysis, preparing trees, ...
        FourTop(std::vector<std::string>& argvString, int mode = 0, bool produceFile = true);
        ~FourTop();

        // Prepare
        void createMVAHandlers();
        void addBTaggingNormFactors(ReweighterBTagShape* reweighter, std::string dir);
        //void generateBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normFilePath, std::string& var, bool jec, bool up);
        void generateAllBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normFilePath, std::vector<std::string>& variations, bool jec);

        // Event selection components
        void setYearString(std::string year) {yearString = year;}
        std::string getYearString() {return yearString;}

        // Main loop functions
        void analyze(std::string method);
        void analyzeToTree(std::string method, std::string uncertaintyflag);
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
        double FakeRateWeightVariation(bool isUp, bool electrons);

        std::vector<std::string> GetSubClasses(eventClass currClass);

        CombinedSampleReweighter* createSampleReweighter(std::string dir);

        std::vector<std::pair<int, double>> FillNpNmDistributions(eventClass currentClass, std::map<eventClass,int>& offsets);

        // matrix calculation + inversion for storage
        // 
        void generateMatrix();
        std::vector<double> transformWeights(unsigned nWeights, double* eftWeights);
        double calcEntry(unsigned nWeights, double* eftWeights, std::vector<int>& combination);

        void WriteMetadata(TFile* file);
        std::string GetOutputfileTags() {return outputFileTags;};
};


#endif
