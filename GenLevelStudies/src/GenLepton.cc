#include "../interface/GenLepton.h"

GenLepton::GenLepton(const TreeReader& treeReader, const unsigned leptonIndex) : 
    PhysicsObject(treeReader._gen_lPt[leptonIndex], treeReader._gen_lEta[leptonIndex], treeReader._gen_lPhi[leptonIndex], treeReader._gen_lE[leptonIndex], treeReader.is2016(), treeReader.is2017()),
    _Flavor(treeReader._gen_lFlavor[leptonIndex]),_Charge(treeReader._gen_lCharge[leptonIndex]),
    _MomPdg(treeReader._gen_lMomPdg[leptonIndex]), _IsPrompt(treeReader._gen_lIsPrompt[leptonIndex])
    {

}


GenLepton::GenLepton( const GenLepton& rhs ):
    PhysicsObject( rhs),
    _Flavor(rhs._Flavor),
    _Charge(rhs._Charge),
    _MomPdg(rhs._MomPdg),
    _IsPrompt(rhs._IsPrompt)
    {}    


GenLepton::GenLepton( GenLepton&& rhs ) noexcept:
    PhysicsObject( std::move( rhs )),
    _Flavor(rhs._Flavor),
    _Charge(rhs._Charge),
    _MomPdg(rhs._MomPdg),
    _IsPrompt(rhs._IsPrompt)
    {}    