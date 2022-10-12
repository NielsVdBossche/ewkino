#include "../interface/MVAHandler.h"
#include "../interface/EventFourT.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h"
#endif

MVAHandler_4T::MVAHandler_4T(MVAConfigs config, EventFourT* selec, bool isLean) : currentConfig(config), selection(selec), lean(isLean) {
    initReader();
}

void MVAHandler_4T::initReader() {
    reader = new TMVA::Reader("!Color:!Silent");

    std::string weightFilePath = "../MVATraining/VeryEpicLoader/weights/";
    if (currentConfig == Binary_DL) {
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == Binary_ML) {
        isML = true;
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == TriClass_DL) {
        if (lean) {
            //weightFilePath += "FourTopClassification_OrigSel_TEST__DL_BDTG_B_1000_3_20_0.1_5_0.6.weights.xml";
            weightFilePath += "FourTopClassification_LeanSel_DL_BDTG_B_2000_6_20_0.1_1_1.weights.xml";
            //weightFilePath += "FourTopClassification_UL_2022-10-10_09-45__BDT_VAR_DLG_B_1500_3_20_0.05_1_0.5.weights.xml";
        } else {
            weightFilePath += "FourTopClassification__OrigSel_DL_BDTG_B_1000_3_0.1_20.weights.xml";
        }
    } else if (currentConfig == TriClass_ML) {
        if (lean) {
            //weightFilePath += "FourTopClassification_OrigSel_TEST__ML_BDTG_B_1000_3_20_0.1_5_0.6.weights.xml";
            weightFilePath += "FourTopClassification_LeanSel_ML_BDTG_B_1500_5_20_0.08_1_0.6.weights.xml";
            //weightFilePath += "FourTopClassification_UL_2022-10-10_13-55__BDT_VAR_MLG_B_1250_3_20_0.05_1_0.25.weights.xml";
        } else {
            weightFilePath += "FourTopClassification__OrigSel_ML_BDTG_B_1000_3_0.1_20.weights.xml";
        }
        isML = true;
    } else if (currentConfig == FourClass_DL) {
        weightFilePath += "WEIGHTS";
    } else if (currentConfig == FourClass_ML) {
        isML = true;
        weightFilePath += "WEIGHTS";
    }
    /*
    if (isML) {
        reader->AddVariable("dr_bJets", &deltaRBjets);
        reader->AddVariable("dr_leps", &dRleps);
        reader->AddVariable("ht",  &ht);
        reader->AddVariable("mToPt", &massToPt);
        reader->AddVariable("pt_jet_one", &ptJetOne);
        reader->AddVariable("pt_jet_two", &ptJetTwo);
        reader->AddVariable("pt_jet_three", &ptJetThree);
        reader->AddVariable("pt_jet_five", &ptJetFive);
        reader->AddVariable("pt_lep_two", &ptLepTwo);
        reader->AddVariable("pt_lep_three", &ptLepThree);

        reader->AddVariable("bTagSub",         &bTagSub);
        reader->AddVariable("bTagThird",       &bTagThird);
        reader->AddVariable("bTagPtLead",      &bTagPtLead);
        reader->AddVariable("bTagPtSub",       &bTagPtSub);
        reader->AddVariable("massBestTop",     &massBestTop);
        reader->AddVariable("massBestTopW",    &massBestTopW);
        reader->AddVariable("MET",             &met);
    } else {
        reader->AddVariable("N_jets", &n_jets_f);
        reader->AddVariable("dr_bJets", &deltaRBjets);
        reader->AddVariable("dr_leps", &dRleps);
        reader->AddVariable("ht",  &ht);
        reader->AddVariable("mToPt", &massToPt);
        reader->AddVariable("pt_jet_four", &ptJetFour);
        reader->AddVariable("pt_jet_five", &ptJetFive);
        reader->AddVariable("pt_lep_two", &ptLepTwo);
        reader->AddVariable("bTagSub",         &bTagSub);
        reader->AddVariable("bTagThird",       &bTagThird);
        reader->AddVariable("bTagPtLead",      &bTagPtLead);
        reader->AddVariable("massBestTop",     &massBestTop);
        reader->AddVariable("massBestTopW",    &massBestTopW);
        reader->AddVariable("MET",             &met);
    }*/
    

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

    reader->AddVariable("bTagLead",        &bTagLead);
    reader->AddVariable("bTagSub",         &bTagSub);
    reader->AddVariable("bTagThird",       &bTagThird);
    reader->AddVariable("bTagFourth",      &bTagFourth);
    reader->AddVariable("bTagPtLead",      &bTagPtLead);
    reader->AddVariable("bTagPtSub",       &bTagPtSub);
    reader->AddVariable("bTagPtThird",     &bTagPtThird);
    reader->AddVariable("bTagPtFourth",    &bTagPtFourth);
    reader->AddVariable("m2ll",            &m2ll);
    reader->AddVariable("m2bb",            &m2bb);
    reader->AddVariable("m2lblb",          &m2lblb);
    reader->AddVariable("mtLeadLepMET",    &mtLeadLepMET);
    reader->AddVariable("mtSubLeadLepMET", &mtSubLeadLepMET);
    reader->AddVariable("massBestTop",     &massBestTop);
    reader->AddVariable("massBestTopW",    &massBestTopW);
    reader->AddVariable("massSecTop",      &massSecTop);
    reader->AddVariable("massSecTopW",     &massSecTopW);
    reader->AddVariable("MET",             &met);
    
    if (!isML) {

    }

    if (isML) {
        reader->AddVariable("pt_lep_three", &ptLepThree);
    }

    reader->BookMVA("BDTCurr", weightFilePath);

    createHistograms("");
}

