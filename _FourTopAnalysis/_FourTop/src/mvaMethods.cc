#include "../interface/FourTop.h"

// other methods: load and build methods from files

void FourTop::fillMVAVariables(bool isML) {
    JetCollection* bJets = selection->getBtagJetCol();
    LeptonCollection* lightLeps = selection->getMediumLepCol();

    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *lightLeps);

    n_jets_f         =  selection->getJetCol()->size();
    n_bjets_f        =  bJets->size();
    deltaRBjets      =  mindR_Bjets[0];
    n_b_loose        =  currentEvent->numberOfLooseBTaggedJets();
    n_b_tight        =  currentEvent->numberOfTightBTaggedJets();
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
