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

std::vector<double> fourTopHists::fillAllHists(bool multilep, EventSelection4T* selec) {
    JetCollection* jets = selec->getJetCol();
    JetCollection* bJets = selec->getBtagJetCol();
    LightLeptonCollection* lightLeps = (LightLeptonCollection*) selec->getMediumLepCol();
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
        
    };

    if (multilep) {
        fillVal.push_back((*lightLeps)[2].pt());
        fillVal.push_back((*lightLeps)[2].eta());
        fillVal.push_back((*lightLeps)[2].phi());
        fillVal.push_back((*lightLeps)[2].energy());
        fillVal.push_back((*lightLeps)[2].leptonMVATOP());
    }

    return fillVal;
}
