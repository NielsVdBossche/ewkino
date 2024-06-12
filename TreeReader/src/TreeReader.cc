#include "../interface/TreeReader.h"

//include c++ library classes 
#include <fstream>
#include <iostream>
#include <typeinfo>

//include other parts of analysis framework
#include "../../Tools/interface/analysisTools.h"
#include "../../Tools/interface/stringTools.h"
#include "../../Tools/interface/systemTools.h"
#include "../../Tools/interface/analysisTools.h"
#include "../../Event/interface/Event.h"
#include "../../constants/luminosities.h"

// constructor //
TreeReader::TreeReader() : BaseReader() {
    _sourcesJEC_Ids = new std::map<std::string, size_t>();
    _groupedJEC_Ids = new std::map<std::string, size_t>();
}

TreeReader::TreeReader( const std::string& sampleListFile, const std::string& sampleDirectory ) :
    BaseReader( sampleListFile, sampleDirectory )
{
    _sourcesJEC_Ids = new std::map<std::string, size_t>();
    _groupedJEC_Ids = new std::map<std::string, size_t>();
}

// functions for initializing maps of branches rather than hard-coded names //
std::pair<std::map<std::string, bool>, std::map<std::string, TBranch*> > buildBranchMap(
    TTree* treePtr,
    const std::vector<std::string> nameIdentifiers,
    const std::string& antiIdentifier) {
    // build a map of branches from a given tree
    // all branches whose name contains nameIdentifier and not antiIdentifier will be added
    // the return type is a pair of two objects:
    // - map of branch name to boolean
    //   (e.g. usable for triggers)
    //   (values are set to false everywhere)
    // - map of branch name to branch pointer
    //   (e.g. usable for many branches with similar names)
    //   (branch pointers are set to nullptr everywhere)
    std::map<std::string, bool> decisionMap;
    std::map<std::string, TBranch*> branchMap;
    TObjArray* branch_list = treePtr->GetListOfBranches();
    for (const auto& branchPtr : *branch_list) {
        std::string branchName = branchPtr->GetName();
        bool select = true;
        for (std::string nameIdentifier : nameIdentifiers) {
            if (!stringTools::stringContains(branchName, nameIdentifier)) select = false;
        }
        if (!select) continue;
        if (antiIdentifier != "" && stringTools::stringContains(branchName, antiIdentifier)) continue;
        decisionMap[branchName] = false;
        branchMap[branchName] = nullptr;
    }
    return {decisionMap, branchMap};
}

void TreeReader::initializeTriggerMap( TTree* treePtr ){
    auto triggerMaps = buildBranchMap( treePtr, {"HLT"}, "prescale" );
    _triggerMap = triggerMaps.first;
    b__triggerMap = triggerMaps.second;
}


void TreeReader::initializeMetFilterMap( TTree* treePtr ){
    //WARNING: Currently one MET filter contains 'updated' rather than 'Flag' in the name. 
    //If this changes, make sure to modify the code here!
    auto filterMaps = buildBranchMap( treePtr, {"Flag"} );
    auto filterMaps_part2 = buildBranchMap( treePtr , {"updated"} );
    filterMaps.first.insert( filterMaps_part2.first.cbegin(), filterMaps_part2.first.cend() );
    filterMaps.second.insert( filterMaps_part2.second.cbegin(), filterMaps_part2.second.cend() );

    _MetFilterMap = filterMaps.first;
    b__MetFilterMap = filterMaps.second;
}

void TreeReader::initializeJecSourcesMaps(TTree* treePtr) {
    b__jetPt_JECSourcesUp = buildBranchMap(treePtr, {"_jetPt_", "_JECSourcesUp"}).second;
    unsigned ctr=0;
    for (auto mapEl : b__jetPt_JECSourcesUp) {
        std::string jecNameCleaned = stringTools::removeOccurencesOf(mapEl.first,"_jetPt_");
        jecNameCleaned = stringTools::removeOccurencesOf(jecNameCleaned,"_jetSmearedPt_");
        jecNameCleaned = stringTools::removeOccurencesOf(jecNameCleaned,"_JECSourcesUp");
        jecNameCleaned = stringTools::removeOccurencesOf(jecNameCleaned,"_JECSourcesDown");
        (*_sourcesJEC_Ids)[jecNameCleaned] = ctr;

        _jetPt_JECSourcesUp_Ids[mapEl.first] = ctr;

        std::string down = stringTools::replace(mapEl.first, "Up", "Down");
        if (stringTools::stringContains(mapEl.first, "PileUp")) {
            down = stringTools::replace(down, "PileDown", "PileUp");
        }
        b__jetPt_JECSourcesDown[down] = nullptr;
        _jetPt_JECSourcesDown_Ids[down] = ctr;

        std::string smeared  = stringTools::replace(mapEl.first, "_jetPt", "_jetSmearedPt");
        b__jetSmearedPt_JECSourcesUp[smeared] = nullptr;
        _jetSmearedPt_JECSourcesUp_Ids[smeared] = ctr;

        std::string smeareddown = stringTools::replace(smeared, "Up", "Down");
        if (stringTools::stringContains(smeared, "PileUp")) {
            smeareddown = stringTools::replace(smeareddown, "PileDown", "PileUp");
        }
        b__jetSmearedPt_JECSourcesDown[smeareddown] = nullptr;
        _jetSmearedPt_JECSourcesDown_Ids[smeareddown] = ctr;

        std::string corrmetX = stringTools::replace(mapEl.first, "_jetPt", "_corrMETx");
        b__corrMETx_JECSourcesUp[corrmetX] = nullptr;
        _corrMETx_JECSourcesUp_Ids[corrmetX] = ctr;

        std::string corrmetY = stringTools::replace(mapEl.first, "_jetPt", "_corrMETy");
        b__corrMETy_JECSourcesUp[corrmetY] = nullptr;
        _corrMETy_JECSourcesUp_Ids[corrmetY] = ctr;

        std::string corrmetXDown = stringTools::replace(corrmetX, "Up", "Down");
        if (stringTools::stringContains(corrmetX, "PileUp")) {
            corrmetXDown = stringTools::replace(corrmetXDown, "PileDown", "PileUp");
        }
        b__corrMETx_JECSourcesDown[corrmetXDown] = nullptr;
        _corrMETx_JECSourcesDown_Ids[corrmetXDown] = ctr;

        std::string corrmetYDown = stringTools::replace(corrmetY, "Up", "Down");
        if (stringTools::stringContains(corrmetY, "PileUp")) {
            corrmetYDown = stringTools::replace(corrmetYDown, "PileDown", "PileUp");
        }
        b__corrMETy_JECSourcesDown[corrmetYDown] = nullptr;
        _corrMETy_JECSourcesDown_Ids[corrmetYDown] = ctr;
        
        ctr++;
    }
    _jetPt_JECSourcesUp = std::vector<Double_t[nJets_max]>(_jetPt_JECSourcesUp_Ids.size());
    _jetPt_JECSourcesDown = std::vector<Double_t[nJets_max]>(_jetPt_JECSourcesDown_Ids.size());
    _jetSmearedPt_JECSourcesUp = std::vector<Double_t[nJets_max]>(_jetSmearedPt_JECSourcesUp_Ids.size());
    _jetSmearedPt_JECSourcesDown = std::vector<Double_t[nJets_max]>(_jetSmearedPt_JECSourcesDown_Ids.size());

    _corrMETx_JECSourcesUp = std::vector<double>(_corrMETx_JECSourcesUp_Ids.size());
    _corrMETx_JECSourcesDown = std::vector<double>(_corrMETx_JECSourcesDown_Ids.size());
    _corrMETy_JECSourcesUp = std::vector<double>(_corrMETy_JECSourcesUp_Ids.size());
    _corrMETy_JECSourcesDown = std::vector<double>(_corrMETy_JECSourcesDown_Ids.size());
}

void TreeReader::initializeJecSourcesGroupedMaps(TTree* treePtr) {
    // initialize jetPt branches
    b__jetPt_JECGroupedUp = buildBranchMap(treePtr, {"_jetPt_", "_JECGroupedUp"}).second;
    unsigned ctr = 0;
    for (auto mapEl : b__jetPt_JECGroupedUp) {
        std::string jecNameCleaned = stringTools::removeOccurencesOf(mapEl.first,"_jetPt_");
        jecNameCleaned = stringTools::removeOccurencesOf(jecNameCleaned,"_jetSmearedPt_");
        jecNameCleaned = stringTools::removeOccurencesOf(jecNameCleaned,"_JECGroupedUp");
        jecNameCleaned = stringTools::removeOccurencesOf(jecNameCleaned,"_JECGroupedDown");

        (*_groupedJEC_Ids)[jecNameCleaned] = ctr;
        _jetPt_JECGroupedUp_Ids[mapEl.first] = ctr;
        

        std::string down = stringTools::replace(mapEl.first, "Up", "Down");
        b__jetPt_JECGroupedDown[down] = nullptr;
        _jetPt_JECGroupedDown_Ids[down] = ctr;        

        std::string smeared  = stringTools::replace(mapEl.first, "_jetPt", "_jetSmearedPt");
        b__jetSmearedPt_JECGroupedUp[smeared] = nullptr;
        _jetSmearedPt_JECGroupedUp_Ids[smeared] = ctr;        

        std::string smeareddown = stringTools::replace(smeared, "Up", "Down");
        b__jetSmearedPt_JECGroupedDown[smeareddown] = nullptr;
        _jetSmearedPt_JECGroupedDown_Ids[smeareddown] = ctr;        

        std::string corrmetX = stringTools::replace(mapEl.first, "_jetPt", "_corrMETx");
        b__corrMETx_JECGroupedUp[corrmetX] = nullptr;
        _corrMETx_JECGroupedUp_Ids[corrmetX] = ctr;        

        std::string corrmetY = stringTools::replace(mapEl.first, "_jetPt", "_corrMETy");
        b__corrMETy_JECGroupedUp[corrmetY] = nullptr;
        _corrMETy_JECGroupedUp_Ids[corrmetY] = ctr;        

        std::string corrmetXDown = stringTools::replace(corrmetX, "Up", "Down");
        b__corrMETx_JECGroupedDown[corrmetXDown] = nullptr;
        _corrMETx_JECGroupedDown_Ids[corrmetXDown] = ctr;        

        std::string corrmetYDown = stringTools::replace(corrmetY, "Up", "Down");
        b__corrMETy_JECGroupedDown[corrmetYDown] = nullptr;
        _corrMETy_JECGroupedDown_Ids[corrmetYDown] = ctr;        
        
        ctr++;
    }
    _jetPt_JECGroupedUp = std::vector<Double_t[nJets_max]>(_jetPt_JECGroupedUp_Ids.size());
    _jetPt_JECGroupedDown = std::vector<Double_t[nJets_max]>(_jetPt_JECGroupedDown_Ids.size());
    _jetSmearedPt_JECGroupedUp = std::vector<Double_t[nJets_max]>(_jetSmearedPt_JECGroupedUp_Ids.size());
    _jetSmearedPt_JECGroupedDown = std::vector<Double_t[nJets_max]>(_jetSmearedPt_JECGroupedDown_Ids.size());

    _corrMETx_JECGroupedUp = std::vector<double>(_corrMETx_JECGroupedUp_Ids.size());
    _corrMETx_JECGroupedDown = std::vector<double>(_corrMETx_JECGroupedDown_Ids.size());
    _corrMETy_JECGroupedUp = std::vector<double>(_corrMETy_JECGroupedUp_Ids.size());
    _corrMETy_JECGroupedDown = std::vector<double>(_corrMETy_JECGroupedDown_Ids.size());
}

