#include "../interface/Jet.h"

//include c++ library classes 
#include <cmath>
#include <stdexcept>
#include <string>

//include other parts of framework
#include "../interface/JetSelector.h"


Jet::Jet( const TreeReader& treeReader, const unsigned jetIndex,
	    const bool readAllJECVariations, const bool readGroupedJECVariations ):
    PhysicsObject( 
        treeReader._jetSmearedPt[jetIndex], 
        treeReader._jetEta[jetIndex], 
        treeReader._jetPhi[jetIndex], 
        treeReader._jetE[jetIndex] * ( treeReader._jetSmearedPt[jetIndex] / treeReader._jetPt[jetIndex] ),
        treeReader.is2016(),
        treeReader.is2016PreVFP(),
        treeReader.is2016PostVFP(), 
        treeReader.is2017(),
        treeReader.is2018()
    ),
    _deepCSV( treeReader._jetDeepCsv_b[jetIndex] 
		+ treeReader._jetDeepCsv_bb[jetIndex] ),
    _deepFlavor( treeReader._jetDeepFlavor_b[jetIndex] 
		+ treeReader._jetDeepFlavor_bb[jetIndex] 
		+ treeReader._jetDeepFlavor_lepb[jetIndex] ),
    _isTight( treeReader._jetIsTight[jetIndex] ),
    _isTightLeptonVeto( treeReader._jetIsTightLepVeto[jetIndex] ),

    //WARNING : is hadron flavor defined for jets in data?
    _hadronFlavor( treeReader._jetHadronFlavor[jetIndex] ),
    _pt_JECDown( treeReader._jetSmearedPt_JECDown[jetIndex] ),
    _pt_JECUp( treeReader._jetSmearedPt_JECUp[jetIndex] ),
    _pt_JERDown( treeReader._jetSmearedPt_JERDown[jetIndex] ),
    _pt_JERUp( treeReader._jetSmearedPt_JERUp[jetIndex] ),

    _JECSourcesUp_Ids(treeReader._jetSmearedPt_JECSourcesUp_Ids),
    _JECSourcesDown_Ids(treeReader._jetSmearedPt_JECSourcesDown_Ids),
    _JECGroupedUp_Ids(treeReader._jetSmearedPt_JECGroupedUp_Ids),
    _JECGroupedDown_Ids(treeReader._jetSmearedPt_JECGroupedDown_Ids),
    selector( new JetSelector( this ) )
{
    if (readAllJECVariations) {
        for (const auto mapEl : treeReader._jetSmearedPt_JECSourcesUp) {
            _pt_JECSourcesUp.push_back(mapEl[jetIndex]);
        }
        for (const auto mapEl : treeReader._jetSmearedPt_JECSourcesDown) {
            _pt_JECSourcesDown.push_back(mapEl[jetIndex]);
        }
    }
    if (readGroupedJECVariations) {
        for (const auto mapEl : treeReader._jetSmearedPt_JECGroupedUp) {
            _pt_JECGroupedUp.push_back(mapEl[jetIndex]);
        }
        for (const auto mapEl : treeReader._jetSmearedPt_JECGroupedDown) {
            _pt_JECGroupedDown.push_back(mapEl[jetIndex]);
        }
    }

    //catch potential invalid values of deepCSV and deepFlavor
    if( std::isnan( _deepCSV ) ){
        _deepCSV = 0.;

    //set minimum value to 0 to avoid default values
    } else if( _deepCSV < 0 ){
        _deepCSV = 0.;
    }

    if( std::isnan( _deepFlavor ) ){
        _deepFlavor = 0.;
    } else if( _deepFlavor < 0 ){
        _deepFlavor = 0.;
    }

    //check that _hadronFlavor has a known value
    if( ! ( ( _hadronFlavor == 0 ) || ( _hadronFlavor == 4 ) || ( _hadronFlavor == 5 ) ) ){
        throw std::invalid_argument( "jet hadronFlavor is '" + std::to_string( _hadronFlavor ) + "' while it should be 0, 4 or 5." );
    }
}


