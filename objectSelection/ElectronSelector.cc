#include "../objects/interface/ElectronSelector.h"

//include c++ library classes
#include <cmath>

//include other parts of framework
#include "bTagWP.h"

///// general comments /////
// - tzq loose was simply copied from Marek's code, 
//   in order to perform a trigger efficiency measurement;
//   it was not yet tested or used in any other way.

// define here what mva threshold to use in tZq ID's listed below
double electronMVACut(){
     return 0.81;
}

// define here what mva value to use in tZq ID's listed below
double electronMVAValue(const Electron* electronPtr){
    return electronPtr->leptonMVATOPUL();
}

// define here what b-tagger to use in all tZq ID's listed below
// warning: deepFlavor is hard-coded in ttH ID!
double electronJetBTagValue(const Electron* electronPtr){
    //return electronPtr->closestJetDeepCSV();
    return electronPtr->closestJetDeepFlavor();
}

// define here what cone correction factor to use
double electronConeCorrectionFactor(){
    return 0.67;
}

/*
----------------------------------------------------------------
loose electron selection (common for ttH and tZq IDs)
----------------------------------------------------------------
*/
bool ElectronSelector::isLooseBase() const{
    if( electronPtr->uncorrectedPt() < 10) return false;
    //if( electronPtr->uncorrectedPt() < 5 ) return false; // for syncing with TT
    if( electronPtr->absEta() >= 2.5 ) return false;
    if( fabs( electronPtr->dxy() ) >= 0.05 ) return false;
    if( fabs( electronPtr->dz() ) >= 0.1 ) return false;
    if( electronPtr->sip3d() >= 8 ) return false;
    if( electronPtr->numberOfMissingHits() >= 2 ) return false;
    if( electronPtr->miniIso() >= 0.4 ) return false;

    if ( std::abs(electronPtr->etaSuperCluster()) > 1.4442 && std::abs(electronPtr->etaSuperCluster()) < 1.566) return false;

    return true;
}

bool ElectronSelector::isLoose2016() const{ 
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
--------------------------------------------------------------------------------
help function for FO ID's
-------------------------------------------------------------------------------
*/

//interpolation between two working points of deepFlavor between two pT values
double electronSlidingDeepFlavorThreshold( const double lowPt, const double lowPtWP,
                                    const double highPt, const double highPtWP,
                                    const double pt ){
    if( pt < lowPt ){
        return lowPtWP;
    } else if( pt > highPt ){
        return highPtWP;
    } else {
        return ( lowPtWP + ( highPtWP - lowPtWP ) / ( highPt - lowPt ) * ( pt - lowPt ) );
    }
}

/*
-------------------------------------------------------------------
FO electron selection for medium 0p4 tZq ID
-------------------------------------------------------------------
*/

bool ElectronSelector::isFOBase() const{
    if( !isLoose() ) return false;
    if( electronPtr->uncorrectedPt() <= 10 ) return false;

    if( !electronPtr->passConversionVeto() ) return false;
    if( !electronPtr->passChargeConsistency() ) return false; // for testing if this fixes closure

    return true;
}

bool ElectronSelector::isFO2016() const{
    if( electronMVAValue(electronPtr) < electronMVACut() ){

	if( electronPtr->closestJetDeepFlavor() > 0.5 ) return false;
        if( electronPtr->ptRatio() < 0.5 ) return false;
        if( !electronPtr->passElectronMVAFall17NoIsoLoose() ) return false;	

    }
    return true;
}



bool ElectronSelector::isFO2016PreVFP() const{
    if( electronMVAValue(electronPtr) < electronMVACut() ){

	if( electronPtr->closestJetDeepFlavor() > 0.5 ) return false;
        if( electronPtr->ptRatio() < 0.5 ) return false;
        if( !electronPtr->passElectronMVAFall17NoIsoLoose() ) return false;	

    }
    return true;
}


bool ElectronSelector::isFO2016PostVFP() const {
    if( electronMVAValue(electronPtr) < electronMVACut() ){

	if( electronPtr->closestJetDeepFlavor() > 0.5 ) return false;
        if( electronPtr->ptRatio() < 0.5 ) return false;
        if( !electronPtr->passElectronMVAFall17NoIsoLoose() ) return false;	

    }
    return true;
}

bool ElectronSelector::isFO2017() const{
    if( electronMVAValue(electronPtr) < electronMVACut() ){

	if( electronPtr->closestJetDeepFlavor() > 0.5 ) return false;
        if( electronPtr->ptRatio() < 0.5 ) return false;
        if( !electronPtr->passElectronMVAFall17NoIsoLoose() ) return false;

    }
    return true;
}

bool ElectronSelector::isFO2018() const{
    if( electronMVAValue(electronPtr) < electronMVACut() ){

        if( electronPtr->closestJetDeepFlavor() > 0.5 ) return false;
	if( electronPtr->ptRatio() < 0.5 ) return false;
        if( !electronPtr->passElectronMVAFall17NoIsoLoose() ) return false;

    }
    return true;
}

/*
-------------------------------------------------------------------------
tight electron selection for medium 0p4 tZq ID
-------------------------------------------------------------------------
*/

bool ElectronSelector::isTightBase() const{
    if( !isFO() ) return false;
    if( electronMVAValue(electronPtr) <= electronMVACut() ) return false;
    return true;
}

bool ElectronSelector::isTight2016() const{
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

// Tight charge requirement
bool ElectronSelector::isTightCharge() const {
    if( !electronPtr->passChargeConsistency() ) return false;
    return true;
}


/*
cone correction
*/

double ElectronSelector::coneCorrection() const{
    return ( electronConeCorrectionFactor() / electronPtr->ptRatio() );
}

// New loose electron selection for new TOPleptonMVA version


bool ElectronSelector::isLooseV2Base() const{
    if( electronPtr->uncorrectedPt() < 7 ) return false;
    if( electronPtr->absEta() >= 2.5 ) return false;
    if( electronPtr->sip3d() >= 15 ) return false;
    if( electronPtr->relIso0p3() >= 1. ) return false;
    return true;
}

bool ElectronSelector::isLooseV22016() const{ 
    return true;
}

bool ElectronSelector::isLooseV22016PreVFP() const{ 
    return true;
}

bool ElectronSelector::isLooseV22016PostVFP() const{ 
    return true;
}


bool ElectronSelector::isLooseV22017() const{
    return true;
}


bool ElectronSelector::isLooseV22018() const{
    return true;
}

