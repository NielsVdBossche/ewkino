#include "../interface/GeneratorInfo.h"

//include c++ library classes 
#include <stdexcept>

GeneratorInfo::GeneratorInfo( const TreeReader& treeReader ) :
    _numberOfLheWeights( treeReader._nLheWeights ),
    _numberOfPsWeights( treeReader._nPsWeights ),
    _prefireWeight( treeReader._prefireWeight ),
    _prefireWeightDown( treeReader._prefireWeightDown ),
    _prefireWeightUp( treeReader._prefireWeightUp ),
    _ttgEventType( treeReader._ttgEventType ),
    _zgEventType( treeReader._zgEventType ),
    _numberOfTrueInteractions( treeReader._nTrueInt ),
    _genMetPtr( new GenMet( treeReader ) )
{ 
    if( _numberOfLheWeights > maxNumberOfLheWeights ){
        throw std::out_of_range( "_numberOfLheWeights is larger than 148, which is the maximum array size of _lheWeights." );
    }
    for( unsigned i = 0; i < _numberOfLheWeights; ++i  ){
        _lheWeights[i] = treeReader._lheWeight[i];
    }

    if( _numberOfPsWeights > maxNumberOfPsWeights ){
        throw std::out_of_range( "_numberOfPsWeights is larger than 46, which is the maximum array size of _psWeights." );
    }
    for( unsigned i = 0; i < _numberOfPsWeights; ++i ){
        _psWeights[i] = treeReader._psWeight[i];
    }

    if (treeReader.containsLheInfo()) {
        lheCollectionPtr = new LheCollection(treeReader);
    }

    _firstScaleIndex = 0;
    _numberOfScaleVariations = std::min( treeReader._nLheWeights, unsigned(9) );
    _firstPdfIndex = 9;
    _numberOfPdfVariations = std::min( std::max( treeReader._nLheWeights, unsigned(9) ) - 9, unsigned(103)); 
}

GeneratorInfo::GeneratorInfo(const NanoReader& nanoReader) : 
    miniAODSetup(false),
    _numberOfLHEPdfWeights(nanoReader._nLHEPdfWeight),
    _numberOfLHEScaleWeights(nanoReader._nLHEScaleWeight),
    _numberOfPsWeights(nanoReader._nPSWeight),
    _prefireWeight(nanoReader._L1PreFiringWeight_Nom),
    _prefireWeightDown(nanoReader._L1PreFiringWeight_Dn),
    _prefireWeightUp(nanoReader._L1PreFiringWeight_Up),
    _numberOfTrueInteractions(nanoReader._Pileup_nTrueInt),
    _genMetPtr(new GenMet(nanoReader))
{
    // fill pdf weights
    if (_numberOfLHEPdfWeights > maxNumberOfLHEPdfWeights) {
        std::string message = "ERROR in GeneratorInfo::GeneratorInfo:";
        message.append(" _numberOfLHEPdfWeights is " + std::to_string(_numberOfLHEPdfWeights));
        message.append(" which is larger than " + std::to_string(maxNumberOfLHEPdfWeights));
        message.append(" (the maximum array size of _LHEPdfWeights).");
        throw std::out_of_range(message);
    }
    for (unsigned i = 0; i < _numberOfLHEPdfWeights; ++i) {
        _LHEPdfWeights[i] = nanoReader._LHEPdfWeight[i];
    }

    // fill scale weights
    if (_numberOfLHEScaleWeights > maxNumberOfLHEScaleWeights) {
        std::string message = "ERROR in GeneratorInfo::GeneratorInfo:";
        message.append(" _numberOfLHEScaleWeights is " + std::to_string(_numberOfLHEScaleWeights));
        message.append(" which is larger than " + std::to_string(maxNumberOfLHEScaleWeights));
        message.append(" (the maximum array size of _LHEScaleWeights).");
        throw std::out_of_range(message);
    }
    for (unsigned i = 0; i < _numberOfLHEScaleWeights; ++i) {
        _LHEScaleWeights[i] = nanoReader._LHEScaleWeight[i];
    }

    // fill parton shower weights
    if (_numberOfPsWeights > maxNumberOfPsWeightsNano) {
        std::string message = "ERROR in GeneratorInfo::GeneratorInfo:";
        message.append(" _numberOfPsWeights is " + std::to_string(_numberOfPsWeights));
        message.append(" which is larger than " + std::to_string(maxNumberOfPsWeightsNano));
        message.append(" (the maximum array size of _psWeights).");
        throw std::out_of_range(message);
    }
    // check if sample contains less PS weights than expected
    if (_numberOfPsWeights < maxNumberOfPsWeightsNano) {
        std::string message = "WARNING in GeneratorInfo::GeneratorInfo:";
        message.append(" _numberOfPsWeights is " + std::to_string(_numberOfPsWeights));
        message.append(" which is smaller than " + std::to_string(maxNumberOfPsWeightsNano));
        message.append(" (the expected array size of _psWeights).");
        throw std::out_of_range(message);
    }
    for (unsigned i = 0; i < _numberOfPsWeights; ++i) {
        _psWeights[i] = nanoReader._PSWeight[i];
    }
    // not really necessary normally. Might remove
    for (unsigned i = _numberOfPsWeights; i < maxNumberOfPsWeightsNano; ++i) {
        _psWeights[i] = 0.;
    }
}

GeneratorInfo::~GeneratorInfo() {
    delete lheCollectionPtr;
}

double retrieveWeight( const double* array, const unsigned index, const unsigned offset, const unsigned maximumIndex, const std::string& name ){
    if( index >= maximumIndex ){
        std::string maximumIndexStr = std::to_string( maximumIndex );
        throw std::out_of_range( "Only " + maximumIndexStr + " " + name + " variations are available, and an index larger or equal than " + maximumIndexStr + " is requested." );
    }
    return array[ index + offset ];
}


double GeneratorInfo::relativeWeightPdfVar( const unsigned pdfIndex ) const{
    if (miniAODSetup) {
        return retrieveWeight( _lheWeights, pdfIndex, 9, std::min( std::max( _numberOfLheWeights, unsigned(9) ) - 9, unsigned(103) ), "pdf" );
    } else {
        return retrieveWeight( _LHEPdfWeights, pdfIndex, 0, std::min( _numberOfLheWeights, unsigned(103) ), "pdf" );
    }
}


double GeneratorInfo::relativeWeightScaleVar( const unsigned scaleIndex ) const{
    if (miniAODSetup) {
        return retrieveWeight( _lheWeights, scaleIndex, 0, std::min( _numberOfLheWeights, unsigned(9) ), "scale" );
    } else {
        return retrieveWeight( _LHEScaleWeights, scaleIndex, 0, std::min( _numberOfLheWeights, unsigned(9) ), "scale" );
    }
}


double GeneratorInfo::relativeWeightPsVar( const unsigned psIndex ) const{
    return retrieveWeight( _psWeights, psIndex, 0, std::min( _numberOfPsWeights, unsigned(45) ), "parton shower" ); 
}
