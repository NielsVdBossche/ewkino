#include "../interface/LightLepton.h"

//include c++ library classes 
#include <cmath>


// constructor from explicitly passed values
/*LightLepton::LightLepton( const double pt, const double eta, const double phi, const double e,
                const bool is2016, const bool is2016PreVFP, const bool is2016PostVFP,
                const bool is2017, const bool is2018,
                LeptonSelector* leptonSelector,
                const int charge, const double dxy, const double dz, const double sip3d,
                LeptonGeneratorInfo* leptonGeneratorInfo,
		const double relIso0p3, const double relIso0p4,
		const double miniIso, const double miniIsoCharged,
		const double ptRatio, const double ptRel,
		const double closestJetDeepCSV, const double closestJetDeepFlavor,
		const double closestJetTrackMultiplicity,
		const double leptonMVATOPUL, const double leptonMVATOPv2UL ):
    Lepton( pt, eta, phi, e, is2016, is2016PreVFP, is2016PostVFP, is2017, is2018,
            leptonSelector, charge, dxy, dz, sip3d, leptonGeneratorInfo ),
    _relIso0p3( relIso0p3 ),
    _relIso0p4( relIso0p4 ), 
    _miniIso( miniIso ),
    _miniIsoCharged( miniIsoCharged ),
    _ptRatio( ptRatio ),
    _ptRel( ptRel ), 
    _closestJetDeepCSV( closestJetDeepCSV ), 
    _closestJetDeepFlavor( closestJetDeepFlavor ),
    _closestJetTrackMultiplicity( closestJetTrackMultiplicity ),
    _leptonMVATOPUL( leptonMvaTOPUL ),
    _leptonMVATOPv2UL( leptonMvaTOPv2UL )
    {}*/


// constructor from TreeReader
LightLepton::LightLepton(
    const TreeReader& treeReader,
    const std::string& leptonType,
    const unsigned leptonIndex,
    LeptonSelector* leptonSelector ):
    Lepton( treeReader, leptonType, leptonIndex, leptonSelector)
{
    if(leptonType=="electron"){
	/*_relIso0p3 = 
	_relIso0p4 = 
	_miniIso = 
	_miniIsoCharged = 
	_ptRatio = 
	_ptRel = 
	_closestJetDeepCSV = 
	_closestJetDeepFlavor = 
	_closestJetTrackMultiplicity = 
	_leptonMVATOPUL = 
	_leptonMVATOPv2UL = */
    } else if(leptonType=="muon"){
	/*_relIso0p3 = 
        _relIso0p4 = 
        _miniIso = 
        _miniIsoCharged = 
        _ptRatio = 
        _ptRel = 
        _closestJetDeepCSV = 
        _closestJetDeepFlavor = 
        _closestJetTrackMultiplicity = 
        _leptonMVATOPUL = 
        _leptonMVATOPv2UL = */
    } else{
        std::string msg = "ERROR in Lepton::Lepton:";
        msg += " unrecognized lepton type " + leptonType;
        throw std::invalid_argument(msg);
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
    _leptonMVAtZq( rhs._leptonMVAtZq ),
    _leptonMVAttH( rhs._leptonMVAttH ),
    _leptonMVATOP( rhs._leptonMVATOP ),
    _leptonMVATOPUL( rhs._leptonMVATOPUL ),
    _leptonMVATOPv2UL( rhs._leptonMVATOPv2UL )
    {}


std::ostream& LightLepton::print( std::ostream& os ) const{
    Lepton::print( os );
    os << " relIso 0.3 = " << _relIso0p3; 
    os << " / relIso 0.4 = " << _relIso0p4;
    os << " / miniIso = " << _miniIso;
    os << " / miniIsoCharged = " << _miniIsoCharged;
    os << " / ptRatio = " << _ptRatio;
    os << " / ptRel = " << _ptRel;
    os << " / closestJetDeepCSV = " << _closestJetDeepCSV;
    os << " / closestJetDeepFlavor = " << _closestJetDeepFlavor;
    os << " / closestJetTrackMultiplicity = " << _closestJetTrackMultiplicity;
    os << " / leptonMVAtZq = " << _leptonMVAtZq;
    os << " / leptonMVAttH = " << _leptonMVAttH;
    os << " / leptonMVATOP = " << _leptonMVATOP;
    os << " / leptonMVATOPUL = " << _leptonMVATOPUL;
    os << " / leptonMVATOPv2UL = " << _leptonMVATOPv2UL;
    return os;
}
