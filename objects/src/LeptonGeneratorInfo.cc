#include "../interface/LeptonGeneratorInfo.h"

#include "../interface/LorentzVector.h"

bool considerForMatching( int lepPdgId, const NanoReader& treeReader, int genIndex, bool allowPhotons) {
    // decide whether to consider a given generator particle for matching to a given lepton.
    // copied from ntuplizer
    // (https://github.com/GhentAnalysis/heavyNeutrino/blob/UL_master/multilep/src/GenTools.cc)

    // get properties of gen particle
    int genPdgId = treeReader._GenPart_pdgId[genIndex];
    int genStatus = treeReader._GenPart_status[genIndex];
    std::bitset<15> genStatusBits = std::bitset<15>(treeReader._GenPart_statusFlags[genIndex]);
    bool genIsLastCopy = genStatusBits[13];

    // standard criterion: reject if different pdg id
    if (!allowPhotons || std::abs(genPdgId) != 22) {
        if (std::abs(lepPdgId) != std::abs(genPdgId)) return false;
    }
    // for taus, allow special status
    if (std::abs(lepPdgId) == 15 && std::abs(genPdgId) == 15) {
        return (genStatus == 2 && genIsLastCopy);
    }
    // return true if stable
    return (genStatus == 1);
}

int findGeometricMatch( const NanoReader::LeptonReader* leptonReader, bool isElectron, bool isMuon, bool isTau, 
                        const unsigned leptonIndex, bool allowPhotons = false) {
    // find a geometric gen match for a given lepton,
    // in case the builtin matching does not point to a valid index
    // copied from ntuplizer
    // (https://github.com/GhentAnalysis/heavyNeutrino/blob/UL_master/multilep/src/GenTools.cc)
    LorentzVector lepvec;
    int absPdgToMatch = 0;
    if (isElectron) {
        lepvec = LorentzVector(
            leptonReader->_Lepton_pt[leptonIndex],
            leptonReader->_Lepton_eta[leptonIndex],
            leptonReader->_Lepton_phi[leptonIndex],
            0.);
        absPdgToMatch = 11;
    } else if (isMuon) {
        lepvec = LorentzVector(
            leptonReader->_Lepton_pt[leptonIndex],
            leptonReader->_Lepton_eta[leptonIndex],
            leptonReader->_Lepton_phi[leptonIndex],
            0.);
        absPdgToMatch = 13;
    } else if (isTau) {
        lepvec = LorentzVector(
            leptonReader->_Lepton_pt[leptonIndex],
            leptonReader->_Lepton_eta[leptonIndex],
            leptonReader->_Lepton_phi[leptonIndex],
            0.);
        absPdgToMatch = 15;
    }
    const NanoReader& nanoReader = leptonReader->GetNanoReader();
    int matchIndex = -1;
    double minDeltaR = 99.;
    for (unsigned int genIndex = 0; genIndex < nanoReader._nGenPart; genIndex++) {
        if (!considerForMatching(absPdgToMatch, nanoReader, genIndex, allowPhotons)) continue;
        LorentzVector genvec = LorentzVector(
            nanoReader._GenPart_pt[genIndex],
            nanoReader._GenPart_eta[genIndex],
            nanoReader._GenPart_phi[genIndex],
            0.);
        double dr = deltaR(lepvec, genvec);
        if (dr < minDeltaR) {
            matchIndex = genIndex;
            minDeltaR = dr;
        }
    }
    if (minDeltaR > 0.2) {
        if (!allowPhotons)
            matchIndex = findGeometricMatch(leptonReader, isElectron, isMuon, isTau, leptonIndex, true);
        else
            return -1;
    }
    return matchIndex;
}

LeptonGeneratorInfo::LeptonGeneratorInfo( const TreeReader& treeReader, const unsigned leptonIndex ):
    _isPrompt( treeReader._lIsPrompt[leptonIndex] ),
    _matchPdgId( treeReader._lMatchPdgId[leptonIndex] ),
    _matchCharge( treeReader._lMatchCharge[leptonIndex] ),
    _momPdgId( treeReader._lMomPdgId[leptonIndex] ),
    _provenance( treeReader._lProvenance[leptonIndex] ), 
    _provenanceCompressed( treeReader._lProvenanceCompressed[leptonIndex] ),
    _provenanceConversion( treeReader._lProvenanceConversion[leptonIndex] ) 
    {}

