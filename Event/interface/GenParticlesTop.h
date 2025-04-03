#ifndef GenParticlesTop_H
#define GenParticlesTop_H

#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

class GenParticlesTop {
    public:
        GenParticlesTop(const TreeReader&);
        GenParticlesTop(const NanoReader&);
        ~GenParticlesTop() = default;

        std::vector<std::shared_ptr<LorentzVector>> GetBottomquarkCollection() const {return bottomQuarks;}
        std::vector<int> GetPdgIdsHiggsDecays() const {return pdgIdsHiggsDecays;}
    private:
        std::vector<std::shared_ptr<LorentzVector>> bottomQuarks;
        std::vector<int> pdgIdsHiggsDecays;
};

#endif