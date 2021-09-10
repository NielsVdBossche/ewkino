#include "../interface/objectSelection4T.h"

bool ObjectSelection4T::muonID(const Muon& muon) {
    if (muon.pt() < 20) return false;
    if (muon.absEta() > 2.4) return false;
    //if (muon.leptonMVAttH() < 0.8) return false; // PLACEHOLDER
    //if (! _mu_isTightCharge->at(i)) return false;

    return true;
}
