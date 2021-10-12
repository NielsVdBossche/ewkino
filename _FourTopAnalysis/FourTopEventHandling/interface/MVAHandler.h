#ifndef MVAHANDLER_4T_H
#define MVAHANDLER_4T_H

#include "EventSelection4T.h"
#include "../../additionalTools/interface/calculators.h"

#include "../../../Tools/interface/HistInfo.h"
#include "../../../Tools/interface/histogramTools.h"

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
    TTTT,
    TTBar,
    TTW,
    TTZH
};

class MVAHandler_4T {
    private:
        TMVA::Reader* reader;

        MVAConfigs currentConfig;
        double isML = false;
        int maxClass = 1;

        EventSelection4T* selection;

        Float_t n_jets_f, n_bjets_f, deltaRBjets, n_b_loose;
        Float_t n_b_tight, dRleps, aziAngle, ht;
        Float_t massToPt;
        Float_t min_dr_lep_b, sec_min_dr_lep_b;
        Float_t ptJetOne, ptJetFour, ptJetFive, ptJetSix;
        Float_t ptLepOne, ptLepTwo, ptLepThree;

    public:
        MVAHandler_4T(MVAConfigs config, EventSelection4T* selec);
        ~MVAHandler_4T();
        
        void initReader();
        std::vector<HistInfo>* createHistograms();
        //std::string histogramIdentifier();

        std::vector<Float_t> scoreEvent();
        void fillVariables();
        void fillHistograms(std::vector<std::shared_ptr<TH1D>>& histograms, double weight);

        std::pair<MVAClasses, Float_t> getClassAndScore();
};  

#endif