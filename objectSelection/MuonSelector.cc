#include "../objects/interface/MuonSelector.h"

//b-tagging working points
#include "bTagWP.h"

///// general comments /////
// - tzq loose was simply copied from Marek's code, 
//   in order to perform a trigger efficiency measurement;
//   it was not yet tested or used in any other way.

// define here what mva threshold to use in tZq ID's listed below
double muonMVACut(){
    return 0.64;
    //return 0.4;
}

// define here what mva value to use in tZq ID's listed below
double muonMVAValue(const Muon* muonPtr){
    return muonPtr->leptonMVATOPUL();
    //return muonPtr->leptonMVATOP();
}

// define here what b-tagger to use in all tZq ID's listed below
// warning: deepFlavor is hard-coded in ttH ID!
double muonJetBTagValue(const Muon* muonPtr){
    //return muonPtr->closestJetDeepCSV();
    return muonPtr->closestJetDeepFlavor();
}

double muonConeCorrectionFactor(){
    return 0.66;
}

/*
-------------------------------------------------------------------
loose muon selection (common to tZq, ttH and old tZq ID)
-------------------------------------------------------------------
*/

bool MuonSelector::isLooseBase() const{
    if( muonPtr->uncorrectedPt() <= 10 ) return false;
    if( muonPtr->absEta() >= 2.4 ) return false; 
    if( fabs( muonPtr->dxy() ) >= 0.05 ) return false;
    if( fabs( muonPtr->dz() ) >= 0.1 ) return false;
    if( muonPtr->sip3d() >= 8 ) return false;
    if( muonPtr->miniIso() >= 0.4 ) return false;
    if( !muonPtr->isMediumPOGMuon() ) return false;
    return true;
}

bool MuonSelector::isLoose2016() const{
    return true;
}

bool MuonSelector::isLoose2016PreVFP() const{
    return true;
}


bool MuonSelector::isLoose2016PostVFP() const{
    return true;
}


bool MuonSelector::isLoose2017() const{
    return true;
}


bool MuonSelector::isLoose2018() const{
    return true;
}

/*
--------------------------------------------------------------------------------
help function for FO ID's
-------------------------------------------------------------------------------
*/

//interpolation between two working points of deepFlavor between two pT values
double muonSlidingDeepFlavorThreshold( const double lowPt, const double lowPtWP, 
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
---------------------------------------------------------------------------------
FO muon selection for medium 0.4 tZq ID
---------------------------------------------------------------------------------
*/

bool MuonSelector::isFOBase() const{
    if( !isLoose() ) return false;
    //if( muonPtr->uncorrectedPt() <= 10 ) return false;
    // put tunable FO-cuts below
    //if( muonMVAValue(muonPtr) <= muonMVACut() ){
    //}
    return true;
}

bool MuonSelector::isFO2016() const{
    if( muonMVAValue(muonPtr) <= muonMVACut() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}

bool MuonSelector::isFO2016PreVFP() const{
    if( muonMVAValue(muonPtr) <= muonMVACut() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}

bool MuonSelector::isFO2016PostVFP() const{
    if( muonMVAValue(muonPtr) <= muonMVACut() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


bool MuonSelector::isFO2017() const{
    if( muonMVAValue(muonPtr) <= muonMVACut() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


bool MuonSelector::isFO2018() const{
    if( muonMVAValue(muonPtr) <= muonMVACut() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


/*
----------------------------------------------------------------------------
tight muon selection for medium 0p4 tZq ID
----------------------------------------------------------------------------
*/

bool MuonSelector::isTightBase() const{
    if( !isFO() ) return false;
    if( muonMVAValue(muonPtr) <= muonMVACut() ) return false;
    return true;
}


bool MuonSelector::isTight2016() const{
    return true;
}

bool MuonSelector::isTight2016PreVFP() const{
    return true;
}

bool MuonSelector::isTight2016PostVFP() const{
    return true;
}


bool MuonSelector::isTight2017() const{
    return true;
}


bool MuonSelector::isTight2018() const{
    return true;
}

// Tight charge requirement
bool MuonSelector::isTightCharge() const {
    return true;
}

/*
cone correction
*/

double MuonSelector::coneCorrection() const{
    return ( muonConeCorrectionFactor() / muonPtr->ptRatio() );
}

// New loose muon selection for new TOPleptonMVA version


bool MuonSelector::isLooseV2Base() const{
    if( muonPtr->uncorrectedPt() <= 5 ) return false;
    if( muonPtr->absEta() >= 2.4 ) return false; 
    if( muonPtr->sip3d() >= 15 ) return false;
    if( muonPtr->relIso0p3() >= 1. ) return false;
    return true;
}

bool MuonSelector::isLooseV22016() const{
    return true;
}

bool MuonSelector::isLooseV22016PreVFP() const{
    return true;
}


bool MuonSelector::isLooseV22016PostVFP() const{
    return true;
}


bool MuonSelector::isLooseV22017() const{
    return true;
}


bool MuonSelector::isLooseV22018() const{
    return true;
}


