#include "../interface/TauCollection.h"


TauCollection::TauCollection( const TreeReader& treeReader ){
    for( unsigned t = 0; t < treeReader._nTau; ++t){
        push_back( Tau( treeReader, t ) );
    }
}