// functions to find if a tree has branches with certain types of info 
bool TreeReader::containsGeneratorInfo() const{
    return treeHasBranchWithName( _currentTreePtr, "_gen_" );
}

bool TreeReader::containsFullGeneratorInfo() const{
    return treeHasBranchWithName( _currentTreePtr, "_gen_pt" );
}

bool TreeReader::containsParticleLevelInfo() const{
    return treeHasBranchWithName( _currentTreePtr, "_pl_" );
}

bool TreeReader::containsLheInfo() const{
    return treeHasBranchWithName( _currentTreePtr, "_lhePt" );
}

bool TreeReader::containsEFTInfo() const{
    return treeHasBranchWithName( _currentTreePtr, "_nEFTWeights" );
}

bool TreeReader::containsSusyMassInfo() const{
    return treeHasBranchWithName( _currentTreePtr, "_mChi" );
}


bool TreeReader::containsTriggerInfo( const std::string& triggerPath ) const{
    return treeHasBranchWithName( _currentTreePtr, triggerPath );
}


double TreeReader::getSumSimulatedEventWeights() {
    //read sum of simulated event weights
    TH1D* hCounter = new TH1D( "hCounter", "Events counter", 1, 0, 1 );
    _currentFilePtr->cd( "blackJackAndHookers" );
    hCounter->Read( "hCounter" ); 
    double sumSimulatedEventWeights = hCounter->GetBinContent(1);
    delete hCounter;
    return sumSimulatedEventWeights;
}

TTree* TreeReader::getTreePtr() {
    return (TTree*) _currentFilePtr->Get( "blackJackAndHookers/blackJackAndHookersTree" );
}

Event TreeReader::buildEvent( const Sample& samp, long unsigned entry, 
	const bool readIndividualTriggers, const bool readIndividualMetFilters,
	const bool readAllJECVariations, const bool readGroupedJECVariations ){
    GetEntry( samp, entry );
    return Event( *this, readIndividualTriggers, readIndividualMetFilters,
			readAllJECVariations, readGroupedJECVariations );
}

Event* TreeReader::buildEventPtr( const Sample& samp, long unsigned entry, 
	const bool readIndividualTriggers, const bool readIndividualMetFilters,
	const bool readAllJECVariations, const bool readGroupedJECVariations ){
    GetEntry( samp, entry );
    return new Event( *this, readIndividualTriggers, readIndividualMetFilters,
			readAllJECVariations, readGroupedJECVariations );
}

Event TreeReader::buildEvent( long unsigned entry, 
	const bool readIndividualTriggers, const bool readIndividualMetFilters,
	const bool readAllJECVariations, const bool readGroupedJECVariations ){
    GetEntry( entry );
    return Event( *this, readIndividualTriggers, readIndividualMetFilters,
			readAllJECVariations, readGroupedJECVariations );
}

Event* TreeReader::buildEventPtr( long unsigned entry, 
	const bool readIndividualTriggers, const bool readIndividualMetFilters,
	const bool readAllJECVariations, const bool readGroupedJECVariations ){
    GetEntry( entry );
    return new Event( *this, readIndividualTriggers, readIndividualMetFilters,
			readAllJECVariations, readGroupedJECVariations );
}

template <typename T>
void setMapBranchAddresses(TTree* treePtr,
                           std::map<std::string, T>& variableMap,
                           std::map<std::string, TBranch*> branchMap) {
    for (const auto& variable : variableMap) {
        treePtr->SetBranchAddress(variable.first.c_str(), &variableMap[variable.first], &branchMap[variable.first]);
    }
}

template< typename T > void setMapBranchAddressesAndVectors( TTree* treePtr, std::map< std::string, size_t >& variableIds, std::vector< T >& variableValues, std::map< std::string, TBranch* > branchMap ){
    for (const auto& variable : variableIds) {
        treePtr->SetBranchAddress( variable.first.c_str(), &variableValues[variable.second], &branchMap[ variable.first ] );
    }
}

template< typename T> void setMapOutputBranches( TTree* treePtr, 
			    std::map< std::string, T >& variableMap,
			    std::string branchDataType ){
    // note: branchDataType should be e.g. "/O" for boolean, "[nJets]/D" for an array of doubles.
    // mind the slash!
    for( const auto& variable : variableMap ){
        treePtr->Branch( variable.first.c_str(), 
	    &variableMap[ variable.first ], 
	    ( variable.first + branchDataType ).c_str() );
    }    
}

template< typename T> void setMapOutputBranchesWithVectors( TTree* treePtr, 
			    std::map< std::string, size_t >& variableIds,
                std::vector< T >& variableValues,
			    std::string branchDataType,
                std::string prefix="",
                std::string postfix="" ){
    // note: branchDataType should be e.g. "/O" for boolean, "[nJets]/D" for an array of doubles.
    // mind the slash!
    for (const auto& variable : variableIds) {
        std::string tmp_branchname = prefix + variable.first + postfix;
        treePtr->Branch(tmp_branchname.c_str(), &variableValues[variable.second], (tmp_branchname + branchDataType).c_str());
    }
}



