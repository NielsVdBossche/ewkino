#include "../interface/TriggerInfo.h"

//include c++ library classes
#include <iostream>
#include <stdexcept>

//include other parts of framework
#include "../../Tools/interface/stringTools.h"


// internal helper functions

std::string cleanName( const std::string& name ){
    // name cleaning
    std::string ret( name );
    if( stringTools::stringStartsWith( name, "_" ) ){
        ret.erase(0, 1);
    }
    return ret;
}


bool passTriggerOrFilter(
    const std::map< std::string, bool >& decisionMap,
    const std::string& name ){
    // check if a trigger of filter is passed
    auto decisionIt = decisionMap.find( name );
    // throw error if non-existing trigger or MET filter is requested
    if( decisionIt == decisionMap.cend() ){
	std::string msg = "ERROR in TriggerInfo::passTriggerOrFilter:";
	msg += " requested trigger or MET filter '" + name + "' does not exist.";
        throw std::invalid_argument(msg);
	//std::cout << msg << std::endl;
    }
    return (*decisionIt).second;
}


bool passAtLeastOneTrigger(
    const std::map<std::string, bool>& decisionMap,
    const std::vector<std::string>& names ){
    // check if at least one trigger is passed
    for( std::string name : names ){
        if( passTriggerOrFilter(decisionMap,name) ) return true;
    }
    return false;
}


bool passAllTriggers(
    const std::map<std::string, bool>& decisionMap,
    const std::vector<std::string>& names ){
    // check if at least one trigger is passed
    for( std::string name : names ){
        if( !passTriggerOrFilter(decisionMap,name) ) return false;
    }
    return true;
}


// constructor

