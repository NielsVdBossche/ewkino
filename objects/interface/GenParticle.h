#ifndef GenParticle_H
#define GenParticle_H

#include "PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"


template< typename ObjectType > class PhysicsObjectCollection;
class GenParticle : public PhysicsObject {
    friend class PhysicsObjectCollection<GenParticle>;
    
    public:
        GenParticle(const TreeReader&, const unsigned);
        GenParticle(const NanoReader&, const unsigned);
        GenParticle(const GenParticle&);
        GenParticle(GenParticle&&) noexcept;

        GenParticle& operator=( const GenParticle& ) = default;
        GenParticle& operator=( GenParticle&& ) = default;

        ~GenParticle();

        GenParticle* GetMother() {return mother;}
        void SetMother(GenParticle* newMother) {
            mother = newMother;
            mother->AddDaughter(this);
        }

        int GetPdgID() {return _genPdgID;};
        int GetCharge() {return _genCharge;};
        int GetStatus() {return _genStatus;};
        int GetIndex() {return _genIndex;};
        
        bool isPromptFinalState() {return _genIsPromptFinalState;};
        bool isDirectPromptTauDecayProductFinalState() {return _genIsDirectPromptTauDecayProductFinalState;};
        bool isLastCopy() {return _genIsLastCopy;};
        
        std::vector<GenParticle*>& GetDaughters() {return _genDaughters;};
        void AddDaughter(GenParticle* daughter) {
            // ideally we do a find based on getIndex() ti see if we're safe
            _genDaughters.push_back(daughter);
            daughter->SetMother(this);
        }
        int NumberOfDaughters() {return _genDaughters.size();};

        void ClearCopies();
        
        
    private:
        int _genPdgID;
        int _genCharge;
        int _genStatus;
        bool _genIsPromptFinalState;
        bool _genIsDirectPromptTauDecayProductFinalState;
        bool _genIsLastCopy;
        int _genIndex;

        GenParticle* mother;
        std::vector<GenParticle*> _genDaughters;

        virtual GenParticle* clone() const &{ return new GenParticle( *this ); }
        virtual GenParticle* clone() &&{ return new GenParticle( std::move(*this) ); }
};

#endif