Jet::Jet( const Jet& rhs ) : 
    PhysicsObject( rhs ),
    _deepCSV( rhs._deepCSV ),
    _deepFlavor( rhs._deepFlavor ),
    _isTight( rhs._isTight ),
    _isTightLeptonVeto( rhs._isTightLeptonVeto ),
    _hadronFlavor( rhs._hadronFlavor ),
    _pt_JECDown( rhs._pt_JECDown ),
    _pt_JECUp( rhs._pt_JECUp ),
    _pt_JERDown( rhs._pt_JERDown ),
    _pt_JERUp( rhs._pt_JERUp ),
    _pt_JECSourcesUp( rhs._pt_JECSourcesUp ),
    _pt_JECSourcesDown( rhs._pt_JECSourcesDown ),
    _pt_JECGroupedUp( rhs._pt_JECGroupedUp ),
    _pt_JECGroupedDown( rhs._pt_JECGroupedDown ),
    selector( new JetSelector( this ) )
    {}


Jet::Jet( Jet&& rhs ) noexcept :
    PhysicsObject( std::move( rhs ) ),
	_deepCSV( rhs._deepCSV ),
    _deepFlavor( rhs._deepFlavor ),
    _isTight( rhs._isTight ),
    _isTightLeptonVeto( rhs._isTightLeptonVeto ),
    _hadronFlavor( rhs._hadronFlavor ),
    _pt_JECDown( rhs._pt_JECDown ),
    _pt_JECUp( rhs._pt_JECUp ),
    _pt_JERDown( rhs._pt_JERDown ),
    _pt_JERUp( rhs._pt_JERUp ),
    _pt_JECSourcesUp( rhs._pt_JECSourcesUp ),
    _pt_JECSourcesDown( rhs._pt_JECSourcesDown ),
    _pt_JECGroupedUp( rhs._pt_JECGroupedUp ),
    _pt_JECGroupedDown( rhs._pt_JECGroupedDown ),
    selector( new JetSelector( this ) )
    {}


Jet::~Jet(){
    delete selector;
}


void Jet::copyNonPointerAttributes( const Jet& rhs ){
    _deepCSV = rhs._deepCSV;
    _deepFlavor = rhs._deepFlavor;
    _isTight = rhs._isTight;
    _isTightLeptonVeto = rhs._isTightLeptonVeto;
    _hadronFlavor = rhs._hadronFlavor;
    _pt_JECDown = rhs._pt_JECDown;
    _pt_JECUp = rhs._pt_JECUp;
    _pt_JERDown = rhs._pt_JERDown;
    _pt_JERUp = rhs._pt_JERUp;
    _pt_JECSourcesUp = rhs._pt_JECSourcesUp;
    _pt_JECSourcesDown = rhs._pt_JECSourcesDown;
    _pt_JECGroupedUp = rhs._pt_JECGroupedUp;
    _pt_JECGroupedDown = rhs._pt_JECGroupedDown;
}


Jet& Jet::operator=( const Jet& rhs ){

    PhysicsObject::operator=(rhs);

    //note that the jet selector does not have to be changed, it will just keep pointing to this object 
    if( this != &rhs ){
        copyNonPointerAttributes( rhs );
    }
    return *this;
}


Jet& Jet::operator=( Jet&& rhs ) noexcept {

    PhysicsObject::operator=( std::move(rhs) );

    if( this != &rhs ){
        copyNonPointerAttributes( rhs );

        //current selector can still keep pointing to this object
    }
    return *this;
}


Jet Jet::variedJet(const double newPt) const{
    Jet variedJet( *this );
    variedJet.setLorentzVector( newPt, eta(), phi(), energy()*( newPt / pt() ) );
    return variedJet;
} 


Jet Jet::JetJECDown() const{
    return variedJet( _pt_JECDown );
}


Jet Jet::JetJECUp() const{
    return variedJet( _pt_JECUp );
}


Jet Jet::JetJERDown() const{
    return variedJet( _pt_JERDown );
}


Jet Jet::JetJERUp() const{
    return variedJet( _pt_JERUp );
}

Jet Jet::JetJER_1p93_Down() const {
    double newPt = pt();
    if (absEta() <= 1.93) newPt = _pt_JERDown;

    return variedJet( newPt );
}

Jet Jet::JetJER_1p93_Up() const {
    double newPt = pt();
    if (absEta() <= 1.93) newPt = _pt_JERUp;

    return variedJet( newPt );
}

