#ifndef GENPARTICLETREE_H
#define GENPARTICLETREE_H

#include "../../objects/interface/GenParticle.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

class GenParticleTree {
    public:
        // At the moment: don't make it a tree, honestly seems like a bad idea for computation intensity, don't need the whole gen information
        GenParticleTree(const TreeReader&);
        GenParticleTree(const NanoReader&);

        ~GenParticleTree();

    private:
        std::vector<GenParticle*> _genParticles;
        std::vector<GenParticle*> _seeds;       
};


#endif