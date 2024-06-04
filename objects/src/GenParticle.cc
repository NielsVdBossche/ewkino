#include "../interface/GenParticle.h"

GenParticle::GenParticle(const TreeReader& treeReader, const unsigned genIndex) :
    PhysicsObject(treeReader._gen_pt[genIndex], treeReader._gen_eta[genIndex], treeReader._gen_phi[genIndex], treeReader._gen_E[genIndex], treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(), treeReader.is2017(), treeReader.is2018()),
    _genPdgID(treeReader._gen_pdgId[genIndex]),
    _genCharge(treeReader._gen_charge[genIndex]),
    _genStatus(treeReader._gen_status[genIndex]),
    _genIsPromptFinalState(treeReader._gen_isPromptFinalState[genIndex]),
    _genIsDirectPromptTauDecayProductFinalState(treeReader._gen_isDirectPromptTauDecayProductFinalState[genIndex]),
    _genIsLastCopy(treeReader._gen_isLastCopy[genIndex]),
    _genIndex(genIndex)
{ 
    //// initialize array of daughter indices -> Can we exchange these for pointers?
    
}

GenParticle::GenParticle(const NanoReader& nanoReader, const unsigned genIndex) :
    PhysicsObject(nanoReader._GenPart_pt[genIndex], nanoReader._GenPart_eta[genIndex], nanoReader._GenPart_phi[genIndex], nanoReader._GenPart_energy[genIndex], nanoReader.is2016(), nanoReader.is2016PreVFP(), nanoReader.is2016PostVFP(), nanoReader.is2017(), nanoReader.is2018()),
    _genPdgID(nanoReader._GenPart_pdgId[genIndex]),
    _genStatus(nanoReader._GenPart_status[genIndex]),
    _genIndex(genIndex)
{ 
    //// initialize array of daughter indices -> Can we exchange these for pointers?
    // TODO: fix PhysicsObject -> lorentzvector from mass instead of energy
    // get charge from sign of pdgId
    _genCharge = _genPdgID > 0 ? 1 : -1;
    // next few lines: use bits to check.
    // bit 0 == 1: isPromptFinalState
    _genIsPromptFinalState = _genStatus & 1;
    // bit 5
    _genIsDirectPromptTauDecayProductFinalState = _genStatus & (1 << 5);
    // bit 13
    _genIsLastCopy = _genStatus & (1 << 13);
}

void GenParticle::ClearCopies() {
    // recursive 
    // if the daughter has the same flavor it gets deleted? Maybe take into account _genStatus
    // Before deletion: change daughters of current object

    for (unsigned daughterIndex = 0; daughterIndex < _genDaughters.size(); daughterIndex++) {
        if (_genDaughters[daughterIndex]->GetPdgID() == _genPdgID) {
            // delete daughters?
            
        }
    }

}
