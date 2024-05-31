#include "../interface/GenParticleTree.h"

GenParticleTree::GenParticleTree(const TreeReader& treeReader) {
    ///// Start looping and creating particles, also create links
    for (unsigned i = 0; i < treeReader._gen_n; i++) {
        GenParticle* newPart = new GenParticle(treeReader, i);
        _genParticles.push_back(newPart);

        if (treeReader._gen_motherIndex[i] < _genParticles.size()) {
            newPart->SetMother(_genParticles[treeReader._gen_motherIndex[i]]);
        }

        for (unsigned j=0; j < treeReader._gen_daughter_n[i]; j++) {
            if (treeReader._gen_daughterIndex[i][j] < _genParticles.size()) {
                newPart->AddDaughter(_genParticles[treeReader._gen_daughterIndex[i][j]]);
            }
        }
        
        // what is the intrinsic oredering of these things?
        // assume chaos -> count on nothing making sense so have a general thing to set everything
        // if we set a mother, the daughter was definitely not set. Vice verse if we set a daughter, their mother has not yet been set
        // ideally we can safely set both and it should be fine?
    }

    // Next up is finding the seeds - ie the top particle of each of these things
    // loop all particles and just GetMother? --- or we see which particles dont have a mother! should be clear in the variable in some way


    // last step is cleaning up all the crap -> ie single daughter and it being copies of each other.

    for (auto seed : _seeds) {
        seed->
    }
}