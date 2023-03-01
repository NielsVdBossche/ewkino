#include "../interface/GenMet.h"

GenMet::GenMet( const TreeReader& treeReader ) :
    PhysicsObject( treeReader._GenMET_pt, 0., treeReader._GenMET_phi, treeReader._GenMET_phi,
		    treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(),
		    treeReader.is2017(), treeReader.is2018() )
{}
