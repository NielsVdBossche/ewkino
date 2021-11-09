#include "histogramSetup.h"

#if MEMLEAK
#include "../../memleak/debug_new.h" 
#endif

std::vector<double> fourTopHists::fillLepInfoDL(Event* event) {

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
        event->lightLepton(1).leptonMVATOP()
    };

    return fillVal;
}

std::vector<double> fourTopHists::fillLepInfoML(Event* event) {
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
        event->lightLepton(2).leptonMVATOP()
    };

    return fillVal;
}


std::vector<double> fourTopHists::fillJetInfoDL(Event* event) {
    JetCollection jets = event->jetCollection();

    std::vector<double> fillVal = {
        jets[0].pt(),
        jets[1].pt(),
        jets[2].pt(),
        jets[3].pt(),
        double(event->mediumBTagCollection().size()),
        jets.scalarPtSum()
    };

    return fillVal;
}

std::vector<double> fourTopHists::fillJetInfoML(Event* event) {
    JetCollection jets = event->jetCollection();

    std::vector<double> fillVal = {
        jets[0].pt(),
        jets[1].pt(),
        jets[2].pt(),
        jets[3].pt(),
        double(event->mediumBTagCollection().size()),
        jets.scalarPtSum()
    };

    return fillVal;
}

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
    JetCollection* jets = selec->getJetCol();
    JetCollection* bJets = selec->getBtagJetCol();
    LightLeptonCollection* lightLeps;
    if (selec->isEventNormalSelected()) {
        lightLeps = (LightLeptonCollection*) selec->getMediumLepCol();
    } else {
        lightLeps = (LightLeptonCollection*) selec->getAltLeptonCol();
    }
    std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
    std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *((LeptonCollection*)lightLeps));
    
    int nb = bJets->size();

    std::vector<double> fillVal = {
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

        (*jets)[0].pt(),
        (*jets)[1].pt(),
        (*jets)[2].pt(),
        (jets->size() > 3 ? (*jets)[3].pt() : 0.),
        double(bJets->size()),
        double(selec->numberOfLooseBJets()),
        double(selec->numberOfTightBJets()),
        double(jets->size()),

        jets->scalarPtSum(),
        selec->getEvent()->metPt(),

        // Calculate DR? What is best way...
        (nb >= 2 ? mindR_Bjets[0] : 5.),

        (nb > 0 ? mindR_Bjet_lep[0] : 5.),
        (nb > 0 ? mindR_Bjet_lep[1] : 5.),

        lightLeps->scalarPtSum(), // LT
        double(lightLeps->size()),

        jets->at(0)->deepFlavor(),
        jets->at(1)->deepFlavor(),
        jets->at(2)->deepFlavor(),
        (jets->size() > 3 ? jets->at(3)->deepFlavor() : -1.)

    };

    jets->sortByAttribute([](const std::shared_ptr< Jet >& lhs, const std::shared_ptr< Jet >& rhs){ return lhs->deepFlavor() > rhs->deepFlavor(); } );

    fillVal.push_back(jets->at(0)->deepFlavor());
    fillVal.push_back(jets->at(1)->deepFlavor());
    fillVal.push_back(jets->at(2)->deepFlavor());
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