TriggerInfo::TriggerInfo( const TreeReader& treeReader,
			  const bool readIndividualTriggers, 
			  const bool readIndividualMetFilters ){
    if( readIndividualTriggers ){
        for( const auto& trigger : treeReader._triggerMap ){
            individualTriggerMap.insert( { cleanName( trigger.first ), trigger.second } );
        }
    }
    if( readIndividualMetFilters ){
        for( const auto& filter : treeReader._METFilterMap ){
            individualMetFilterMap.insert( { cleanName( filter.first ), filter.second } );
        }   
    }
    // composite trigger definitions
    // definition copied from ntuplizer
    // (see here: https://github.com/GhentAnalysis/heavyNeutrino/blob/
    //  UL_master/multilep/src/TriggerAnalyzer.cc)
    std::map<std::string, std::vector<std::string>> triggerNames;
    if( treeReader.is2018() ){
	triggerNames["passTrigger_FR"] = {"HLT_Mu3_PFJet40", "HLT_Mu8", "HLT_Mu17", "HLT_Mu20", 
					  "HLT_Mu27", "HLT_Ele8_CaloIdM_TrackIdM_PFJet30", 
					  "HLT_Ele17_CaloIdM_TrackIdM_PFJet30", 
					  "HLT_Ele23_CaloIdM_TrackIdM_PFJet30"};
	triggerNames["passTrigger_FR_iso"] = {"HLT_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL", 
					      "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					      "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					      //"HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					      "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"};
	triggerNames["passTrigger_m"] = {"HLT_IsoMu24", "HLT_IsoMu27", "HLT_Mu50", "HLT_OldMu100", "HLT_TkMu100"};
	triggerNames["passTrigger_e"]   = {"HLT_Ele32_WPTight_Gsf", "HLT_Ele115_CaloIdVT_GsfTrkIdT", "HLT_Photon200"};
	triggerNames["passTrigger_mm"]  = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8", "HLT_Mu37_TkMu27"};
	triggerNames["passTrigger_em"]  = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", 
					   "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
					   "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ", 
					   "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
					   "HLT_Mu27_Ele37_CaloIdL_MW", "HLT_Mu37_Ele27_CaloIdL_MW"};
	triggerNames["passTrigger_ee"]  = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", "HLT_DoubleEle25_CaloIdL_MW"};
	triggerNames["passTrigger_mmm"] = {"HLT_TripleMu_10_5_5_DZ",
					    //"HLT_TripleMu_5_3_3_Mass3p8to60_DZ",
					    "HLT_TripleMu_5_3_3_Mass3p8_DZ",
					    //"TripleMu_12_10_5",
					    "HLT_TripleMu_12_10_5"};
	triggerNames["passTrigger_emm"] = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ"};
	triggerNames["passTrigger_eem"] = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_eee"] = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_ref"] = {"HLT_CaloMET350_HBHECleaned", "HLT_CaloJet500_NoJetID", "HLT_AK8PFJet500", "HLT_AK8PFJet400_TrimMass30",
                                   "HLT_DiJet110_35_Mjj650_PFMET110", "HLT_PFHT800_PFMET75_PFMHT75_IDTight", "HLT_PFHT700_PFMET85_PFMHT85_IDTight",
                                   "HLT_PFHT500_PFMET100_PFMHT100_IDTight", "HLT_PFHT1050", "HLT_PFJet500", "HLT_PFMET120_PFMHT120_IDTight", 
                                   "HLT_PFMET250_HBHECleaned", "HLT_PFMET200_HBHE_BeamHaloCleaned", "HLT_PFMETTypeOne140_PFMHT140_IDTight",
                                   "HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned", "HLT_TripleJet110_35_35_Mjj650_PFMET110"};

    } else if( treeReader.is2017() ){
	triggerNames["passTrigger_FR"] = {"HLT_Mu3_PFJet40", "HLT_Mu8", "HLT_Mu17", "HLT_Mu20", "HLT_Mu27", 
					    "HLT_Ele8_CaloIdM_TrackIdM_PFJet30", "HLT_Ele17_CaloIdM_TrackIdM_PFJet30", 
					    "HLT_Ele23_CaloIdM_TrackIdM_PFJet30"};
	triggerNames["passTrigger_FR_iso"] = {"HLT_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL",
					    "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					    "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30",
					    //"HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					    "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"};
	triggerNames["passTrigger_m"] = {"HLT_IsoMu24", "HLT_IsoMu24_eta2p1", "HLT_IsoMu27", "HLT_Mu50", "HLT_OldMu100", "HLT_TkMu100"};
	triggerNames["passTrigger_e"] = {"HLT_Ele32_WPTight_Gsf", "HLT_Ele35_WPTight_Gsf", "HLT_Ele115_CaloIdVT_GsfTrkIdT", "HLT_Photon200"};
	triggerNames["passTrigger_mm"] = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", "HLT_Mu37_TkMu27",
					    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"};
	triggerNames["passTrigger_em"] = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
					"HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
					"HLT_Mu27_Ele37_CaloIdL_MW", "HLT_Mu37_Ele27_CaloIdL_MW"};
	triggerNames["passTrigger_ee"] = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", "HLT_DoubleEle33_CaloIdL_MW"};
	triggerNames["passTrigger_mmm"] = {"HLT_TripleMu_10_5_5_DZ",
					    //"HLT_TripleMu_5_3_3_Mass3p8to60_DZ",
					    "HLT_TripleMu_5_3_3_Mass3p8_DZ",
					    //"TripleMu_12_10_5",
					    "HLT_TripleMu_12_10_5"};
	triggerNames["passTrigger_emm"] = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ"};
	triggerNames["passTrigger_eem"] = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_eee"] = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_ref"] = {"HLT_PFJet500", "HLT_PFMET140_PFMHT140_IDTight", "HLT_PFHT500_PFMET100_PFMHT100_IDTight",
					    "HLT_PFHT700_PFMET85_PFMHT85_IDTight", "HLT_PFHT800_PFMET75_PFMHT75_IDTight", "HLT_CaloJet500_NoJetID",
					    "HLT_AK8PFJet500"};
  
    } else {
	triggerNames["passTrigger_FR"] = {"HLT_Mu3_PFJet40", "HLT_Mu8", "HLT_Mu17", "HLT_Mu20", "HLT_Mu27", 
					    "HLT_Ele8_CaloIdM_TrackIdM_PFJet30", "HLT_Ele12_CaloIdM_TrackIdM_PFJet30", 
					    "HLT_Ele17_CaloIdM_TrackIdM_PFJet30", "HLT_Ele23_CaloIdM_TrackIdM_PFJet30"};
	triggerNames["passTrigger_FR_iso"] = {"HLT_Mu8_TrkIsoVVL", 
					    "HLT_Mu17_TrkIsoVVL", 
					    "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					    "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					    //"HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30", 
					    "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"};
	triggerNames["passTrigger_m"] = {"HLT_IsoMu24", "HLT_IsoTkMu24", "HLT_Mu50", "HLT_TkMu50", "HLT_Mu45_eta2p1"};
	triggerNames["passTrigger_e"] = {"HLT_Ele27_WPTight_Gsf", "HLT_Ele105_CaloIdVT_GsfTrkIdT", "HLT_Ele115_CaloIdVT_GsfTrkIdT", "HLT_Photon175"};
	triggerNames["passTrigger_mm"] = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",
					    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",
					    "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL", "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",
					    "HLT_Mu30_TkMu11"};
	triggerNames["passTrigger_em"] = {"HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
					    "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL", "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ",
					    "HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL", "HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL"};
	triggerNames["passTrigger_ee"] = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL", 
					    "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW"};
	triggerNames["passTrigger_mmm"] = {"HLT_TripleMu_12_10_5"};
	triggerNames["passTrigger_emm"] = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_eem"] = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_eee"] = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
	triggerNames["passTrigger_ref"] = {"HLT_MET200", "HLT_PFMET300", "HLT_PFMET170_HBHECleaned", "HLT_PFMET120_PFMHT120_IDTight",
					    "HLT_PFHT300_PFMET110", "HLT_PFHT350_DiPFJetAve90_PFAlphaT0p53", "HLT_PFHT400_DiPFJetAve90_PFAlphaT0p52",
					    "HLT_PFHT400_SixJet30_DoubleBTagCSV_p056", "HLT_PFHT900", "HLT_PFHT650_WideJetMJJ900DEtaJJ1p5", 
					    "HLT_CaloJet500_NoJetID"};
    }
    // check if all triggers exist
    for(auto const& el : triggerNames){
	for(std::string triggerName : el.second){
	    if( treeReader._triggerMap.count(triggerName)==0 ){
		std::string msg = "ERROR in TriggerInfo constructor:";
		msg += " trigger " + triggerName + " not found in TreeReader.";
		throw std::runtime_error(msg);
	    }
	}
    }
    // set the composite trigger variables
    _passTriggers_e = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_e"] );
    _passTriggers_m = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_m"] );
    _passTriggers_ee = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_ee"] );
    _passTriggers_em = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_em"] );
    _passTriggers_mm = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_mm"] );
    _passTriggers_eee = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_eee"] );
    _passTriggers_eem = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_eem"] );
    _passTriggers_emm = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_emm"] );
    _passTriggers_mmm = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_mmm"] );
    _passTriggers_FR = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_FR"] );
    _passTriggers_FR_iso = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_FR_iso"] );
    _passTriggers_ref = passAtLeastOneTrigger( treeReader._triggerMap, triggerNames["passTrigger_ref"] );
    // composite MET filter definitions
    // can use builtin "Flag_METFilters" in nanoAOD!
    std::vector<std::string> metNames = {"Flag_METFilters"};
    // check if all MET filters exist
    for(std::string metName : metNames){
        if( treeReader._METFilterMap.find(metName)==treeReader._METFilterMap.end()){
            std::string msg = "ERROR in TriggerInfo constructor:";
            msg += " MET filter " + metName + " not found in TreeReader.";
            throw std::runtime_error(msg);
        }
    }
    // set the composite MET filter variables
    _passMetFilters = passAllTriggers( treeReader._METFilterMap, metNames );
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
