#include "../interface/LheCollection.h"

LheCollection::LheCollection( const TreeReader& treeReader) {
    for( unsigned m = 0; m < treeReader._nLheParticles; ++m){
        push_back( LheParticle( treeReader, m ) );
    }
}
