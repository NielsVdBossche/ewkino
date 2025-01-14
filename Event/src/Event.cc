#include "../interface/Event.h"


//include other pars of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../constants/particleMasses.h"


Event::Event( const TreeReader& treeReader, 
		const bool readIndividualTriggers , const bool readIndividualMetFilters,
		const bool readAllJECVariations, const bool readGroupedJECVariations ) :
    // make collections of physics objects
    _leptonCollectionPtr( new LeptonCollection( treeReader ) ),
    _jetCollectionPtr( new JetCollection( treeReader,
			readAllJECVariations, readGroupedJECVariations ) ),
    _metPtr( new Met( treeReader,
			readAllJECVariations, readGroupedJECVariations ) ),
    // make additional information structures
    _triggerInfoPtr( new TriggerInfo( treeReader, 
			readIndividualTriggers, readIndividualMetFilters ) ),
    _jetInfoPtr( new JetInfo( treeReader, 
			readAllJECVariations, readGroupedJECVariations ) ),
    _eventTagsPtr( new EventTags( treeReader ) ),
    _generatorInfoPtr( treeReader.isMC() ? new GeneratorInfo( treeReader ) : nullptr ),
    _numberOfVertices( treeReader._nVertex ),

    //WARNING : use treeReader::_scaledWeight instead of treeReader::_weight since the former already includes cross-section and lumiosity scaling
    _weight( treeReader._scaledWeight ),
    _samplePtr( treeReader.currentSamplePtr() ),
    _particleLevelInfoPtr( treeReader.hasPL() ? new ParticleLevelInfo(treeReader) : nullptr),
    _genLevelPtr( treeReader.hasGenLvl() ? new GenParticlesTop(treeReader) : nullptr) {
    // std::cout << "Event constructed from HeavyNeutrino reader" << std::endl;
}

Event::Event(const NanoReader& nanoReader,
        const bool readIndividualTriggers , const bool readIndividualMetFilters,
        const bool readAllJECVariations, const bool readGroupedJECVariations) :  // make collections of physics objects
    _leptonCollectionPtr(new LeptonCollection(nanoReader)),
    _jetCollectionPtr(new JetCollection(nanoReader)),
    _metPtr(new Met(nanoReader, readAllJECVariations, readGroupedJECVariations) ),
    // make additional information structures
    _triggerInfoPtr(new TriggerInfo(nanoReader, readIndividualTriggers, readIndividualMetFilters)),
    _jetInfoPtr( new JetInfo( nanoReader, readAllJECVariations, readGroupedJECVariations) ),
    _eventTagsPtr(new EventTags(nanoReader)),
    _generatorInfoPtr(nanoReader.isMC() ? new GeneratorInfo(nanoReader) : nullptr),
    _numberOfVertices( nanoReader._PV_npvs ),
    _weight(nanoReader._scaledWeight),
    _genWeight(nanoReader._genWeight),
    _samplePtr(nanoReader.currentSamplePtr()),
    _particleLevelInfoPtr( nanoReader.hasGenLvl() ? new ParticleLevelInfo(nanoReader) : nullptr),
    _genLevelPtr( nanoReader.hasGenLvl() ? new GenParticlesTop(nanoReader) : nullptr)
{
    // std::cout << "Event constructed from NanoAOD reader" << std::endl;
}

Event::~Event(){
    delete _leptonCollectionPtr;
    delete _jetCollectionPtr;
    delete _metPtr;
    delete _triggerInfoPtr;
    delete _jetInfoPtr;
    delete _eventTagsPtr;

    if (_lightLeptonCollPtr) {
        delete _lightLeptonCollPtr;
    }
    if (_electronCollectionPtr) {
        delete _electronCollectionPtr;
    }
    if (_muonCollectionPtr) {
        delete _muonCollectionPtr;
    }
    if (_tauCollectionPtr) {
        delete _tauCollectionPtr;
    }
    if( hasGeneratorInfo() ){
        delete _generatorInfoPtr;
    }
    if (hasPLInfo()){
        delete _particleLevelInfoPtr;
    }
    if (hasGenLevelInfo()) {
        delete _genLevelPtr;
    }
}


Event::Event( const Event& rhs ) :
    _leptonCollectionPtr( new LeptonCollection( *rhs._leptonCollectionPtr ) ),
    _jetCollectionPtr( new JetCollection( *rhs._jetCollectionPtr ) ),
    _metPtr( new Met( *rhs._metPtr ) ),
    _triggerInfoPtr( new TriggerInfo( *rhs._triggerInfoPtr ) ),
    _jetInfoPtr( new JetInfo( *rhs._jetInfoPtr ) ),
    _eventTagsPtr( new EventTags( *rhs._eventTagsPtr ) ),
    _generatorInfoPtr( rhs.hasGeneratorInfo() ? new GeneratorInfo( *rhs._generatorInfoPtr ) : nullptr ),
    _numberOfVertices( rhs._numberOfVertices ),
    _weight( rhs._weight ),
    _samplePtr( rhs._samplePtr )
    {}


