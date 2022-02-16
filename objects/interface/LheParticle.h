#ifndef LheParticle_H
#define LheParticle_H

#include "PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"


template< typename ObjectType > class PhysicsObjectCollection;

class LheParticle : public PhysicsObject {
    friend class PhysicsObjectCollection<LheParticle>;
    
    public:
        LheParticle(const TreeReader&, const unsigned);
        LheParticle(const LheParticle&) = delete;
        LheParticle(LheParticle&&) noexcept = delete;

        virtual ~LheParticle();

        int getStatus() const {return _lheStatus;}
        int getPdgId() const {return _lhePdgId;}
        int getMother1() const {return _lheMother1;}
        int getMother2() const {return _lheMother2;}
        double getPt() const {return _lhePt;}
        double getEta() const {return _lheEta;}
        double getPhi() const {return _lhePhi;}
        double getE() const {return _lheE;}
        double getMass() const {return _lheMass;}
    
    private:
        int _lheStatus;
        int _lhePdgId;
        int _lheMother1;
        int _lheMother2;
        double _lhePt;
        double _lheEta;
        double _lhePhi;
        double _lheE;
        double _lheMass;

};

#endif