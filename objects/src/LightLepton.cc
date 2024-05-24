#include "../interface/LightLepton.h"

//include c++ library classes 
#include <cmath>


LightLepton::LightLepton( const TreeReader& treeReader, const unsigned leptonIndex, LeptonSelector* leptonSelector ) :
    Lepton( treeReader, leptonIndex, leptonSelector ),
    _relIso0p3( treeReader._relIso[leptonIndex] ),
    _relIso0p4( treeReader._relIso0p4[leptonIndex] ), 
    _miniIso( treeReader._miniIso[leptonIndex] ),
    _miniIsoCharged( treeReader._miniIsoCharged[leptonIndex] ),
    _ptRatio( treeReader._ptRatio[leptonIndex] ),
    _ptRel( treeReader._ptRel[leptonIndex] ), 
    _closestJetDeepCSV( treeReader._closestJetDeepCsv_b[leptonIndex] + treeReader._closestJetDeepCsv_bb[leptonIndex] ), 
    _closestJetDeepFlavor( treeReader._closestJetDeepFlavor_b[leptonIndex] + treeReader._closestJetDeepFlavor_bb[leptonIndex] + treeReader._closestJetDeepFlavor_lepb[leptonIndex] ),
    _closestJetTrackMultiplicity( treeReader._selectedTrackMult[leptonIndex] ),
    _sip3d( treeReader._3dIPSig[leptonIndex] ),
    _leptonMVATOPUL( treeReader._leptonMvaTOPUL[leptonIndex] ),
    _leptonMVATOPULv2( treeReader._leptonMvaTOPULv2[leptonIndex] )
{

    //catch nan deep CSV values 
    if( std::isnan( _closestJetDeepCSV ) ){
        _closestJetDeepCSV = 0.;

    //catch default values in deep CSV
    } else if( _closestJetDeepCSV < 0. ){
        _closestJetDeepCSV = 0.;
    }

    //catch nan deep Flavor values
    if( std::isnan( _closestJetDeepFlavor ) ){
        _closestJetDeepFlavor = 0.;

    //catch default values in deep Flavor
    } else if( _closestJetDeepFlavor < 0. ){
        _closestJetDeepFlavor = 0.;
    }
}

LightLepton::LightLepton(const NanoReader::LightLeptonReader& leptonReader, const unsigned leptonIndex, LeptonSelector* leptonSelector) : 
    Lepton(leptonReader, leptonIndex, leptonSelector),
    _relIso0p3( leptonReader._Lepton_pfRelIso03_all[leptonIndex] ),
    _miniIso( leptonReader._Lepton_miniPFRelIso_all[leptonIndex] ),
    _miniIsoCharged( leptonReader._Lepton_miniPFRelIso_chg[leptonIndex] ),
    _ptRatio( leptonReader._Lepton_jetPtRatio[leptonIndex] ),
    _ptRel( leptonReader._Lepton_jetPtRelv2[leptonIndex] ), 
    _closestJetNumberOfChargedDaughters( leptonReader._Lepton_jetNDauCharged[leptonIndex]),
    _jetIdx( leptonReader._Lepton_jetIdx[leptonIndex] ),
    _sip3d( leptonReader._Lepton_sip3d[leptonIndex] ),
    _isPFCandidate( leptonReader._Lepton_isPFCand[leptonIndex] ),
    _leptonMVATOPUL( leptonReader._Lepton_TOPLeptonMVAUL[leptonIndex] ),
    _closestJetDeepFlavor( leptonReader._Lepton_jetBTagDJ[leptonIndex] )
{
    if (leptonSelector->isElectronSelector()) {
        _relIso0p4 = 0;  // seems to be not stored in nanoAOD
    } else if (leptonSelector->isMuonSelector()) {
        _relIso0p4 = leptonReader.GetNanoReader()._Muon_pfRelIso04_all[leptonIndex];
    } else {
        std::cerr << "Lightlepton constructor handed non-light selector" << std::endl;
        throw 1;
    }

    if (_jetIdx >= (int)leptonReader.GetNanoReader()._nJet) {
        std::string msg = "WARNING in LightLepton constructor:";
        msg += " index of closest jet is " + std::to_string(_jetIdx);
        msg += " while only " + std::to_string(leptonReader.GetNanoReader()._nJet);
        msg += " jets are present; ignoring closest jet info.";
        std::cerr << msg << std::endl;
    } else if (_jetIdx >= 0) {
        // _closestJetDeepCSV = leptonReader.GetNanoReader()._Jet_bTagDeepB[_jetIdx];
        // _closestJetDeepFlavor = leptonReader.GetNanoReader()._Jet_bTagDeepFlavB[_jetIdx];
        _closestJetTrackMultiplicity = leptonReader.GetNanoReader()._Jet_nConstituents[_jetIdx];
        // (note: nConstituents is seen to be very different from trackMultiplicity;
        // keep this variable as a proxy for now, but numerical thresholds will have to be adjusted!)
    }

    // catch nan deep CSV values
    if (std::isnan(_closestJetDeepCSV)) {
        _closestJetDeepCSV = 0.;

        // catch default values in deep CSV
    } else if (_closestJetDeepCSV < 0.) {
        _closestJetDeepCSV = 0.;
    }

    // catch nan deep Flavor values
    if (std::isnan(_closestJetDeepFlavor)) {
        _closestJetDeepFlavor = 0.;

        // catch default values in deep Flavor
    } else if (_closestJetDeepFlavor < 0.) {
        _closestJetDeepFlavor = 0.;
    }
}

