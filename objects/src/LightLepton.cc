#include "../interface/LightLepton.h"

//include c++ library classes 
#include <cmath>


// constructor from TreeReader
LightLepton::LightLepton(
    const TreeReader& treeReader,
    const std::string& leptonType,
    const unsigned leptonIndex,
    LeptonSelector* leptonSelector ):
    Lepton( treeReader, leptonType, leptonIndex, leptonSelector)
{
    int jetIdx = -1;
    if(leptonType=="electron"){
	_relIso0p3 = treeReader._Electron_pfRelIso03_all[leptonIndex];
	_relIso0p4 = 0; // seems to be not stored in nanoAOD
	_miniIso = treeReader._Electron_miniPFRelIso_all[leptonIndex];
	_miniIsoCharged = treeReader._Electron_miniPFRelIso_chg[leptonIndex];
	_ptRatio = 1./(treeReader._Electron_jetRelIso[leptonIndex] + 1);
	_ptRel = treeReader._Electron_jetPtRelv2[leptonIndex];
	jetIdx = treeReader._Electron_jetIdx[leptonIndex];
	_sip3d = treeReader._Electron_sip3d[leptonIndex];
    } else if(leptonType=="muon"){
	_relIso0p3 = treeReader._Muon_pfRelIso03_all[leptonIndex];
        _relIso0p4 = 0; // seems to be not stored in nanoAOD
        _miniIso = treeReader._Muon_miniPFRelIso_all[leptonIndex];
        _miniIsoCharged = treeReader._Muon_miniPFRelIso_chg[leptonIndex];
	_ptRatio = 1./(treeReader._Muon_jetRelIso[leptonIndex] + 1);
        _ptRel = treeReader._Muon_jetPtRelv2[leptonIndex];
        jetIdx = treeReader._Muon_jetIdx[leptonIndex];
	_sip3d = treeReader._Muon_sip3d[leptonIndex];
    } else{
        std::string msg = "ERROR in Lepton::Lepton:";
        msg += " unrecognized lepton type " + leptonType;
        throw std::invalid_argument(msg);
    }

    if( jetIdx >= (int)treeReader._nJet ){
	std::string msg = "WARNING in LightLepton constructor:";
	msg += " index of closest jet is " + std::to_string(jetIdx);
	msg += " while only " + std::to_string(treeReader._nJet);
	msg += " jets are present; ignoring closest jet info.";
	std::cerr << msg << std::endl;
    } else if( jetIdx>=0 ){
	_closestJetDeepCSV = treeReader._Jet_bTagDeepB[jetIdx];
        _closestJetDeepFlavor = treeReader._Jet_bTagDeepFlavB[jetIdx];
        _closestJetTrackMultiplicity = treeReader._Jet_nConstituents[jetIdx];
    }

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
    _sip3d( rhs._sip3d ),
    _leptonMVAtZq( rhs._leptonMVAtZq ),
    _leptonMVAttH( rhs._leptonMVAttH ),
    _leptonMVATOP( rhs._leptonMVATOP ),
    _leptonMVATOPUL( rhs._leptonMVATOPUL ),
    _leptonMVATOPv2UL( rhs._leptonMVATOPv2UL )
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
    _sip3d( rhs._sip3d ),
    _leptonMVAtZq( rhs._leptonMVAtZq ),
    _leptonMVAttH( rhs._leptonMVAttH ),
    _leptonMVATOP( rhs._leptonMVATOP ),
    _leptonMVATOPUL( rhs._leptonMVATOPUL ),
    _leptonMVATOPv2UL( rhs._leptonMVATOPv2UL )
    {}


std::ostream& LightLepton::print( std::ostream& os ) const{
    Lepton::print( os );
    os << " / relIso 0.3 = " << _relIso0p3; 
    os << " / relIso 0.4 = " << _relIso0p4;
    os << " / miniIso = " << _miniIso;
    os << " / miniIsoCharged = " << _miniIsoCharged;
    os << " / ptRatio = " << _ptRatio;
    os << " / ptRel = " << _ptRel;
    os << " / closestJetDeepCSV = " << _closestJetDeepCSV;
    os << " / closestJetDeepFlavor = " << _closestJetDeepFlavor;
    os << " / closestJetTrackMultiplicity = " << _closestJetTrackMultiplicity;
    os << " / sip3d = " << _sip3d;
    os << " / leptonMVAtZq = " << _leptonMVAtZq;
    os << " / leptonMVAttH = " << _leptonMVAttH;
    os << " / leptonMVATOP = " << _leptonMVATOP;
    os << " / leptonMVATOPUL = " << _leptonMVATOPUL;
    os << " / leptonMVATOPv2UL = " << _leptonMVATOPv2UL;
    return os;
}
