#include "../interface/GenParticle.h"

GenParticle::GenParticle(const TreeReader& treeReader, const unsigned genIndex) :
    PhysicsObject(treeReader._gen_pt[genIndex], treeReader._gen_eta[genIndex], treeReader._gen_phi[genIndex], treeReader._gen_E[genIndex], treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(), treeReader.is2017(), treeReader.is2018()),
    _genPdgID(treeReader._gen_pdgId[genIndex]),
    _genCharge(treeReader._gen_charge[genIndex]),
    _genStatus(treeReader._gen_status[genIndex]),
    _genIsPromptFinalState(treeReader._gen_isPromptFinalState[genIndex]),
    _genIsDirectPromptTauDecayProductFinalState(treeReader._gen_isDirectPromptTauDecayProductFinalState[genIndex]),
    _genIsLastCopy(treeReader._gen_isLastCopy[genIndex]),
    _genIndex(treeReader._gen_index[genIndex]),
    _genMotherIndex(treeReader._gen_motherIndex[genIndex]),
    _genDaughter_n(treeReader._gen_daughter_n[genIndex])
{ 
    //// initialize array of daughter indices -> Can we exchange these for pointers?
    
}
