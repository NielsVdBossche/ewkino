#ifndef PARTICLELEVELINFO_H
#define PARTICLELEVELINFO_H

#include "../../TreeReader/interface/TreeReader.h"


class ParticleLevelInfo {
    public:
        ParticleLevelInfo(const TreeReader&);

        ~ParticleLevelInfo() = default;

        unsigned GetParticleLevelBees() const { return particleLevelBees; }

    private:
        // save worked out stuff like how many b's for now - should be sufficient
        unsigned particleLevelBees;

};


#endif