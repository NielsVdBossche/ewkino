#ifndef ElectronCollection_H
#define ElectronCollection_H

//include other parts of code 
#include "PhysicsObjectCollection.h"
#include "../../objects/interface/Electron.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

class LeptonCollection;

class ElectronCollection : public PhysicsObjectCollection< Electron > {

    friend class LeptonCollection;

    public:
        ElectronCollection( const TreeReader& );
        ElectronCollection( const NanoReader& );
    
	// make varied Electron collections
	ElectronCollection electronScaleUpCollection() const;
	ElectronCollection electronScaleDownCollection() const;
	ElectronCollection electronResUpCollection() const;
	ElectronCollection electronResDownCollection() const;

    private:
        ElectronCollection( const std::vector< std::shared_ptr< Electron > >& electronVector ): 
	    PhysicsObjectCollection< Electron >( electronVector ) {}
	ElectronCollection buildVariedCollection(
                    Electron (Electron::*variedElectron)() const ) const;

};
#endif 
