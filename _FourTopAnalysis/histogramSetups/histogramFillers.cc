#include "histogramSetup.h"

#if MEMLEAK
#include "../../memleak/debug_new.h" 
#endif

std::vector<double> fourTopHists::fillAllHistsDL(Event* event) {
    JetCollection jets = event->jetCollection();
    JetCollection bJets = event->mediumBTagCollection();
    LeptonCollection lightLeps = event->leptonCollection();
    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(bJets, lightLeps);


    std::vector<double> fillVal = {
        event->lightLepton(0).pt(),
        event->lightLepton(1).pt(),
        event->lightLepton(0).eta(),
        event->lightLepton(1).eta(),
        event->lightLepton(0).phi(),
        event->lightLepton(1).phi(),
        event->lightLepton(0).energy(),
        event->lightLepton(1).energy(),
        event->lightLepton(0).leptonMVATOP(),
        event->lightLepton(1).leptonMVATOP(),

        jets[0].pt(),
        jets[1].pt(),
        jets[2].pt(),
        jets[3].pt(),
        double(event->mediumBTagCollection().size()),
        double(event->looseBTagCollection().size()),
        double(event->tightBTagCollection().size()),
        double(event->numberOfJets()),

        jets.scalarPtSum(),
        event->metPt(),

        // Calculate DR? What is best way...
        mindR_Bjets[0],

        mindR_Bjet_lep[0],
        mindR_Bjet_lep[1],

        event->LT(),

        (event->lepton(0).isElectron() ? event->electron(0).numberOfMissingHits() : -1.),
        (event->lepton(1).isElectron() ? (*(event->electronCollection().end() - 1))->numberOfMissingHits() : -1.) // This is why  the approach in Event.h is stupid
        
    };

    return fillVal;

}

std::vector<double> fourTopHists::fillAllHistsML(Event* event) {

    JetCollection jets = event->jetCollection();
    JetCollection bJets = event->mediumBTagCollection();
    LeptonCollection lightLeps = event->leptonCollection();
    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(bJets, lightLeps);


    std::vector<double> fillVal = {
        event->lightLepton(0).pt(),
        event->lightLepton(1).pt(),
        event->lightLepton(2).pt(),
        event->lightLepton(0).eta(),
        event->lightLepton(1).eta(),
        event->lightLepton(2).eta(),
        event->lightLepton(0).phi(),
        event->lightLepton(1).phi(),
        event->lightLepton(2).phi(),
        event->lightLepton(0).energy(),
        event->lightLepton(1).energy(),
        event->lightLepton(2).energy(),
        event->lightLepton(0).leptonMVATOP(),
        event->lightLepton(1).leptonMVATOP(),
        event->lightLepton(2).leptonMVATOP(),

        jets[0].pt(),
        jets[1].pt(),
        jets[2].pt(),
        jets[3].pt(),
        double(event->mediumBTagCollection().size()),
        double(event->looseBTagCollection().size()),
        double(event->tightBTagCollection().size()),
        double(event->numberOfJets()),

        jets.scalarPtSum(),
        event->metPt(),

        mindR_Bjets[0],

        mindR_Bjet_lep[0],
        mindR_Bjet_lep[1],

        event->LT()


    };

    return fillVal;

}

