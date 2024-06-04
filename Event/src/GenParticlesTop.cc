#include "../interface/GenParticlesTop.h"

#include "../../objects/interface/LorentzVector.h"

GenParticlesTop::GenParticlesTop(const TreeReader& treeReader) {
    for (unsigned i=0; i<treeReader._gen_n; i++) {
        if (abs(treeReader._gen_pdgId[i]) != 5 || abs(treeReader._gen_pdgId[treeReader._gen_motherIndex[i]]) != 6 || abs(treeReader._gen_status[treeReader._gen_motherIndex[i]]) != 62) continue;
        std::shared_ptr<LorentzVector> newLV = std::make_shared<LorentzVector>(treeReader._gen_pt[i], treeReader._gen_eta[i], treeReader._gen_phi[i], treeReader._gen_E[i]);
        bottomQuarks.push_back(newLV);
    }
}

GenParticlesTop::GenParticlesTop(const NanoReader& nanoReader) {
    for (unsigned i=0; i<nanoReader._nGenPart; i++) {
        if (abs(nanoReader._GenPart_pdgId[i]) != 5 || abs(nanoReader._GenPart_pdgId[nanoReader._GenPart_genPartIdxMother[i]]) != 6 || abs(nanoReader._GenPart_status[nanoReader._GenPart_genPartIdxMother[i]]) != 62) continue;
        std::shared_ptr<LorentzVector> newLV = std::make_shared<LorentzVector>(nanoReader._GenPart_pt[i], nanoReader._GenPart_eta[i], nanoReader._GenPart_phi[i], -1.);
        bottomQuarks.push_back(newLV);
    }
}