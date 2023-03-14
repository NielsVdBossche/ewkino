#include "../interface/LeptonGeneratorInfo.h"

// internal helper functions

bool considerForMatching( 
    int lepPdgId, 
    const TreeReader& treeReader, 
    int genIndex, 
    bool allowPhotons ){
    // decide whether to consider a given generator particle for matching to a given lepton.
    // copied from ntuplizer
    // (https://github.com/GhentAnalysis/heavyNeutrino/blob/UL_master/multilep/src/GenTools.cc)

    // get properties of gen particle
    int genPdgId = treeReader._GenPart_pdgId[genIndex];
    int genStatus = treeReader._GenPart_status[genIndex];
    std::bitset<15> genStatusBits = std::bitset<15>( treeReader._GenPart_statusFlags[genIndex] );
    bool genIsLastCopy = genStatusBits[13];

    // standard criterion: reject if different pdg id
    if( !allowPhotons || std::abs(genPdgId) != 22 ){
	if( std::abs(lepPdgId) != std::abs(genPdgId) ) return false;
    }
    // for taus, allow special status
    if( std::abs(lepPdgId)==15 && std::abs(genPdgId)==15 ){
	return (genStatus==2 && genIsLastCopy);
    }
    // return true if stable
    return (genStatus==1);
}


int findGeometricMatch(
    const TreeReader& treeReader,
    const std::string& leptonType,
    const unsigned leptonIndex,
    bool allowPhotons=false ){
    // find a geometric gen match for a given lepton,
    // in case the builtin matching does not point to a valid index
    // copied from ntuplizer
    // (https://github.com/GhentAnalysis/heavyNeutrino/blob/UL_master/multilep/src/GenTools.cc)
    LorentzVector lepvec;
    int absPdgToMatch = 0;
    if(leptonType=="electron"){
        lepvec = LorentzVector( 
	    treeReader._Electron_pt[leptonIndex],
	    treeReader._Electron_eta[leptonIndex],
	    treeReader._Electron_phi[leptonIndex],
	    0. );
	absPdgToMatch = 11;
    } else if(leptonType=="muon"){
        lepvec = LorentzVector( 
            treeReader._Muon_pt[leptonIndex],
            treeReader._Muon_eta[leptonIndex],
            treeReader._Muon_phi[leptonIndex],
            0. );
	absPdgToMatch = 13;
    } else if(leptonType=="tau"){
        lepvec = LorentzVector( 
            treeReader._Tau_pt[leptonIndex],
            treeReader._Tau_eta[leptonIndex],
            treeReader._Tau_phi[leptonIndex],
            0. );
	absPdgToMatch = 15;
    }
    int matchIndex = -1;
    double minDeltaR = 99.;
    for( unsigned int genIndex=0; genIndex<treeReader._nGenPart; genIndex++ ){
	if( !considerForMatching(absPdgToMatch, treeReader, genIndex, allowPhotons) ) continue;
	LorentzVector genvec = LorentzVector(
            treeReader._GenPart_pt[genIndex],
            treeReader._GenPart_eta[genIndex],
            treeReader._GenPart_phi[genIndex],
            0. );
	double dr = deltaR(lepvec, genvec);
	if( dr < minDeltaR ){
	    matchIndex = genIndex;
	    minDeltaR = dr;
	}
    }
    if( minDeltaR > 0.2 ){
	if(!allowPhotons) matchIndex = findGeometricMatch(treeReader, leptonType, leptonIndex, true);
	else return -1;
    }
    return matchIndex;
}


LeptonGeneratorInfo::LeptonGeneratorInfo( 
    const TreeReader& treeReader, 
    const std::string& leptonType,
    const unsigned leptonIndex ){

    int genIdx = -1;
    int recoPdgId = 0;
    if(leptonType=="electron"){
	genIdx = treeReader._Electron_genPartIdx[leptonIndex];
	recoPdgId = treeReader._Electron_charge[leptonIndex] * (-11);
    } else if(leptonType=="muon"){
	genIdx = treeReader._Muon_genPartIdx[leptonIndex];
	recoPdgId = treeReader._Muon_charge[leptonIndex] * (-13);
    } else if(leptonType=="tau"){
	genIdx = treeReader._Tau_genPartIdx[leptonIndex];
	recoPdgId = treeReader._Tau_charge[leptonIndex] * (-15);
    } else{
	std::string msg = "ERROR in LeptonGeneratorInfo constructor:";
	msg += "unrecognized lepton type " + leptonType;
	throw std::invalid_argument(msg);
    }

    if( genIdx >= (int)treeReader._nGenPart ){
        std::string msg = "WARNING in LeptonGeneratorConstructor constructor:";
        msg += " index of gen particle is " + std::to_string(genIdx);
        msg += " while only " + std::to_string(treeReader._nGenPart);
        msg += " gen particles are present; ignoring gen info.";
        std::cerr << msg << std::endl;
	return;
    }

    if( genIdx<0 || treeReader._GenPart_pdgId[genIdx]!=recoPdgId ){
	genIdx = findGeometricMatch(treeReader, leptonType, leptonIndex);
	if( genIdx<0 ) return;
    }

    _hasGenMatch = true;
    std::bitset<15> statusBits = std::bitset<15>( treeReader._GenPart_statusFlags[genIdx] );
    // see definition of status bits here:
    // https://cms-nanoaod-integration.web.cern.ch/autoDoc/
    _isPrompt = (statusBits[0] // isPrompt
		|| statusBits[5] // isDirectPromptTauDecayProduct
		|| statusBits[7] // isHardProcess
		|| statusBits[8] // fromHardProcess
		|| statusBits[11] // fromHardProcessBeforeFSR 
		);
    _matchPdgId = treeReader._GenPart_pdgId[genIdx];
    _matchCharge = (_matchPdgId==11 || _matchPdgId==13 || _matchPdgId==15) ? -1 :
		   (_matchPdgId==-11 || _matchPdgId==-13 || _matchPdgId==-15) ? 1 : 0;
    int momIdx = treeReader._GenPart_genPartIdxMother[genIdx];
    if( momIdx >= (int)treeReader._nGenPart ){
        std::string msg = "WARNING in LeptonGeneratorConstructor constructor:";
        msg += " index of mother particle is " + std::to_string(momIdx);
        msg += " while only " + std::to_string(treeReader._nGenPart);
        msg += " gen particles are present; ignoring mother info.";
        std::cerr << msg << std::endl;
    } else if(momIdx>=0){ _momPdgId = treeReader._GenPart_pdgId[momIdx]; }
}

// following variables do not seem to be directly defined at nanoAOD level;
// maybe implementable as functions operating on the GenPart info (to do later).
/*    _provenance( treeReader._lProvenance[leptonIndex] ), 
    _provenanceCompressed( treeReader._lProvenanceCompressed[leptonIndex] ),
    _provenanceConversion( treeReader._lProvenanceConversion[leptonIndex] ) */