LightLepton::LightLepton( const LightLepton& rhs, LeptonSelector* leptonSelector ):
    Lepton( rhs, leptonSelector ),
    _relIso0p3( rhs._relIso0p3 ),
    _relIso0p4( rhs._relIso0p4 ),
    _miniIso( rhs._miniIso ),
    _miniIsoCharged( rhs._miniIsoCharged ),
    _ptRatio( rhs._ptRatio ),
    _ptRel( rhs._ptRel ),
    _closestJetDeepCSV( rhs._closestJetDeepCSV ),
    _closestJetDeepFlavor( rhs._closestJetDeepFlavor ),
    _closestJetTrackMultiplicity( rhs._closestJetTrackMultiplicity ),
    _closestJetNumberOfChargedDaughters( rhs._closestJetNumberOfChargedDaughters ),
    _jetIdx( rhs._jetIdx ),
    _sip3d( rhs._sip3d ),
    _isPFCandidate( rhs._isPFCandidate),
    _leptonMVATOPUL( rhs._leptonMVATOPUL),
    _leptonMVATOPULv2( rhs._leptonMVATOPULv2)
    {}


LightLepton::LightLepton( LightLepton&& rhs, LeptonSelector* leptonSelector ):
    Lepton( std::move( rhs ), leptonSelector ),
    _relIso0p3( rhs._relIso0p3 ),
    _relIso0p4( rhs._relIso0p4 ),
    _miniIso( rhs._miniIso ),
    _miniIsoCharged( rhs._miniIsoCharged ),
    _ptRatio( rhs._ptRatio ),
    _ptRel( rhs._ptRel ),
    _closestJetDeepCSV( rhs._closestJetDeepCSV ),
    _closestJetDeepFlavor( rhs._closestJetDeepFlavor ),
    _closestJetTrackMultiplicity( rhs._closestJetTrackMultiplicity ),
    _closestJetNumberOfChargedDaughters( rhs._closestJetNumberOfChargedDaughters ),
    _jetIdx( rhs._jetIdx ),
    _sip3d( rhs._sip3d ),
    _isPFCandidate( rhs._isPFCandidate),
    _leptonMVATOPUL( rhs._leptonMVATOPUL),
    _leptonMVATOPULv2( rhs._leptonMVATOPULv2)
    {}


std::ostream& LightLepton::print( std::ostream& os ) const{
    Lepton::print( os );
    os << " relIso 0.3 = " << _relIso0p3 << " / relIso 0.4 = " << _relIso0p4 << " / miniIso = " << _miniIso << " / miniIsoCharged = " << _miniIsoCharged << " / ptRatio = " << _ptRatio << " / ptRel = " << _ptRel << " / closestJetDeepCSV = " << _closestJetDeepCSV << " / closestJetDeepFlavor = " << _closestJetDeepFlavor << " / closestJetTrackMultiplicity = " << _closestJetTrackMultiplicity;
    os << " / isPFCandidate = " << _isPFCandidate;
    return os;
}
