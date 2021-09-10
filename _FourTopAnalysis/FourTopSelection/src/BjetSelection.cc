#include "../interface/objectSelection4T.h"

bool ObjectSelection4T::bJetID(const Jet& jet) {
    // Subset of jet!

    //if (! _jet_isMediumBTag->at(i)) return false;
    if (jet.deepCSV() < 0.5) return false; // PLACEHOLDER
    return true;
}
