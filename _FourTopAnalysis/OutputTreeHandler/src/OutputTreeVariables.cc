#include "../interface/OutputTreeVariables.h"

#include "../../../Tools/interface/stringTools.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"
#include "../../additionalTools/interface/variables.h"

OutputTreeVariables::OutputTreeVariables(TFile* outputfile, std::string& treeName, std::string& outputLevel) : 
    OutputTree(outputfile, treeName), outputLevel(outputLevel)
{
    outputInt = 0;
    if (stringTools::stringContains(outputLevel, "fullJets")) outputInt += 1;

    // init tree
    pInitTree();

}

OutputTreeVariables::~OutputTreeVariables() {
}

void OutputTreeVariables::FillTree(EventFourT* ftEvent, double weight) {
    FillBaseTree(weight, ftEvent);
}

void OutputTreeVariables::FillBaseTree(double weight, EventFourT* ftEvent) {
    // fill base variables
    // makes sense to do this here
    
    // nominalWeight = weight;

    // Base variables
    nominalWeight     = weight;
    //noSFWeight        = ftEvent->getEvent()->weight();
    eventNb           = ftEvent->getEvent()->eventTags().eventNumber();
    
    nJets             = ftEvent->numberOfJets();//ftEvent->numberOfJets();
    jetPt             = {}; // only tight jets
    bTagWP            = {}; // 0=no, 1=loose, 2=med, 3=tight?
    for (unsigned i=0; i < nJets; i++) {
        Jet* jet = ftEvent->getJet(i);
        jetPt.push_back(jet->pt());
        if (jet->isBTaggedTight()) bTagWP.push_back(3);
        else if (jet->isBTaggedMedium()) bTagWP.push_back(2);
        else if (jet->isBTaggedLoose()) bTagWP.push_back(1);
        else bTagWP.push_back(0);
    }

    // potentially change this to just have the nBTight/med/loose? Rather than wp based, then again count 3/2/1/0 is also possible
    nElectrons        = 0; // potentially in 1 variable?
    electronPt        = {};
    electronCharge    = {};

    nMuons            = 0;
    muonPt            = {};
    muonCharge        = {};
    // isElectron        = {};
    for (unsigned i=0; i < ftEvent->numberOfLeps(); i++) {
        Lepton* lepton = ftEvent->getLepton(i);
        if (lepton->isElectron()) {
            nElectrons++;
            electronPt.push_back(lepton->pt());
            electronCharge.push_back(lepton->charge());
        } else if (lepton->isMuon()) {
            nMuons++;
            muonPt.push_back(lepton->pt());
            muonCharge.push_back(lepton->charge());
        }
    }

    ptMiss            = ftEvent->getMET();

    fourTopScore      = ftEvent->getMVAScores()[1];
    ttWScore          = ftEvent->getMVAScores()[2];
    ttbarScore        = ftEvent->getMVAScores()[0];
    // std::cout << ftEvent->getCurrentClass() << std::endl;
    eventClass        = ftEvent->getCurrentClass();

    if (ftEvent->getEvent()->hasOSSFLightLeptonPair()) invariant_mass_ll = ftEvent->getEvent()->bestZBosonCandidateMass();   
    else invariant_mass_ll = 0.;

    MVAHandler_4T* mva = ftEvent->GetDLMVA();
    unsigned evClass =unsigned(ftEvent->getCurrentClass());
    if (evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass > eventClass::ssdl) mva = ftEvent->GetMLMVA();

    // bdt vars
    min_dR_bb       = mva->deltaRBjets;
    dR_l1l2         = mva->dRleps;
    dPhi_l1l2       = mva->aziAngle;
    dEta_l1l2       = deltaEta(*ftEvent->getLepton(0), *ftEvent->getLepton(1));
    min_dR_ll       = 0.;
    min_dPhi_ll     = 0.;
    min_dEta_ll     = 0.;
    max_mOverPt     = mva->massToPt;
    min_dR_lepB     = mva->min_dr_lep_b;
    minTwo_dR_lepB  = mva->sec_min_dr_lep_b;
    max_DF_1        = mva->bTagLead;
    max_DF_2        = mva->bTagSub;
    max_DF_3        = mva->bTagThird;
    max_DF_4        = mva->bTagFourth;
    df_j1           = mva->bTagPtLead;
    df_j2           = mva->bTagPtSub;
    df_j3           = mva->bTagPtThird;
    df_j4           = mva->bTagPtFourth;
    mtop1           = mva->massBestTop;
    mtop2           = mva->massBestTopW;
    mW1             = mva->massSecTop;
    mW2             = mva->massSecTopW;
    mt_l1           = mva->mtLeadLepMET;
    mt_l2           = mva->mtSubLeadLepMET;
    mt2_ll          = mva->m2ll;
    mt2_bb          = mva->m2bb;
    mt2_lblb        = mva->m2lblb;
    m_l1l2          = (*ftEvent->getLepton(0) + *ftEvent->getLepton(1)).mass();
    if (nElectrons + nMuons >= 3) {
        m_l2l3          = (*ftEvent->getLepton(1) + *ftEvent->getLepton(2)).mass();
        m_l1l3          = (*ftEvent->getLepton(0) + *ftEvent->getLepton(2)).mass();
    } else {
        m_l2l3 = 0.;
        m_l1l3 = 0.;
    }
    maxPtLepJet = variables::MaxPtAllDiobjectSystems(*ftEvent->getMediumLepCol(), *ftEvent->getJetCol());
}