std::vector<HistInfo>* MVAHandler_4T::createHistograms(std::string additionalFlag, bool fourLep) {
    std::string identifier = "";

    std::vector<HistInfo>* histInfoVec = new std::vector<HistInfo>;

    if (currentConfig < 2) {
        identifier += "_Binary";
    } else if (currentConfig < 4) {
        identifier += "_TriClass";
        maxClass = 3;
    } else if (currentConfig < 6) {
        identifier += "_FourClass";
        maxClass = 4;
    }

    if (fourLep) {
        identifier += "_SR-4L";
    } else if (isML) {
        identifier += "_SR-3L";
    } else {
        identifier += "_SR-2L";
    }

    std::map<MVAClasses, std::string> translator = {
        {TTTT, "Signal"},
        {TTBar, "TTBar"},
        {TTW, "TTV"},
        {TTZH, "TTZH"}
    }; // Review names when appropriate

    for (int el = 0; el < maxClass; el++) {
        std::string name = "BDTScore_" + translator[(MVAClasses) el] + identifier + additionalFlag;
        std::string xaxis = "BDT score " + translator[(MVAClasses) el];

        histInfoVec->push_back(HistInfo(name, xaxis, 100, 0., 1.));
    }

    for (int el = 0; el < maxClass; el++) {
        std::string name = "BDT_Finalresult" + translator[(MVAClasses) el] + identifier + additionalFlag;
        std::string xaxis = "BDT score " + translator[(MVAClasses) el];

        histInfoVec->push_back(HistInfo(name, xaxis , 240, 0.4, 1.));
    }

    return histInfoVec;
}

