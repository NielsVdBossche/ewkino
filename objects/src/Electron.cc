#include "../interface/Electron.h"
#include "../interface/ElectronSelector.h"


Electron::Electron( const TreeReader& treeReader, const unsigned electronIndex ):
    LightLepton( treeReader, "electron", electronIndex, new ElectronSelector(this) ),
    _passConversionVeto( treeReader._Electron_convVeto[electronIndex] ),
    _numberOfMissingHits( treeReader._Electron_lostHits[electronIndex] ),
    _electronMVAFall17Iso( treeReader._Electron_mvaFall17V2Iso[electronIndex] ),
    _electronMVAFall17NoIso( treeReader._Electron_mvaFall17V2noIso[electronIndex] ),
    _passElectronMVAFall17NoIsoLoose( treeReader._Electron_mvaFall17V2noIso_WPL[electronIndex] ), 
    _passElectronMVAFall17NoIsoWP90( treeReader._Electron_mvaFall17V2noIso_WP90[electronIndex] ),
    _passElectronMVAFall17NoIsoWP80( treeReader._Electron_mvaFall17V2noIso_WP80[electronIndex] ),
    _hOverE( treeReader._Electron_hoe[electronIndex] ),
    _inverseEMinusInverseP( treeReader._Electron_eInvMinusPInv[electronIndex] ),
    _sigmaIEtaEta( treeReader._Electron_sieie[electronIndex] )
{
    _passChargeConsistency = ( treeReader._Electron_tightCharge[electronIndex]==2 );
    _etaSuperCluster = eta() + treeReader._Electron_deltaEtaSC[electronIndex];
    _isPFCandidate = treeReader._Electron_isPFCand[electronIndex];
    _isVetoPOGElectron = ( treeReader._Electron_cutBased[electronIndex]>=1 );
    _isLoosePOGElectron = ( treeReader._Electron_cutBased[electronIndex]>=2 );
    _isMediumPOGElectron = ( treeReader._Electron_cutBased[electronIndex]>=3 );
    _isTightPOGElectron = ( treeReader._Electron_cutBased[electronIndex]==4 );
    // documentation on scale and resolution variations is not clear;
    // current implementation is based on what was done in miniAOD ntuplizer
    // (https://github.com/GhentAnalysis/heavyNeutrino/blob/
    //  36671d7943a77b4fa8bc2d42d78fd6be07487409/multilep/src/LeptonAnalyzer.cc#L343)
    // using the input variables as defined here:
    // https://github.com/cms-sw/cmssw/blob/bdc7d19ab5a25b724b78716513ef41070b717616/
    // PhysicsTools/NanoAOD/python/electrons_cff.py#L374
    double energy = momentum(); // energy does not seem to be stored in nanoAOD, use proxy.
    _pt_ScaleUp = pt() * (energy-treeReader._Electron_dEscaleUp[electronIndex])/energy;
    _pt_ScaleDown = pt() * (energy-treeReader._Electron_dEscaleDown[electronIndex])/energy;
    _pt_ResUp = pt() * (energy-treeReader._Electron_dEsigmaUp[electronIndex])/energy;
    _pt_ResDown = pt() * (energy-treeReader._Electron_dEsigmaDown[electronIndex])/energy;
    _e_ScaleUp = (energy-treeReader._Electron_dEscaleUp[electronIndex]);
    _e_ScaleDown = (energy-treeReader._Electron_dEscaleDown[electronIndex]);
    _e_ResUp = (energy-treeReader._Electron_dEsigmaUp[electronIndex]);
    _e_ResDown = (energy-treeReader._Electron_dEsigmaDown[electronIndex]);

    // evaluate lepton MVAs on the fly
    // (for MVAs not stored in default nanoAOD)
    // note: cannot be done in LightLepton constructor,
    // because some input variables are only defined/set in Electron constructor.
    std::vector<std::string> mvaids = treeReader.leptonMVAReaderIDs();
    for( std::string mvaid : mvaids ){
	double mvascore = treeReader.leptonMVAReader(mvaid)->leptonMVAElectron(this);
	if( mvaid=="TOP-UL" ) _leptonMVATOPUL = mvascore;
	else if( mvaid=="TOPv2-UL" ) _leptonMVATOPv2UL = mvascore;
	else{
	    std::string msg = "ERROR in Electron constructor:";
	    msg.append(" lepton MVA with ID " + mvaid + " not yet implemented.");
	    throw std::runtime_error(msg);
	}
    }
}


Electron::Electron( const Electron& rhs ) :
    LightLepton( rhs, new ElectronSelector( this ) ),
    _passChargeConsistency( rhs._passChargeConsistency ),
    _passConversionVeto( rhs._passConversionVeto ),
    _numberOfMissingHits( rhs._numberOfMissingHits ),
    _electronMVAFall17Iso( rhs._electronMVAFall17Iso ),
    _electronMVAFall17NoIso( rhs._electronMVAFall17NoIso ),
    _passElectronMVAFall17NoIsoLoose( rhs._passElectronMVAFall17NoIsoLoose ),
    _passElectronMVAFall17NoIsoWP90( rhs._passElectronMVAFall17NoIsoWP90 ),
    _passElectronMVAFall17NoIsoWP80( rhs._passElectronMVAFall17NoIsoWP80 ),
    _etaSuperCluster( rhs._etaSuperCluster ),
    _hOverE( rhs._hOverE ),
    _inverseEMinusInverseP( rhs._inverseEMinusInverseP ),
    _sigmaIEtaEta( rhs._sigmaIEtaEta ),
    _isPFCandidate( rhs._isPFCandidate ),
    _isVetoPOGElectron( rhs._isVetoPOGElectron ),
    _isLoosePOGElectron( rhs._isLoosePOGElectron ),
    _isMediumPOGElectron( rhs._isMediumPOGElectron ),
    _isTightPOGElectron( rhs._isTightPOGElectron ),
    _pt_ScaleUp( rhs._pt_ScaleUp ),
    _pt_ScaleDown( rhs._pt_ScaleDown ),
    _pt_ResUp( rhs._pt_ResUp ),
    _pt_ResDown( rhs._pt_ResDown ),
    _e_ScaleUp( rhs._e_ScaleUp ),
    _e_ScaleDown( rhs._e_ScaleDown ),     
    _e_ResUp( rhs._e_ResUp ),
    _e_ResDown( rhs._e_ResDown )
    {}


