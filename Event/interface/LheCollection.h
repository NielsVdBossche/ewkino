#ifndef LheCollection_H
#define LheCollection_H

#include "../../objects/interface/LheParticle.h"
#include "PhysicsObjectCollection.h"

class LheCollection : public PhysicsObjectCollection< LheParticle > {
    
    public:
        LheCollection( const TreeReader& );
        // LheCollection( const NanoReader& );
        LheCollection() = default;

    private:
        //build collection of objects passing given selection
        LheCollection selectedCollection( void (LheCollection::*applySelection)() ) const;

        LheCollection( const std::vector< std::shared_ptr< LheParticle > >& leptonVector ) : PhysicsObjectCollection< LheParticle >( leptonVector ) {}

};



#endif