Jet Jet::JetJER_1p93_To_2p5_Down() const {
    double newPt = pt();
    if (absEta() > 1.93 && absEta() <= 2.5) newPt = _pt_JERDown;

    return variedJet( newPt );
}

Jet Jet::JetJER_1p93_To_2p5_Up() const {
    double newPt = pt();
    if (absEta() > 1.93 && absEta() <= 2.5) newPt = _pt_JERUp;
    
    return variedJet( newPt );
}

Jet Jet::JetJECUp( const std::string source ) const {
    double newpt = 0.;
    if (_JECSourcesUp_Ids) {
        for (auto mapEl : *_JECSourcesUp_Ids) {
            if (source == mapEl.first) {
                newpt = _pt_JECSourcesUp[mapEl.second];
            }
        }
    }
    if (_JECGroupedUp_Ids) {
        for (auto mapEl : *_JECGroupedUp_Ids) {
            if (source == mapEl.first) newpt = _pt_JECGroupedUp[mapEl.second];
        }
    }

    return variedJet(newpt);
}
Jet Jet::JetJECDown( const std::string source ) const {
    double newpt = 0.;
    if (_JECSourcesDown_Ids) {
        for (auto mapEl : *_JECSourcesDown_Ids) {
            if (source == mapEl.first) {
                newpt = _pt_JECSourcesDown[mapEl.second];
            }
        }
    }
    if (_JECGroupedDown_Ids) {
        for (auto mapEl : *_JECGroupedDown_Ids) {
            if (source == mapEl.first) newpt = _pt_JECGroupedDown[mapEl.second];
        }
    }
    return variedJet(newpt);
}

Jet Jet::JetJECGroupedDown( const unsigned source_id ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    double newpt = _pt_JECGroupedDown[source_id];
    return variedJet( newpt );
}


Jet Jet::JetJECGroupedUp( const unsigned source_id ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    double newpt = _pt_JECGroupedUp[source_id];
    return variedJet( newpt );
}

Jet Jet::JetJECSourcesDown( const unsigned source_id ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    double newpt = _pt_JECSourcesUp[source_id];
    return variedJet( newpt );
}


Jet Jet::JetJECSourcesUp( const unsigned source_id ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    double newpt = _pt_JECSourcesDown[source_id];
    return variedJet( newpt );
}


bool Jet::isGood() const{
    return selector->isGood();
}


bool Jet::isGoodAnyVariation() const{
    return (
        isGood() ||
        JetJECDown().isGood() ||
        JetJECUp().isGood() ||
        JetJERDown().isGood() ||
        JetJERUp().isGood()
    );
}


bool Jet::inBTagAcceptance() const{
    return selector->inBTagAcceptance();
}


bool Jet::isBTaggedLoose() const{
    return selector->isBTaggedLoose();
}


bool Jet::isBTaggedMedium() const{
    return selector->isBTaggedMedium();
}


bool Jet::isBTaggedTight() const{
    return selector->isBTaggedTight();
}


bool Jet::isBTaggedAnyVariation( bool ( Jet::*isBTaggedWP )() const ) const{
    return (
        ( *this.*isBTaggedWP )() ||
        ( JetJECDown().*isBTaggedWP )() ||
        ( JetJECUp().*isBTaggedWP )() ||
        ( JetJERDown().*isBTaggedWP )() ||
        ( JetJERUp().*isBTaggedWP )()
    );
}


bool Jet::isBTaggedLooseAnyVariation() const{
    return isBTaggedAnyVariation( &Jet::isBTaggedLoose );
}


bool Jet::isBTaggedMediumAnyVariation() const{
    return isBTaggedAnyVariation( &Jet::isBTaggedMedium );
}


bool Jet::isBTaggedTightAnyVariation() const{
    return isBTaggedAnyVariation( &Jet::isBTaggedTight );
}


std::ostream& Jet::print( std::ostream& os ) const{
    os << "Jet : ";
    PhysicsObject::print( os );
    os << " / deepCSV = " << _deepCSV << " / deepFlavor = " << _deepFlavor << " / isTight = " << _isTight << " / isTightLeptonVeto = " << _isTightLeptonVeto << " / hadronFlavor = " << _hadronFlavor << " / pt_JECDown = " << _pt_JECDown << " / pt_JECUp = " << _pt_JECUp;
    return os;
}
