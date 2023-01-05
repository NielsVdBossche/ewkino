#include "../interface/ParticleLevelInfo.h"

ParticleLevelInfo::ParticleLevelInfo(const TreeReader& treeReader) {
    particleLevelBees = 0;
    for (unsigned i=0; i<treeReader._pl_nJets; i++) {
        if (treeReader._pl_jetHadronFlavor[i] == 5) particleLevelBees++;
    }
}