void TreeReader::initTree( const bool resetTriggersAndFilters ){
    // Set branch addresses and branch pointers
    checkCurrentTree();
    _hasPLInfo = false;
    _hasGenLevelInfo = false;
    _hasEFTInfo = false;

    _currentTreePtr->SetMakeClass(1);

    _currentTreePtr->SetBranchAddress("_runNb", &_runNb, &b__runNb);
    _currentTreePtr->SetBranchAddress("_lumiBlock", &_lumiBlock, &b__lumiBlock);
    _currentTreePtr->SetBranchAddress("_eventNb", &_eventNb, &b__eventNb);
    _currentTreePtr->SetBranchAddress("_nVertex", &_nVertex, &b__nVertex);
    _currentTreePtr->SetBranchAddress("_passTrigger_e", &_passTrigger_e, &b__passTrigger_e);
    _currentTreePtr->SetBranchAddress("_passTrigger_ee", &_passTrigger_ee, &b__passTrigger_ee);
    _currentTreePtr->SetBranchAddress("_passTrigger_eee", &_passTrigger_eee, &b__passTrigger_eee);
    _currentTreePtr->SetBranchAddress("_passTrigger_em", &_passTrigger_em, &b__passTrigger_em);
    _currentTreePtr->SetBranchAddress("_passTrigger_m", &_passTrigger_m, &b__passTrigger_m);
    _currentTreePtr->SetBranchAddress("_passTrigger_eem", &_passTrigger_eem, &b__passTrigger_eem);
    _currentTreePtr->SetBranchAddress("_passTrigger_mm", &_passTrigger_mm, &b__passTrigger_mm);
    _currentTreePtr->SetBranchAddress("_passTrigger_emm", &_passTrigger_emm, &b__passTrigger_emm);
    _currentTreePtr->SetBranchAddress("_passTrigger_mmm", &_passTrigger_mmm, &b__passTrigger_mmm);
    _currentTreePtr->SetBranchAddress("_passTrigger_et", &_passTrigger_et, &b__passTrigger_et);
    _currentTreePtr->SetBranchAddress("_passTrigger_mt", &_passTrigger_mt, &b__passTrigger_mt);
    _currentTreePtr->SetBranchAddress("_passTrigger_FR", &_passTrigger_FR, &b__passTrigger_FR);
    _currentTreePtr->SetBranchAddress("_passTrigger_FR_iso", &_passTrigger_FR_iso, &b__passTrigger_FR_iso);
    _currentTreePtr->SetBranchAddress("_passMETFilters", &_passMETFilters, &b__passMETFilters);
    _currentTreePtr->SetBranchAddress("_nL", &_nL, &b__nL);
    _currentTreePtr->SetBranchAddress("_nMu", &_nMu, &b__nMu);
    _currentTreePtr->SetBranchAddress("_nEle", &_nEle, &b__nEle);
    _currentTreePtr->SetBranchAddress("_nLight", &_nLight, &b__nLight);
    _currentTreePtr->SetBranchAddress("_nTau", &_nTau, &b__nTau);
    _currentTreePtr->SetBranchAddress("_lPt", _lPt, &b__lPt);
    _currentTreePtr->SetBranchAddress("_lPtCorr", _lPtCorr, &b__lPtCorr);
    _currentTreePtr->SetBranchAddress("_lPtScaleUp", _lPtScaleUp, &b__lPtScaleUp);
    _currentTreePtr->SetBranchAddress("_lPtScaleDown", _lPtScaleDown, &b__lPtScaleDown);
    _currentTreePtr->SetBranchAddress("_lPtResUp", _lPtResUp, &b__lPtResUp);
    _currentTreePtr->SetBranchAddress("_lPtResDown", _lPtResDown, &b__lPtResDown);
    _currentTreePtr->SetBranchAddress("_lEta", _lEta, &b__lEta);
    _currentTreePtr->SetBranchAddress("_lEtaSC", _lEtaSC, &b__lEtaSC);
    _currentTreePtr->SetBranchAddress("_lPhi", _lPhi, &b__lPhi);
    _currentTreePtr->SetBranchAddress("_lE", _lE, &b__lE);
    _currentTreePtr->SetBranchAddress("_lECorr", _lECorr, &b__lECorr);
    _currentTreePtr->SetBranchAddress("_lEScaleUp", _lEScaleUp, &b__lEScaleUp);
    _currentTreePtr->SetBranchAddress("_lEScaleDown", _lEScaleDown, &b__lEScaleDown);
    _currentTreePtr->SetBranchAddress("_lEResUp", _lEResUp, &b__lEResUp);
    _currentTreePtr->SetBranchAddress("_lEResDown", _lEResDown, &b__lEResDown);
    _currentTreePtr->SetBranchAddress("_lFlavor", _lFlavor, &b__lFlavor);
    _currentTreePtr->SetBranchAddress("_lCharge", _lCharge, &b__lCharge);
    _currentTreePtr->SetBranchAddress("_dxy", _dxy, &b__dxy);
    _currentTreePtr->SetBranchAddress("_dz", _dz, &b__dz);
    _currentTreePtr->SetBranchAddress("_3dIP", _3dIP, &b__3dIP);
    _currentTreePtr->SetBranchAddress("_3dIPSig", _3dIPSig, &b__3dIPSig);
    _currentTreePtr->SetBranchAddress("_lElectronSummer16MvaGP", _lElectronSummer16MvaGP, &b__lElectronSummer16MvaGP);
    _currentTreePtr->SetBranchAddress("_lElectronSummer16MvaHZZ", _lElectronSummer16MvaHZZ, &b__lElectronSummer16MvaHZZ);
    _currentTreePtr->SetBranchAddress("_lElectronMvaFall17Iso", _lElectronMvaFall17Iso, &b__lElectronMvaFall17Iso);
    _currentTreePtr->SetBranchAddress("_lElectronMvaFall17NoIso", _lElectronMvaFall17NoIso, &b__lElectronMvaFall17NoIso);
    _currentTreePtr->SetBranchAddress("_lElectronPassMVAFall17NoIsoWPLoose", _lElectronPassMVAFall17NoIsoWPLoose, &b__lElectronPassMVAFall17NoIsoWPLoose);
    _currentTreePtr->SetBranchAddress("_lElectronPassMVAFall17NoIsoWP90", _lElectronPassMVAFall17NoIsoWP90, &b__lElectronPassMVAFall17NoIsoWP90);
    _currentTreePtr->SetBranchAddress("_lElectronPassMVAFall17NoIsoWP80", _lElectronPassMVAFall17NoIsoWP80, &b__lElectronPassMVAFall17NoIsoWP80);
    _currentTreePtr->SetBranchAddress("_lElectronPassEmu", _lElectronPassEmu, &b__lElectronPassEmu);
    _currentTreePtr->SetBranchAddress("_lElectronPassConvVeto", _lElectronPassConvVeto, &b__lElectronPassConvVeto);
    _currentTreePtr->SetBranchAddress("_lElectronChargeConst", _lElectronChargeConst, &b__lElectronChargeConst);
    _currentTreePtr->SetBranchAddress("_lElectronMissingHits", _lElectronMissingHits, &b__lElectronMissingHits);
    _currentTreePtr->SetBranchAddress("_lElectronEInvMinusPInv", _lElectronEInvMinusPInv, &b__lElectronEInvMinusPInv);
    _currentTreePtr->SetBranchAddress("_lElectronHOverE", _lElectronHOverE, &b__lElectronHOverE);
    _currentTreePtr->SetBranchAddress("_lElectronSigmaIetaIeta", _lElectronSigmaIetaIeta, &b__lElectronSigmaIetaIeta);
    _currentTreePtr->SetBranchAddress("_leptonMvaTTH", _leptonMvaTTH, &b__leptonMvaTTH);
    _currentTreePtr->SetBranchAddress("_leptonMvatZq", _leptonMvatZq, &b__leptonMvatZq);
    _currentTreePtr->SetBranchAddress("_leptonMvaTOP", _leptonMvaTOP, &b__leptonMvaTOP);
    _currentTreePtr->SetBranchAddress("_leptonMvaTOPUL", _leptonMvaTOPUL, &b__leptonMvaTOPUL);
    _currentTreePtr->SetBranchAddress("_leptonMvaTOPv2UL", _leptonMvaTOPULv2, &b__leptonMvaTOPv2UL);
    _currentTreePtr->SetBranchAddress("_lPOGVeto", _lPOGVeto, &b__lPOGVeto);
    _currentTreePtr->SetBranchAddress("_lPOGLoose", _lPOGLoose, &b__lPOGLoose);
    _currentTreePtr->SetBranchAddress("_lPOGMedium", _lPOGMedium, &b__lPOGMedium);
    _currentTreePtr->SetBranchAddress("_lPOGTight", _lPOGTight, &b__lPOGTight);

    _currentTreePtr->SetBranchAddress("_tauDecayMode", _tauDecayMode, &b__tauDecayMode);
    _currentTreePtr->SetBranchAddress("_decayModeFinding", _decayModeFinding, &b__decayModeFinding);
    _currentTreePtr->SetBranchAddress("_decayModeFindingNew", _decayModeFindingNew, &b__decayModeFindingNew);
    _currentTreePtr->SetBranchAddress("_tauMuonVetoLoose", _tauMuonVetoLoose, &b__tauMuonVetoLoose);

    if (treeHasBranchWithName(_currentTreePtr, "_tauMuonVetoTight")) {
        _currentTreePtr->SetBranchAddress("_tauMuonVetoTight", _tauMuonVetoTight, &b__tauMuonVetoTight);
        _currentTreePtr->SetBranchAddress("_tauEleVetoVLoose", _tauEleVetoVLoose, &b__tauEleVetoVLoose);
        _currentTreePtr->SetBranchAddress("_tauEleVetoLoose", _tauEleVetoLoose, &b__tauEleVetoLoose);
        _currentTreePtr->SetBranchAddress("_tauEleVetoMedium", _tauEleVetoMedium, &b__tauEleVetoMedium);
        _currentTreePtr->SetBranchAddress("_tauEleVetoTight", _tauEleVetoTight, &b__tauEleVetoTight);
        _currentTreePtr->SetBranchAddress("_tauEleVetoVTight", _tauEleVetoVTight, &b__tauEleVetoVTight);
        _currentTreePtr->SetBranchAddress("_tauPOGVLoose2015", _tauPOGVLoose2015, &b__tauPOGVLoose2015);
        _currentTreePtr->SetBranchAddress("_tauPOGLoose2015", _tauPOGLoose2015, &b__tauPOGLoose2015);
        _currentTreePtr->SetBranchAddress("_tauPOGMedium2015", _tauPOGMedium2015, &b__tauPOGMedium2015);
        _currentTreePtr->SetBranchAddress("_tauPOGTight2015", _tauPOGTight2015, &b__tauPOGTight2015);
        _currentTreePtr->SetBranchAddress("_tauPOGVTight2015", _tauPOGVTight2015, &b__tauPOGVTight2015);
        _currentTreePtr->SetBranchAddress("_tauVLooseMvaNew2015", _tauVLooseMvaNew2015, &b__tauVLooseMvaNew2015);
        _currentTreePtr->SetBranchAddress("_tauLooseMvaNew2015", _tauLooseMvaNew2015, &b__tauLooseMvaNew2015);
        _currentTreePtr->SetBranchAddress("_tauMediumMvaNew2015", _tauMediumMvaNew2015, &b__tauMediumMvaNew2015);
        _currentTreePtr->SetBranchAddress("_tauTightMvaNew2015", _tauTightMvaNew2015, &b__tauTightMvaNew2015);
        _currentTreePtr->SetBranchAddress("_tauVTightMvaNew2015", _tauVTightMvaNew2015, &b__tauVTightMvaNew2015);
        _currentTreePtr->SetBranchAddress("_tauPOGVVLoose2017v2", _tauPOGVVLoose2017v2, &b__tauPOGVVLoose2017v2);
        _currentTreePtr->SetBranchAddress("_tauPOGVTight2017v2", _tauPOGVTight2017v2, &b__tauPOGVTight2017v2);
        _currentTreePtr->SetBranchAddress("_tauPOGVVTight2017v2", _tauPOGVVTight2017v2, &b__tauPOGVVTight2017v2);
        _currentTreePtr->SetBranchAddress("_tauVLooseMvaNew2017v2", _tauVLooseMvaNew2017v2, &b__tauVLooseMvaNew2017v2);
        _currentTreePtr->SetBranchAddress("_tauLooseMvaNew2017v2", _tauLooseMvaNew2017v2, &b__tauLooseMvaNew2017v2);
        _currentTreePtr->SetBranchAddress("_tauMediumMvaNew2017v2", _tauMediumMvaNew2017v2, &b__tauMediumMvaNew2017v2);
        _currentTreePtr->SetBranchAddress("_tauTightMvaNew2017v2", _tauTightMvaNew2017v2, &b__tauTightMvaNew2017v2);
        _currentTreePtr->SetBranchAddress("_tauVTightMvaNew2017v2", _tauVTightMvaNew2017v2, &b__tauVTightMvaNew2017v2);
    }

    _currentTreePtr->SetBranchAddress("_relIso", _relIso, &b__relIso);
    _currentTreePtr->SetBranchAddress("_relIso0p4", _relIso0p4, &b__relIso0p4);
    _currentTreePtr->SetBranchAddress("_relIso0p4MuDeltaBeta", _relIso0p4MuDeltaBeta, &b__relIso0p4MuDeltaBeta);
    _currentTreePtr->SetBranchAddress("_miniIso", _miniIso, &b__miniIso);
    _currentTreePtr->SetBranchAddress("_miniIsoCharged", _miniIsoCharged, &b__miniIsoCharged);
    _currentTreePtr->SetBranchAddress("_ptRel", _ptRel, &b__ptRel);
    _currentTreePtr->SetBranchAddress("_ptRatio", _ptRatio, &b__ptRatio);
    _currentTreePtr->SetBranchAddress("_closestJetCsvV2", _closestJetCsvV2, &b__closestJetCsvV2);
    _currentTreePtr->SetBranchAddress("_closestJetDeepCsv_b", _closestJetDeepCsv_b, &b__closestJetDeepCsv_b);
    _currentTreePtr->SetBranchAddress("_closestJetDeepCsv_bb", _closestJetDeepCsv_bb, &b__closestJetDeepCsv_bb);
    _currentTreePtr->SetBranchAddress("_closestJetDeepFlavor_b", _closestJetDeepFlavor_b, &b__closestJetDeepFlavor_b);
    _currentTreePtr->SetBranchAddress("_closestJetDeepFlavor_bb", _closestJetDeepFlavor_bb, &b__closestJetDeepFlavor_bb);
    _currentTreePtr->SetBranchAddress("_closestJetDeepFlavor_lepb", _closestJetDeepFlavor_lepb, &b__closestJetDeepFlavor_lepb);
    _currentTreePtr->SetBranchAddress("_selectedTrackMult", _selectedTrackMult, &b__selectedTrackMult);
    _currentTreePtr->SetBranchAddress("_lMuonSegComp", _lMuonSegComp, &b__lMuonSegComp);
    _currentTreePtr->SetBranchAddress("_lMuonTrackPt", _lMuonTrackPt, &b__lMuonTrackPt);
    _currentTreePtr->SetBranchAddress("_lMuonTrackPtErr", _lMuonTrackPtErr, &b__lMuonTrackPtErr);
    _currentTreePtr->SetBranchAddress("_nJets", &_nJets, &b__nJets);
    _currentTreePtr->SetBranchAddress("_jetPt", _jetPt, &b__jetPt);
    _currentTreePtr->SetBranchAddress("_jetSmearedPt", _jetSmearedPt, &b__jetSmearedPt);
    _currentTreePtr->SetBranchAddress("_jetSmearedPt_JECDown", _jetSmearedPt_JECDown, &b__jetSmearedPt_JECDown);
    _currentTreePtr->SetBranchAddress("_jetSmearedPt_JECUp", _jetSmearedPt_JECUp, &b__jetSmearedPt_JECUp);
    _currentTreePtr->SetBranchAddress("_jetSmearedPt_JERDown", _jetSmearedPt_JERDown, &b__jetSmearedPt_JERDown);
    _currentTreePtr->SetBranchAddress("_jetSmearedPt_JERUp", _jetSmearedPt_JERUp, &b__jetSmearedPt_JERUp);
    _currentTreePtr->SetBranchAddress("_jetPt_JECUp", _jetPt_JECUp, &b__jetPt_JECUp);
    _currentTreePtr->SetBranchAddress("_jetPt_JECDown", _jetPt_JECDown, &b__jetPt_JECDown);
    _currentTreePtr->SetBranchAddress("_jetEta", _jetEta, &b__jetEta);
    _currentTreePtr->SetBranchAddress("_jetPhi", _jetPhi, &b__jetPhi);
    _currentTreePtr->SetBranchAddress("_jetE", _jetE, &b__jetE);
    _currentTreePtr->SetBranchAddress("_jetPt_Uncorrected",_jetPt_Uncorrected, &b__jetPt_Uncorrected);
    _currentTreePtr->SetBranchAddress("_jetPt_L1", _jetPt_L1, &b__jetPt_L1);
    _currentTreePtr->SetBranchAddress("_jetPt_L2", _jetPt_L2, &b__jetPt_L2);
    _currentTreePtr->SetBranchAddress("_jetPt_L3", _jetPt_L3, &b__jetPt_L3);
    _currentTreePtr->SetBranchAddress("_jetCsvV2", _jetCsvV2, &b__jetCsvV2);
    _currentTreePtr->SetBranchAddress("_jetDeepCsv_udsg", _jetDeepCsv_udsg, &b__jetDeepCsv_udsg);
    _currentTreePtr->SetBranchAddress("_jetDeepCsv_b", _jetDeepCsv_b, &b__jetDeepCsv_b);
    _currentTreePtr->SetBranchAddress("_jetDeepCsv_c", _jetDeepCsv_c, &b__jetDeepCsv_c);
    _currentTreePtr->SetBranchAddress("_jetDeepCsv_bb", _jetDeepCsv_bb, &b__jetDeepCsv_bb);
    _currentTreePtr->SetBranchAddress("_jetDeepFlavor_b", _jetDeepFlavor_b, &b__jetDeepFlavor_b);
    _currentTreePtr->SetBranchAddress("_jetDeepFlavor_bb", _jetDeepFlavor_bb, &b__jetDeepFlavor_bb);
    _currentTreePtr->SetBranchAddress("_jetDeepFlavor_lepb", _jetDeepFlavor_lepb, &b__jetDeepFlavor_lepb);
    _currentTreePtr->SetBranchAddress("_jetHadronFlavor", _jetHadronFlavor, &b__jetHadronFlavor);
    _currentTreePtr->SetBranchAddress("_jetIsTight", _jetIsTight, &b__jetIsTight);
    _currentTreePtr->SetBranchAddress("_jetIsTightLepVeto", _jetIsTightLepVeto, &b__jetIsTightLepVeto);
    _currentTreePtr->SetBranchAddress("_jetNeutralHadronFraction", _jetNeutralHadronFraction, &b__jetNeutralHadronFraction);
    _currentTreePtr->SetBranchAddress("_jetChargedHadronFraction", _jetChargedHadronFraction, &b__jetChargedHadronFraction);
    _currentTreePtr->SetBranchAddress("_jetNeutralEmFraction", _jetNeutralEmFraction, &b__jetNeutralEmFraction);
    _currentTreePtr->SetBranchAddress("_jetChargedEmFraction", _jetChargedEmFraction, &b__jetChargedEmFraction);
    _currentTreePtr->SetBranchAddress("_jetHFHadronFraction", _jetHFHadronFraction, &b__jetHFHadronFraction);
    _currentTreePtr->SetBranchAddress("_jetHFEmFraction", _jetHFEmFraction, &b__jetHFEmFraction);

    _currentTreePtr->SetBranchAddress("_met", &_met, &b__met);
    _currentTreePtr->SetBranchAddress("_met_JECDown", &_met_JECDown, &b__met_JECDown);
    _currentTreePtr->SetBranchAddress("_met_JECUp", &_met_JECUp, &b__met_JECUp);
    _currentTreePtr->SetBranchAddress("_met_UnclDown", &_met_UnclDown, &b__met_UnclDown);
    _currentTreePtr->SetBranchAddress("_met_UnclUp", &_met_UnclUp, &b__met_UnclUp);
    _currentTreePtr->SetBranchAddress("_metPhi", &_metPhi, &b__metPhi);
    _currentTreePtr->SetBranchAddress("_metPhi_JECDown", &_metPhi_JECDown, &b__metPhi_JECDown);
    _currentTreePtr->SetBranchAddress("_metPhi_JECUp", &_metPhi_JECUp, &b__metPhi_JECUp);
    _currentTreePtr->SetBranchAddress("_metPhi_UnclDown", &_metPhi_UnclDown, &b__metPhi_UnclDown);
    _currentTreePtr->SetBranchAddress("_metPhi_UnclUp", &_metPhi_UnclUp, &b__metPhi_UnclUp);
    _currentTreePtr->SetBranchAddress("_metSignificance", &_metSignificance, &b__metSignificance);
    
    if( containsGeneratorInfo() ){
        _currentTreePtr->SetBranchAddress("_weight", &_weight, &b__weight);
        _currentTreePtr->SetBranchAddress("_nLheWeights", &_nLheWeights, &b__nLheWeights);
        _currentTreePtr->SetBranchAddress("_lheWeight", _lheWeight, &b__lheWeight);
        _currentTreePtr->SetBranchAddress("_nPsWeights", &_nPsWeights, &b__nPsWeights);
        _currentTreePtr->SetBranchAddress("_psWeight", _psWeight, &b__psWeight);
        _currentTreePtr->SetBranchAddress("_nTrueInt", &_nTrueInt, &b__nTrueInt);
        _currentTreePtr->SetBranchAddress("_lheHTIncoming", &_lheHTIncoming, &b__lheHTIncoming);
        _currentTreePtr->SetBranchAddress("_gen_met", &_gen_met, &b__gen_met);
        _currentTreePtr->SetBranchAddress("_gen_metPhi", &_gen_metPhi, &b__gen_metPhi);
        _currentTreePtr->SetBranchAddress("_gen_nL", &_gen_nL, &b__gen_nL);
        _currentTreePtr->SetBranchAddress("_gen_lPt", _gen_lPt, &b__gen_lPt);
        _currentTreePtr->SetBranchAddress("_gen_lEta", _gen_lEta, &b__gen_lEta);
        _currentTreePtr->SetBranchAddress("_gen_lPhi", _gen_lPhi, &b__gen_lPhi);
        _currentTreePtr->SetBranchAddress("_gen_lE", _gen_lE, &b__gen_lE);
        _currentTreePtr->SetBranchAddress("_gen_lFlavor", _gen_lFlavor, &b__gen_lFlavor);
        _currentTreePtr->SetBranchAddress("_gen_lCharge", _gen_lCharge, &b__gen_lCharge);
        _currentTreePtr->SetBranchAddress("_gen_lMomPdg", _gen_lMomPdg, &b__gen_lMomPdg);
        _currentTreePtr->SetBranchAddress("_gen_lIsPrompt", _gen_lIsPrompt, &b__gen_lIsPrompt);
        _currentTreePtr->SetBranchAddress("_lIsPrompt", _lIsPrompt, &b__lIsPrompt);
        _currentTreePtr->SetBranchAddress("_lMatchPdgId", _lMatchPdgId, &b__lMatchPdgId);
        _currentTreePtr->SetBranchAddress("_lMatchCharge", _lMatchCharge, &b__lMatchCharge);
        _currentTreePtr->SetBranchAddress("_lMomPdgId",  _lMomPdgId, &b__lMomPdgId);
        _currentTreePtr->SetBranchAddress("_lProvenance", _lProvenance, &b__lProvenance);
        _currentTreePtr->SetBranchAddress("_lProvenanceCompressed", _lProvenanceCompressed, &b__lProvenanceCompressed);
        _currentTreePtr->SetBranchAddress("_lProvenanceConversion", _lProvenanceConversion, &b__lProvenanceConversion);
        _currentTreePtr->SetBranchAddress("_ttgEventType", &_ttgEventType, &b__ttgEventType);
        _currentTreePtr->SetBranchAddress("_zgEventType", &_zgEventType, &b__zgEventType);

        if ( containsFullGeneratorInfo() ) {
            _hasGenLevelInfo = true;
            
            _currentTreePtr->SetBranchAddress("_gen_n",                                        &_gen_n,                                        &b__gen_n);
            _currentTreePtr->SetBranchAddress("_gen_pt",                                       _gen_pt,                                       &b__gen_pt);
            _currentTreePtr->SetBranchAddress("_gen_eta",                                      _gen_eta,                                      &b__gen_eta);
            _currentTreePtr->SetBranchAddress("_gen_phi",                                      _gen_phi,                                      &b__gen_phi);
            _currentTreePtr->SetBranchAddress("_gen_E",                                        _gen_E,                                        &b__gen_E);
            _currentTreePtr->SetBranchAddress("_gen_pdgId",                                    _gen_pdgId,                                    &b__gen_pdgId);
            _currentTreePtr->SetBranchAddress("_gen_charge",                                   _gen_charge,                                   &b__gen_charge);
            _currentTreePtr->SetBranchAddress("_gen_status",                                   _gen_status,                                   &b__gen_status);
            _currentTreePtr->SetBranchAddress("_gen_isPromptFinalState",                       _gen_isPromptFinalState,                       &b__gen_isPromptFinalState);
            _currentTreePtr->SetBranchAddress("_gen_isDirectPromptTauDecayProductFinalState",  _gen_isDirectPromptTauDecayProductFinalState,  &b__gen_isDirectPromptTauDecayProductFinalState);
            _currentTreePtr->SetBranchAddress("_gen_isLastCopy",                               _gen_isLastCopy,                               &b__gen_isLastCopy);
            _currentTreePtr->SetBranchAddress("_gen_index",                                    _gen_index,                                    &b__gen_index);
            _currentTreePtr->SetBranchAddress("_gen_motherIndex",                              _gen_motherIndex,                              &b__gen_motherIndex);
            _currentTreePtr->SetBranchAddress("_gen_daughter_n",                               _gen_daughter_n,                               &b__gen_daughter_n);
            _currentTreePtr->SetBranchAddress("_gen_daughterIndex",                            _gen_daughterIndex,                            &b__gen_daughterIndex);
        }

        if ( containsEFTInfo()) {
            _hasEFTInfo = true;
            _currentTreePtr->SetBranchAddress("_nDynScaleWeights",              &_nDynScaleWeights,  &b__nDynScaleWeights);
            _currentTreePtr->SetBranchAddress("_dynScaleWeight",                _dynScaleWeight,     &b__dynScaleWeight);
            _currentTreePtr->SetBranchAddress("_nEFTWeights",                   &_nEFTWeights,       &b__nEFTWeights);
            _currentTreePtr->SetBranchAddress("_eftWeight",                     _eftWeight,          &b__eftWeight);
        }
    }

    if (containsParticleLevelInfo()) {
        _hasPLInfo = true;

        _currentTreePtr->SetBranchAddress("_pl_met",                   &_pl_met,                   &b__pl_met);
        _currentTreePtr->SetBranchAddress("_pl_metPhi",                &_pl_metPhi,                &b__pl_metPhi);
        _currentTreePtr->SetBranchAddress("_pl_nPh",                   &_pl_nPh,                   &b__pl_nPh);
        _currentTreePtr->SetBranchAddress("_pl_phPt",                  _pl_phPt,                  &b__pl_phPt);
        _currentTreePtr->SetBranchAddress("_pl_phEta",                 _pl_phEta,                 &b__pl_phEta);
        _currentTreePtr->SetBranchAddress("_pl_phPhi",                 _pl_phPhi,                 &b__pl_phPhi);
        _currentTreePtr->SetBranchAddress("_pl_phE",                   _pl_phE,                   &b__pl_phE);
        _currentTreePtr->SetBranchAddress("_pl_nL",                    &_pl_nL,                    &b__pl_nL);
        _currentTreePtr->SetBranchAddress("_pl_lPt",                   _pl_lPt,                   &b__pl_lPt);
        _currentTreePtr->SetBranchAddress("_pl_lEta",                  _pl_lEta,                  &b__pl_lEta);
        _currentTreePtr->SetBranchAddress("_pl_lPhi",                  _pl_lPhi,                  &b__pl_lPhi);
        _currentTreePtr->SetBranchAddress("_pl_lE",                    _pl_lE,                    &b__pl_lE);
        _currentTreePtr->SetBranchAddress("_pl_lFlavor",               _pl_lFlavor,               &b__pl_lFlavor);
        _currentTreePtr->SetBranchAddress("_pl_lCharge",               _pl_lCharge,               &b__pl_lCharge);
        _currentTreePtr->SetBranchAddress("_pl_nJets",                 &_pl_nJets,                 &b__pl_nJets);
        _currentTreePtr->SetBranchAddress("_pl_jetPt",                 _pl_jetPt,                 &b__pl_jetPt);
        _currentTreePtr->SetBranchAddress("_pl_jetEta",                _pl_jetEta,                &b__pl_jetEta);
        _currentTreePtr->SetBranchAddress("_pl_jetPhi",                _pl_jetPhi,                &b__pl_jetPhi);
        _currentTreePtr->SetBranchAddress("_pl_jetE",                  _pl_jetE,                  &b__pl_jetE);
        _currentTreePtr->SetBranchAddress("_pl_jetHadronFlavor",       _pl_jetHadronFlavor,       &b__pl_jetHadronFlavor);
    }

    if (containsLheInfo()) {
        _currentTreePtr->SetBranchAddress("_nLheTau", &_nLheTau, &b__nLheTau);
        _currentTreePtr->SetBranchAddress("_nLheParticles", &_nLheParticles, &b__nLheParticles);
        _currentTreePtr->SetBranchAddress("_lheStatus", _lheStatus, &b__lheStatus);
        _currentTreePtr->SetBranchAddress("_lhePdgId", _lhePdgId, &b__lhePdgId);
        _currentTreePtr->SetBranchAddress("_lheMother1", _lheMother1, &b__lheMother1);
        _currentTreePtr->SetBranchAddress("_lheMother2", _lheMother2, &b__lheMother2);
        _currentTreePtr->SetBranchAddress("_lhePt", _lhePt, &b__lhePt);
        _currentTreePtr->SetBranchAddress("_lheEta", _lheEta, &b__lheEta);
        _currentTreePtr->SetBranchAddress("_lhePhi", _lhePhi, &b__lhePhi);
        _currentTreePtr->SetBranchAddress("_lheE", _lheE, &b__lheE);
        _currentTreePtr->SetBranchAddress("_lheMass", _lheMass, &b__lheMass);
    }

    if( isMC() ){
        _currentTreePtr->SetBranchAddress("_prefireWeight", &_prefireWeight, &b__prefireWeight);
        _currentTreePtr->SetBranchAddress("_prefireWeightDown", &_prefireWeightDown, &b__prefireWeightDown);
        _currentTreePtr->SetBranchAddress("_prefireWeightUp", &_prefireWeightUp, &b__prefireWeightUp);
    }

    if( containsSusyMassInfo() ){
	_currentTreePtr->SetBranchAddress("_mChi1", &_mChi1, &b__mChi1);
	_currentTreePtr->SetBranchAddress("_mChi2", &_mChi2, &b__mChi2);
    }

    //add all individually stored triggers 
    // always reset triggers instead of rare case of combining primary datasets 
    // to prevent invalidating addresses set by setOutputTree
    if( resetTriggersAndFilters || _triggerMap.empty() ){
        initializeTriggerMap( _currentTreePtr );
    }
    setMapBranchAddresses( _currentTreePtr, _triggerMap, b__triggerMap );

    //add all individually stored MET filters
    // always reset filters instead of rare case of combining primary datasets 
    // to prevent invalidating addresses set by setOutputTree
    if( resetTriggersAndFilters || _MetFilterMap.empty() ){
        initializeMetFilterMap( _currentTreePtr );
    }
    setMapBranchAddresses( _currentTreePtr, _MetFilterMap, b__MetFilterMap );

    // add split JEC uncertainties
    std::cout << "pre source" << std::endl;
    initializeJecSourcesMaps( _currentTreePtr );
    std::cout << "source init" << std::endl;

    setMapBranchAddressesAndVectors( _currentTreePtr, _jetPt_JECSourcesUp_Ids, _jetPt_JECSourcesUp, b__jetPt_JECSourcesUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _jetPt_JECSourcesDown_Ids, _jetPt_JECSourcesDown, b__jetPt_JECSourcesDown );
    setMapBranchAddressesAndVectors( _currentTreePtr, _jetSmearedPt_JECSourcesUp_Ids, _jetSmearedPt_JECSourcesUp, b__jetSmearedPt_JECSourcesUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _jetSmearedPt_JECSourcesDown_Ids, _jetSmearedPt_JECSourcesDown, b__jetSmearedPt_JECSourcesDown );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETx_JECSourcesUp_Ids, _corrMETx_JECSourcesUp, b__corrMETx_JECSourcesUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETx_JECSourcesDown_Ids, _corrMETx_JECSourcesDown, b__corrMETx_JECSourcesDown );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETy_JECSourcesUp_Ids, _corrMETy_JECSourcesUp, b__corrMETy_JECSourcesUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETy_JECSourcesDown_Ids, _corrMETy_JECSourcesDown, b__corrMETy_JECSourcesDown );

    std::cout << "pre group" << std::endl;

    initializeJecSourcesGroupedMaps( _currentTreePtr );

    std::cout << "group init" << std::endl;

    setMapBranchAddressesAndVectors( _currentTreePtr, _jetPt_JECGroupedUp_Ids, _jetPt_JECGroupedUp, b__jetPt_JECGroupedUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _jetPt_JECGroupedDown_Ids, _jetPt_JECGroupedDown, b__jetPt_JECGroupedDown );
    setMapBranchAddressesAndVectors( _currentTreePtr, _jetSmearedPt_JECGroupedUp_Ids, _jetSmearedPt_JECGroupedUp, b__jetSmearedPt_JECGroupedUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _jetSmearedPt_JECGroupedDown_Ids, _jetSmearedPt_JECGroupedDown, b__jetSmearedPt_JECGroupedDown );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETx_JECGroupedUp_Ids, _corrMETx_JECGroupedUp, b__corrMETx_JECGroupedUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETx_JECGroupedDown_Ids, _corrMETx_JECGroupedDown, b__corrMETx_JECGroupedDown );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETy_JECGroupedUp_Ids, _corrMETy_JECGroupedUp, b__corrMETy_JECGroupedUp );
    setMapBranchAddressesAndVectors( _currentTreePtr, _corrMETy_JECGroupedDown_Ids, _corrMETy_JECGroupedDown, b__corrMETy_JECGroupedDown );
}


