#include "../interface/GenMet.h"

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

GenMet::GenMet(const TreeReader& treeReader) : 
PhysicsObject(treeReader._gen_met, 0., treeReader._gen_metPhi, treeReader._gen_met,
              treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(),
              treeReader.is2017(), treeReader.is2018()) 
{}

GenMet::GenMet(const NanoReader& nanoReader) : 
PhysicsObject(nanoReader._GenMET_pt, 0., nanoReader._GenMET_phi, nanoReader._GenMET_pt,
              nanoReader.is2016(), nanoReader.is2016PreVFP(), nanoReader.is2016PostVFP(),
              nanoReader.is2017(), nanoReader.is2018()) 
{}