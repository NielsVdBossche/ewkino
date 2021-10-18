#include "../interface/EventSelection4T.h"

bool leptonPtReq(const Lepton& lep) {
    if (lep.pt() < 20) return false;

    return true;
}

bool EventSelection4T::passBaselineEventSelectionWithAltLeptons() {
    altObjectSelection();
    // Allow one loose lepton in selected leptons
    double n_lep_Loose = altLeps->size();
    int sizeDiff = altLeps->size() - nLep;

    if (n_lep_Loose < 2) return false; // only see if one of the leptons is loose
    if (sizeDiff > 1) return false;
    if (n_lep_Loose == 2 && altLeps->hasOSPair()) return false;

    if (altLeps->at(0)->pt() < 25) return false; // not necessary for MVA samples
    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    if (jets->size() < 3) return false;

    // 1 bjets
    if (bTagJets->size() < 1) return false;

    if (event->met().pt() < 25) return false;
    if (jets->scalarPtSum() < 300) return false;

    return true;
}

bool EventSelection4T::passFullEventSelectonWithAltLeptons() {
    // Completely the same as before. Difference in baseline.

    if (jets->size() < 4) return false;

    if (bTagJets->size() < 2) return false;

    return true;
}

void EventSelection4T::altObjectSelection() {
    altLeps = new LeptonCollection(*looseLeps);
    altLeps->selectObjects(leptonPtReq);
}