std::vector<HistInfo_2D>* MVAHandler_4T::create2DHistograms(std::string additionalFlag, bool fourLep) {
    std::string identifier = "";

    std::vector<HistInfo_2D>* histInfoVec = new std::vector<HistInfo_2D>;

    if (currentConfig < 2) {
        identifier += "_Binary";
    } else if (currentConfig < 4) {
        identifier += "_TriClass";
    } else if (currentConfig < 6) {
        identifier += "_FourClass";
    }

    if (fourLep) {
        identifier += "_SR-4L";
    } else if (isML) {
        identifier += "_SR-3L";
    } else {
        identifier += "_SR-2L";
    }

    std::map<MVAClasses, std::string> translator = {
        {TTTT, "Signal"},
        {TTBar, "TTBar"},
        {TTW, "TTW"},
        {TTZH, "TTZH"}
    }; // Review names when appropriate

    for (int el = 0; el < maxClass; el++) {
        std::string name = "2D_BDTScore_" + translator[(MVAClasses) el] + "_" + translator[(MVAClasses) ((el + 1) % maxClass)] + identifier + additionalFlag;

        std::string xaxis = "BDT score " + translator[(MVAClasses) el];
        std::string yaxis = "BDT score " + translator[(MVAClasses) ((el + 1) % maxClass)];

        histInfoVec->push_back(HistInfo_2D(name, xaxis, 100, 0., 1., yaxis, 100, 0., 1.));
    }

    return histInfoVec;
}
/*
void MVAHandler_4T::fillVector(std::vector<Float_t>& scores, std::vector<double>& fillVec) {
    for (int i=0; i < maxClass; i++) {
        fillVec.push_back(scores[i]);
    }

    std::pair<MVAClasses, Float_t> classific = getClassAndScore(scores);

    histogram::fillValue(histograms[classific.first + maxClass].get(), classific.second, weight);
}
*/

std::vector<std::pair<double, double>> MVAHandler_4T::fill2DVector() {
    std::vector<std::pair<double, double>> fillVec;
    for (int i=0; i < maxClass; i++) {
        fillVec.push_back({scoresCurrent[i], scoresCurrent[(i + 1) % 3]});
    }

    return fillVec;
}

void MVAHandler_4T::fillHistograms(std::vector<std::shared_ptr<TH1D>>& histograms, double weight) {
    for (int i=0; i < maxClass; i++) {
        histogram::fillValue(histograms[i].get(), scoresCurrent[i], weight);
    }

    std::pair<MVAClasses, double> classific = getClassAndScore();

    histogram::fillValue(histograms[classific.first + maxClass].get(), classific.second, weight);
}

void MVAHandler_4T::fill2DHistograms(std::vector<std::shared_ptr<TH2D>>& histograms, double weight) {
    for (int i=0; i < maxClass; i++) {
        histogram::fillValues(histograms[i].get(), scoresCurrent[i], scoresCurrent[(i + 1) % 3], weight);
    }
}


std::vector<double> MVAHandler_4T::scoreEvent() {
    fillVariables();
    
    std::vector<double> returnVec;

    if (currentConfig <= Binary_ML) {
        returnVec.push_back(double(reader->EvaluateMVA("BDTCurr")));
    } else {
        std::vector<Float_t> returnVecTemp;
        returnVecTemp = reader->EvaluateMulticlass("BDTCurr");
        returnVec = std::vector<double>(returnVecTemp.begin(), returnVecTemp.end());
    }

    scoresCurrent = returnVec;

    return returnVec;
}

