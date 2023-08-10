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


// ParticleLevelProducer
ParticleLevelInfo::ParticleLevelInfo(const NanoReader& nanoReader) {
    // no particle level jets in nanoAOD -> check later in nanosrcfile and compare to original file to see if it works out
    particleLevelBees = 0;
    for (unsigned i=0; i<nanoReader._nGenJet; i++) {
        if (nanoReader._GenJet_partonFlavour[i] == 5) {
            particleLevelBees++;
            std::shared_ptr<LorentzVector> newLV = std::make_shared<LorentzVector>(nanoReader._GenJet_pt[i], nanoReader._GenJet_eta[i], nanoReader._GenJet_phi[i], -1.);
            plBeeVectors.push_back(newLV);
        }
    }
}