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
    _partonLevelHT( treeReader._lheHTIncoming ),
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
        throw std::out_of_range( "_numberOfPsWeights is larger than 14, which is the maximum array size of _psWeights." );
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


double retrieveWeight( const double* array, const unsigned index, const unsigned offset, const unsigned maximumIndex, const std::string& name ){
    if( index >= maximumIndex ){
        std::string maximumIndexStr = std::to_string( maximumIndex );
        throw std::out_of_range( "Only " + maximumIndexStr + " " + name + " variations are available, and an index larger or equal than " + maximumIndexStr + " is requested." );
    }
    return array[ index + offset ];
}


double GeneratorInfo::relativeWeightPdfVar( const unsigned pdfIndex ) const{
    return retrieveWeight( _lheWeights, pdfIndex, 9, std::min( std::max( _numberOfLheWeights, unsigned(9) ) - 9, unsigned(103) ), "pdf" );
}


double GeneratorInfo::relativeWeightScaleVar( const unsigned scaleIndex ) const{
    return retrieveWeight( _lheWeights, scaleIndex, 0, std::min( _numberOfLheWeights, unsigned(9) ), "scale" );
}


double GeneratorInfo::relativeWeightPsVar( const unsigned psIndex ) const{
    return retrieveWeight( _psWeights, psIndex, 0, std::min( _numberOfPsWeights, unsigned(14) ), "parton shower" ); 
}