void MVAHandler_4T::fillVariables() {

    JetCollection* bJets = selection->getBtagJetCol();
    LeptonCollection* lightLeps;
    if (selection->isEventNormalSelected()) {
        lightLeps = selection->getMediumLepCol();
    } else {
        lightLeps = selection->getAltLeptonCol();
    }

    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *lightLeps);

    n_jets_f         =  selection->getJetCol()->size();
    n_bjets_f        =  bJets->size();
    deltaRBjets      =  (n_bjets_f >= 2. ? mindR_Bjets[0] : 5.);
    n_b_loose        =  selection->numberOfLooseBJets();
    n_b_tight        =  selection->numberOfTightBJets();
    dRleps           =  deltaR(*lightLeps->at(0), *lightLeps->at(1));
    aziAngle         =  deltaPhi(*lightLeps->at(0), *lightLeps->at(1));
    ht               =  selection->getJetCol()->scalarPtSum();
    met              =  selection->getMET();

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
    min_dr_lep_b     =  (mindR_Bjet_lep.size() > 0 ? mindR_Bjet_lep[0] : 5.);
    sec_min_dr_lep_b =  (mindR_Bjet_lep.size() > 0 ? mindR_Bjet_lep[1] : 5.);

    jetCol->sortByPt();
    ptJetOne         =  (n_jets_f >= 1 ? jetCol->at(0)->pt() : 0.);
    ptJetTwo         =  (n_jets_f >= 2 ? jetCol->at(1)->pt() : 0.);
    ptJetThree       =  (n_jets_f >= 3 ? jetCol->at(2)->pt() : 0.);
    ptJetFour        =  (n_jets_f >= 4 ? jetCol->at(3)->pt() : 0.);
    ptJetFive        =  (n_jets_f >= 5 ? jetCol->at(4)->pt() : 0.);
    ptJetSix         =  (n_jets_f >= 6 ? jetCol->at(5)->pt() : 0.);

    lightLeps->sortByPt();
    ptLepOne         =  lightLeps->at(0)->pt();
    ptLepTwo         =  lightLeps->at(1)->pt();

    if (isML) {
        ptLepThree   =  (selection->numberOfLeps() > 2 ? lightLeps->at(2)->pt() : 0.);
    }

    bTagPtLead = (jetCol->size() > 0 ? jetCol->at(0)->deepFlavor() : -1.);
    bTagPtSub = (jetCol->size() > 1 ? jetCol->at(1)->deepFlavor() : -1.);
    bTagPtThird = (jetCol->size() > 2 ? jetCol->at(2)->deepFlavor() : -1.);
    bTagPtFourth = (jetCol->size() > 3 ? jetCol->at(3)->deepFlavor() : -1.);
    
    jetCol->sortByAttribute([](const std::shared_ptr< Jet >& lhs, const std::shared_ptr< Jet >& rhs){ return lhs->deepFlavor() > rhs->deepFlavor(); } );

    bTagLead = jetCol->size() > 0 ? jetCol->at(0)->deepFlavor() : -1.;
    bTagSub = jetCol->size() > 1 ? jetCol->at(1)->deepFlavor() : -1.;
    bTagThird = jetCol->size() > 2 ? jetCol->at(2)->deepFlavor() : -1.;
    bTagFourth = jetCol->size() > 3 ? jetCol->at(3)->deepFlavor() : -1.;

    TopReconstructionNew* topReco = selection->getTopReco();

    topReco->RecoBestTwoTops();
    massBestTop = topReco->getBestRecoTop().first;
    massBestTopW = topReco->getBestRecoTop().second;
    massSecTop = topReco->getSecondBestRecoTop().first;
    massSecTopW = topReco->getSecondBestRecoTop().second;

    mtLeadLepMET = mt(*lightLeps->at(0), selection->getEvent()->met());
    mtSubLeadLepMET = mt(*lightLeps->at(1), selection->getEvent()->met());
    m2ll = mt2::mt2Alt(*lightLeps->at(0), *lightLeps->at(1), selection->getEvent()->met());

    m2bb = (n_bjets_f >= 2 ? mt2::mt2bb((*bJets)[0], (*bJets)[1], (*lightLeps)[0], (*lightLeps)[1], selection->getEvent()->met()) : -1);
    m2lblb = (n_bjets_f >= 2 ? mt2::mt2lblb((*bJets)[0], (*bJets)[1], (*lightLeps)[0], (*lightLeps)[1], selection->getEvent()->met()) : -1);
}

std::pair<MVAClasses, double> MVAHandler_4T::getClassAndScore() {
    if (maxClass == 1) {
        return {(MVAClasses) 0, scoresCurrent[0]};
    }

    int indexMaxScore = std::max_element(scoresCurrent.begin(),scoresCurrent.end()) - scoresCurrent.begin();

    return {(MVAClasses) indexMaxScore, scoresCurrent[indexMaxScore]};
}