std::vector<double> fourTopHists::fillAllHists(bool multilep, EventFourT* selec, bool fourLep) {
    MVAHandler_4T* mva;
    if (multilep) {
        mva = selec->GetMLMVA();
    } else {
        mva = selec->GetDLMVA();
    }

    JetCollection* jets = selec->getJetCol();
    JetCollection* bJets = selec->getBtagJetCol();
    LightLeptonCollection* lightLeps;
    if (selec->isEventNormalSelected()) {
        lightLeps = (LightLeptonCollection*) selec->getMediumLepCol();
    } else {
        lightLeps = (LightLeptonCollection*) selec->getAltLeptonCol();
    }
    //std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
    //std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *((LeptonCollection*)lightLeps));
    
    //int nb = bJets->size();

    jets->sortByPt();

    std::vector<double> fillVal = {
        0.5,
        double(selec->getMediumLepCol()->numberOfMuons()),
        double(selec->getMediumLepCol()->numberOfElectrons()),
        (*lightLeps)[0].pt(),
        (*lightLeps)[1].pt(),
        (*lightLeps)[0].eta(),
        (*lightLeps)[1].eta(),
        (*lightLeps)[0].phi(),
        (*lightLeps)[1].phi(),
        (*lightLeps)[0].energy(),
        (*lightLeps)[1].energy(),
        (*lightLeps)[0].leptonMVATOP(),
        (*lightLeps)[1].leptonMVATOP(),

        (jets->size() > 0 ? (*jets)[0].pt() : 0.),
        (jets->size() > 1 ? (*jets)[1].pt() : 0.),
        (jets->size() > 2 ? (*jets)[2].pt() : 0.),
        (jets->size() > 3 ? (*jets)[3].pt() : 0.),
        double(bJets->size()),
        double(selec->numberOfLooseBJets()),
        double(selec->numberOfTightBJets()),
        double(jets->size()),

        jets->scalarPtSum(),
        jets->scalarPtSum() / double(jets->size()),
        selec->getEvent()->metPt(),

        // Calculate DR? What is best way...
        mva->deltaRBjets, //(nb >= 2 ? mindR_Bjets[0] : 5.),

        mva->min_dr_lep_b, // (nb > 0 ? mindR_Bjet_lep[0] : 5.),
        mva->sec_min_dr_lep_b, // (nb > 0 ? mindR_Bjet_lep[1] : 5.),

        lightLeps->scalarPtSum(), // LT
        double(lightLeps->size()),

        (jets->size() > 0 ? jets->at(0)->deepFlavor() : -1.),
        (jets->size() > 1 ? jets->at(1)->deepFlavor() : -1.),
        (jets->size() > 2 ? jets->at(2)->deepFlavor() : -1.),
        (jets->size() > 3 ? jets->at(3)->deepFlavor() : -1.),

        mva->mtLeadLepMET, // mt(*lightLeps->at(0), selec->getEvent()->met()),
        mva->mtSubLeadLepMET, // mt(*lightLeps->at(1), selec->getEvent()->met()),

        //mt2::mt2(*lightLeps->at(0), *lightLeps->at(1), selec->getEvent()->met()),
        mva->m2ll, //mt2::mt2Alt(*lightLeps->at(0), *lightLeps->at(1), selec->getEvent()->met()),
        mva->m2bb, //(nb >= 2 ? mt2::mt2bb((*bJets)[0], (*bJets)[1], (*lightLeps)[0], (*lightLeps)[1], selec->getEvent()->met()) : -1),
        mva->m2lblb, //(nb >= 2 ? mt2::mt2lblb((*bJets)[0], (*bJets)[1], (*lightLeps)[0], (*lightLeps)[1], selec->getEvent()->met()) : -1),


    };

    jets->sortByAttribute([](const std::shared_ptr< Jet >& lhs, const std::shared_ptr< Jet >& rhs){ return lhs->deepFlavor() > rhs->deepFlavor(); } );

    fillVal.push_back(jets->size() > 0 ? jets->at(0)->deepFlavor() : -1.);
    fillVal.push_back(jets->size() > 1 ? jets->at(1)->deepFlavor() : -1.);
    fillVal.push_back(jets->size() > 2 ? jets->at(2)->deepFlavor() : -1.);
    fillVal.push_back(jets->size() > 3 ? jets->at(3)->deepFlavor() : -1.);

    
    if (selec->getEvent()->hasOSSFLeptonPair()) {
        double mass = selec->getEvent()->bestZBosonCandidateMass();
        fillVal.push_back(mass);
    } else {
        fillVal.push_back(0.);

    }

    unsigned currentSize = fillVal.size();
    for( const auto& leptonPtrPair : lightLeps->pairCollection() ){
        //veto SF pairs of low mass
        Lepton& lepton1 = *( leptonPtrPair.first );
        Lepton& lepton2 = *( leptonPtrPair.second );
        if(! sameFlavor( lepton1, lepton2 )){
            continue;
        }
        if(( lepton1 + lepton2 ).mass() < 15.){
            fillVal.push_back((lepton1 + lepton2).mass());
            break;
        }
    }

    if (currentSize == fillVal.size()) {
        fillVal.push_back(16.);
    }
    

    //TopReconstructionNew* topReco = selec->getTopReco();
//
    //topReco->RecoBestTwoTops();
    fillVal.push_back(mva->massBestTop);   //topReco->getBestRecoTop().first);
    fillVal.push_back(mva->massBestTopW);   //topReco->getBestRecoTop().second);
    fillVal.push_back(mva->massSecTop);   //topReco->getSecondBestRecoTop().first);
    fillVal.push_back(mva->massSecTopW);   //topReco->getSecondBestRecoTop().second);


    if (multilep) {
        fillVal.push_back((*lightLeps)[2].pt());
        fillVal.push_back((*lightLeps)[2].eta());
        fillVal.push_back((*lightLeps)[2].phi());
        fillVal.push_back((*lightLeps)[2].energy());
        fillVal.push_back((*lightLeps)[2].leptonMVATOP());

        if (fourLep) {
            fillVal.push_back((*lightLeps)[3].pt());
            fillVal.push_back((*lightLeps)[3].eta());
            fillVal.push_back((*lightLeps)[3].phi());
            fillVal.push_back((*lightLeps)[3].energy());
            fillVal.push_back((*lightLeps)[3].leptonMVATOP());
        }
    }

    return fillVal;
}