Event::Event( Event&& rhs ) noexcept :
    _leptonCollectionPtr( rhs._leptonCollectionPtr ),
    _jetCollectionPtr( rhs._jetCollectionPtr ),
    _metPtr( rhs._metPtr ),
    _triggerInfoPtr( rhs._triggerInfoPtr ),
    _jetInfoPtr( rhs._jetInfoPtr ),
    _eventTagsPtr( rhs._eventTagsPtr ),
    _generatorInfoPtr( rhs._generatorInfoPtr ),
    _numberOfVertices( rhs._numberOfVertices ),
    _weight( rhs._weight ),
    _samplePtr( rhs._samplePtr )
{
    rhs._leptonCollectionPtr = nullptr;
    rhs._jetCollectionPtr = nullptr;
    rhs._metPtr = nullptr;
    rhs._triggerInfoPtr = nullptr;
    rhs._jetInfoPtr = nullptr;
    rhs._eventTagsPtr = nullptr;
    rhs._generatorInfoPtr = nullptr;
    rhs._samplePtr = nullptr;
}
    

Event& Event::operator=( const Event& rhs ){
    if( this != &rhs ){
        delete _leptonCollectionPtr;
        delete _jetCollectionPtr;
        delete _metPtr;
        delete _triggerInfoPtr;
	delete _jetInfoPtr;
        delete _eventTagsPtr;
        if( hasGeneratorInfo() ){
            delete _generatorInfoPtr;
        }

        _leptonCollectionPtr = new LeptonCollection( *rhs._leptonCollectionPtr );
        _jetCollectionPtr = new JetCollection( *rhs._jetCollectionPtr );
        _metPtr = new Met( *rhs._metPtr );
        _triggerInfoPtr = new TriggerInfo( *rhs._triggerInfoPtr );
	_jetInfoPtr = new JetInfo( *rhs._jetInfoPtr );
        _eventTagsPtr = new EventTags( *rhs._eventTagsPtr );
        _generatorInfoPtr = rhs.hasGeneratorInfo() ? new GeneratorInfo( *rhs._generatorInfoPtr ) : nullptr;

        _numberOfVertices = rhs._numberOfVertices;
        _weight = rhs._weight;
        _samplePtr = rhs._samplePtr;
    }
    return *this;
}


Event& Event::operator=( Event&& rhs ) noexcept{
    if( this != &rhs ){
        delete _leptonCollectionPtr;
        delete _jetCollectionPtr;
        delete _metPtr;
        delete _triggerInfoPtr;
	delete _jetInfoPtr;
        delete _eventTagsPtr;
        if( hasGeneratorInfo() ){
            delete _generatorInfoPtr;
        }

        _leptonCollectionPtr = rhs._leptonCollectionPtr;
        rhs._leptonCollectionPtr = nullptr;
        _jetCollectionPtr = rhs._jetCollectionPtr;
        rhs._jetCollectionPtr = nullptr;
        _metPtr = rhs._metPtr;
        rhs._metPtr = nullptr;
        _triggerInfoPtr = rhs._triggerInfoPtr;
        rhs._triggerInfoPtr = nullptr;
	_jetInfoPtr = rhs._jetInfoPtr;
	rhs._jetInfoPtr = nullptr;
        _eventTagsPtr = rhs._eventTagsPtr;
        rhs._eventTagsPtr = nullptr;
        _generatorInfoPtr = rhs._generatorInfoPtr;
        rhs._generatorInfoPtr = nullptr;

        _numberOfVertices = rhs._numberOfVertices;
        _weight = rhs._weight;
        _samplePtr = rhs._samplePtr;
    }
    return *this;
}


void Event::checkGeneratorInfo() const{
    if( !hasGeneratorInfo() ){
        throw std::domain_error( "Trying to access generator information for a data event!" );
    }
}


GeneratorInfo& Event::generatorInfo() const{
    checkGeneratorInfo();
    return *_generatorInfoPtr;
}

GeneratorInfo* Event::getGeneratorInfoPtr() const{
    checkGeneratorInfo();
    return _generatorInfoPtr;
}

void Event::selectMediumBjets() {
    _bJetCollectionPtr = new JetCollection();
    *_bJetCollectionPtr = _jetCollectionPtr->mediumBTagCollection();
}


