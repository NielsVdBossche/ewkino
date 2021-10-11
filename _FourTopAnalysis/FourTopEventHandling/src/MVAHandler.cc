#include "../interface/MVAHandler.h"
#include "../interface/EventSelection4T.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

MVAHandler_4T::MVAHandler_4T(MVAConfigs config, EventSelection4T* selec) : currentConfig(config), selection(selection) {
    initReader();
}

void MVAHandler_4T::initReader() {
    reader = new TMVA::Reader("!Color:!Silent");

    std::string weightFilePath = "../MVATraining/Classifiers/VeryEpicLoader/weights/";
    if (currentConfig == Binary_DL) {
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == Binary_ML) {
        isML = true;
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == TriClass_DL) {
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == TriClass_ML) {
        isML = true;
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == FourClass_DL) {
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == FourClass_ML) {
        isML = true;
        weightFilePath += "WEIGHTS";
    }

    reader->AddVariable("N_jets", &n_jets_f);
    reader->AddVariable("N_b", &n_bjets_f);
    reader->AddVariable("N_b_tight", &n_b_tight);
    reader->AddVariable("N_b_loose", &n_b_loose);
    reader->AddVariable("dr_bJets", &deltaRBjets);
    reader->AddVariable("dr_leps", &dRleps);
    reader->AddVariable("aziAngle", &aziAngle);
    reader->AddVariable("ht",  &ht);
    reader->AddVariable("mToPt", &massToPt);
    reader->AddVariable("min_dr_lep_b", &min_dr_lep_b);
    reader->AddVariable("sec_min_dr_lep_b", &sec_min_dr_lep_b);
    reader->AddVariable("pt_jet_one", &ptJetOne);
    reader->AddVariable("pt_jet_four", &ptJetFour);
    reader->AddVariable("pt_jet_five", &ptJetFive);
    reader->AddVariable("pt_jet_six", &ptJetSix);
    reader->AddVariable("pt_lep_one", &ptLepOne);
    reader->AddVariable("pt_lep_two", &ptLepTwo);

    if (isML) {
        reader->AddVariable("pt_lep_three", &ptLepThree);
    }

    reader->BookMVA("NAME", weightFilePath);
}

std::vector<Float_t> MVAHandler_4T::scoreEvent() {
    fillVariables();
    
    std::vector<Float_t> returnVec;

    if (currentConfig <= Binary_ML) {
        returnVec.push_back(reader->EvaluateMVA("NAME"));
    } else {
        returnVec = reader->EvaluateMulticlass("NAME");
    }

    return returnVec;
}

void MVAHandler_4T::fillVariables() {

    JetCollection* bJets = selection->getBtagJetCol();
    LeptonCollection* lightLeps = selection->getMediumLepCol();

    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *lightLeps);

    n_jets_f         =  selection->getJetCol()->size();
    n_bjets_f        =  bJets->size();
    deltaRBjets      =  mindR_Bjets[0];
    n_b_loose        =  selection->numberOfLooseBJets();
    n_b_tight        =  selection->numberOfTightBJets();
    dRleps           =  deltaR(*lightLeps->at(0), *lightLeps->at(1));
    aziAngle         =  deltaPhi(*lightLeps->at(0), *lightLeps->at(1));
    ht               =  selection->getJetCol()->scalarPtSum();

    JetCollection* jetCol = selection->getJetCol();
    massToPt = 0.;
    for (JetCollection::iterator jetIt = jetCol->begin(); jetIt != jetCol->end(); jetIt++) {
        double mass = (*jetIt)->mass();

        if ((*jetIt)->pt() == 0) {
            std::cout << "DIV BY ZERO AVOIDED" << std::endl;
            continue;
        }

        if (mass / (*jetIt)->pt() > massToPt) {
            massToPt = mass / (*jetIt)->pt();
        }
    }

    //massToPt         =  currentEvent;
    min_dr_lep_b     =  mindR_Bjet_lep[0];
    sec_min_dr_lep_b =  mindR_Bjet_lep[1];

    jetCol->sortByPt();
    ptJetOne         =  jetCol->at(0)->pt();
    ptJetFour        =  jetCol->at(3)->pt();
    ptJetFive        =  (n_jets_f >= 5 ? jetCol->at(4)->pt() : 0.);
    ptJetSix         =  (n_jets_f >= 6 ? jetCol->at(5)->pt() : 0.);

    lightLeps->sortByPt();
    ptLepOne         =  lightLeps->at(0)->pt();
    ptLepTwo         =  lightLeps->at(1)->pt();

    if (isML) {
        ptLepThree   =  lightLeps->at(2)->pt();
    }
}
