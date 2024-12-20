#include "../interface/Jet.h"

//include c++ library classes 
#include <cmath>
#include <stdexcept>
#include <string>
#include <bitset>

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

    // _JECSources_Ids(readAllJECVariations ? treeReader._sourcesJEC_Ids : nullptr),
    // _JECGrouped_Ids(readGroupedJECVariations ? treeReader._groupedJEC_Ids : nullptr),
    selector( new JetSelector( this ) )
{
    if (readAllJECVariations) {
        for (const auto& mapEl : treeReader._jetSmearedPt_JECSourcesUp) {
            std::string key = mapEl.first;
            key = stringTools::removeOccurencesOf(key, "_jetSmearedPt_");
            key = stringTools::removeOccurencesOf(key, "_JECSourcesUp");
            _pt_JECSourcesUp.insert({key, mapEl.second[jetIndex]});
        }
        for (const auto& mapEl : treeReader._jetSmearedPt_JECSourcesDown) {
            std::string key = mapEl.first;
            key = stringTools::removeOccurencesOf(key, "_jetSmearedPt_");
            key = stringTools::removeOccurencesOf(key, "_JECSourcesDown");
            _pt_JECSourcesDown.insert({key, mapEl.second[jetIndex]});
        }
    }
    if (readGroupedJECVariations) {
        for (const auto& mapEl : treeReader._jetSmearedPt_JECGroupedUp) {
            std::string key = mapEl.first;
            key = stringTools::removeOccurencesOf(key, "_jetSmearedPt_");
            key = stringTools::removeOccurencesOf(key, "_JECGroupedUp");
            _pt_JECGroupedUp.insert({key, mapEl.second[jetIndex]});
        }
        for (const auto& mapEl : treeReader._jetSmearedPt_JECGroupedDown) {
            std::string key = mapEl.first;
            key = stringTools::removeOccurencesOf(key, "_jetSmearedPt_");
            key = stringTools::removeOccurencesOf(key, "_JECGroupedDown");
            _pt_JECGroupedDown.insert({key, mapEl.second[jetIndex]});
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

Jet::Jet(const NanoReader& nanoReader, const unsigned jetIndex, const bool useAllJECVariations, const bool readGroupedJECVariations) :
        // TODO: work with mass for fourvector intialization. Maybe put true/false in physobj constructor
        PhysicsObject(
                nanoReader._Jet_pt_nom[jetIndex],
                nanoReader._Jet_eta[jetIndex],
                nanoReader._Jet_phi[jetIndex],
                -1.,
                nanoReader.is2016(),
                nanoReader.is2016PreVFP(),
                nanoReader.is2016PostVFP(),
                nanoReader.is2017(),
                nanoReader.is2018()),
        _deepCSV(nanoReader._Jet_bTagDeepB[jetIndex]),
        _deepFlavor(nanoReader._Jet_bTagDeepFlavB[jetIndex]),
        // JEC:
        _pt_JECDown(nanoReader._Jet_pt_jesTotalDown[jetIndex]),
        _pt_JECUp(nanoReader._Jet_pt_jesTotalUp[jetIndex]),
        // _mass_JECDown(nanoReader._Jet_mass_jesTotalDown[jetIndex]),
        // _mass_JECUp(nanoReader._Jet_mass_jesTotalUp[jetIndex]),
        // JERs:
        _pt_JER_1p93_Down(nanoReader._Jet_pt_jer0Down[jetIndex]),
        _pt_JER_1p93_Up(nanoReader._Jet_pt_jer0Up[jetIndex]),
        _pt_JER_1p93_To_2p5_Down(nanoReader._Jet_pt_jer1Down[jetIndex]),
        _pt_JER_1p93_To_2p5_Up(nanoReader._Jet_pt_jer1Down[jetIndex]),
        // Make sure ignoring mass variations works, if not, add it in!
        // _mass_JER_1p93_Down(nanoReader._Jet_mass_jer0Down[jetIndex]),
        // _mass_JER_1p93_Up(nanoReader._Jet_mass_jer0Up[jetIndex]),
        // _mass_JER_1p93_To_2p5_Down(nanoReader._Jet_mass_jer1Down[jetIndex]),
        // _mass_JER_1p93_To_2p5_Up(nanoReader._Jet_mass_jer1Up[jetIndex]),
        _jetJERIndividualVariationsInitialized(true),
        _pileupid(nanoReader._Jet_puId[jetIndex]),
        selector(new JetSelector(this))
{
    setLorentzVectorWithMass(pt(), eta(), phi(), nanoReader._Jet_mass_nom[jetIndex]);
    if (useAllJECVariations) {
        // get correction stuff from nanoreader, calc corrections and store it here.
    }
    if (readGroupedJECVariations) {
        // reading from nanoreader:
        for (const auto& mapEl : nanoReader._Jet_pt_jesSourcesUp) {
            std::string key = mapEl.first;
            _pt_JECGroupedUp.insert({key, mapEl.second[jetIndex]});
        }
        for (const auto& mapEl : nanoReader._Jet_pt_jesSourcesDown) {
            std::string key = mapEl.first;
            _pt_JECGroupedDown.insert({key, mapEl.second[jetIndex]});
        }
    }

    // set jet hadron flavor, but only for simulation
    if (nanoReader.containsGeneratorInfo()) {
        _hadronFlavor = nanoReader._Jet_hadronFlavor[jetIndex];
    }

    // set jet ID
    std::bitset<3> jetIdBits = std::bitset<3>(nanoReader._Jet_jetId[jetIndex]);
    // TODO: haven't tested this, but it should work. If all events dissappear from high njet regions, this is the culprit.
    _isTight = jetIdBits[1];
    _isTightLeptonVeto = jetIdBits[2];

    // catch potential invalid values of deepCSV and deepFlavor
    if (std::isnan(_deepCSV)) {
        _deepCSV = 0.;
    } else if (_deepCSV < 0) {
        _deepCSV = 0.;
    }
    if (std::isnan(_deepFlavor)) {
        _deepFlavor = 0.;
    } else if (_deepFlavor < 0) {
        _deepFlavor = 0.;
    }

    // check that _hadronFlavor has a known value
    if (!((_hadronFlavor == 0) || (_hadronFlavor == 4) || (_hadronFlavor == 5))) {
        std::string msg = "ERROR in Jet constructor:";
        msg += " jet hadronFlavor is " + std::to_string(_hadronFlavor);
        msg += " while it should be 0, 4 or 5.";
        throw std::runtime_error(msg);
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
    _pt_JER_1p93_Down(rhs._pt_JER_1p93_Down),
    _pt_JER_1p93_Up(rhs._pt_JER_1p93_Up),
    _pt_JER_1p93_To_2p5_Down(rhs._pt_JER_1p93_To_2p5_Down),
    _pt_JER_1p93_To_2p5_Up(rhs._pt_JER_1p93_To_2p5_Up),
    _pt_JECSourcesUp( rhs._pt_JECSourcesUp ),
    _pt_JECSourcesDown( rhs._pt_JECSourcesDown ),
    _pt_JECGroupedUp( rhs._pt_JECGroupedUp ),
    _pt_JECGroupedDown( rhs._pt_JECGroupedDown ),
    _jetJERIndividualVariationsInitialized(rhs._jetJERIndividualVariationsInitialized),
    _pileupid(rhs._pileupid),
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
    _pt_JER_1p93_Down(rhs._pt_JER_1p93_Down),
    _pt_JER_1p93_Up(rhs._pt_JER_1p93_Up),
    _pt_JER_1p93_To_2p5_Down(rhs._pt_JER_1p93_To_2p5_Down),
    _pt_JER_1p93_To_2p5_Up(rhs._pt_JER_1p93_To_2p5_Up),
    _pt_JECSourcesUp( rhs._pt_JECSourcesUp ),
    _pt_JECSourcesDown( rhs._pt_JECSourcesDown ),
    _pt_JECGroupedUp( rhs._pt_JECGroupedUp ),
    _pt_JECGroupedDown( rhs._pt_JECGroupedDown ),
    _jetJERIndividualVariationsInitialized(rhs._jetJERIndividualVariationsInitialized),
    _pileupid(rhs._pileupid),
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
    _pt_JER_1p93_Down = rhs._pt_JER_1p93_Down;
    _pt_JER_1p93_Up = rhs._pt_JER_1p93_Up;
    _pt_JER_1p93_To_2p5_Down = rhs._pt_JER_1p93_To_2p5_Down;
    _pt_JER_1p93_To_2p5_Up = rhs._pt_JER_1p93_To_2p5_Up;
    _pt_JECSourcesUp = rhs._pt_JECSourcesUp;
    _pt_JECSourcesDown = rhs._pt_JECSourcesDown;
    _pt_JECGroupedUp = rhs._pt_JECGroupedUp;
    _pt_JECGroupedDown = rhs._pt_JECGroupedDown;
    _pileupid = rhs._pileupid;
    _jetJERIndividualVariationsInitialized = rhs._jetJERIndividualVariationsInitialized;
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
    if (jetJERIndividualVariationsInitialized()) {
        if (absEta() <= 1.93) return variedJet( _pt_JER_1p93_Down );
        if (absEta() > 1.93 && absEta() <= 2.5) return variedJet( _pt_JER_1p93_To_2p5_Down );
    }
    return variedJet( _pt_JERDown );
}


Jet Jet::JetJERUp() const{
    if (jetJERIndividualVariationsInitialized()) {
        if (absEta() <= 1.93) return variedJet( _pt_JER_1p93_Up );
        if (absEta() > 1.93 && absEta() <= 2.5) return variedJet( _pt_JER_1p93_To_2p5_Up );
    }
    return variedJet( _pt_JERUp );
}

Jet Jet::JetJER_1p93_Down() const {
    if (jetJERIndividualVariationsInitialized()) {
        return variedJet( _pt_JER_1p93_Down );
    }
    double newPt = pt();
    if (absEta() <= 1.93) newPt = _pt_JERDown;

    return variedJet( newPt );
}

Jet Jet::JetJER_1p93_Up() const {
    if (jetJERIndividualVariationsInitialized()) {
        return variedJet( _pt_JER_1p93_Up );
    }
    double newPt = pt();
    if (absEta() <= 1.93) newPt = _pt_JERUp;

    return variedJet( newPt );
}

Jet Jet::JetJER_1p93_To_2p5_Down() const {
    if (jetJERIndividualVariationsInitialized()) {
        return variedJet( _pt_JER_1p93_To_2p5_Down );
    }
    double newPt = pt();
    if (absEta() > 1.93 && absEta() <= 2.5) newPt = _pt_JERDown;

    return variedJet( newPt );
}

Jet Jet::JetJER_1p93_To_2p5_Up() const {
    if (jetJERIndividualVariationsInitialized()) {
        return variedJet( _pt_JER_1p93_To_2p5_Up );
    }
    double newPt = pt();
    if (absEta() > 1.93 && absEta() <= 2.5) newPt = _pt_JERUp;
    
    return variedJet( newPt );
}

Jet Jet::JetJECUp( const std::string& variation ) const {
    double newpt = 0.;
    try {
        newpt = _pt_JECGroupedUp.at(variation);
    } catch(const std::exception& e) {
        try {
            newpt = _pt_JECSourcesUp.at(variation);
        } catch(const std::exception& e) {
            std::cerr << "Jet::JetJECUp Variation " << variation << " not found in JEC sources or grouped variations." << '\n';
            std::cerr << e.what() << '\n';
            exit(1);
        }
    }

    return variedJet(newpt);
}

Jet Jet::JetJECDown( const std::string& variation ) const {
    double newpt = 0.;
    try {
        newpt = _pt_JECGroupedDown.at(variation);
    } catch(const std::exception& e) {
        try {
            newpt = _pt_JECSourcesDown.at(variation);
        } catch(const std::exception& e) {
            std::cerr << "Jet::JetJECDown Variation " << variation << " not found in JEC sources or grouped variations." << '\n';
            std::cerr << e.what() << '\n';
            exit(1);
        }
    }
    return variedJet(newpt);
}

#if JECONRUNTIME

Jet Jet::JetJECUp(JetCorrectionUncertainty* jetCorrUnc) {
    jetCorrUnc->setJetPt( pt() );
    jetCorrUnc->setJetEta( eta() );

    double uncJec = jetCorrUnc->getUncertainty( true );

    double newPt = pt()*( 1 + uncJec );

    return variedJet(newPt);
}

Jet Jet::JetJECDown(JetCorrectionUncertainty* jetCorrUnc) {
    jetCorrUnc->setJetPt( pt() );
    jetCorrUnc->setJetEta( eta() );

    double uncJec = jetCorrUnc->getUncertainty( true );

    double newPt = pt()*( 1 - uncJec );

    return variedJet(newPt);
}

#endif

Jet Jet::JetJECGroupedDown(const std::string& variation) const {
    double newpt = _pt_JECGroupedDown.at(variation);
    return variedJet( newpt );
}


Jet Jet::JetJECGroupedUp(const std::string& variation) const {
    double newpt = _pt_JECGroupedUp.at(variation);
    return variedJet( newpt );
}

Jet Jet::JetJECSourcesDown(const std::string& variation) const {
    double newpt = _pt_JECSourcesUp.at(variation);
    return variedJet( newpt );
}


Jet Jet::JetJECSourcesUp(const std::string& variation) const {
    double newpt = _pt_JECSourcesDown.at(variation);
    return variedJet( newpt );
}

Jet Jet::HEMIssue() const {
    // NanoAOD contains these calculated variations already. Up == nominal, down == hem issue
    if (! is2018()) return variedJet(pt());
    if (! isTight() || pt() < 15.) return variedJet(pt());
    double newPt = 0.;
    if ((eta() > -2.5 && eta() < -1.3) && (phi() > -1.57 && phi() < -0.87)) {
        newPt = pt() * 0.8;
        return variedJet(newPt);
    } else if ((eta() > -3.0 && eta() < -2.5) && (phi() > -1.57 && phi() < -0.87)) {
        newPt = pt() * 0.65;
        return variedJet(newPt);
    } else {
        newPt = pt();
        return variedJet(newPt);
    }
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