std::vector<double> fourTopHists::fillAllLean(bool multilep, EventFourT* selec){
    JetCollection* jets = selec->getJetCol();
    JetCollection* bJets = selec->getBtagJetCol();
    LightLeptonCollection* lightLeps;
    if (selec->isEventNormalSelected()) {
        lightLeps = (LightLeptonCollection*) selec->getMediumLepCol();
    } else {
        lightLeps = (LightLeptonCollection*) selec->getAltLeptonCol();
    }

    //int nb = bJets->size();
    int nlep = lightLeps->size();

    std::vector<double> fillVal = {
        0.5,
        double(selec->getMediumLepCol()->numberOfMuons()),
        double(selec->getMediumLepCol()->numberOfElectrons()),
        (*lightLeps)[0].pt(),
        (*lightLeps)[1].pt(),
        (*lightLeps)[0].eta(),
        (*lightLeps)[1].eta(),
        (*lightLeps)[0].energy(),
        (*lightLeps)[1].energy(),

        double(bJets->size()),
        double(jets->size()),

        jets->scalarPtSum(),
        selec->getEvent()->metPt(),

        lightLeps->scalarPtSum(), // LT
        double(lightLeps->size()),
    };



    
    if (selec->getEvent()->hasOSSFLeptonPair()) {
        double mass = selec->getEvent()->bestZBosonCandidateMass();
        fillVal.push_back(mass);
    } else {
        fillVal.push_back(0.);

    }

    if (multilep) {
        fillVal.push_back(nlep >= 3? (*lightLeps)[2].pt() : 0.);
        fillVal.push_back(nlep >= 3? (*lightLeps)[2].eta() : 0.);
        fillVal.push_back(nlep >= 3? (*lightLeps)[2].energy() : 0.);
    }

    return fillVal;
}


std::vector<double> fourTopHists::fillTestHists(eventClass evClass, EventFourT* selec) {
    LeptonCollection* leps;
    leps = selec->getMediumLepCol();

    leps->selectTightChargeLeptons();

    std::vector<double> fillVal = {
        (double) leps->size()
    };
    
    return fillVal;
}