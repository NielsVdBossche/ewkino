#include "../objects/interface/ElectronSelector.h"

//include c++ library classes
#include <cmath>

//include other parts of framework
#include "bTagWP.h"


/*
loose electron selection
*/


double leptonMVACutElectron(){
    return 0.6;
}


bool ElectronSelector::isLooseBase() const{

    /*
    // Willem's cuts
    if( electronPtr->uncorrectedPt() < 7 ) return false;
    if( electronPtr->absEta() >= 2.5 ) return false;
    if( fabs( electronPtr->dxy() ) >= 0.05 ) return false;
    if( fabs( electronPtr->dz() ) >= 0.1 ) return false;
    if( electronPtr->sip3d() >= 8 ) return false;
    if( electronPtr->numberOfMissingHits() >= 2 ) return false;
    if( electronPtr->miniIso() >= 0.4 ) return false;
    if( !electronPtr->passElectronMVAFall17NoIsoLoose() ) return false;

    if (electronPtr->leptonMVATOP() < 0.5) return false;
    */

    if (electronPtr->uncorrectedPt() < 7) return false;
    if (electronPtr->absEta() >= 2.5) return false;
    // IsGSF
    if (electronPtr->numberOfMissingHits() >= 2) return false;
    if (electronPtr->dxy() >= 0.05) return false;
    if (electronPtr->dz() >= 0.1) return false;
    if (electronPtr->sip3d() >= 8) return false;
    if (electronPtr->miniIso() >= 0.4) return false;
    if (fabs(electronPtr->etaSuperCluster()) > 1.4442 && fabs(electronPtr->etaSuperCluster()) < 1.566) return false;

    // Tight charge requirements:
    //if (! electronPtr->passChargeConsistency()) return false;
    // already necessary here?
    //if (electronPtr->leptonMVATOP() < 0.) return false;

    return true;
}



bool ElectronSelector::isLoose2016PreVFP() const{ 
    return true;
}

bool ElectronSelector::isLoose2016PostVFP() const{ 
    return true;
}


bool ElectronSelector::isLoose2017() const{
    return true;
}


bool ElectronSelector::isLoose2018() const{
    return true;
}


/*
FO electron selection
*/

bool ElectronSelector::isFOBase() const{
    if( !isLoose() ) return false;
    /*
    // Old selection
    if( electronPtr->uncorrectedPt() <= 10 ) return false;
    if( electronPtr->numberOfMissingHits() > 0 ) return false;
    if( electronPtr->hOverE() >= 0.1 ) return false;
    if( electronPtr->inverseEMinusInverseP() <= -0.04 ) return false;
    if( electronPtr->etaSuperCluster() <= 1.479 ){
        if( electronPtr->sigmaIEtaEta() >= 0.011 ) return false;
    } else {
        if( electronPtr->sigmaIEtaEta() >= 0.030 ) return false;
    }
    
    Seems not relevant when leptonMVATOP is available?
    if( electronPtr->leptonMVAttH() <= leptonMVACutElectron() ){
        if( !electronPtr->passElectronMVAFall17NoIsoWP80() ) return false;
        if( electronPtr->ptRatio() <= 0.7 ) return false;
    }
    
    if( !electronPtr->passConversionVeto() ) return false;
    */

    //if (electronPtr->pt() < 20) return false;

    // Fakeable object
    if (electronPtr->uncorrectedPt() <= 10) return false;
    if( electronPtr->hOverE() >= 0.1 ) return false;
    if( electronPtr->inverseEMinusInverseP() <= -0.04 ) return false;
    if (! electronPtr->passChargeConsistency()) return false;
    if( !electronPtr->passConversionVeto() ) return false;

    if (electronPtr->leptonMVATOP() < 0.6) {
        if (! electronPtr->isLoosePOGElectron()) return false;
        if (electronPtr->ptRatio() < 0.45) return false; 
    }

    return true;
}


bool ElectronSelector::isFO2016PreVFP() const{
    if (electronPtr->leptonMVATOP() < 0.6) {
        if( electronPtr->closestJetDeepFlavor() >= bTagWP::tightDeepFlavor2016PreVFP() ) return false;
    }
    return true;
}

bool ElectronSelector::isFO2016PostVFP() const{
    if (electronPtr->leptonMVATOP() < 0.6) {
        if( electronPtr->closestJetDeepFlavor() >= bTagWP::tightDeepFlavor2016PostVFP() ) return false;
    }
    return true;
}


bool ElectronSelector::isFO2017() const{
    if (electronPtr->leptonMVATOP() < 0.6) {
        if( electronPtr->closestJetDeepFlavor() >= bTagWP::tightDeepFlavor2017() ) return false;
    }
    return true;
}


bool ElectronSelector::isFO2018() const{
    if (electronPtr->leptonMVATOP() < 0.6) {
        if( electronPtr->closestJetDeepFlavor() >= bTagWP::tightDeepFlavor2018() ) return false;
    }
    return true;
}


/*
tight electron selection
*/

bool ElectronSelector::isTightBase() const{
    if( !isFO() ) return false;
    //if( electronPtr->leptonMVAttH() <= leptonMVACutElectron() ) return false;

    //if (electronPtr->pt() < 20) return false;
    if (electronPtr->leptonMVATOP() < 0.6) return false;


    return true;
}


bool ElectronSelector::isTight2016PreVFP() const{
    return true;
}

bool ElectronSelector::isTight2016PostVFP() const{
    return true;
}


bool ElectronSelector::isTight2017() const{
    return true;
}


bool ElectronSelector::isTight2018() const{
    return true;
}


/*
cone correction
*/

double ElectronSelector::coneCorrection() const{
    return ( 0.71 / electronPtr->ptRatio() );
}