LeptonGeneratorInfo::LeptonGeneratorInfo( const NanoReader::LeptonReader* leptonReader, const unsigned leptonindex) {
    // Starting directly from NanoAOD definitions to fix this all.
    if (leptonReader->isLightLeptonReader()) {
        _isPrompt = ((NanoReader::LightLeptonReader&) leptonReader)._Lepton_isPrompt[leptonindex];
        _matchPdgId = ((NanoReader::LightLeptonReader&) leptonReader)._Lepton_matchPdgId[leptonindex];
        _momPdgId = ((NanoReader::LightLeptonReader&) leptonReader)._Lepton_motherPdgId[leptonindex];
        _provenanceConversion = ((NanoReader::LightLeptonReader&) leptonReader)._Lepton_provenanceConversion[leptonindex];
    }
    // use sign of pdgid to extract match charge:
    _matchCharge = (_matchPdgId > 0) ? -1 : (_matchPdgId < 0) ? 1 : 0;
    if (std::abs(_matchPdgId) > 16) _matchCharge = 0;
}


LeptonGeneratorInfo::LeptonGeneratorInfo( const NanoReader::LeptonReader* leptonReader, const unsigned leptonIndex, bool isElectron, bool isMuon, bool isTau) {
    int genIdx = leptonReader->_Lepton_genPartIdx[leptonIndex];
    int recoPdgId = leptonReader->_Lepton_genPartFlav[leptonIndex];

    if (isElectron) {
        recoPdgId *= (-11);
    } else if (isMuon) {
        recoPdgId *= (-13);
    } else if (isTau) {
        recoPdgId *= (-15);
    } else {
        std::string msg = "ERROR in LeptonGeneratorInfo constructor: no leptonType is True";
        throw std::invalid_argument(msg);
    }

    const NanoReader& nanoReader = leptonReader->GetNanoReader();
    // safety check on index of gen particle
    if (genIdx >= (int)nanoReader._nGenPart) {
        std::string msg = "WARNING in LeptonGeneratorConstructor constructor:";
        msg += " index of gen particle is " + std::to_string(genIdx);
        msg += " while only " + std::to_string(nanoReader._nGenPart);
        msg += " gen particles are present; ignoring gen info.";
        std::cerr << msg << std::endl;
        return;
    }

    // if the index of gen particle is not valid,
    // or if the pdgId of the gen particle is not equal to that of the reco particle,
    // try geometric match instead.
    if (genIdx < 0 || nanoReader._GenPart_pdgId[genIdx] != recoPdgId) {
        genIdx = findGeometricMatch(leptonReader, isElectron, isMuon, isTau, leptonIndex);
        if (genIdx < 0) return;
    }
    // temporary alternative for testing: no geometric matching
    // if( genIdx<0 ) return;
    // temporary alternative for testing: only geometric matching
    // genIdx = findGeometricMatch(treeReader, leptonType, leptonIndex);
    // if( genIdx<0 ) return;

    // set basic gen-level variables
    _hasGenMatch = true;
    std::bitset<15> statusBits = std::bitset<15>(nanoReader._GenPart_statusFlags[genIdx]);
    // see definition of status bits here:
    // https://cms-nanoaod-integration.web.cern.ch/autoDoc/
    _isPrompt = (statusBits[0]      // isPrompt
                 || statusBits[5]   // isDirectPromptTauDecayProduct
                 || statusBits[7]   // isHardProcess
                 || statusBits[8]   // fromHardProcess
                 || statusBits[11]  // fromHardProcessBeforeFSR
    );
    _matchPdgId = nanoReader._GenPart_pdgId[genIdx];
    _matchCharge = (_matchPdgId == 11 || _matchPdgId == 13 || _matchPdgId == 15) ? -1 : (_matchPdgId == -11 || _matchPdgId == -13 || _matchPdgId == -15) ? 1
                                                                                                                                                         : 0;

    // set gen-level info of mother particle
    int momIdx = nanoReader._GenPart_genPartIdxMother[genIdx];
    if (momIdx >= (int)nanoReader._nGenPart) {
        std::string msg = "WARNING in LeptonGeneratorConstructor constructor:";
        msg += " index of mother particle is " + std::to_string(momIdx);
        msg += " while only " + std::to_string(nanoReader._nGenPart);
        msg += " gen particles are present; ignoring mother info.";
        std::cerr << msg << std::endl;
    } else if (momIdx >= 0) {
        _momPdgId = nanoReader._GenPart_pdgId[momIdx];
    }

    // set more involved gen-level flags
    // provenanceConversion: see ntuplizer/GenTools.cc
    // (not exactly reproducible however, it seems).
    // - if match particle is no photon: 99
    // - nonprompt photon: 2
    // - fragmentation photon: 1
    // - direct prompt photon: 0
    if (_matchPdgId == 22) {
        if (!statusBits[0]) _provenanceConversion = 2;
        if (nanoReader._GenPart_pt[genIdx] < 10.) _provenanceConversion = 1;
        _provenanceConversion = 0;
    }
}