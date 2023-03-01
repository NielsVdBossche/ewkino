#include "../interface/GeneratorInfo.h"

//include c++ library classes 
#include <stdexcept>


GeneratorInfo::GeneratorInfo( const TreeReader& treeReader ) :
    _numberOfLHEPdfWeights( treeReader._nLHEPdfWeight ),
    _numberOfLHEScaleWeights( treeReader._nLHEScaleWeight ),
    _numberOfPsWeights( treeReader._nPSWeight ),
    //_numberOfTrueInteractions( treeReader._nTrueInt ),
    _genMetPtr( new GenMet( treeReader ) )
{
    // fill pdf weights
    if( _numberOfLHEPdfWeights > maxNumberOfLHEPdfWeights ){
	std::string message = "ERROR in GeneratorInfo::GeneratorInfo:";
	message.append( " _numberOfLHEPdfWeights is " + std::to_string(_numberOfLHEPdfWeights) );
	message.append( " which is larger than " + std::to_string(maxNumberOfLHEPdfWeights) );
	message.append( " (the maximum array size of _LHEPdfWeights)." );
	throw std::out_of_range( message );
    }
    for( unsigned i = 0; i < _numberOfLHEPdfWeights; ++i  ){
        _LHEPdfWeights[i] = treeReader._LHEPdfWeight[i];
    }

    // fill scale weights
    if( _numberOfLHEScaleWeights > maxNumberOfLHEScaleWeights ){
        std::string message = "ERROR in GeneratorInfo::GeneratorInfo:";
        message.append( " _numberOfLHEScaleWeights is " + std::to_string(_numberOfLHEScaleWeights) );
        message.append( " which is larger than " + std::to_string(maxNumberOfLHEScaleWeights) );
        message.append( " (the maximum array size of _LHEScaleWeights)." );
        throw std::out_of_range( message );
    }
    for( unsigned i = 0; i < _numberOfLHEScaleWeights; ++i  ){
        _LHEScaleWeights[i] = treeReader._LHEScaleWeight[i];
    }

    // fill parton shower weights
    if( _numberOfPsWeights > maxNumberOfPsWeights ){
	std::string message = "ERROR in GeneratorInfo::GeneratorInfo:";
	message.append( " _numberOfPsWeights is " + std::to_string(_numberOfPsWeights) );
	message.append( " which is larger than " + std::to_string(maxNumberOfPsWeights) );
	message.append( " (the maximum array size of _psWeights)." );
	throw std::out_of_range( message );
    }
    // check if sample contains less PS weights than expected
    if( _numberOfPsWeights < maxNumberOfPsWeights ){
        std::string message = "WARNING in GeneratorInfo::GeneratorInfo:";
        message.append( " _numberOfPsWeights is " + std::to_string(_numberOfPsWeights) );
        message.append( " which is smaller than " + std::to_string(maxNumberOfPsWeights) );
        message.append( " (the expected array size of _psWeights)." );
        throw std::out_of_range( message );
    }
    for( unsigned i = 0; i < _numberOfPsWeights; ++i ){
        _psWeights[i] = treeReader._PSWeight[i];
    }
}

double retrieveWeight( const double* array, const unsigned index, 
    const unsigned offset, const unsigned maximumIndex, 
    const std::string& name ){
    if( index >= maximumIndex ){
        std::string maximumIndexStr = std::to_string( maximumIndex );
	std::string msg = "ERROR: only " + maximumIndexStr + " " + name + " variations are available";
        msg += " and an index larger or equal than " + maximumIndexStr + " is requested.";
	throw std::out_of_range(msg);	
    }
    return array[ index + offset ];
}


double GeneratorInfo::relativeWeightPdfVar( const unsigned pdfIndex ) const{
    return retrieveWeight( _LHEPdfWeights, pdfIndex, 0, _numberOfLHEPdfWeights, "pdf" );
}


double GeneratorInfo::relativeWeightScaleVar( const unsigned scaleIndex ) const{
    return retrieveWeight( _LHEScaleWeights, scaleIndex, 0, _numberOfLHEScaleWeights, "scale" );
}


double GeneratorInfo::relativeWeightPsVar( const unsigned psIndex ) const{
    return retrieveWeight( _psWeights, psIndex, 0, _numberOfPsWeights, "parton shower" ); 
}
