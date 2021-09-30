#include "../interface/EventSelection4T.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

EventSelection4T::EventSelection4T() {
    looseLeps = new LeptonCollection();
    mediumLeps = new LeptonCollection();
    jets = new JetCollection();
    bTagJets = new JetCollection();
}

void EventSelection4T::cleanSelection() {
    delete looseLeps;
    delete mediumLeps;
    delete jets;
    delete bTagJets;
}

void EventSelection4T::addNewEvent(Event* newEvent) {
    cleanSelection();
    event = newEvent;
    objectSelection();
}

void EventSelection4T::objectSelection() {
    event->removeTaus();
    event->selectLooseLeptons();
    event->cleanElectronsFromLooseMuons(); // consider making loose lep sel the original one
    // FO would be the loose one now and tight the medium mva WP + higher pt

    event->selectGoodJets();
    event->cleanJetsFromLooseLeptons(); // Clean jets from leps: is loose leps good or again like earlier

    event->sortLeptonsByPt();

    looseLeps = new LeptonCollection(event->looseLeptonCollection());
    mediumLeps = new LeptonCollection(event->FOLeptonCollection());

    jets = new JetCollection(event->jetCollection());
    bTagJets = new JetCollection(event->mediumBTagCollection());
}

bool EventSelection4T::passBaselineEventSelection() {
    // Baseline event selection keeping most leptons in order to correctly veto resonances

    double n_lep = mediumLeps->size();

    if (n_lep < 2) return false;
    if (n_lep == 2 && mediumLeps->hasOSPair()) return false;

    if ((*mediumLeps)[0].pt() < 25) return false;
    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    if (jets->size() < 3) return false;

    // 1 bjets
    if (bTagJets->size() < 1) return false;

    if (event->met().pt() < 25) return false;
    if (jets->scalarPtSum() < 300) return false;

    return true;
}

bool EventSelection4T::passFullEventSelection() {
    // already included in baseline selection
    //double n_lep = mediumLeps->size();
    //if (n_lep < 2) return false;
    //if (n_lep == 2 && mediumLeps->hasOSPair()) return false;

    if (jets->size() < 4) return false;

    if (bTagJets->size() < 2) return false;


    return true;
}

bool EventSelection4T::passLowMassVeto() {  
    // Reject same flavor lepton pairs (indep of charge) w inv mass below 12 gev

    for( const auto& leptonPtrPair : looseLeps->pairCollection() ){

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

bool EventSelection4T::passZBosonVeto() {
    // Reject OSSF lepton pairs with inv mass close to Z boson mass
    if (event->hasOSSFLeptonPair()) {
        double mass = event->bestZBosonCandidateMass();
        if (mass > 76 && mass < 106) return false;
    }

    return true;
}

