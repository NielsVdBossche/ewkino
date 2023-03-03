#include "../objects/interface/MuonSelector.h"

//b-tagging working points
#include "bTagWP.h"


double leptonMVACutMuon(){
    return 0.85;
}


/*
loose muon selection
*/

bool MuonSelector::isLooseBase() const{
    // cuts to replicate selection at ntuplizing stage
    // (presumably not done at nanoAOD creation)
    if( !muonPtr->isPFCandidate() ) return false;
    if( !(muonPtr->isTracker() || muonPtr->isGlobal()) ) return false;
    // other loose selection criteria
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
FO muon selection
*/

//interpolation between loose and medium working point of deep flavor from 20 to 45 GeV in muon pT as defined in the ttH analysis
double slidingDeepFlavorThreshold( const double looseWP, const double mediumWP, const double pt ){
    static const double minPt = 20.;
    static const double maxPt = 45.;
    if( pt < minPt ){
        return mediumWP;
    } else if( pt > maxPt ){
        return looseWP;
    } else {
        return ( mediumWP - ( mediumWP - looseWP ) / ( maxPt - minPt ) * ( pt - minPt ) );
    }
}


bool MuonSelector::isFOBase() const{
    if( !isLoose() ) return false;
    return true;
}


bool MuonSelector::isFO2016() const{
    if( muonPtr->leptonMVAttH() <= leptonMVACutMuon() ){
    if( muonPtr->closestJetDeepFlavor() > 0.025 ) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


bool MuonSelector::isFO2016PreVFP() const{
    if( muonPtr->leptonMVAttH() <= leptonMVACutMuon() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025 ) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


bool MuonSelector::isFO2016PostVFP() const{
    if( muonPtr->leptonMVAttH() <= leptonMVACutMuon() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025 ) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


bool MuonSelector::isFO2017() const{
    if( muonPtr->leptonMVAttH() <= leptonMVACutMuon() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025 ) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


bool MuonSelector::isFO2018() const{
    if( muonPtr->leptonMVAttH() <= leptonMVACutMuon() ){
        if( muonPtr->closestJetDeepFlavor() > 0.025 ) return false;
        if( muonPtr->ptRatio() <= 0.45 ) return false;
    }
    return true;
}


/*
tight muon selection
*/

bool MuonSelector::isTightBase() const{
    if( !isFO() ) return false;
    if( muonPtr->leptonMVAttH() <= leptonMVACutMuon() ) return false;
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


/*
cone correction
*/


double MuonSelector::coneCorrection() const{
    return ( 0.9 / muonPtr->ptRatio() );
}
