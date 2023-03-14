#include "../interface/GenParticle.h"

GenParticle::GenParticle(const TreeReader& treeReader, const unsigned genIndex) :
    PhysicsObject(treeReader._gen_pt[genIndex], treeReader._gen_eta[genIndex], treeReader._gen_phi[genIndex], treeReader._gen_E[genIndex], treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(), treeReader.is2017(), treeReader.is2018()),
    _genPdgID(treeReader._gen_pdgId[genIndex]),
    _genCharge(treeReader._gen_charge[genIndex]),
    _genStatus(treeReader._gen_status[genIndex]),
    _genIsPromptFinalState(treeReader._gen_isPromptFinalState[genIndex]),
    _genIsDirectPromptTauDecayProductFinalState(treeReader._gen_isDirectPromptTauDecayProductFinalState[genIndex]),
    _genIsLastCopy(treeReader._gen_isLastCopy[genIndex]),
    _genIndex(treeReader._gen_index[genIndex])
{ 
    //// initialize array of daughter indices -> Can we exchange these for pointers?
    
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
