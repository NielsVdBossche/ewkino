#include "../interface/MuonCollection.h"


MuonCollection::MuonCollection( const TreeReader& treeReader ){
    for( unsigned m = 0; m < treeReader._nMu; ++m){
        push_back( Muon( treeReader, m ) );
    }
}

MuonCollection::MuonCollection( const NanoReader& nanoReader ){
    for( unsigned m = 0; m < nanoReader.GetMuonReader()->_nLepton; ++m){
        push_back( Muon( nanoReader, m ) );
    }
}
