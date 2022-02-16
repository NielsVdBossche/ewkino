#include "../interface/LheParticle.h"

LheParticle::LheParticle(const TreeReader& treeReader, const unsigned lheIndex) :
    PhysicsObject(treeReader._lhePt[lheIndex], treeReader._lheEta[lheIndex], treeReader._lhePhi[lheIndex], treeReader._lheE[lheIndex], treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(), treeReader.is2017(), treeReader.is2018()),
    _lheStatus(treeReader._lheStatus[lheIndex]),
    _lhePdgId(treeReader._lhePdgId[lheIndex]),
    _lheMother1(treeReader._lheMother1[lheIndex]),
    _lheMother2(treeReader._lheMother2[lheIndex]),
    _lheMass(treeReader._lheMass[lheIndex]) {
    
}


LheParticle::~LheParticle() {

}


LheParticle::LheParticle( const LheParticle& rhs ):
    PhysicsObject( rhs),
    _lheStatus(rhs._lheStatus),
    _lhePdgId(rhs._lhePdgId),
    _lheMother1(rhs._lheMother1),
    _lheMother2(rhs._lheMother2),
    _lheMass(rhs._lheMass)
    {}    


LheParticle::LheParticle( LheParticle&& rhs ) noexcept:
    PhysicsObject( std::move( rhs )),
    _lheStatus(rhs._lheStatus),
    _lhePdgId(rhs._lhePdgId),
    _lheMother1(rhs._lheMother1),
    _lheMother2(rhs._lheMother2),
    _lheMass(rhs._lheMass)
    {}    