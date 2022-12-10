#ifndef GenParticle_H
#define GenParticle_H

#include "PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"


template< typename ObjectType > class PhysicsObjectCollection;
class GenParticle : public PhysicsObject {
    friend class PhysicsObjectCollection<GenParticle>;
    
    public:
        GenParticle(const TreeReader&, const unsigned);
        GenParticle(const GenParticle&);
        GenParticle(GenParticle&&) noexcept;

        GenParticle& operator=( const GenParticle& ) = default;
        GenParticle& operator=( GenParticle&& ) = default;

        ~GenParticle();

        GenParticle* GetMother() {return mother;}
        void SetMother(GenParticle* newMother) {mother = newMother;}

        int getPdgID() {return _genPdgID;};
        int getCharge() {return _genCharge;};
        int getStatus() {return _genStatus;};
        int getIndex() {return _genIndex;};
        int getMotherIndex() {return _genMotherIndex;};
        
        bool isPromptFinalState() {return _genIsPromptFinalState;};
        bool isDirectPromptTauDecayProductFinalState() {return _genIsDirectPromptTauDecayProductFinalState;};
        bool isLastCopy() {return _genIsLastCopy;};
        
        std::vector<GenParticle*>& GetDaughters() {return _genDaughters;};
        int NumberOfDaughters() {return _genDaughter_n;};
        
        
    private:
        int _genPdgID;
        int _genCharge;
        int _genStatus;
        bool _genIsPromptFinalState;
        bool _genIsDirectPromptTauDecayProductFinalState;
        bool _genIsLastCopy;
        int _genIndex;
        int _genMotherIndex;
        GenParticle* mother;
        int _genDaughter_n;
        int _genDaughterIndex[10];
        std::vector<GenParticle*> _genDaughters;

        virtual GenParticle* clone() const &{ return new GenParticle( *this ); }
        virtual GenParticle* clone() &&{ return new GenParticle( std::move(*this) ); }
};

#endif
