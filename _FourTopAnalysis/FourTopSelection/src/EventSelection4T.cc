#include "../interface/EventSelection4T.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void EventSelection4T::applyBaselineObjectSelection(Event* event) {
    event->removeTaus();
    event->selectLooseLeptons();
    event->cleanElectronsFromLooseMuons();

    event->selectGoodJets();
    event->cleanJetsFromLooseLeptons(); // Might change this to tight leps?
}

void EventSelection4T::applyFullObjectSelection(Event* event) {
    event->selectElectrons(ObjectSelection4T::elID);
    event->selectMuons(ObjectSelection4T::muonID);
    event->selectJets(ObjectSelection4T::jetID); // Still necessary?
    // Medium b-tagged jets

}


bool EventSelection4T::passBaselineEventSelection(Event* event) {
    // Baseline event selection keeping most leptons in order to correctly veto resonances

    double n_lep = event->numberOfLightLeptons();
    if (n_lep < 2) return false;
    //if (n_lep == 2 && event->lightLeptonCollection()[0].charge() != event->lightLeptonCollection()[1].charge()) return false;
    if (n_lep == 2 && event->hasOSLeptonPair()) return false;
    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    if (event->numberOfJets() < 3) return false;

    // 1 bjets
    if (event->numberOfMediumBTaggedJets() < 1) return false;

    // Min MET of 25 (50?)
    // Min HT of 300(?)
    if (event->met().pt() < 25) return false;
    if (event->HT() < 300) return false;

    return true;
}

bool EventSelection4T::passFullEventSelection(Event* event) {
    double n_lep = event->numberOfLightLeptons();
    if (n_lep < 2) return false;
    if (n_lep == 2 && event->hasOSLeptonPair()) return false;

    if (event->numberOfJets() < 4) return false;

    if (event->numberOfMediumBTaggedJets() < 2) return false;


    return true;
}

bool EventSelection4T::passLowMassVeto(Event* event) {  
    // Reject same flavor lepton pairs (indep of charge) w inv mass below 12 gev

    for( const auto& leptonPtrPair : event->lightLeptonCollection().pairCollection() ){

        //veto SF pairs of low mass
        Lepton& lepton1 = *( leptonPtrPair.first );
        Lepton& lepton2 = *( leptonPtrPair.second );
        if(! sameFlavor( lepton1, lepton2 ) ){
            continue;
        }
        if(( lepton1 + lepton2 ).mass() < 12){
            return false;
        }
    }
    return true;
}

bool EventSelection4T::passZBosonVeto(Event* event) {
    // Reject OSSF lepton pairs with inv mass close to Z boson mass
    if (event->hasOSSFLeptonPair()) {
        double mass = event->bestZBosonCandidateMass();
        if (mass > 76 && mass < 106) return false;
    }

    return true;
}

