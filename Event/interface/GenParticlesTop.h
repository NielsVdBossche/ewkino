#ifndef GenParticlesTop_H
#define GenParticlesTop_H

#include "../../TreeReader/interface/TreeReader.h"

class GenParticlesTop {
    public:
        GenParticlesTop(const TreeReader&);
        ~GenParticlesTop() = default;

        std::vector<std::shared_ptr<LorentzVector>> GetBottomquarkCollection() const {return bottomQuarks;}
    private:
        std::vector<std::shared_ptr<LorentzVector>> bottomQuarks;
};

#endif