void OutputTreeVariables::pInitTree() {
    // get tree
    std::cout << "init tree" << std::endl;
    std::shared_ptr<TTree> tree = GetTree();

    tree->Branch("nominalWeight",  &nominalWeight,           "nominalWeight/D");
    //tree->Branch("noSFWeight",     &noSFWeight,              "noSFWeight/D");
    tree->Branch("eventNb",        &eventNb,                 "eventNb/l");
    //tree->Branch("noSFWeight",     &noSFWeight,              "noSFWeight/D");
    
    tree->Branch("nJets",          &nJets,                   "nJets/i");
    tree->Branch("jetPt",          &jetPt);
    tree->Branch("bTagWP",         &bTagWP);
    tree->Branch("nElectrons",     &nElectrons,              "nElectrons/i");
    tree->Branch("electronPt",     &electronPt);
    //tree->Branch("isElectron",     &isElectron);
    tree->Branch("electronCharge", &electronCharge);
    tree->Branch("muonCharge",     &muonCharge);
    tree->Branch("nMuons",         &nMuons,                  "nMuons/i");
    tree->Branch("muonPt",         &muonPt);
    tree->Branch("ptMiss",         &ptMiss,                  "ptMiss/D");
    tree->Branch("fourTopScore",   &fourTopScore,            "fourTopScore/D");
    tree->Branch("ttWScore",       &ttWScore,                "ttWScore/D");
    tree->Branch("ttbarScore",     &ttbarScore,              "ttbarScore/D");
    tree->Branch("eventClass",     &eventClass,              "eventClass/i");
    tree->Branch("min_dR_bb",      &min_dR_bb,               "min_dR_bb/D");
    tree->Branch("dR_l1l2",        &dR_l1l2,                 "dR_l1l2/D");
    tree->Branch("dPhi_l1l2",      &dPhi_l1l2,               "dPhi_l1l2/D");
    tree->Branch("dEta_l1l2",      &dEta_l1l2,               "dEta_l1l2/D");
    tree->Branch("min_dR_ll",      &min_dR_ll,               "min_dR_ll/D");
    tree->Branch("min_dPhi_ll",    &min_dPhi_ll,             "min_dPhi_ll/D");
    tree->Branch("min_dEta_ll",    &min_dEta_ll,             "min_dEta_ll/D");
    tree->Branch("max_mOverPt",    &max_mOverPt,             "max_mOverPt/D");
    tree->Branch("min_dR_lepB",    &min_dR_lepB,             "min_dR_lepB/D");
    tree->Branch("minTwo_dR_lepB", &minTwo_dR_lepB,          "minTwo_dR_lepB/D");
    tree->Branch("max_DF_1",       &max_DF_1,                "max_DF_1/D");
    tree->Branch("max_DF_2",       &max_DF_2,                "max_DF_2/D");
    tree->Branch("max_DF_3",       &max_DF_3,                "max_DF_3/D");
    tree->Branch("max_DF_4",       &max_DF_4,                "max_DF_4/D");
    tree->Branch("df_j1",          &df_j1,                   "df_j1/D");
    tree->Branch("df_j2",          &df_j2,                   "df_j2/D");
    tree->Branch("df_j3",          &df_j3,                   "df_j3/D");
    tree->Branch("df_j4",          &df_j4,                   "df_j4/D");
    tree->Branch("mtop1",          &mtop1,                   "mtop1/D");
    tree->Branch("mtop2",          &mtop2,                   "mtop2/D");
    tree->Branch("mW1",            &mW1,                     "mW1/D");
    tree->Branch("mW2",            &mW2,                     "mW2/D");
    tree->Branch("mt_l1",          &mt_l1,                   "mt_l1/D");
    tree->Branch("mt_l2",          &mt_l2,                   "mt_l2/D");
    tree->Branch("mt2_ll",         &mt2_ll,                  "mt2_ll/D");
    tree->Branch("mt2_bb",         &mt2_bb,                  "mt2_bb/D");
    tree->Branch("mt2_lblb",       &mt2_lblb,                "mt2_lblb/D");
    tree->Branch("m_l1l2",         &m_l1l2,                  "m_l1l2/D");
    tree->Branch("m_l2l3",         &m_l2l3,                  "m_l2l3/D");
    tree->Branch("m_l1l3",         &m_l1l3,                  "m_l1l3/D");
    tree->Branch("maxPtLepJet",    &maxPtLepJet,                  "maxPtLepJet/D");
}

