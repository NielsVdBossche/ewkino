#include "../interface/OutputTreeVariables.h"

#include "../../../Tools/interface/stringTools.h"

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
    bTagWP            = {0}; // 0=no, 1=loose, 2=med, 3=tight?
    for (int i=0; i<nJets; i++) {
        Jet* jet = ftEvent->getJet(i);
        jetPt.push_back(jet->pt());
        if (jet->isBTaggedTight()) bTagWP.push_back(3);
        else if (jet->isBTaggedMedium()) bTagWP.push_back(2);
        else if (jet->isBTaggedLoose()) bTagWP.push_back(1);
        else bTagWP.push_back(0);
    }

    // potentially change this to just have the nBTight/med/loose? Rather than wp based, then again count 3/2/1/0 is also possible

    nElectrons        = ftEvent->numberOfLeps(); // potentially in 1 variable?
    electronPt        = {};
    electronCharge    = {};
    isElectron        = {};
    for (int i=0; i<nElectrons; i++) {
        electronPt.push_back(ftEvent->getLepton(i)->pt());
        electronCharge.push_back(ftEvent->getLepton(i)->charge());
        isElectron.push_back(ftEvent->getLepton(i)->isElectron());
    }

    muonCharge        = {};
    nMuons            = 0;
    muonPt            = {};

    ptMiss            = ftEvent->getMET();

    fourTopScore      = ftEvent->getMVAScores()[1];
    ttWScore          = ftEvent->getMVAScores()[2];
    ttbarScore        = ftEvent->getMVAScores()[0];
    // std::cout << ftEvent->getCurrentClass() << std::endl;
    eventClass        = ftEvent->getCurrentClass();

    if (ftEvent->getEvent()->hasOSSFLightLeptonPair()) invariant_mass_ll = ftEvent->getEvent()->bestZBosonCandidateMass();   
    else invariant_mass_ll = 0.;

    // bdt vars
    min_dR_bb       = 0.;
    dR_l1l2         = 0.;
    dPhi_l1l2       = 0.;
    dEta_l1l2       = 0.;
    min_dR_ll       = 0.;
    min_dPhi_ll     = 0.;
    min_dEta_ll     = 0.;
    max_mOverPt     = 0.;
    min_dR_lepB     = 0.;
    minTwo_dR_lepB  = 0.;
    max_DF_1        = 0.;
    max_DF_2        = 0.;
    max_DF_3        = 0.;
    max_DF_4        = 0.;
    df_j1           = 0.;
    df_j2           = 0.;
    df_j3           = 0.;
    df_j4           = 0.;
    mtop1           = 0.;
    mtop2           = 0.;
    mW1             = 0.;
    mW2             = 0.;
    mt_l1           = 0.;
    mt_l2           = 0.;
    mt2_ll          = 0.;
    mt2_bb          = 0.;
    mt2_lblb        = 0.;
    m_l1l2          = 0.;
    m_l2l3          = 0.;
    m_l1l3          = 0.;
}

void OutputTreeVariables::pInitTree() {
    // get tree

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
    tree->Branch("isElectron",     &isElectron);
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
}

