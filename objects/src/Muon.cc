#include "../interface/Muon.h"
#include "../interface/MuonSelector.h"


Muon::Muon( const TreeReader& treeReader, const unsigned muonIndex ):
    LightLepton( treeReader, "muon", muonIndex, new MuonSelector(this) )
    /*_segmentCompatibility( treeReader._lMuonSegComp[muonIndex] ),
    _trackPt( treeReader._lMuonTrackPt[muonIndex] ),
    _trackPtError( treeReader._lMuonTrackPtErr[muonIndex] ),
    _relIso0p4DeltaBeta( treeReader._relIso0p4MuDeltaBeta[muonIndex] ),
    _isLoosePOGMuon( treeReader._lPOGLoose[muonIndex] ),
    _isMediumPOGMuon( treeReader._lPOGMedium[muonIndex] ),
    _isTightPOGMuon( treeReader._lPOGTight[muonIndex] )*/
{}


Muon::Muon( const Muon& rhs ):
    LightLepton( rhs, new MuonSelector( this ) ),
    _segmentCompatibility( rhs._segmentCompatibility ),
    _trackPt( rhs._trackPt ),
    _trackPtError( rhs._trackPtError ),
    _relIso0p4DeltaBeta( rhs._relIso0p4DeltaBeta ),
    _isLoosePOGMuon( rhs._isLoosePOGMuon ),
    _isMediumPOGMuon( rhs._isMediumPOGMuon ),
    _isTightPOGMuon( rhs._isTightPOGMuon )
    {}    


Muon::Muon( Muon&& rhs ) noexcept:
    LightLepton( std::move( rhs ), new MuonSelector( this ) ),
    _segmentCompatibility( rhs._segmentCompatibility ),
    _trackPt( rhs._trackPt ),
    _trackPtError( rhs._trackPtError ),
    _relIso0p4DeltaBeta( rhs._relIso0p4DeltaBeta ),
    _isLoosePOGMuon( rhs._isLoosePOGMuon ),
    _isMediumPOGMuon( rhs._isMediumPOGMuon ),
    _isTightPOGMuon( rhs._isTightPOGMuon )
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
    return os;
}