void TreeReader::setOutputTree( TTree* outputTree ){
    outputTree->Branch("_runNb",                        &_runNb,                        "_runNb/l");
    outputTree->Branch("_lumiBlock",                    &_lumiBlock,                    "_lumiBlock/l");
    outputTree->Branch("_eventNb",                      &_eventNb,                      "_eventNb/l");
    outputTree->Branch("_nVertex",                      &_nVertex,                      "_nVertex/i");
    outputTree->Branch("_met",                          &_met,                          "_met/D");
    outputTree->Branch("_met_JECDown",                  &_met_JECDown,                  "_met_JECDown/D");
    outputTree->Branch("_met_JECUp",                    &_met_JECUp,                    "_met_JECUp/D");
    outputTree->Branch("_met_UnclDown",                 &_met_UnclDown,                 "_met_UnclDown/D");
    outputTree->Branch("_met_UnclUp",                   &_met_UnclUp,                   "_met_UnclUp/D");
    outputTree->Branch("_metPhi",                       &_metPhi,                       "_metPhi/D");
    outputTree->Branch("_metPhi_JECDown",               &_metPhi_JECDown,               "_metPhi_JECDown/D");
    outputTree->Branch("_metPhi_JECUp",                 &_metPhi_JECUp,                 "_metPhi_JECUp/D");
    outputTree->Branch("_metPhi_UnclDown",              &_metPhi_UnclDown,              "_metPhi_UnclDown/D");
    outputTree->Branch("_metPhi_UnclUp",                &_metPhi_UnclUp,                "_metPhi_UnclUp/D");
    outputTree->Branch("_metSignificance",              &_metSignificance,              "_metSignificance/D");
    outputTree->Branch("_passTrigger_e", &_passTrigger_e, "_passTrigger_e/O");
    outputTree->Branch("_passTrigger_ee", &_passTrigger_ee, "_passTrigger_ee/O");
    outputTree->Branch("_passTrigger_eee", &_passTrigger_eee, "_passTrigger_eee/O");
    outputTree->Branch("_passTrigger_em", &_passTrigger_em, "_passTrigger_em/O");
    outputTree->Branch("_passTrigger_m", &_passTrigger_m, "_passTrigger_m/O");
    outputTree->Branch("_passTrigger_eem", &_passTrigger_eem, "_passTrigger_eem/O");
    outputTree->Branch("_passTrigger_mm", &_passTrigger_mm, "_passTrigger_mm/O");
    outputTree->Branch("_passTrigger_emm", &_passTrigger_emm, "_passTrigger_emm/O");
    outputTree->Branch("_passTrigger_mmm", &_passTrigger_mmm, "_passTrigger_mmm/O");
    outputTree->Branch("_passTrigger_et", &_passTrigger_et, "_passTrigger_et/O");
    outputTree->Branch("_passTrigger_mt", &_passTrigger_mt, "_passTrigger_mt/O");
    outputTree->Branch("_passTrigger_FR", &_passTrigger_FR, "_passTrigger_FR/O");
    outputTree->Branch("_passTrigger_FR_iso", &_passTrigger_FR_iso, "_passTrigger_FR_iso/O");
    outputTree->Branch("_passMETFilters", &_passMETFilters, "_passMETFilters/O");
    outputTree->Branch("_nL",                           &_nL,                           "_nL/i");
    outputTree->Branch("_nMu",                          &_nMu,                          "_nMu/i");
    outputTree->Branch("_nEle",                         &_nEle,                         "_nEle/i");
    outputTree->Branch("_nLight",                       &_nLight,                       "_nLight/i");
    outputTree->Branch("_nTau",                         &_nTau,                         "_nTau/i");
    outputTree->Branch("_lPt",                          &_lPt,                          "_lPt[_nL]/D");
    outputTree->Branch("_lPtCorr",                      &_lPtCorr,                      "_lPtCorr[_nLight]/D");
    outputTree->Branch("_lPtScaleUp",			&_lPtScaleUp,                   "_lPtScaleUp[_nLight]/D");
    outputTree->Branch("_lPtScaleDown",                 &_lPtScaleDown,                 "_lPtScaleDown[_nLight]/D");
    outputTree->Branch("_lPtResUp",                     &_lPtResUp,                     "_lPtResUp[_nLight]/D");
    outputTree->Branch("_lPtResDown",                   &_lPtResDown,                   "_lPtResDown[_nLight]/D");
    outputTree->Branch("_lEta",                         &_lEta,                         "_lEta[_nL]/D");
    outputTree->Branch("_lEtaSC",                       &_lEtaSC,                       "_lEtaSC[_nLight]/D");
    outputTree->Branch("_lPhi",                         &_lPhi,                         "_lPhi[_nL]/D");
    outputTree->Branch("_lE",                           &_lE,                           "_lE[_nL]/D");
    outputTree->Branch("_lECorr",                       &_lECorr,                       "_lECorr[_nLight]/D");
    outputTree->Branch("_lEScaleUp",                    &_lEScaleUp,                    "_lEScaleUp[_nLight]/D");
    outputTree->Branch("_lEScaleDown",                  &_lEScaleDown,                  "_lEScaleDown[_nLight]/D");
    outputTree->Branch("_lEResUp",                      &_lEResUp,                      "_lEResUp[_nLight]/D");
    outputTree->Branch("_lEResDown",                    &_lEResDown,                    "_lEResDown[_nLight]/D");
    outputTree->Branch("_lFlavor",                      &_lFlavor,                      "_lFlavor[_nL]/i");
    outputTree->Branch("_lCharge",                      &_lCharge,                      "_lCharge[_nL]/I");
    outputTree->Branch("_dxy",                          &_dxy,                          "_dxy[_nL]/D");
    outputTree->Branch("_dz",                           &_dz,                           "_dz[_nL]/D");
    outputTree->Branch("_3dIP",                         &_3dIP,                         "_3dIP[_nL]/D");
    outputTree->Branch("_3dIPSig",                      &_3dIPSig,                      "_3dIPSig[_nL]/D");
    outputTree->Branch("_lElectronSummer16MvaGP",       &_lElectronSummer16MvaGP,       "_lElectronSummer16MvaGP[_nLight]/F");
    outputTree->Branch("_lElectronSummer16MvaHZZ",      &_lElectronSummer16MvaHZZ,      "_lElectronSummer16MvaHZZ[_nLight]/F");
    outputTree->Branch("_lElectronMvaFall17Iso",        &_lElectronMvaFall17Iso,        "_lElectronMvaFall17Iso[_nLight]/F");
    outputTree->Branch("_lElectronMvaFall17NoIso",      &_lElectronMvaFall17NoIso,      "_lElectronMvaFall17NoIso[_nLight]/F");
    outputTree->Branch("_lElectronPassMVAFall17NoIsoWPLoose", &_lElectronPassMVAFall17NoIsoWPLoose, "_lElectronPassMVAFall17NoIsoWPLoose[_nLight]/O");
    outputTree->Branch("_lElectronPassMVAFall17NoIsoWP90", &_lElectronPassMVAFall17NoIsoWP90, "_lElectronPassMVAFall17NoIsoWP90[_nLight]/O");
    outputTree->Branch("_lElectronPassMVAFall17NoIsoWP80", &_lElectronPassMVAFall17NoIsoWP80, "_lElectronPassMVAFall17NoIsoWP80[_nLight]/O");
    outputTree->Branch("_lElectronPassEmu",             &_lElectronPassEmu,             "_lElectronPassEmu[_nLight]/O");
    outputTree->Branch("_lElectronPassConvVeto",        &_lElectronPassConvVeto,        "_lElectronPassConvVeto[_nLight]/O");
    outputTree->Branch("_lElectronChargeConst",         &_lElectronChargeConst,         "_lElectronChargeConst[_nLight]/O");
    outputTree->Branch("_lElectronMissingHits",         &_lElectronMissingHits,         "_lElectronMissingHits[_nLight]/i");
    outputTree->Branch("_lElectronEInvMinusPInv",       &_lElectronEInvMinusPInv,       "_lElectronEInvMinusPInv[_nLight]/D");
    outputTree->Branch("_lElectronHOverE",              &_lElectronHOverE,              "_lElectronHOverE[_nLight]/D");
    outputTree->Branch("_lElectronSigmaIetaIeta",       &_lElectronSigmaIetaIeta,       "_lElectronSigmaIetaIeta[_nLight]/D");
    outputTree->Branch("_leptonMvaTTH",                 &_leptonMvaTTH,                 "_leptonMvaTTH[_nLight]/D");
    outputTree->Branch("_leptonMvatZq",                 &_leptonMvatZq,                 "_leptonMvatZq[_nLight]/D");
    outputTree->Branch("_leptonMvaTOP",                 &_leptonMvaTOP,                 "_leptonMvaTOP[_nLight]/D");
    outputTree->Branch("_leptonMvaTOPUL",               &_leptonMvaTOPUL,               "_leptonMvaTOPUL[_nLight]/D");
    outputTree->Branch("_leptonMvaTOPv2UL",             &_leptonMvaTOPULv2,             "_leptonMvaTOPv2UL[_nLight]/D");
    outputTree->Branch("_lPOGVeto",                     &_lPOGVeto,                     "_lPOGVeto[_nL]/O");
    outputTree->Branch("_lPOGLoose",                    &_lPOGLoose,                    "_lPOGLoose[_nL]/O");
    outputTree->Branch("_lPOGMedium",                   &_lPOGMedium,                   "_lPOGMedium[_nL]/O");
    outputTree->Branch("_lPOGTight",                    &_lPOGTight,                    "_lPOGTight[_nL]/O");

    outputTree->Branch("_tauDecayMode",                 &_tauDecayMode,                 "_tauDecayMode[_nL]/i");
    outputTree->Branch("_decayModeFinding",             &_decayModeFinding,             "_decayModeFinding[_nL]/O");
    outputTree->Branch("_decayModeFindingNew",          &_decayModeFindingNew,          "_decayModeFindingNew[_nL]/O");
    outputTree->Branch("_tauMuonVetoLoose",             &_tauMuonVetoLoose,             "_tauMuonVetoLoose[_nL]/O");

    if (treeHasBranchWithName(_currentTreePtr, "_tauMuonVetoTight")) {
        outputTree->Branch("_tauPOGVLoose2015",             &_tauPOGVLoose2015,             "_tauPOGVLoose2015[_nL]/O");
        outputTree->Branch("_tauPOGLoose2015",              &_tauPOGLoose2015,              "_tauPOGLoose2015[_nL]/O");
        outputTree->Branch("_tauPOGMedium2015",             &_tauPOGMedium2015,             "_tauPOGMedium2015[_nL]/O");
        outputTree->Branch("_tauPOGTight2015",              &_tauPOGTight2015,              "_tauPOGTight2015[_nL]/O");
        outputTree->Branch("_tauPOGVTight2015",             &_tauPOGVTight2015,             "_tauPOGVTight2015[_nL]/O");
        outputTree->Branch("_tauVLooseMvaNew2015",          &_tauVLooseMvaNew2015,          "_tauVLooseMvaNew2015[_nL]/O");
        outputTree->Branch("_tauLooseMvaNew2015",           &_tauLooseMvaNew2015,           "_tauLooseMvaNew2015[_nL]/O");
        outputTree->Branch("_tauMediumMvaNew2015",          &_tauMediumMvaNew2015,          "_tauMediumMvaNew2015[_nL]/O");
        outputTree->Branch("_tauTightMvaNew2015",           &_tauTightMvaNew2015,           "_tauTightMvaNew2015[_nL]/O");
        outputTree->Branch("_tauVTightMvaNew2015",          &_tauVTightMvaNew2015,          "_tauVTightMvaNew2015[_nL]/O");
        outputTree->Branch("_tauPOGVVLoose2017v2",          &_tauPOGVVLoose2017v2,          "_tauPOGVVLoose2017v2[_nL]/O");
        outputTree->Branch("_tauPOGVTight2017v2",           &_tauPOGVTight2017v2,           "_tauPOGVTight2017v2[_nL]/O");
        outputTree->Branch("_tauPOGVVTight2017v2",          &_tauPOGVVTight2017v2,          "_tauPOGVVTight2017v2[_nL]/O");
        outputTree->Branch("_tauVLooseMvaNew2017v2",        &_tauVLooseMvaNew2017v2,        "_tauVLooseMvaNew2017v2[_nL]/O");
        outputTree->Branch("_tauLooseMvaNew2017v2",         &_tauLooseMvaNew2017v2,         "_tauLooseMvaNew2017v2[_nL]/O");
        outputTree->Branch("_tauMediumMvaNew2017v2",        &_tauMediumMvaNew2017v2,        "_tauMediumMvaNew2017v2[_nL]/O");
        outputTree->Branch("_tauTightMvaNew2017v2",         &_tauTightMvaNew2017v2,         "_tauTightMvaNew2017v2[_nL]/O");
        outputTree->Branch("_tauVTightMvaNew2017v2",        &_tauVTightMvaNew2017v2,        "_tauVTightMvaNew2017v2[_nL]/O");
        outputTree->Branch("_tauMuonVetoTight",             &_tauMuonVetoTight,             "_tauMuonVetoTight[_nL]/O");
        outputTree->Branch("_tauEleVetoVLoose",             &_tauEleVetoVLoose,             "_tauEleVetoVLoose[_nL]/O");
        outputTree->Branch("_tauEleVetoLoose",              &_tauEleVetoLoose,              "_tauEleVetoLoose[_nL]/O");
        outputTree->Branch("_tauEleVetoMedium",             &_tauEleVetoMedium,             "_tauEleVetoMedium[_nL]/O");
        outputTree->Branch("_tauEleVetoTight",              &_tauEleVetoTight,              "_tauEleVetoTight[_nL]/O");
        outputTree->Branch("_tauEleVetoVTight",             &_tauEleVetoVTight,             "_tauEleVetoVTight[_nL]/O"); 
    }

    outputTree->Branch("_relIso",                       &_relIso,                       "_relIso[_nLight]/D");
    outputTree->Branch("_relIso0p4",                    &_relIso0p4,                    "_relIso0p4[_nLight]/D");
    outputTree->Branch("_relIso0p4MuDeltaBeta",         &_relIso0p4MuDeltaBeta,         "_relIso0p4MuDeltaBeta[_nMu]/D");
    outputTree->Branch("_miniIso",                      &_miniIso,                      "_miniIso[_nLight]/D");
    outputTree->Branch("_miniIsoCharged",               &_miniIsoCharged,               "_miniIsoCharged[_nLight]/D");
    outputTree->Branch("_ptRel",                        &_ptRel,                        "_ptRel[_nLight]/D");
    outputTree->Branch("_ptRatio",                      &_ptRatio,                      "_ptRatio[_nLight]/D");
    outputTree->Branch("_closestJetCsvV2",              &_closestJetCsvV2,              "_closestJetCsvV2[_nLight]/D");
    outputTree->Branch("_closestJetDeepCsv_b",          &_closestJetDeepCsv_b,          "_closestJetDeepCsv_b[_nLight]/D");
    outputTree->Branch("_closestJetDeepCsv_bb",         &_closestJetDeepCsv_bb,         "_closestJetDeepCsv_bb[_nLight]/D");
	outputTree->Branch("_closestJetDeepFlavor_b",       &_closestJetDeepFlavor_b,       "_closestJetDeepFlavor_b[_nLight]/D");
    outputTree->Branch("_closestJetDeepFlavor_bb",      &_closestJetDeepFlavor_bb,      "_closestJetDeepFlavor_bb[_nLight]/D");
    outputTree->Branch("_closestJetDeepFlavor_lepb",    &_closestJetDeepFlavor_lepb,    "_closestJetDeepFlavor_lepb[_nLight]/D");
    outputTree->Branch("_selectedTrackMult",            &_selectedTrackMult,            "_selectedTrackMult[_nLight]/i");
    outputTree->Branch("_lMuonSegComp",                 &_lMuonSegComp,                 "_lMuonSegComp[_nMu]/D");
    outputTree->Branch("_lMuonTrackPt",                 &_lMuonTrackPt,                 "_lMuonTrackPt[_nMu]/D");
    outputTree->Branch("_lMuonTrackPtErr",              &_lMuonTrackPtErr,              "_lMuonTrackPtErr[_nMu]/D");
    outputTree->Branch("_nJets",                     &_nJets,                    "_nJets/i");
    outputTree->Branch("_jetPt",                     &_jetPt,                    "_jetPt[_nJets]/D");
    outputTree->Branch("_jetPt_JECUp",               &_jetPt_JECUp,              "_jetPt_JECUp[_nJets]/D");
    outputTree->Branch("_jetPt_JECDown",             &_jetPt_JECDown,            "_jetPt_JECDown[_nJets]/D");
    outputTree->Branch("_jetSmearedPt",              &_jetSmearedPt,             "_jetSmearedPt[_nJets]/D");
    outputTree->Branch("_jetSmearedPt_JECDown",      &_jetSmearedPt_JECDown,     "_jetSmearedPt_JECDown[_nJets]/D");
    outputTree->Branch("_jetSmearedPt_JECUp",        &_jetSmearedPt_JECUp,       "_jetSmearedPt_JECUp[_nJets]/D");
    outputTree->Branch("_jetSmearedPt_JERDown",      &_jetSmearedPt_JERDown,     "_jetSmearedPt_JERDown[_nJets]/D");
    outputTree->Branch("_jetSmearedPt_JERUp",        &_jetSmearedPt_JERUp,       "_jetSmearedPt_JERUp[_nJets]/D");
    outputTree->Branch("_jetEta",                    &_jetEta,                   "_jetEta[_nJets]/D");
    outputTree->Branch("_jetPhi",                    &_jetPhi,                   "_jetPhi[_nJets]/D");
    outputTree->Branch("_jetE",                      &_jetE,                     "_jetE[_nJets]/D");
    outputTree->Branch("_jetPt_Uncorrected",         &_jetPt_Uncorrected,        "_jetPt_Uncorrected[_nJets]/D");
    outputTree->Branch("_jetPt_L1",                  &_jetPt_L1,                 "_jetPt_L1[_nJets]/D");
    outputTree->Branch("_jetPt_L2",                  &_jetPt_L2,                 "_jetPt_L2[_nJets]/D");
    outputTree->Branch("_jetPt_L3",                  &_jetPt_L3,                 "_jetPt_L3[_nJets]/D");
    outputTree->Branch("_jetCsvV2",                  &_jetCsvV2,                 "_jetCsvV2[_nJets]/D");
    outputTree->Branch("_jetDeepCsv_udsg",           &_jetDeepCsv_udsg,          "_jetDeepCsv_udsg[_nJets]/D");
    outputTree->Branch("_jetDeepCsv_b",              &_jetDeepCsv_b,             "_jetDeepCsv_b[_nJets]/D");
    outputTree->Branch("_jetDeepCsv_c",              &_jetDeepCsv_c,             "_jetDeepCsv_c[_nJets]/D");
    outputTree->Branch("_jetDeepCsv_bb",             &_jetDeepCsv_bb,            "_jetDeepCsv_bb[_nJets]/D");
	outputTree->Branch("_jetDeepFlavor_b",           &_jetDeepFlavor_b,          "_jetDeepFlavor_b[_nJets]/D");
    outputTree->Branch("_jetDeepFlavor_bb",          &_jetDeepFlavor_bb,         "_jetDeepFlavor_bb[_nJets]/D");
    outputTree->Branch("_jetDeepFlavor_lepb",        &_jetDeepFlavor_lepb,       "_jetDeepFlavor_lepb[_nJets]/D");
    outputTree->Branch("_jetHadronFlavor",           &_jetHadronFlavor,          "_jetHadronFlavor[_nJets]/i");
    outputTree->Branch("_jetIsTight",                &_jetIsTight,               "_jetIsTight[_nJets]/O");
    outputTree->Branch("_jetIsTightLepVeto",         &_jetIsTightLepVeto,        "_jetIsTightLepVeto[_nJets]/O");
    outputTree->Branch("_jetNeutralHadronFraction",  &_jetNeutralHadronFraction, "_jetNeutralHadronFraction[_nJets]/D");
    outputTree->Branch("_jetChargedHadronFraction",  &_jetChargedHadronFraction, "_jetChargedHadronFraction[_nJets]/D");
    outputTree->Branch("_jetNeutralEmFraction",      &_jetNeutralEmFraction,     "_jetNeutralEmFraction[_nJets]/D");
    outputTree->Branch("_jetChargedEmFraction",      &_jetChargedEmFraction,     "_jetChargedEmFraction[_nJets]/D");
    outputTree->Branch("_jetHFHadronFraction",       &_jetHFHadronFraction,      "_jetHFHadronFraction[_nJets]/D");
    outputTree->Branch("_jetHFEmFraction",           &_jetHFEmFraction,          "_jetHFEmFraction[_nJets]/D");


    if( containsGeneratorInfo() ){
        outputTree->Branch("_nLheWeights",               &_nLheWeights,               "_nLheWeights/i");
        outputTree->Branch("_lheWeight",                 &_lheWeight,                 "_lheWeight[_nLheWeights]/D");
        outputTree->Branch("_weight",                    &_weight,                    "_weight/D");
        outputTree->Branch("_nPsWeights",                &_nPsWeights,                "_nPsWeights/i");
        outputTree->Branch("_psWeight",                  &_psWeight,                  "_psWeight[_nPsWeights]/D");
        outputTree->Branch("_nTrueInt",                  &_nTrueInt,                  "_nTrueInt/F");
        outputTree->Branch("_lheHTIncoming",             &_lheHTIncoming,             "_lheHTIncoming/D");
        outputTree->Branch("_lIsPrompt",                 &_lIsPrompt,                 "_lIsPrompt[_nL]/O");
        outputTree->Branch("_lMatchPdgId",               &_lMatchPdgId,               "_lMatchPdgId[_nL]/I");
        outputTree->Branch("_lMatchCharge",              &_lMatchCharge,              "_lMatchCharge[_nL]/I");
        outputTree->Branch("_lMomPdgId",                 &_lMomPdgId,                 "_lMomPdgId[_nL]/I");
        outputTree->Branch("_lProvenance",               &_lProvenance,               "_lProvenance[_nL]/i");
        outputTree->Branch("_lProvenanceCompressed",     &_lProvenanceCompressed,     "_lProvenanceCompressed[_nL]/i");
        outputTree->Branch("_lProvenanceConversion",     &_lProvenanceConversion,     "_lProvenanceConversion[_nL]/i");
        outputTree->Branch("_gen_met",                   &_gen_met,                   "_gen_met/D");
        outputTree->Branch("_gen_metPhi",                &_gen_metPhi,                "_gen_metPhi/D");
        outputTree->Branch("_gen_nL",                    &_gen_nL,                    "_gen_nL/i");
        outputTree->Branch("_gen_lPt",                   &_gen_lPt,                   "_gen_lPt[_gen_nL]/D");
        outputTree->Branch("_gen_lEta",                  &_gen_lEta,                  "_gen_lEta[_gen_nL]/D");
        outputTree->Branch("_gen_lPhi",                  &_gen_lPhi,                  "_gen_lPhi[_gen_nL]/D");
        outputTree->Branch("_gen_lE",                    &_gen_lE,                    "_gen_lE[_gen_nL]/D");
        outputTree->Branch("_gen_lFlavor",               &_gen_lFlavor,               "_gen_lFlavor[_gen_nL]/i");
        outputTree->Branch("_gen_lCharge",               &_gen_lCharge,               "_gen_lCharge[_gen_nL]/I");
        outputTree->Branch("_gen_lMomPdg",               &_gen_lMomPdg,               "_gen_lMomPdg[_gen_nL]/I");
        outputTree->Branch("_gen_lIsPrompt",             &_gen_lIsPrompt,             "_gen_lIsPrompt[_gen_nL]/O");
        outputTree->Branch("_ttgEventType",              &_ttgEventType,              "_ttgEventType/i");
        outputTree->Branch("_zgEventType",               &_zgEventType,               "_zgEventType/i");

        if ( containsFullGeneratorInfo() ) {
            outputTree->Branch("_gen_n",                                        &_gen_n,                                        "_gen_n/I");
            outputTree->Branch("_gen_pt",                                       &_gen_pt,                                       "_gen_pt[_gen_n]/D");
            outputTree->Branch("_gen_eta",                                      &_gen_eta,                                      "_gen_eta[_gen_n]/D");
            outputTree->Branch("_gen_phi",                                      &_gen_phi,                                      "_gen_phi[_gen_n]/D");
            outputTree->Branch("_gen_E",                                        &_gen_E,                                        "_gen_E[_gen_n]/D");
            outputTree->Branch("_gen_pdgId",                                    &_gen_pdgId,                                    "_gen_pdgId[_gen_n]/I");
            outputTree->Branch("_gen_charge",                                   &_gen_charge,                                   "_gen_charge[_gen_n]/I");
            outputTree->Branch("_gen_status",                                   &_gen_status,                                   "_gen_status[_gen_n]/I");
            outputTree->Branch("_gen_isPromptFinalState",                       &_gen_isPromptFinalState,                       "_gen_isPromptFinalState[_gen_n]/O");
            outputTree->Branch("_gen_isDirectPromptTauDecayProductFinalState",  &_gen_isDirectPromptTauDecayProductFinalState,  "_gen_isDirectPromptTauDecayProductFinalState[_gen_n]/O");
            outputTree->Branch("_gen_isLastCopy",                               &_gen_isLastCopy,                               "_gen_isLastCopy[_gen_n]/O");
            outputTree->Branch("_gen_index",                                    &_gen_index,                                    "_gen_index[_gen_n]/I");
            outputTree->Branch("_gen_motherIndex",                              &_gen_motherIndex,                              "_gen_motherIndex[_gen_n]/I");
            outputTree->Branch("_gen_daughter_n",                               &_gen_daughter_n,                               "_gen_daughter_n[_gen_n]/I");
            outputTree->Branch("_gen_daughterIndex",                            &_gen_daughterIndex,                            "_gen_daughterIndex[_gen_n][10]/I");
        }

        if ( containsEFTInfo()) {
            outputTree->Branch("_nDynScaleWeights",     &_nDynScaleWeights,   "_nDynScaleWeights/i");
            outputTree->Branch("_dynScaleWeight",       &_dynScaleWeight,     "_dynScaleWeight[_nDynScaleWeights]/D");
            outputTree->Branch("_nEFTWeights",          &_nEFTWeights,        "_nEFTWeights/i");
            outputTree->Branch("_eftWeight",            &_eftWeight,          "_eftWeight[_nEFTWeights]/D");
        }
    } 

    if (containsParticleLevelInfo()) {
        outputTree->Branch("_pl_met",                   &_pl_met,                   "_pl_met/D");
        outputTree->Branch("_pl_metPhi",                &_pl_metPhi,                "_pl_metPhi/D");
        outputTree->Branch("_pl_nPh",                   &_pl_nPh,                   "_pl_nPh/i");
        outputTree->Branch("_pl_phPt",                  &_pl_phPt,                  "_pl_phPt[_pl_nPh]/D");
        outputTree->Branch("_pl_phEta",                 &_pl_phEta,                 "_pl_phEta[_pl_nPh]/D");
        outputTree->Branch("_pl_phPhi",                 &_pl_phPhi,                 "_pl_phPhi[_pl_nPh]/D");
        outputTree->Branch("_pl_phE",                   &_pl_phE,                   "_pl_phE[_pl_nPh]/D");
        outputTree->Branch("_pl_nL",                    &_pl_nL,                    "_pl_nL/i");
        outputTree->Branch("_pl_lPt",                   &_pl_lPt,                   "_pl_lPt[_pl_nL]/D");
        outputTree->Branch("_pl_lEta",                  &_pl_lEta,                  "_pl_lEta[_pl_nL]/D");
        outputTree->Branch("_pl_lPhi",                  &_pl_lPhi,                  "_pl_lPhi[_pl_nL]/D");
        outputTree->Branch("_pl_lE",                    &_pl_lE,                    "_pl_lE[_pl_nL]/D");
        outputTree->Branch("_pl_lFlavor",               &_pl_lFlavor,               "_pl_lFlavor[_pl_nL]/i");
        outputTree->Branch("_pl_lCharge",               &_pl_lCharge,               "_pl_lCharge[_pl_nL]/I");
        outputTree->Branch("_pl_nJets",                 &_pl_nJets,                 "_pl_nJets/i");
        outputTree->Branch("_pl_jetPt",                 &_pl_jetPt,                 "_pl_jetPt[_pl_nJets]/D");
        outputTree->Branch("_pl_jetEta",                &_pl_jetEta,                "_pl_jetEta[_pl_nJets]/D");
        outputTree->Branch("_pl_jetPhi",                &_pl_jetPhi,                "_pl_jetPhi[_pl_nJets]/D");
        outputTree->Branch("_pl_jetE",                  &_pl_jetE,                  "_pl_jetE[_pl_nJets]/D");
        outputTree->Branch("_pl_jetHadronFlavor",       &_pl_jetHadronFlavor,       "_pl_jetHadronFlavor[_pl_nJets]/i");
    }

    if (containsLheInfo()) {
        outputTree->Branch("_nLheTau",                   &_nLheTau,                   "_nLheTau/i");
        outputTree->Branch("_nLheParticles",             &_nLheParticles,             "_nLheParticles/i");
        outputTree->Branch("_lheStatus",                 &_lheStatus,                 "_lheStatus[_nLheParticles]/I");
        outputTree->Branch("_lhePdgId",                  &_lhePdgId,                  "_lhePdgId[_nLheParticles]/I");
        outputTree->Branch("_lheMother1",                &_lheMother1,                "_lheMother1[_nLheParticles]/I");
        outputTree->Branch("_lheMother2",                &_lheMother2,                "_lheMother2[_nLheParticles]/I");
        outputTree->Branch("_lhePt",                     &_lhePt,                     "_lhePt[_nLheParticles]/F");
        outputTree->Branch("_lheEta",                    &_lheEta,                    "_lheEta[_nLheParticles]/F");
        outputTree->Branch("_lhePhi",                    &_lhePhi,                    "_lhePhi[_nLheParticles]/F");
        outputTree->Branch("_lheE",                      &_lheE,                      "_lheE[_nLheParticles]/F");
        outputTree->Branch("_lheMass",                   &_lheMass,                   "_lheMass[_nLheParticles]/F");
    }

    if( isMC() ){
       	outputTree->Branch("_prefireWeight",             &_prefireWeight,             "_prefireWeight/F");
        outputTree->Branch("_prefireWeightUp",           &_prefireWeightUp,           "_prefireWeightUp/F");
        outputTree->Branch("_prefireWeightDown",         &_prefireWeightDown,         "_prefireWeightDown/F"); 
    }

    if( containsSusyMassInfo() ){
	outputTree->Branch("_mChi1", &_mChi1, "_mChi1/D");
	outputTree->Branch("_mChi2", &_mChi2, "_mChi2/D");
    }

    //write individual trigger decisions to output tree 
    setMapOutputBranches( outputTree, _triggerMap, "/O" );

    //write individual MET filters to output tree
    setMapOutputBranches( outputTree, _MetFilterMap, "/O" );

    // write split JEC uncertainties to output tree
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _jetPt_JECSourcesUp, "[_nJets]/D", "_jetPt_", "_JECSourcesUp" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _jetPt_JECSourcesDown, "[_nJets]/D", "_jetPt_", "_JECSourcesDown" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _jetSmearedPt_JECSourcesUp, "[_nJets]/D", "_jetSmearedPt_", "_JECSourcesUp" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _jetSmearedPt_JECSourcesDown, "[_nJets]/D", "_jetSmearedPt_", "_JECSourcesDown" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _jetPt_JECGroupedUp, "[_nJets]/D", "_jetPt_", "_JECGroupedUp" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _jetPt_JECGroupedDown, "[_nJets]/D", "_jetPt_", "_JECGroupedDown" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _jetSmearedPt_JECGroupedUp, "[_nJets]/D", "_jetSmearedPt_", "_JECGroupedUp" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _jetSmearedPt_JECGroupedDown, "[_nJets]/D", "_jetSmearedPt_", "_JECGroupedDown" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _corrMETx_JECSourcesUp, "/D", "_corrMETx_", "_JECSourcesUp" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _corrMETx_JECSourcesDown, "/D", "_corrMETx_", "_JECSourcesDown" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _corrMETy_JECSourcesUp, "/D", "_corrMETy_", "_JECSourcesUp" );
    setMapOutputBranchesWithVectors( outputTree, *_sourcesJEC_Ids, _corrMETy_JECSourcesDown, "/D", "_corrMETy_", "_JECSourcesDown" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _corrMETx_JECGroupedUp, "/D", "_corrMETx_", "_JECGroupedUp" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _corrMETx_JECGroupedDown, "/D", "_corrMETx_", "_JECGroupedDown" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _corrMETy_JECGroupedUp, "/D", "_corrMETy_", "_JECGroupedUp" );
    setMapOutputBranchesWithVectors( outputTree, *_groupedJEC_Ids, _corrMETy_JECGroupedDown, "/D", "_corrMETy_", "_JECGroupedDown" );
}
