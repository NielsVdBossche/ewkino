#include "../interface/ParticleLevelInfo.h"

#include "../../objects/interface/LorentzVector.h"

ParticleLevelInfo::ParticleLevelInfo(const TreeReader& treeReader) {
    particleLevelBees = 0;
    for (unsigned i=0; i<treeReader._pl_nJets; i++) {
        if (treeReader._pl_jetHadronFlavor[i] == 5) {
            particleLevelBees++;
            std::shared_ptr<LorentzVector> newLV = std::make_shared<LorentzVector>(treeReader._pl_jetPt[i], treeReader._pl_jetEta[i], treeReader._pl_jetPhi[i], treeReader._pl_jetE[i]);
            plBeeVectors.push_back(newLV);
        }
    }
}