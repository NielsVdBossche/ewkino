#ifndef MuonCollection_H
#define MuonCollection_H


//import other parts of code 
#include "PhysicsObjectCollection.h"
#include "../../objects/interface/Muon.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

class LeptonCollection;

class MuonCollection : public PhysicsObjectCollection< Muon >{
    
    friend class LeptonCollection;

    public:
        MuonCollection( const TreeReader& );
        MuonCollection( const NanoReader& );

    private:
        MuonCollection( const std::vector< std::shared_ptr< Muon > >& muonVector ) : PhysicsObjectCollection< Muon >( muonVector ) {} 
};
#endif 
