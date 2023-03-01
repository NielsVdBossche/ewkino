#include "../interface/MuonCollection.h"


MuonCollection::MuonCollection( const TreeReader& treeReader ){
	for( unsigned m = 0; m < treeReader._nMuon; ++m){
        push_back( Muon( treeReader, m ) );
    }
}
