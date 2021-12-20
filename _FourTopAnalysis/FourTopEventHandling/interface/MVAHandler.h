#ifndef MVAHANDLER_4T_H
#define MVAHANDLER_4T_H

#include "EventFourT.h"
#include "../../additionalTools/interface/calculators.h"

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/HistInfo2D.h"
#include "../../../Tools/interface/histogramTools.h"
#include "../../../Tools/interface/mt2.h"


#include <utility>
#include <memory>
#include <TMVA/Reader.h>

#include <TH1.h>

enum MVAConfigs {
    Binary_DL,
    Binary_ML,
    TriClass_DL,
    TriClass_ML,
    FourClass_DL,
    FourClass_ML
};

enum MVAClasses {
    TTBar,
    TTTT,
    TTW,
    TTZH
};

class MVAHandler_4T {
    private:
        TMVA::Reader* reader;

        MVAConfigs currentConfig;
        double isML = false;
        int maxClass = 1;

        EventFourT* selection;

        Float_t n_jets_f, n_bjets_f, deltaRBjets, n_b_loose;
        Float_t n_b_tight, dRleps, aziAngle, ht;
        Float_t massToPt;
        Float_t min_dr_lep_b, sec_min_dr_lep_b;
        Float_t ptJetOne, ptJetFour, ptJetFive, ptJetSix;
        Float_t ptLepOne, ptLepTwo, ptLepThree;
        Float_t bTagLead, bTagSub, bTagThird, bTagFourth;
        Float_t bTagPtLead, bTagPtSub, bTagPtThird, bTagPtFourth;
        Float_t ptJetTwo, ptJetThree, ptLepFour;
        Float_t massBestTop, massBestTopW, massSecTop, massSecTopW;
        Float_t m2ll, m2bb, m2lblb, mtLeadLepMET, mtSubLeadLepMET;

        std::vector<double> scoresCurrent;

    public:
        MVAHandler_4T(MVAConfigs config, EventFourT* selec);
        ~MVAHandler_4T();
        
        void initReader();
        std::vector<HistInfo>* createHistograms(std::string additionalFlag, bool fourLep = false);
        std::vector<HistInfo_2D>* create2DHistograms(std::string additionalFlag, bool fourLep = false);

        int getMaxClass() const {return maxClass;}

        //std::string histogramIdentifier();

        std::vector<double> scoreEvent();
        void fillVariables();
        void fillVector(std::vector<double>& fillVec);
        std::vector<std::pair<double, double>> fill2DVector();
        void fillHistograms(std::vector<std::shared_ptr<TH1D>>& histograms, double weight);
        void fill2DHistograms(std::vector<std::shared_ptr<TH2D>>& histograms, double weight);

        std::pair<MVAClasses, double> getClassAndScore();
};  

#endif