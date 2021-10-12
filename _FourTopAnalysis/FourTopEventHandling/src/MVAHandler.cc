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
        weightFilePath += "FourTopClassification_BDTG_DL.weights.xml";
    } else if (currentConfig == TriClass_ML) {
        isML = true;
        weightFilePath += "FourTopClassification_BDTG_ML.weights.xml";
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

    createHistograms();
}

std::vector<HistInfo> MVAHandler_4T::createHistograms() {
    std::string identifier = "";

    std::vector<HistInfo> histInfoVec;

    if (currentConfig < 2) {
        identifier += "_Binary";
    } else if (currentConfig < 4) {
        identifier += "_TriClass";
        maxClass = 3;
    } else if (currentConfig < 6) {
        identifier += "_FourClass";
        maxClass = 4;
    }

    if (isML) {
        identifier += "_ML";
    } else {
        identifier += "_DL";
    }

    std::map<MVAClasses, std::string> translator = {
        {TTTT, "Signal"},
        {TTBar, "TTBar"},
        {TTW, "TTW"},
        {TTZH, "TTZH"}
    }; // Review names when appropriate

    for (int el = 0; el < maxClass; el++) {
        std::string name = "BDTScore_" + translator[(MVAClasses) el] + identifier;

        histInfoVec.push_back(HistInfo(name, "BDT score", 20, -1., 1.));
    }

    for (int el = 0; el < maxClass; el++) {
        std::string name = "BDT_Finalresult" + translator[(MVAClasses) el] + identifier;

        histInfoVec.push_back(HistInfo(name, "BDT score", 20, -1., 1.));
    }

    return histInfoVec;
}

void MVAHandler_4T::fillHistograms(std::vector<std::shared_ptr<TH1D>>& histograms, double weight) {
    std::vector<Float_t> scores = scoreEvent();

    for (int i=0; i < maxClass; i++) {
        histogram::fillValue(histograms[i].get(), scores[i], weight);
    }

    std::pair<MVAClasses, Float_t> classific = getClassAndScore();

    histogram::fillValue(histograms[classific.first + maxClass].get(), classific.second, weight);
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

std::pair<MVAClasses, Float_t> MVAHandler_4T::getClassAndScore() {
    std::vector<Float_t> scores = scoreEvent();

    if (scores.size() == 1) {
        return {TTTT, scores[0]};
    }

    int indexMaxScore = std::max_element(scores.begin(),scores.end()) - scores.begin();

    return {(MVAClasses) indexMaxScore, scores[indexMaxScore]};
}