void Event::initializeZBosonCandidate(){
    if( !ZIsInitialized ){

        //check that there are at least two leptons is performed automatically in LeptonCollection
        
        //leading lepton not used in this pairing is considered to be from the W decay (in trilepton events )
        //BUT in order to have consistent indices, sort leptons by pT already here.
        sortLeptonsByPt();

        //reconstruct the best Z boson
        std::pair< std::pair< LeptonCollection::size_type, LeptonCollection::size_type >, double > ZBosonCandidateIndicesAndMass = _leptonCollectionPtr->bestZBosonCandidateIndicesAndMass();
        _bestZBosonCandidateIndices = ZBosonCandidateIndicesAndMass.first;
        _bestZBosonCandidateMass = ZBosonCandidateIndicesAndMass.second;

        //note that the third lepton can also be a tau in this case!
        if( numberOfLeptons() >= 3 ){
            for( LeptonCollection::size_type leptonIndex = 0; leptonIndex < numberOfLeptons(); ++leptonIndex ){
                if( !( leptonIndex == _bestZBosonCandidateIndices.first || leptonIndex == _bestZBosonCandidateIndices.second ) ){
                    _WLeptonIndex = leptonIndex;
                    break;
                }
            }
        }

        ZIsInitialized = true;
    }
}


std::pair< std::pair< LeptonCollection::size_type, LeptonCollection::size_type >, double > Event::bestZBosonCandidateIndicesAndMass(){
    initializeZBosonCandidate();    
    return { _bestZBosonCandidateIndices, _bestZBosonCandidateMass };
}


std::pair< LeptonCollection::size_type, LeptonCollection::size_type > Event::bestZBosonCandidateIndices(){
    initializeZBosonCandidate();
    return _bestZBosonCandidateIndices;
}


double Event::bestZBosonCandidateMass(){
    initializeZBosonCandidate();
    return _bestZBosonCandidateMass;
}


bool Event::hasZTollCandidate( const double oneSidedMassWindow ){
    initializeZBosonCandidate();
    return ( fabs( bestZBosonCandidateMass() - particle::mZ ) < oneSidedMassWindow );
}


LeptonCollection::size_type Event::WLeptonIndex(){
    initializeZBosonCandidate();
    return _WLeptonIndex;
}


double Event::mtW(){
    initializeZBosonCandidate();
    return mt( WLepton(), met() );
}

// copy event with modified energy/momentum scales
// experimental stage! need to check what attributes are copied or modified in-place!

void Event::setLeptonCollection( const LeptonCollection& lepCollection ){
    _leptonCollectionPtr = new LeptonCollection( lepCollection);
}

Event Event::variedLeptonCollectionEvent(
		    LeptonCollection (LeptonCollection::*variedCollection)() const ) const{
    Event newevt = Event( *this );
    LeptonCollection lepCollection = (this->leptonCollection().*variedCollection)();
    //newevt._leptonCollectionPtr = new LeptonCollection( lepCollection );
    // (the above does not work since _leptonCollectionPtr is private,
    //  try to solve by defining a public function doing the same thing, but not optimal.)
    newevt.setLeptonCollection( lepCollection );  
    return newevt;
}

Event Event::electronScaleUpEvent() const{
    return variedLeptonCollectionEvent( &LeptonCollection::electronScaleUpCollection );
}

Event Event::electronScaleDownEvent() const{
    return variedLeptonCollectionEvent( &LeptonCollection::electronScaleDownCollection );
}

Event Event::electronResUpEvent() const{
    return variedLeptonCollectionEvent( &LeptonCollection::electronResUpCollection );
}

Event Event::electronResDownEvent() const{
    return variedLeptonCollectionEvent( &LeptonCollection::electronResDownCollection );
}

bool Event::passTTGOverlap(int sampleType) const {
    if (sampleType == 0 || isData()) return true;

    unsigned int ttgEventType = _generatorInfoPtr->ttgEventType();
    
    if (sampleType == 1 && ttgEventType > 2) {
        return false;
    } else if (sampleType == 2 && ttgEventType < 3) {
        return false;
    }

    if (sampleType == 2) {
        LheCollection* lheInfo = _generatorInfoPtr->getLheCollection();
        if (! lheInfo) return true;

        for (auto it : *lheInfo) {
            if (it->getPdgId() != 22) continue;
            if (it->pt() < 10) return false;
        }
    }

    return true;
}

bool Event::passZGOverlap(int sampleType) const {
    if (sampleType == 0 || isData()) return true;

    unsigned int ttgEventType = _generatorInfoPtr->ttgEventType();
    
    if (sampleType == 1 && ttgEventType > 2) {
        return false;
    } else if (sampleType == 2 && ttgEventType < 3) {
        return false;
    }

    return true;
}