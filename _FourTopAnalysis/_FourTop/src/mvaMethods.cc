#include "../interface/FourTop.h"

// other methods: load and build methods from files

void FourTop::fillMVAVariables(bool isML) {
    JetCollection* bJets = selection->getBtagJetCol();
    LightLeptonCollection* lightLeps; // = selection->getMediumLepCol();

    lightLeps = (LightLeptonCollection*) selection->getMediumLepCol();

    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *((LeptonCollection*)lightLeps));

    n_jets_f         =  selection->getJetCol()->size();
    n_bjets_f        =  bJets->size();
    deltaRBjets      =  (n_bjets_f >= 2. ? mindR_Bjets[0] : 5.);
    n_b_loose        =  selection->numberOfLooseBJets();
    n_b_tight        =  selection->numberOfTightBJets();
    dRleps           =  deltaR(*lightLeps->at(0), *lightLeps->at(1));
    aziAngle         =  deltaPhi(*lightLeps->at(0), *lightLeps->at(1));
    ht               =  selection->getJetCol()->scalarPtSum();
    met = selection->getMET();
    
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
    //ptJetOne         =  (n_jets_f >= 1 ? jetCol->at(0)->pt() : 0.);
    //ptJetTwo         =  (n_jets_f >= 2 ? jetCol->at(1)->pt() : 0.);
    //ptJetThree       =  (n_jets_f >= 3 ? jetCol->at(2)->pt() : 0.);
    //ptJetFour        =  (n_jets_f >= 4 ? jetCol->at(3)->pt() : 0.);
    //ptJetFive        =  (n_jets_f >= 5 ? jetCol->at(4)->pt() : 0.);
    //ptJetSix         =  (n_jets_f >= 6 ? jetCol->at(5)->pt() : 0.);

    ptJetOne = (n_jets_f >= 1 ? jetCol->at(0)->pt() : 0.);
    phiJetOne = (n_jets_f >= 1 ? jetCol->at(0)->phi() : 0.);
    etaJetOne = (n_jets_f >= 1 ? jetCol->at(0)->eta() : 0.);
    btagJetOne = (n_jets_f >= 1 ? jetCol->at(0)->deepFlavor() : 0.);

    ptJetTwo =  (n_jets_f >= 2 ? jetCol->at(1)->pt() : 0.);
    phiJetTwo =  (n_jets_f >= 2 ? jetCol->at(1)->phi() : 0.);
    etaJetTwo =  (n_jets_f >= 2 ? jetCol->at(1)->eta() : 0.);
    btagJetTwo =  (n_jets_f >= 2 ? jetCol->at(1)->deepFlavor() : 0.);

    ptJetThree =  (n_jets_f >= 3 ? jetCol->at(2)->pt() : 0.);
    phiJetThree =  (n_jets_f >= 3 ? jetCol->at(2)->phi() : 0.);
    etaJetThree =  (n_jets_f >= 3 ? jetCol->at(2)->eta() : 0.);
    btagJetThree =  (n_jets_f >= 3 ? jetCol->at(2)->deepFlavor() : 0.);

    ptJetFour =  (n_jets_f >= 4 ? jetCol->at(3)->pt() : 0.);
    phiJetFour =  (n_jets_f >= 4 ? jetCol->at(3)->phi() : 0.);
    etaJetFour =  (n_jets_f >= 4 ? jetCol->at(3)->eta() : 0.);
    btagJetFour =  (n_jets_f >= 4 ? jetCol->at(3)->deepFlavor() : 0.);
    ptJetFive =  (n_jets_f >= 5 ? jetCol->at(4)->pt() : 0.);
    phiJetFive =  (n_jets_f >= 5 ? jetCol->at(4)->phi() : 0.);
    etaJetFive =  (n_jets_f >= 5 ? jetCol->at(4)->eta() : 0.);
    btagJetFive =  (n_jets_f >= 5 ? jetCol->at(4)->deepFlavor() : 0.);
    ptJetSix =  (n_jets_f >= 6 ? jetCol->at(5)->pt() : 0.);
    phiJetSix =  (n_jets_f >= 6 ? jetCol->at(5)->phi() : 0.);
    etaJetSix =  (n_jets_f >= 6 ? jetCol->at(5)->eta() : 0.);
    btagJetSix =  (n_jets_f >= 6 ? jetCol->at(5)->deepFlavor() : 0.);
    ptJetSeven =  (n_jets_f >= 7 ? jetCol->at(6)->pt() : 0.);
    phiJetSeven =  (n_jets_f >= 7 ? jetCol->at(6)->phi() : 0.);
    etaJetSeven =  (n_jets_f >= 7 ? jetCol->at(6)->eta() : 0.);
    btagJetSeven =  (n_jets_f >= 7 ? jetCol->at(6)->deepFlavor() : 0.);
    ptJetEight =  (n_jets_f >= 8 ? jetCol->at(7)->pt() : 0.);
    phiJetEight =  (n_jets_f >= 8 ? jetCol->at(7)->phi() : 0.);
    etaJetEight =  (n_jets_f >= 8 ? jetCol->at(7)->eta() : 0.);
    btagJetEight =  (n_jets_f >= 8 ? jetCol->at(7)->deepFlavor() : 0.);
    ptJetNine =  (n_jets_f >= 9 ? jetCol->at(8)->pt() : 0.);
    phiJetNine =  (n_jets_f >= 9 ? jetCol->at(8)->phi() : 0.);
    etaJetNine =  (n_jets_f >= 9 ? jetCol->at(8)->eta() : 0.);
    btagJetNine =  (n_jets_f >= 9 ? jetCol->at(8)->deepFlavor() : 0.);
    ptJetTen =  (n_jets_f >= 10 ? jetCol->at(9)->pt() : 0.);
    phiJetTen =  (n_jets_f >= 10 ? jetCol->at(9)->phi() : 0.);
    etaJetTen =  (n_jets_f >= 10 ? jetCol->at(9)->eta() : 0.);
    btagJetTen =  (n_jets_f >= 10 ? jetCol->at(9)->deepFlavor() : 0.);

    lightLeps->sortByPt();

    ptLepOne = lightLeps->at(0)->pt();
    phiLepOne = lightLeps->at(0)->phi();
    etaLepOne = lightLeps->at(0)->eta();
    flavLepOne = (lightLeps->at(0)->isMuon() ? 1. : 0.);
    ptLepTwo = lightLeps->at(1)->pt();
    phiLepTwo = lightLeps->at(1)->phi();
    etaLepTwo = lightLeps->at(1)->eta();
    flavLepTwo = (lightLeps->at(1)->isMuon() ? 1. : 0.);

    if (isML) {
        ptLepThree = lightLeps->at(2)->pt();
        phiLepThree = lightLeps->at(2)->phi();
        etaLepThree = lightLeps->at(2)->eta();
        flavLepThree = (lightLeps->at(2)->isMuon() ? 1. : 0.);
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


void FourTop::fillMVAVariablesNormalized(bool is4L) {
    n_jets_f         =  selection->getJetCol()->size();
    n_jets_f         =  (is4L ? (n_jets_f - 2) / 5 : (n_jets_f - 3) / 6);
    ht               =  selection->getJetCol()->scalarPtSum();
    ht               =  (is4L ? ht / 900 : (ht - 220) / 1280);
}

