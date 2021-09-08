#include "../interface/EventSelection4T.h"

void EventSelection4T::applyBaselineObjectSelection(Event* event) {
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
    double n_lep = event->numberOfLightLeptons();
    if (n_lep < 2) return false;
    //if (n_lep == 2 && event->lightLeptonCollection()[0].charge() != event->lightLeptonCollection()[1].charge()) return false;
    if (n_lep == 2 && event->hasOSLeptonPair()) return false;
    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    if (event->numberOfJets() < 4) return false;

    // 2 bjets
    if (event->mediumBTagCollection().size() < 2) return false;

    if (event->met().pt() < 25) return false;
    if (event->HT() < 300) return false;

    // Min MET of 25 (50?)
    // Min HT of 300(?)
    return true;
}

bool EventSelection4T::passFullEventSelection(Event* event) {
    double n_lep = event->numberOfLightLeptons();
    if (n_lep < 2) return false;
    if (n_lep == 2 && event->hasOSLeptonPair()) return false;

    return true;
}

bool EventSelection4T::passLowMassVeto(Event* event) {
    // Reject same flavor lepton pairs (indep of charge) w inv mass below 12 gev
}

bool EventSelection4T::passZBosonVeto(Event* event) {
    // Reject same flevor opposite charge lepton pairs with inv mass close to Z boson mass
}

