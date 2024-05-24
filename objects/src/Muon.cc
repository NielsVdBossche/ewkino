#include "../interface/Muon.h"

//include other parts of framework
#include "../interface/MuonSelector.h"


Muon::Muon( const TreeReader& treeReader, const unsigned leptonIndex ):
    LightLepton( treeReader, leptonIndex, new MuonSelector( this ) ),
    _segmentCompatibility( treeReader._lMuonSegComp[leptonIndex] ),
    _trackPt( treeReader._lMuonTrackPt[leptonIndex] ),
    _trackPtError( treeReader._lMuonTrackPtErr[leptonIndex] ),
    _relIso0p4DeltaBeta( treeReader._relIso0p4MuDeltaBeta[leptonIndex] ),
    _isLoosePOGMuon( treeReader._lPOGLoose[leptonIndex] ),
    _isMediumPOGMuon( treeReader._lPOGMedium[leptonIndex] ),
    _isTightPOGMuon( treeReader._lPOGTight[leptonIndex] )
{
    setLorentzVector( treeReader._lPtCorr[ leptonIndex ], eta(), phi(), 
                treeReader._lECorr[ leptonIndex ] );

}

Muon::Muon( const NanoReader& nanoReader, const unsigned leptonIndex ):
    LightLepton( nanoReader.GetMuonReader(), leptonIndex, new MuonSelector(this) ),
    _segmentCompatibility( nanoReader._Muon_segmentComp[leptonIndex] ),
    //_trackPt( treeReader._lMuonTrackPt[leptonIndex] ), // does not seem to exist in nanoAOD
    _trackPtError( nanoReader._Muon_ptErr[leptonIndex] ),
    _relIso0p4DeltaBeta( nanoReader._Muon_pfRelIso04_all[leptonIndex] ),
    _isLoosePOGMuon( nanoReader._Muon_looseId[leptonIndex] ),
    _isMediumPOGMuon( nanoReader._Muon_mediumId[leptonIndex] ),
    _isTightPOGMuon( nanoReader._Muon_tightId[leptonIndex] ),
    _isGlobal( nanoReader._Muon_isGlobal[leptonIndex] ),
    _isTracker( nanoReader._Muon_isTracker[leptonIndex] ),
    _isStandalone( nanoReader._Muon_isStandalone[leptonIndex] )
{   
    // Roccor applied in nanoSkimming: https://github.com/NielsVdBossche/nanoSkimming/blob/1e690f5b24816ddee5885adcda635a76cf71cb86/condor/condorrun.py#L91-L101    
    _uncorrectedPt = pt();
    _uncorrectedE = energy();

    double energy_corr = _uncorrectedE * nanoReader._Muon_corrected_pt[ leptonIndex ] / _uncorrectedPt;
    setLorentzVector( nanoReader._Muon_corrected_pt[ leptonIndex ], eta(), phi(), 
                energy_corr);

}



Muon::Muon( const Muon& rhs ):
    LightLepton( rhs, new MuonSelector( this ) ),
    _segmentCompatibility( rhs._segmentCompatibility ),
    _trackPt( rhs._trackPt ),
    _trackPtError( rhs._trackPtError ),
    _relIso0p4DeltaBeta( rhs._relIso0p4DeltaBeta ),
    _isLoosePOGMuon( rhs._isLoosePOGMuon ),
    _isMediumPOGMuon( rhs._isMediumPOGMuon ),
    _isTightPOGMuon( rhs._isTightPOGMuon ),
    _isGlobal( rhs._isGlobal ),
    _isTracker( rhs._isTracker ),
    _isStandalone( rhs._isStandalone )
    {}    


Muon::Muon( Muon&& rhs ) noexcept:
    LightLepton( std::move( rhs ), new MuonSelector( this ) ),
    _segmentCompatibility( rhs._segmentCompatibility ),
    _trackPt( rhs._trackPt ),
    _trackPtError( rhs._trackPtError ),
    _relIso0p4DeltaBeta( rhs._relIso0p4DeltaBeta ),
    _isLoosePOGMuon( rhs._isLoosePOGMuon ),
    _isMediumPOGMuon( rhs._isMediumPOGMuon ),
    _isTightPOGMuon( rhs._isTightPOGMuon ),
    _isGlobal( rhs._isGlobal ),
    _isTracker( rhs._isTracker ),
    _isStandalone( rhs._isStandalone )
    {}    


std::ostream& Muon::print( std::ostream& os ) const{
    os << "Muon : ";
    LightLepton::print( os );
    os << " / segmentCompatibility = " << _segmentCompatibility;
    os << " / trackPt = " << _trackPt;
    os << " / trackPtError = " << _trackPtError;
    if( _isTightPOGMuon ){
        os << " / tight POG muon";
    } else if( _isMediumPOGMuon ){
        os << " / medium POG muon";
    } else if( _isLoosePOGMuon ){
        os << " / loose POG muon";
    } else {
        os << " / fails POG muon selection";
    }
    os << " / isGlobal = " << _isGlobal;
    os << " / isTracker = " << _isTracker;
    os << " / isStandalone = " << _isStandalone;
    return os;
}
