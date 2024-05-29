#include "../interface/LheParticle.h"

LheParticle::LheParticle(const TreeReader& treeReader, const unsigned lheIndex) : 
    PhysicsObject(treeReader._lhePt[lheIndex], treeReader._lheEta[lheIndex], 
                treeReader._lhePhi[lheIndex], treeReader._lheE[lheIndex], 
                treeReader.is2016(), 
                treeReader.is2016PreVFP(), 
                treeReader.is2016PostVFP(), 
                treeReader.is2017(), 
                treeReader.is2018()),
            _lheStatus(treeReader._lheStatus[lheIndex]),
            _lhePdgId(treeReader._lhePdgId[lheIndex]),
            _lheMother1(treeReader._lheMother1[lheIndex]),
            _lheMother2(treeReader._lheMother2[lheIndex]),
            _lheMass(treeReader._lheMass[lheIndex]) {
}

LheParticle::LheParticle(const NanoReader& nanoReader, const unsigned lheIndex) : 
    PhysicsObject(nanoReader._LHEPart_pt[lheIndex], nanoReader._LHEPart_eta[lheIndex], 
                nanoReader._LHEPart_phi[lheIndex], -1., 
                nanoReader.is2016(), 
                nanoReader.is2016PreVFP(), 
                nanoReader.is2016PostVFP(), 
                nanoReader.is2017(), 
                nanoReader.is2018()),
            _lheStatus(nanoReader._LHEPart_status[lheIndex]),
            _lhePdgId(nanoReader._LHEPart_pdgId[lheIndex]),
            _lheMass(nanoReader._LHEPart_mass[lheIndex]) 
{
    setLorentzVectorWithMass(pt(), eta(), phi(), nanoReader._LHEPart_mass[lheIndex]);
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