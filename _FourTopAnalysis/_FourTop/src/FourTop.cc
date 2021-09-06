#include "../interface/FourTop.h"

FourTop::FourTop(std::vector< std::string > argvString) {
    // Set settings according to input
    // prepare details of analysis in separate functions



    createHistInfoVec();
}

FourTop::~FourTop() {
    delete histInfoVec_DL;
    delete histInfoVec_ML;
    //delete histInfoVec_Global; // If even assigned?
}

// Prepare 
void FourTop::createHistInfoVec() {

    // Temp solution
    histInfoVec_DL = new std::vector<HistInfo> (fourTopHists::bdtInfoDL());
    histInfoVec_ML = new std::vector<HistInfo> (fourTopHists::bdtInfoML());

    // Go over settings, voor elke setting, add de passende histosetup bij het totaal
    // Niet te veel om easy te houden, wil niet voor elke fill kei veel settings checken

    // Set hier mss ook setting voor lengte -> fill vector zelfde lengte, anders teveel checks/constructing voor vector?

}