Electron::Electron( Electron&& rhs ) noexcept : 
    LightLepton( std::move( rhs ), new ElectronSelector( this ) ),
    _passChargeConsistency( rhs._passChargeConsistency ),
    _passConversionVeto( rhs._passConversionVeto ),
    _numberOfMissingHits( rhs._numberOfMissingHits ),
    _electronMVAFall17Iso( rhs._electronMVAFall17Iso ),
    _electronMVAFall17NoIso( rhs._electronMVAFall17NoIso ),
    _passElectronMVAFall17NoIsoLoose( rhs._passElectronMVAFall17NoIsoLoose ),
    _passElectronMVAFall17NoIsoWP90( rhs._passElectronMVAFall17NoIsoWP90 ),
    _passElectronMVAFall17NoIsoWP80( rhs._passElectronMVAFall17NoIsoWP80 ),
    _etaSuperCluster( rhs._etaSuperCluster ),
    _hOverE( rhs._hOverE ),
    _inverseEMinusInverseP( rhs._inverseEMinusInverseP ),
    _sigmaIEtaEta( rhs._sigmaIEtaEta ),
    _isPFCandidate( rhs._isPFCandidate ),
    _isVetoPOGElectron( rhs._isVetoPOGElectron ),
    _isLoosePOGElectron( rhs._isLoosePOGElectron ),
    _isMediumPOGElectron( rhs._isMediumPOGElectron ),
    _isTightPOGElectron( rhs._isTightPOGElectron ),
    _pt_ScaleUp( rhs._pt_ScaleUp ),
    _pt_ScaleDown( rhs._pt_ScaleDown ),     
    _pt_ResUp( rhs._pt_ResUp ),
    _pt_ResDown( rhs._pt_ResDown ),
    _e_ScaleUp( rhs._e_ScaleUp ),
    _e_ScaleDown( rhs._e_ScaleDown ),  
    _e_ResUp( rhs._e_ResUp ),
    _e_ResDown( rhs._e_ResDown )
    {}


std::ostream& Electron::print( std::ostream& os ) const{
    os << "Electron : ";
    LightLepton::print( os );
    os << " / passChargeConsistency = " << _passChargeConsistency;
    os << " / passConversionVeto = " << _passConversionVeto;
    os << " / numberOfMissingHits = " << _numberOfMissingHits;
    os << " / electronMVAFall17Iso = " << _electronMVAFall17Iso;
    os << " / electronMVAFall17NoIso = " << _electronMVAFall17NoIso;
    os << " / passElectronMVAFall17NoIsoLoose = " << _passElectronMVAFall17NoIsoLoose;
    os << " / passElectronMVAFall17NoIsoWP90 = " << _passElectronMVAFall17NoIsoWP90;
    os << " / passElectronMVAFall17NoIsoWP80 = " << _passElectronMVAFall17NoIsoWP80;
    os << " / etaSuperCluster = " << _etaSuperCluster;
    os << " / hOverE = " << _hOverE;
    os << " / inverseEMinusInverseP = " << _inverseEMinusInverseP;
    os << " / sigmaIEtaEta = " << _sigmaIEtaEta;
    os << " / isPFCandidate = " << _isPFCandidate;
    if( _isTightPOGElectron ){
        os << " / tight POG electron";
    } else if( _isMediumPOGElectron ){
        os << " / medium POG electron";
    } else if( _isLoosePOGElectron ){
        os << " / loose POG electron";
    } else if( _isVetoPOGElectron ){
        os << " / veto POG electron";
    } else{
        os << " / fails POG electron selection";
    }
    os << " / ptScaleUp " << _pt_ScaleUp;
    os << " / ptScaleDown " << _pt_ScaleDown;
    os << " / ptResUp " << _pt_ResUp;
    os << " / ptResDown " << _pt_ResDown;
    os << " / eScaleUp " << _e_ScaleUp;
    os << " / eScaleDown " << _e_ScaleDown;
    os << " / eResUp " << _e_ResUp;
    os << " / eResDown " << _e_ResDown;

    return os;    
}


Electron Electron::variedElectron(const double newPt, const double newE) const{
    Electron variedElectron( *this );
    // the variations must be determined relative w.r.t. the non-cone-corrected pt!
    double relNewPt = newPt/uncorrectedPt();
    double relNewE = newE/uncorrectedE();    
    variedElectron.setLorentzVector( relNewPt*pt(), eta(), phi(), relNewE*energy() );
    return variedElectron;
}


Electron Electron::electronScaleDown() const{
    return variedElectron( _pt_ScaleDown, _e_ScaleDown );
}


Electron Electron::electronScaleUp() const{
    return variedElectron( _pt_ScaleUp, _e_ScaleUp );
}


Electron Electron::electronResDown() const{
    return variedElectron( _pt_ResDown, _e_ResDown );
}


Electron Electron::electronResUp() const{
    return variedElectron( _pt_ResUp, _e_ResUp );
}
