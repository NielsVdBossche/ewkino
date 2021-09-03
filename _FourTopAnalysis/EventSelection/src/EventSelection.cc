#include "../interface/EventSelection.h"

bool FourTop::baselineSelection() {
    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    // 2 bjets
    // Min MET of 25 (50?)
    // Min HT of 300(?)
}

bool FourTop::passLowMassVeto() {
    // Reject same flavor lepton pairs (indep of charge) w inv mass below 12 gev
}

bool FourTop::passZBosonVeto() {
    // Reject same flevor opposite charge lepton pairs with inv mass close to Z boson mass
}