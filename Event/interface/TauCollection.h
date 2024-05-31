#ifndef TauCollection_H
#define TauCollection_H

//include other parts of framework
#include "PhysicsObjectCollection.h"
#include "../../objects/interface/Tau.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

class LeptonCollection;

class TauCollection : public PhysicsObjectCollection< Tau > {

    friend class LeptonCollection;
    
    public:
        TauCollection( const TreeReader& );
        TauCollection( const NanoReader& );

    private:
        TauCollection( const std::vector< std::shared_ptr< Tau > >& tauVector ) : PhysicsObjectCollection< Tau >( tauVector ) {}
};
#endif
