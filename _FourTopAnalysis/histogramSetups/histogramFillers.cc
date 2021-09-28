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

