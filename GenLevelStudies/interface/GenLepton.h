#ifndef GENLEPTON_H
#define GENLEPTON_H

#include "../../objects/interface/PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"

template< typename ObjectType > class PhysicsObjectCollection;

class GenLepton : public PhysicsObject {
    friend class PhysicsObjectCollection<GenLepton>;

    public:
        GenLepton(const TreeReader&, const unsigned leptonIndex);
        ~GenLepton() {};

        GenLepton( const GenLepton& );
        GenLepton( GenLepton&& ) noexcept;

        UInt_t flavor() {return _Flavor;}
        Int_t charge() {return _Charge;}
        Int_t momPdg() {return _MomPdg;}
        bool isPrompt() const {return _IsPrompt;}
        bool isLightLepton() const {return (_Flavor == 0 || _Flavor == 1);}


    private:
        UInt_t _Flavor;
        Int_t _Charge, _MomPdg;
        Bool_t _IsPrompt;
        
        //functions to facilitate dynamically typed dynamic memory allocation 
        virtual GenLepton* clone() const &{ return new GenLepton( *this ); }
        virtual GenLepton* clone() &&{ return new GenLepton( std::move(*this) ); }
};

#endif