#include "../interface/TriggerInfo.h"

//include c++ library classes
#include <iostream>
#include <stdexcept>

//include other parts of framework
#include "../../Tools/interface/stringTools.h"


//remove leading _ from trigger and filter names 
std::string cleanName( const std::string& name ){
    std::string ret( name );
    if( stringTools::stringStartsWith( name, "_" ) ){
        ret.erase(0, 1);
    }
    return ret;
}

bool passAnyTrigger(const std::vector<Bool_t*>& triggerBits) {
    for (auto& trigBit : triggerBits) {
        if (*trigBit) return true;
    }
    return false;
}

TriggerInfo::TriggerInfo(const TreeReader& treeReader, const bool readIndividualTriggers, const bool readIndividualMetFilters) :
    _passTriggers_e( treeReader._passTrigger_e ),
    _passTriggers_m( treeReader._passTrigger_m ),
    _passTriggers_ee( treeReader._passTrigger_ee ),
    _passTriggers_em( treeReader._passTrigger_em ),
    _passTriggers_et( treeReader._passTrigger_et ),
    _passTriggers_mm( treeReader._passTrigger_mm ),
    _passTriggers_mt( treeReader._passTrigger_mt ),
    _passTriggers_eee( treeReader._passTrigger_eee ),
    _passTriggers_eem( treeReader._passTrigger_eem ),
    _passTriggers_emm( treeReader._passTrigger_emm ),
    _passTriggers_mmm( treeReader._passTrigger_mmm ),
    _passTriggers_FR( treeReader._passTrigger_FR ),
    _passTriggers_FR_iso( treeReader._passTrigger_FR_iso ),
    _passMetFilters( treeReader._passMETFilters )
{
    if( readIndividualTriggers ){
        for( const auto& trigger : treeReader._triggerMap ){
            individualTriggerMap.insert( { cleanName( trigger.first ), trigger.second } );
        }
    }
    if( readIndividualMetFilters ){
        for( const auto& filter : treeReader._MetFilterMap ){
            individualMetFilterMap.insert( { cleanName( filter.first ), filter.second } );
        }   
    }
}

// Triggers can be simplified with skimmer, but this is pretty
TriggerInfo::TriggerInfo(const NanoReader& nanoReader, const bool readIndividualTriggers, const bool readIndividualMetFilters) :
    _passTriggers_e(nanoReader._HLT_trigger_e),
    _passTriggers_m(nanoReader._HLT_trigger_m),
    _passTriggers_ee(nanoReader._HLT_trigger_ee),
    _passTriggers_em(nanoReader._HLT_trigger_em),
    _passTriggers_mm(nanoReader._HLT_trigger_mm),
    _passTriggers_eee(nanoReader._HLT_trigger_eee),
    _passTriggers_eem(nanoReader._HLT_trigger_eem),
    _passTriggers_emm(nanoReader._HLT_trigger_emm),
    _passTriggers_mmm(nanoReader._HLT_trigger_mmm),
    _passTriggers_FR(nanoReader._HLT_trigger_FR),
    _passTriggers_FR_iso(nanoReader._HLT_trigger_FR_iso)
{
    if( readIndividualTriggers ){
        // currently not supported
        std::cerr << "Readining individual triggers is not supported for NanoAOD skims\n" << std::endl;
        exit(1);
    }
    if( readIndividualMetFilters ){
        // currently not supported
        std::cerr << "Readining individual MET filters is not supported for NanoAOD skims\n" << std::endl;
        exit(1);
    }
    // MET Filter implementation following recommendations here:
    // https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#UL_data
    // Also note on this page: "2. For analyses using NanoAOD (as of Sep 2020): 
    // Do not use the flag: Flag_METFilters (this also includes some filters 
    // that are deprecated and can lead to a loss in efficiency), instead use 
    // the individual flags for the different recommended filters."
    _passMetFilters = false;
    _passMetFilters = _passMetFilters & nanoReader._Flag_goodVertices;
    _passMetFilters = _passMetFilters & nanoReader._Flag_globalSuperTightHalo2016Filter;
    _passMetFilters = _passMetFilters & nanoReader._Flag_HBHENoiseFilter;
    _passMetFilters = _passMetFilters & nanoReader._Flag_HBHENoiseIsoFilter;
    _passMetFilters = _passMetFilters & nanoReader._Flag_BadPFMuonFilter;
    _passMetFilters = _passMetFilters & nanoReader._Flag_BadPFMuonDzFilter;
    _passMetFilters = _passMetFilters & nanoReader._Flag_eeBadScFilter;
    if (nanoReader.is2017() || nanoReader.is2018()) {
        _passMetFilters = _passMetFilters & nanoReader._Flag_ecalBadCalibFilter;
    }
    if (nanoReader.is2016()) {
        _passMetFilters = _passMetFilters & nanoReader._Flag_EcalDeadCellTriggerPrimitiveFilter;
    }
}

bool passTriggerOrFilter( const std::map< std::string, bool >& decisionMap,  const std::string& name ){
    auto decisionIt = decisionMap.find( name );

    //throw error if non-existing trigger or MET filter is requested
    if( decisionIt == decisionMap.cend() ){
        throw std::invalid_argument( "Requested trigger or MET filter '" + name + "' does not exist." );
    } else {
        return (*decisionIt).second;
    }
}


bool TriggerInfo::passTrigger( const std::string& triggerName ) const{
    return passTriggerOrFilter( individualTriggerMap, triggerName );
}


bool TriggerInfo::passMetFilter( const std::string& filterName ) const{
    return passTriggerOrFilter( individualMetFilterMap, filterName );
}


void printAvailableInfo( const std::map< std::string, bool >& decisionMap, const std::string& decisionType ){
    std::cout << "Available " << decisionType << " :\n";
    for( const auto& decision : decisionMap ){
        std::cout << decision.first << "\n";
    }
}


void TriggerInfo::printAvailableIndividualTriggers() const{
    printAvailableInfo( individualTriggerMap, "triggers" );
}


void TriggerInfo::printAvailableMetFilters() const{
    printAvailableInfo( individualMetFilterMap, "MET filters");
}
