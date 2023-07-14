#include "../interface/GenParticlesTop.h"

#include "../../objects/interface/LorentzVector.h"

GenParticlesTop::GenParticlesTop(const TreeReader& treeReader) {
    for (unsigned i=0; i<treeReader._gen_n; i++) {
        if (abs(treeReader._gen_pdgId[i]) != 5 || abs(treeReader._gen_pdgId[treeReader._gen_motherIndex[i]]) != 6 || abs(treeReader._gen_status[treeReader._gen_motherIndex[i]]) != 62) continue;
        std::shared_ptr<LorentzVector> newLV = std::make_shared<LorentzVector>(treeReader._gen_pt[i], treeReader._gen_eta[i], treeReader._gen_phi[i], treeReader._gen_E[i]);
        bottomQuarks.push_back(newLV);
    }
}
