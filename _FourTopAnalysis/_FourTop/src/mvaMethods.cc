#include "../interface/FourTop.h"

// other methods: load and build methods from files

void FourTop::fillMVAVariables(bool isML) {
    JetCollection* bJets = selection->getBtagJetCol();
    LightLeptonCollection* lightLeps; // = selection->getMediumLepCol();
    if (selection->isEventNormalSelected()) {
        lightLeps = (LightLeptonCollection*) selection->getMediumLepCol();
    } else {
        lightLeps = (LightLeptonCollection*) selection->getAltLeptonCol();
    }
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
        ptLepThree   =  lightLeps->at(2)->pt();
        ptLepFour    =  (selection->numberOfLeps() > 3 ? lightLeps->at(3)->pt() : 0.);
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
