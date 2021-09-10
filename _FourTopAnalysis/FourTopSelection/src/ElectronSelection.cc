#include "../interface/objectSelection4T.h"

bool ObjectSelection4T::elID(const Electron& electron) {
    if (electron.pt() < 20) return false;
    if (electron.absEta() > 2.5) return false;
    //if (electron.leptonMVAtZq() < 0.8) return false; // PLACEHOLDER

    // Andere requirements? dxy, dz, 3dIP, ...
    //if (! _el_isTightCharge->at(i)) return false; -> Todo? Chargeconsistency?

    return true;
}
