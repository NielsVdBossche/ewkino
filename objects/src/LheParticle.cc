#include "../interface/LheParticle.h"

LheParticle::LheParticle(const TreeReader& treeReader, const unsigned lheIndex) :
    _lheStatus(treeReader._lheStatus[lheIndex]),
    _lhePdgId(treeReader._lhePdgId[lheIndex]),
    _lheMother1(treeReader._lheMother1[lheIndex]),
    _lheMother2(treeReader._lheMother2[lheIndex]),
    _lhePt(treeReader._lhePt[lheIndex]),
    _lheEta(treeReader._lheEta[lheIndex]),
    _lhePhi(treeReader._lhePhi[lheIndex]),
    _lheE(treeReader._lheE[lheIndex]),
    _lheMass(treeReader._lheMass[lheIndex]) {
    
}


LheParticle::~LheParticle() {

}
