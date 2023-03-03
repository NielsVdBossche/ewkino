#include "../interface/LeptonGeneratorInfo.h"

LeptonGeneratorInfo::LeptonGeneratorInfo( 
    const TreeReader& treeReader, 
    const std::string& leptonType,
    const unsigned leptonIndex ){

    int genIdx = -1;
    if(leptonType=="electron"){
	genIdx = treeReader._Electron_genPartIdx[leptonIndex];
    } else if(leptonType=="muon"){
	genIdx = treeReader._Muon_genPartIdx[leptonIndex];
    } else if(leptonType=="tau"){
	genIdx = treeReader._Tau_genPartIdx[leptonIndex];
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

    if( genIdx<0 ) return;

    std::bitset<20> statusBits = std::bitset<20>( treeReader._GenPart_statusFlags[genIdx] );
    _isPrompt = statusBits[0];
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
