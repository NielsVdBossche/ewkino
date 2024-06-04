#include "../interface/TauCollection.h"


TauCollection::TauCollection( const TreeReader& treeReader ){
    for( unsigned t = treeReader._nLight; t < treeReader._nL; ++t){
        push_back( Tau( treeReader, t ) );
    }
}

TauCollection::TauCollection( const NanoReader& nanoReader ){
    for( unsigned t = 0; t < nanoReader.GetTauReader()._nLepton; ++t){
        push_back( Tau( nanoReader, t ) );
    }
}
