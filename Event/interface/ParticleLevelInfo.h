#ifndef PARTICLELEVELINFO_H
#define PARTICLELEVELINFO_H

#include "../../TreeReader/interface/TreeReader.h"


class ParticleLevelInfo {
    public:
        ParticleLevelInfo(const TreeReader&);

        ~ParticleLevelInfo() = default;

        // Additional b-jet information
        unsigned GetParticleLevelBees() const { return particleLevelBees; }
        std::vector<std::shared_ptr<LorentzVector>> GetParticleLevelBeeVectors() const {return plBeeVectors;}

    private:
        // save worked out stuff like how many b's for now - should be sufficient
        unsigned particleLevelBees;
        std::vector<std::shared_ptr<LorentzVector>> plBeeVectors;
};


#endif