#include "../interface/JetInfo.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"
// include c++ library classes
#include <iostream>
#include <stdexcept>

// include other parts of the framework
#include "../../Tools/interface/stringTools.h"

std::string cleanJECVariationName( std::string branchName ){
    // convert a branch name holding a JEC variation into the pure JEC variation name
    std::string jecName = stringTools::removeOccurencesOf(branchName,"_jetPt_");
    jecName = stringTools::removeOccurencesOf(jecName,"_jetSmearedPt_");
    jecName = stringTools::removeOccurencesOf(jecName,"_JECSourcesUp");
    jecName = stringTools::removeOccurencesOf(jecName,"_JECSourcesDown");
    jecName = stringTools::removeOccurencesOf(jecName,"_JECGroupedUp");
    jecName = stringTools::removeOccurencesOf(jecName,"_JECGroupedDown");
    return jecName;
}

JetInfo::JetInfo( const TreeReader& treeReader, 
		  const bool readAllJECVariations,
		  const bool readGroupedJECVariations ){
    _JECSources = std::vector<std::string>();
    _JECGrouped = std::vector<std::string>();
    if (readAllJECVariations) {
        _sourcesJEC_Ids = treeReader._sourcesJEC_Ids;

        for (auto mapEl : *_sourcesJEC_Ids) {
            _JECSources.push_back(cleanJECVariationName(mapEl.first));
        }
    }
    if (readGroupedJECVariations) {
        _groupedJEC_Ids = treeReader._groupedJEC_Ids;
        for (auto mapEl : *_groupedJEC_Ids) {
            _JECGrouped.push_back(cleanJECVariationName(mapEl.first));
        }
    }
}

JetInfo::JetInfo( const NanoReader& nanoReader,
		  const bool readAllJECVariations,
		  const bool readGroupedJECVariations  ) {
    _JECSources = {};
    _JECGrouped = {};
    if (readAllJECVariations) {
    }
    if (readGroupedJECVariations) {
    }
}


bool JetInfo::hasJECVariation( const std::string& jecName ) const{
    bool isInAllVariations = ( std::find(_JECSources.begin(),_JECSources.end(),
				jecName) != _JECSources.end() );
    bool isInGroupedVariations = ( std::find(_JECGrouped.begin(),
				    _JECGrouped.end(),
				    jecName) != _JECGrouped.end() );
    return (isInAllVariations || isInGroupedVariations);
}

void printAvailableInfo( const std::vector<std::string>& info, const std::string& message ){
    std::cout << message << std::endl;
    for( std::string el: info ){
	std::cout << el << std::endl;
    } 
}

void JetInfo::printAllJECVariations() const{
    std::string message = "Available JEC variations (complete set):";
    printAvailableInfo( _JECSources, message );
}

void JetInfo::printGroupedJECVariations() const{
    std::string message = "Available JEC variations (reduced set):";
    printAvailableInfo( _JECGrouped, message );
}
