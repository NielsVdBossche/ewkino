#ifndef LheParticle_H
#define LheParticle_H

#include "PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"


template< typename ObjectType > class PhysicsObjectCollection;

class LheParticle : public PhysicsObject {
    friend class PhysicsObjectCollection<LheParticle>;
    
    public:
        LheParticle(const TreeReader&, const unsigned);
        LheParticle(const LheParticle&);
        LheParticle(LheParticle&&) noexcept;

        LheParticle& operator=( const LheParticle& ) = default;
        LheParticle& operator=( LheParticle&& ) = default;

        ~LheParticle();

        int getStatus() const {return _lheStatus;}
        int getPdgId() const {return _lhePdgId;}
        int getMother1() const {return _lheMother1;}
        int getMother2() const {return _lheMother2;}
        double getMass() const {return _lheMass;}

    private:
        int _lheStatus;
        int _lhePdgId;
        int _lheMother1;
        int _lheMother2;
        double _lheMass;

        virtual LheParticle* clone() const &{ return new LheParticle( *this ); }
        virtual LheParticle* clone() &&{ return new LheParticle( std::move(*this) ); }
};

#endif