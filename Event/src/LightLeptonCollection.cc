#include "../interface/LightLeptonCollection.h"

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"


LightLeptonCollection::LightLeptonCollection( const TreeReader& treeReader ){
    for( unsigned m = 0; m < treeReader._nMuon; ++m ){
        push_back( Muon( treeReader, m ) );
    }
    for( unsigned e = 0; e < treeReader._nElectron; ++e ){
        push_back( Electron( treeReader, e ) );
    }
}
