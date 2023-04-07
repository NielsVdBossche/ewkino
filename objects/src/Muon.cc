#include "../interface/Muon.h"
#include "../interface/MuonSelector.h"


Muon::Muon( const TreeReader& treeReader, const unsigned muonIndex ):
    LightLepton( treeReader, "muon", muonIndex, new MuonSelector(this) ),
    _segmentCompatibility( treeReader._Muon_segmentComp[muonIndex] ),
    //_trackPt( treeReader._lMuonTrackPt[muonIndex] ), // does not seem to exist in nanoAOD
    _trackPtError( treeReader._Muon_ptErr[muonIndex] ),
    _relIso0p4DeltaBeta( treeReader._Muon_pfRelIso04_all[muonIndex] ),
    _isLoosePOGMuon( treeReader._Muon_looseId[muonIndex] ),
    _isMediumPOGMuon( treeReader._Muon_mediumId[muonIndex] ),
    _isTightPOGMuon( treeReader._Muon_tightId[muonIndex] ),
    _isPFCandidate( treeReader._Muon_isPFCand[muonIndex] ),
    _isGlobal( treeReader._Muon_isGlobal[muonIndex] ),
    _isTracker( treeReader._Muon_isTracker[muonIndex] ),
    _isStandalone( treeReader._Muon_isStandalone[muonIndex] )
{
    // evaluate lepton MVAs on the fly
    // (for MVAs not stored in default nanoAOD)
    // note: cannot be done in LightLepton constructor,
    // because some input variables are only defined/set in Muon constructor.
    std::vector<std::string> mvaids = treeReader.leptonMVAReaderIDs();
    for( std::string mvaid : mvaids ){
        double mvascore = treeReader.leptonMVAReader(mvaid)->leptonMVAMuon(this);
        if( mvaid=="TOP-UL" ) _leptonMVATOPUL = mvascore;
        else if( mvaid=="TOPv2-UL" ) _leptonMVATOPv2UL = mvascore;
        else{
            std::string msg = "ERROR in Muon constructor:";
            msg.append(" lepton MVA with ID " + mvaid + " not yet implemented.");
            throw std::runtime_error(msg);
        }
    }
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
    _isPFCandidate( rhs._isPFCandidate ),
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
    _isPFCandidate( rhs._isPFCandidate ),
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
    os << " / isPFCandidate = " << _isPFCandidate;
    os << " / isGlobal = " << _isGlobal;
    os << " / isTracker = " << _isTracker;
    os << " / isStandalone = " << _isStandalone;
    return os;
}
