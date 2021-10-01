#include "../interface/FourTop.h"

// other methods: load and build methods from files

void FourTop::fillMVAVariables(bool isML) {
    JetCollection bJets = currentEvent->mediumBTagCollection();
    LeptonCollection lightLeps = currentEvent->leptonCollection();

    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(bJets, lightLeps);

    n_jets_f         =  currentEvent->numberOfJets();
    n_bjets_f        =  currentEvent->numberOfMediumBTaggedJets();
    deltaRBjets      =  mindR_Bjets[0];
    n_b_loose        =  currentEvent->numberOfLooseBTaggedJets();
    n_b_tight        =  currentEvent->numberOfTightBTaggedJets();
    dRleps           =  deltaR(currentEvent->lepton(0), currentEvent->lepton(1));
    aziAngle         =  deltaPhi(currentEvent->lepton(0), currentEvent->lepton(1));
    ht               =  currentEvent->HT();

    JetCollection jetCol = currentEvent->jetCollection();
    massToPt = 0.;
    for (JetCollection::iterator jetIt = jetCol.begin(); jetIt != jetCol.end(); jetIt++) {
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

    currentEvent->sortJetsByPt();
    ptJetOne         =  currentEvent->jet(0).pt();
    ptJetFour        =  currentEvent->jet(3).pt();
    ptJetFive        =  currentEvent->jet(4).pt();
    ptJetSix         =  currentEvent->jet(5).pt();

    currentEvent->sortLeptonsByPt();
    ptLepOne         =  currentEvent->lepton(0).pt();
    ptLepTwo         =  currentEvent->lepton(1).pt();

    if (isML) {
        ptLepThree   =  currentEvent->lepton(2).pt();
    }
}
