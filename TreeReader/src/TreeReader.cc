#include "../interface/TreeReader.h"
#include "../../Event/interface/Event.h"


// constructor //

TreeReader::TreeReader( const std::string& sampleListFile, const std::string& sampleDirectory ){
    readSamples( sampleListFile, sampleDirectory );
}


// functions for sample reading //

void TreeReader::readSamples( const std::string& list, 
			      const std::string& directory, 
			      std::vector<Sample>& sampleVector ){

    //clean current sample list 
    sampleVector.clear();

    //read list of samples from file
    sampleVector = readSampleList(list, directory);

    //print sample information
    for(auto& sample : sampleVector){
        std::cout << "sample: " << sample << std::endl;
    }
}


void TreeReader::readSamples( const std::string& list, const std::string& directory ){
    readSamples( list, directory, this->samples );
}


// functions for initializing maps of branches rather than hard-coded names //

std::pair< std::map< std::string, bool >, std::map< std::string, TBranch* > > buildBranchMap( 
	TTree* treePtr, 
	const std::vector< std::string> nameIdentifiers, 
	const std::string& antiIdentifier = "" ){
    // build a map of variables and of branches from a given tree.
    // only boolean branches supported for now, e.g. triggers and MET filters.
    // all branches whose name contains nameIdentifier and not antiIdentifier will be added.
    // the return type is a pair of two objects:
    // - map of branch name to boolean 
    //   (e.g. usable for triggers)
    //   (values are set to false everywhere)
    // - map of branch name to branch pointer
    //   (e.g. usable for many branches with similar names)
    //   (branch pointers are set to nullptr everywhere)
    // note: use setMapBranchAddresses afterwards to couple branches to variables.
    std::map< std::string, bool > decisionMap;
    std::map< std::string, TBranch* > branchMap;
    // loop over all branches
    TObjArray* branch_list = treePtr->GetListOfBranches();
    for( const auto& branchPtr : *branch_list ){
        std::string branchName = branchPtr->GetName();
	// check if branch needs to be written
	bool select = true;
	for( std::string nameIdentifier: nameIdentifiers ){
	    if( !stringTools::stringContains( branchName, nameIdentifier ) ) select = false;
	}
	if( !select ) continue;
        if( antiIdentifier != "" 
	    && stringTools::stringContains( branchName, antiIdentifier ) ) continue;
	decisionMap[ branchName ] = false;
        branchMap[ branchName ] = nullptr;
    }
    return { decisionMap, branchMap };
}


void TreeReader::initializeTriggerMap( TTree* treePtr ){
    auto triggerMaps = buildBranchMap( treePtr, {"HLT_"} );
    _triggerMap = triggerMaps.first;
    b__triggerMap = triggerMaps.second;
}


void TreeReader::initializeMETFilterMap( TTree* treePtr ){
    auto filterMaps = buildBranchMap( treePtr, {"Flag_"} );
    _METFilterMap = filterMaps.first;
    b__METFilterMap = filterMaps.second;
}


// functions to find if a tree has branches with certain types of info //

bool treeHasBranchWithName( TTree* treePtr, const std::string& nameToFind ){
    TObjArray* branch_list = treePtr->GetListOfBranches();
    for( const auto& branchPtr : *branch_list ){
	std::string branchName = branchPtr->GetName();
	    if( stringTools::stringContains( branchName, nameToFind ) ){
		return true;
	    }
    }
    return false;
}


bool TreeReader::containsGeneratorInfo() const{
    // note: only checking genWeight branch,
    // this does in principle not guarantee that e.g. LHEPdfWeight are present.
    return treeHasBranchWithName( _currentTreePtr, "genWeight");
}


bool TreeReader::containsGenParticles() const{
    return treeHasBranchWithName( _currentTreePtr, "nGenPart" );
}


bool TreeReader::containsTriggerInfo( const std::string& triggerPath ) const{
    return treeHasBranchWithName( _currentTreePtr, triggerPath );
}


bool TreeReader::isData() const{
    if( _currentSamplePtr ) return _currentSamplePtr->isData();
    else return !containsGeneratorInfo();
}


bool TreeReader::isMC() const{
    return !isData();
}


void TreeReader::checkCurrentSample() const{
    if( !_currentSamplePtr ){
        throw std::domain_error( "pointer to current Sample is nullptr." );
    }
}


void TreeReader::checkCurrentTree() const{
    if( !_currentTreePtr ){
        throw std::domain_error( "pointer to current TTree is nullptr." );
    }
}


void TreeReader::checkCurrentFile() const{
    if( !_currentFilePtr ){
	throw std::domain_error( "pointer to current TFile is nullptr." );
    }
}


bool TreeReader::is2016() const{
    checkCurrentSample();
    return _currentSamplePtr->is2016();
}


bool TreeReader::is2016PreVFP() const{
    checkCurrentSample();
    return _currentSamplePtr->is2016PreVFP();
}


bool TreeReader::is2016PostVFP() const{
    checkCurrentSample();
    return _currentSamplePtr->is2016PostVFP();
}


bool TreeReader::is2017() const{
    checkCurrentSample();
    return _currentSamplePtr->is2017();
}


bool TreeReader::is2018() const{
    checkCurrentSample();
    return _currentSamplePtr->is2018();
}


std::string TreeReader::getYearString() const{
    if( is2016PreVFP() ) return "2016PreVFP";
    else if( is2016PostVFP() ) return "2016PostVFP";
    else if( is2016() ) return "2016";
    else if( is2017() ) return "2017";
    else if( is2018() ) return "2018";
    else{
	std::string msg = "ERROR in TreeReader::getYearString:";
	msg += " no valid year string could be returned as all year checks failed.";
	throw std::runtime_error(msg);
    }
}


long unsigned TreeReader::numberOfEntries() const{
    checkCurrentTree();
    return _currentTreePtr->GetEntries();
}


void TreeReader::initSample( const Sample& samp, const bool doInitTree ){ 
    _currentSamplePtr = std::make_shared< Sample >( samp );
    _currentFilePtr = samp.filePtr();
    // note: when initializing multiple samples in series,
    // the currentTreePtr below gets overwritten by the new tree.
    // normally, this implies a memory leak, but in this case it is fine,
    // since ROOT implicitly deletes the previous tree once the file is closed
    // (which is again implicitly done when opening the new file).
    _currentTreePtr = (TTree*) _currentFilePtr->Get( "Events" );
    checkCurrentTree();
    if( doInitTree ) initTree();
    if( !samp.isData() ){
        // get luminosity 
        double lumi;
        if( is2016() ){ lumi = lumi::lumi2016; } 
	else if( is2016PreVFP() ){ lumi = lumi::lumi2016PreVFP; }
	else if( is2016PostVFP() ){ lumi = lumi::lumi2016PostVFP; }
        else if( is2017() ){ lumi = lumi::lumi2017; } 
	else if( is2018() ){ lumi = lumi::lumi2018; }
	else{
	    std::string msg = "ERROR in TreeReader::initSample:";
	    msg += " no valid year could be identified!";
	    throw std::runtime_error(msg);
	}
	// get cross-section
	double xsection = samp.xSec();
	// get sum of gen weights
	TTree* runTreePtr = (TTree*) _currentFilePtr->Get( "Runs" );
	if( !runTreePtr ){
	    std::string msg = "ERROR in TreeReader::initSample:";
	    msg += " tree with sum-of-gen-weight information requested";
	    msg += " but does not seem to exist.";
	}
	double genEventSumOfWeights = 0;
	Double_t genEventSumw = 0;
	runTreePtr->SetBranchAddress("genEventSumw", &genEventSumw);
	for( long int entry=0; entry < runTreePtr->GetEntries(); entry++ ){
	    runTreePtr->GetEntry(entry);
	    genEventSumOfWeights += genEventSumw;
	}
	// calculate the scale
        lumiScale = xsection*lumi*1000 / genEventSumOfWeights;
	// printouts for testing
	std::cout << "lumi: " << lumi << std::endl;
	std::cout << "xsec: " << xsection << std::endl;
	std::cout << "sumweights: " << genEventSumOfWeights << std::endl;
    }
}


// initialize a sample at a given index in the list
void TreeReader::initSample( unsigned int sampleIndex,
			     const bool doInitTree ){
    currentSampleIndex = sampleIndex;
    initSample( samples[ sampleIndex ], doInitTree );
}

// initialize the next sample in the list
void TreeReader::initSample( const bool doInitTree ){
    initSample( samples[ ++currentSampleIndex ], doInitTree );
}


// initialize the current Sample directly from a root file, this is used when skimming
void TreeReader::initSampleFromFile( const std::string& pathToFile, 
				     const bool is2016, 
				     const bool is2016PreVFP,
				     const bool is2016PostVFP,
				     const bool is2017, 
				     const bool is2018, 
				     const bool resetTriggersAndFilters ){

    // check if file exists 
    if( !systemTools::fileExists( pathToFile ) ){
	std::string msg = "ERROR in TreeReader.initSampleFromFile:";
	msg += " file '" + pathToFile + "' does not exist.";
        //throw std::invalid_argument(msg);
	// (suppress this error for now as file might be read from DAS)
    }

    // check year
    if( !(is2016 || is2016PreVFP || is2016PostVFP || is2017 || is2018 ) ){
        std::string msg = "ERROR in TreeReader::initSampleFromFile:";
        msg += " no valid year was given for sample ";
        msg += pathToFile;
        throw std::runtime_error(msg);
    }

    // open file
    std::cout << "INFO in TreeReader::initSampleFromFile: opening " << pathToFile << std::endl;
    _currentFilePtr = std::shared_ptr< TFile >( TFile::Open( pathToFile.c_str(), "READ" ) );

    // set tree pointer
    // note: when initializing multiple samples in series,
    // the currentTreePtr below gets overwritten by the new tree.
    // normally, this implies a memory leak, but in this case it is fine,
    // since ROOT implicitly deletes the previous tree once the file is closed
    // (which is again implicitly done when opening the new file).
    _currentTreePtr = (TTree*) _currentFilePtr->Get( "Events" );
    checkCurrentTree();

    // make a new sample, and make sure the pointer remains valid
    _currentSamplePtr = std::make_shared< Sample >( pathToFile, is2016, is2016PreVFP,
			    is2016PostVFP, is2017, is2018, isData() );

    // initialize tree
    initTree( resetTriggersAndFilters );

    // set scale to default so weights do not become 0 when building the event
    lumiScale = 1.;
}


// automatically determine whether sample is 2017 or 2018 from file name 
void TreeReader::initSampleFromFile( const std::string& pathToFile, 
				     const bool resetTriggersAndFilters ){
    bool is2016 = analysisTools::fileIs2016( pathToFile );
    bool is2016PreVFP = analysisTools::fileIs2016PreVFP( pathToFile );
    bool is2016PostVFP = analysisTools::fileIs2016PostVFP( pathToFile );
    bool is2017 = analysisTools::fileIs2017( pathToFile );
    bool is2018 = analysisTools::fileIs2018( pathToFile );
    initSampleFromFile( pathToFile, is2016, is2016PreVFP, is2016PostVFP, is2017, is2018, 
			resetTriggersAndFilters );
}


void TreeReader::GetEntry( const Sample& samp, long unsigned entry ){
    checkCurrentTree();
    _currentTreePtr->GetEntry( entry );
    // do some checks on array lengths
    if( _nElectron > nElectron_max ){
	std::string msg = "WARNING in TreeReader::GetEntry:";
	msg += " nElectron is " + std::to_string(_nElectron);
	msg += " while nElectron_max is; " + std::to_string(nElectron_max);
	msg += " will ignore objects with index >= " +std::to_string(nElectron_max);
	std::cerr << msg << std::endl;
	_nElectron = nElectron_max;
    }
    if( _nMuon > nMuon_max ){
        std::string msg = "WARNING in TreeReader::GetEntry:";
        msg += " nMuon is " + std::to_string(_nMuon);
        msg += " while nMuon_max is; " + std::to_string(nMuon_max);
        msg += " will ignore objects with index >= " +std::to_string(nMuon_max);
	std::cerr << msg << std::endl;
        _nMuon = nMuon_max;
    }
    if( _nTau > nTau_max ){
        std::string msg = "WARNING in TreeReader::GetEntry:";
        msg += " nTau is " + std::to_string(_nTau);
        msg += " while nTau_max is; " + std::to_string(nTau_max);
        msg += " will ignore objects with index >= " +std::to_string(nTau_max);
	std::cerr << msg << std::endl;
        _nTau = nTau_max;
    }
    if( _nJet > nJet_max ){
        std::string msg = "WARNING in TreeReader::GetEntry:";
        msg += " nJet is " + std::to_string(_nJet);
        msg += " while nJet_max is; " + std::to_string(nJet_max);
        msg += " will ignore objects with index >= " +std::to_string(nJet_max);
	std::cerr << msg << std::endl;
        _nJet = nJet_max;
    }
    if( containsGenParticles() && _nGenPart > nGenPart_max ){
        std::string msg = "WARNING in TreeReader::GetEntry:";
        msg += " nGenPart is " + std::to_string(_nGenPart);
        msg += " while nGenPart_max is; " + std::to_string(nGenPart_max);
        msg += " will ignore objects with index >= " +std::to_string(nGenPart_max);
	std::cerr << msg << std::endl;
        _nGenPart = nGenPart_max;
    }
    // set correctly scaled weight
    if( !samp.isData() ) _scaledWeight = _genWeight*lumiScale;
    else _scaledWeight = 1;
}


// use the currently initialized sample when running in serial
void TreeReader::GetEntry( long unsigned entry ){
    GetEntry( *_currentSamplePtr, entry );
}


Event TreeReader::buildEvent( const Sample& samp, 
	long unsigned entry, 
	const bool readIndividualTriggers, 
	const bool readIndividualMetFilters ){
    GetEntry( samp, entry );
    return Event( *this, readIndividualTriggers, readIndividualMetFilters );
}


Event TreeReader::buildEvent( long unsigned entry, 
	const bool readIndividualTriggers, 
	const bool readIndividualMetFilters){
    GetEntry( entry );
    return Event( *this, readIndividualTriggers, readIndividualMetFilters );
}


template< typename T > void setMapBranchAddresses( TTree* treePtr, 
	std::map< std::string, T >& variableMap, 
	std::map< std::string, TBranch* > branchMap ){
    for( const auto& variable : variableMap ){
        treePtr->SetBranchAddress( variable.first.c_str(), 
	    &variableMap[ variable.first ], 
	    &branchMap[ variable.first ] );
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


void TreeReader::initTree( const bool resetTriggersAndFilters ){

    checkCurrentTree();
    _currentTreePtr->SetMakeClass(1);

    // set branch addresses of all variables
    _currentTreePtr->SetBranchAddress("run", &_run, &b__run);
    _currentTreePtr->SetBranchAddress("luminosityBlock", &_luminosityBlock, &b__luminosityBlock);
    _currentTreePtr->SetBranchAddress("event", &_event, &b__event);
    // generator info
    if( containsGeneratorInfo() ){
	_currentTreePtr->SetBranchAddress("genWeight", &_genWeight, &b__genWeight);
	_currentTreePtr->SetBranchAddress("nLHEPdfWeight", &_nLHEPdfWeight, &b__nLHEPdfWeight);
	_currentTreePtr->SetBranchAddress("LHEPdfWeight", _LHEPdfWeight, &b__LHEPdfWeight);
	_currentTreePtr->SetBranchAddress("nLHEScaleWeight", &_nLHEScaleWeight, &b__nLHEScaleWeight);
	_currentTreePtr->SetBranchAddress("LHEScaleWeight", _LHEScaleWeight, &b__LHEScaleWeight);
	_currentTreePtr->SetBranchAddress("nPSWeight", &_nPSWeight, &b__nPSWeight);
	_currentTreePtr->SetBranchAddress("PSWeight", _PSWeight, &b__PSWeight);
    }
    // gen particles
    if( containsGenParticles() ){
	_currentTreePtr->SetBranchAddress("nGenPart", &_nGenPart, &b__nGenPart);
	_currentTreePtr->SetBranchAddress("GenPart_pt", _GenPart_pt, &b__GenPart_pt);
	_currentTreePtr->SetBranchAddress("GenPart_eta", _GenPart_eta, &b__GenPart_eta);
	_currentTreePtr->SetBranchAddress("GenPart_phi", _GenPart_phi, &b__GenPart_phi);
	_currentTreePtr->SetBranchAddress("GenPart_mass", _GenPart_mass, &b__GenPart_mass);
	_currentTreePtr->SetBranchAddress("GenPart_genPartIdxMother", _GenPart_genPartIdxMother, &b__GenPart_genPartIdxMother);
	_currentTreePtr->SetBranchAddress("GenPart_pdgId", _GenPart_pdgId, &b__GenPart_pdgId);
	_currentTreePtr->SetBranchAddress("GenPart_status", _GenPart_status, &b__GenPart_status);
	_currentTreePtr->SetBranchAddress("GenPart_statusFlags", _GenPart_statusFlags, &b__GenPart_statusFlags);
	_currentTreePtr->SetBranchAddress("GenMET_pt", &_GenMET_pt, &b__GenMET_pt);
	_currentTreePtr->SetBranchAddress("GenMET_phi", &_GenMET_phi, &b__GenMET_phi);
    }
    // variables related to electrons
    _currentTreePtr->SetBranchAddress("nElectron", &_nElectron, &b__nElectron);
    _currentTreePtr->SetBranchAddress("Electron_pt", _Electron_pt, &b__Electron_pt);
    _currentTreePtr->SetBranchAddress("Electron_eta", _Electron_eta, &b__Electron_eta);
    _currentTreePtr->SetBranchAddress("Electron_phi", _Electron_phi, &b__Electron_phi);
    _currentTreePtr->SetBranchAddress("Electron_charge", _Electron_charge, &b__Electron_charge);
    _currentTreePtr->SetBranchAddress("Electron_dxy", _Electron_dxy, &b__Electron_dxy);
    _currentTreePtr->SetBranchAddress("Electron_dz", _Electron_dz, &b__Electron_dz);
    _currentTreePtr->SetBranchAddress("Electron_sip3d", _Electron_sip3d, &b__Electron_sip3d);
    _currentTreePtr->SetBranchAddress("Electron_pfRelIso03_all", _Electron_pfRelIso03_all, &b__Electron_pfRelIso03_all);
    _currentTreePtr->SetBranchAddress("Electron_miniPFRelIso_all", _Electron_miniPFRelIso_all, &b__Electron_miniPFRelIso_all);
    _currentTreePtr->SetBranchAddress("Electron_miniPFRelIso_chg", _Electron_miniPFRelIso_chg, &b__Electron_miniPFRelIso_chg);
    _currentTreePtr->SetBranchAddress("Electron_jetIdx", _Electron_jetIdx, &b__Electron_jetIdx);
    _currentTreePtr->SetBranchAddress("Electron_jetPtRelv2", _Electron_jetPtRelv2, &b__Electron_jetPtRelv2);
    _currentTreePtr->SetBranchAddress("Electron_jetRelIso", _Electron_jetRelIso, &b__Electron_jetRelIso);
    _currentTreePtr->SetBranchAddress("Electron_mvaTTH", _Electron_mvaTTH, &b__Electron_mvaTTH);
    _currentTreePtr->SetBranchAddress("Electron_tightCharge", _Electron_tightCharge, &b__Electron_tightCharge);
    _currentTreePtr->SetBranchAddress("Electron_convVeto", _Electron_convVeto, &b__Electron_convVeto);
    _currentTreePtr->SetBranchAddress("Electron_lostHits", _Electron_lostHits, &b__Electron_lostHits);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2Iso", _Electron_mvaFall17V2Iso, &b__Electron_mvaFall17V2Iso);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso", _Electron_mvaFall17V2noIso, &b__Electron_mvaFall17V2noIso);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WPL", _Electron_mvaFall17V2noIso_WPL, &b__Electron_mvaFall17V2noIso_WPL);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WP80", _Electron_mvaFall17V2noIso_WP80, &b__Electron_mvaFall17V2noIso_WP80);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WP90", _Electron_mvaFall17V2noIso_WP90, &b__Electron_mvaFall17V2noIso_WP90);
    _currentTreePtr->SetBranchAddress("Electron_deltaEtaSC", _Electron_deltaEtaSC, &b__Electron_deltaEtaSC);
    _currentTreePtr->SetBranchAddress("Electron_eInvMinusPInv", _Electron_eInvMinusPInv, &b__Electron_eInvMinusPInv);
    _currentTreePtr->SetBranchAddress("Electron_hoe", _Electron_hoe, &b__Electron_hoe);
    _currentTreePtr->SetBranchAddress("Electron_sieie", _Electron_sieie, &b__Electron_sieie);
    _currentTreePtr->SetBranchAddress("Electron_cutBased", _Electron_cutBased, &b__Electron_cutBased);
    _currentTreePtr->SetBranchAddress("Electron_dEscaleDown", _Electron_dEscaleDown, &b__Electron_dEscaleDown);
    _currentTreePtr->SetBranchAddress("Electron_dEscaleUp", _Electron_dEscaleUp, &b__Electron_dEscaleUp);
    _currentTreePtr->SetBranchAddress("Electron_dEsigmaDown", _Electron_dEsigmaDown, &b__Electron_dEsigmaDown);
    _currentTreePtr->SetBranchAddress("Electron_dEsigmaUp", _Electron_dEsigmaUp, &b__Electron_dEsigmaUp);
    if( containsGenParticles() ){
	_currentTreePtr->SetBranchAddress("Electron_genPartFlav", _Electron_genPartFlav, &b__Electron_genPartFlav);
	_currentTreePtr->SetBranchAddress("Electron_genPartIdx", _Electron_genPartIdx, &b__Electron_genPartIdx);
    }
    _currentTreePtr->SetBranchAddress("Electron_isPFcand", _Electron_isPFCand, &b__Electron_isPFCand);
    _currentTreePtr->SetBranchAddress("Electron_jetNDauCharged", _Electron_jetNDauCharged, &b__Electron_jetNDauCharged);
    // variables related to muons
    _currentTreePtr->SetBranchAddress("nMuon", &_nMuon, &b__nMuon);
    _currentTreePtr->SetBranchAddress("Muon_pt", _Muon_pt, &b__Muon_pt);
    _currentTreePtr->SetBranchAddress("Muon_eta", _Muon_eta, &b__Muon_eta);
    _currentTreePtr->SetBranchAddress("Muon_phi", _Muon_phi, &b__Muon_phi);
    _currentTreePtr->SetBranchAddress("Muon_charge", _Muon_charge, &b__Muon_charge);
    _currentTreePtr->SetBranchAddress("Muon_dxy", _Muon_dxy, &b__Muon_dxy);
    _currentTreePtr->SetBranchAddress("Muon_dz", _Muon_dz, &b__Muon_dz);
    _currentTreePtr->SetBranchAddress("Muon_sip3d", _Muon_sip3d, &b__Muon_sip3d);
    _currentTreePtr->SetBranchAddress("Muon_pfRelIso03_all", _Muon_pfRelIso03_all, &b__Muon_pfRelIso03_all);
    _currentTreePtr->SetBranchAddress("Muon_miniPFRelIso_all", _Muon_miniPFRelIso_all, &b__Muon_miniPFRelIso_all);
    _currentTreePtr->SetBranchAddress("Muon_miniPFRelIso_chg", _Muon_miniPFRelIso_chg, &b__Muon_miniPFRelIso_chg);
    _currentTreePtr->SetBranchAddress("Muon_jetIdx", _Muon_jetIdx, &b__Muon_jetIdx);
    _currentTreePtr->SetBranchAddress("Muon_jetPtRelv2", _Muon_jetPtRelv2, &b__Muon_jetPtRelv2);
    _currentTreePtr->SetBranchAddress("Muon_jetRelIso", _Muon_jetRelIso, &b__Muon_jetRelIso);
    _currentTreePtr->SetBranchAddress("Muon_mvaTTH", _Muon_mvaTTH, &b__Muon_mvaTTH);
    _currentTreePtr->SetBranchAddress("Muon_segmentComp", _Muon_segmentComp, &b__Muon_segmentComp);
    _currentTreePtr->SetBranchAddress("Muon_ptErr", _Muon_ptErr, &b__Muon_ptErr);
    _currentTreePtr->SetBranchAddress("Muon_pfRelIso04_all", _Muon_pfRelIso04_all, &b__Muon_pfRelIso04_all);
    _currentTreePtr->SetBranchAddress("Muon_looseId", _Muon_looseId, &b__Muon_looseId);
    _currentTreePtr->SetBranchAddress("Muon_mediumId", _Muon_mediumId, &b__Muon_mediumId);
    _currentTreePtr->SetBranchAddress("Muon_tightId", _Muon_tightId, &b__Muon_tightId);
    if( containsGenParticles() ){
	_currentTreePtr->SetBranchAddress("Muon_genPartFlav", _Muon_genPartFlav, &b__Muon_genPartFlav);
	_currentTreePtr->SetBranchAddress("Muon_genPartIdx", _Muon_genPartIdx, &b__Muon_genPartIdx);
    }
    _currentTreePtr->SetBranchAddress("Muon_isPFcand", _Muon_isPFCand, &b__Muon_isPFCand);
    _currentTreePtr->SetBranchAddress("Muon_isGlobal", _Muon_isGlobal, &b__Muon_isGlobal);
    _currentTreePtr->SetBranchAddress("Muon_isTracker", _Muon_isTracker, &b__Muon_isTracker);
    _currentTreePtr->SetBranchAddress("Muon_isStandalone", _Muon_isStandalone, &b__Muon_isStandalone);
    _currentTreePtr->SetBranchAddress("Muon_jetNDauCharged", _Muon_jetNDauCharged, &b__Muon_jetNDauCharged);
    // variables related to taus
    _currentTreePtr->SetBranchAddress("nTau", &_nTau, &b__nTau);
    _currentTreePtr->SetBranchAddress("Tau_pt", _Tau_pt, &b__Tau_pt);
    _currentTreePtr->SetBranchAddress("Tau_eta", _Tau_eta, &b__Tau_eta);
    _currentTreePtr->SetBranchAddress("Tau_phi", _Tau_phi, &b__Tau_phi);
    _currentTreePtr->SetBranchAddress("Tau_charge", _Tau_charge, &b__Tau_charge);
    _currentTreePtr->SetBranchAddress("Tau_dxy", _Tau_dxy, &b__Tau_dxy);
    _currentTreePtr->SetBranchAddress("Tau_dz", _Tau_dz, &b__Tau_dz);
    if( containsGenParticles() ){
	_currentTreePtr->SetBranchAddress("Tau_genPartFlav", _Tau_genPartFlav, &b__Tau_genPartFlav);
	_currentTreePtr->SetBranchAddress("Tau_genPartIdx", _Tau_genPartIdx, &b__Tau_genPartIdx);
    }
    // variables related to jets
    _currentTreePtr->SetBranchAddress("nJet", &_nJet, &b__nJet);
    _currentTreePtr->SetBranchAddress("Jet_pt", _Jet_pt, &b__Jet_pt);
    _currentTreePtr->SetBranchAddress("Jet_eta", _Jet_eta, &b__Jet_eta);
    _currentTreePtr->SetBranchAddress("Jet_phi", _Jet_phi, &b__Jet_phi);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepB", _Jet_bTagDeepB, &b__Jet_bTagDeepB);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepFlavB", _Jet_bTagDeepFlavB, &b__Jet_bTagDeepFlavB);
    _currentTreePtr->SetBranchAddress("Jet_nConstituents", _Jet_nConstituents, &b__Jet_nConstituents);
    if( containsGeneratorInfo() ){
	_currentTreePtr->SetBranchAddress("Jet_hadronFlavour", _Jet_hadronFlavor, &b__Jet_hadronFlavor);
	// this branch is not present in data!
    }
    _currentTreePtr->SetBranchAddress("Jet_jetId", _Jet_jetId, &b__Jet_jetId);
    // variables related to missing transverse energy
    _currentTreePtr->SetBranchAddress("MET_pt", &_MET_pt, &b__MET_pt);
    _currentTreePtr->SetBranchAddress("MET_phi", &_MET_phi, &b__MET_phi);

    // add all individually stored triggers 
    if( resetTriggersAndFilters || _triggerMap.empty() ){
        initializeTriggerMap( _currentTreePtr );
    }
    setMapBranchAddresses( _currentTreePtr, _triggerMap, b__triggerMap );

    //add all individually stored MET filters
    if( resetTriggersAndFilters || _METFilterMap.empty() ){
        initializeMETFilterMap( _currentTreePtr );
    }
    setMapBranchAddresses( _currentTreePtr, _METFilterMap, b__METFilterMap );
}


void TreeReader::setOutputTree( TTree* outputTree,
				bool includeGeneratorInfo,
				bool includeGenParticles ){
    outputTree->Branch("run", &_run, "_run/i");
    outputTree->Branch("luminosityBlock", &_luminosityBlock, "_luminosityBlock/i");
    outputTree->Branch("event", &_event, "_event/i");
    // generator info
    if( includeGeneratorInfo ){
	if( !containsGeneratorInfo() ){
            std::string msg = "WARNING in TreeReader.setOutputTree:";
            msg.append(" requested to include generator info in output tree,");
            msg.append(" but there appear to be no generator info branches in the input tree;");
            msg.append(" will skip writing generator info to output tree!");
            std::cerr << msg << std::endl;
        }
	else{
	outputTree->Branch("genWeight", &_genWeight, "_genWeight/F");
	outputTree->Branch("nLHEPdfWeight", &_nLHEPdfWeight, "_nLHEPdfWeight/i");
	outputTree->Branch("LHEPdfWeight", &_LHEPdfWeight, "_LHEPdfWeight[_nLHEPdfWeight]/F");
	outputTree->Branch("nLHEScaleWeight", &_nLHEScaleWeight, "_nLHEScaleWeight/i");
	outputTree->Branch("LHEScaleWeight", &_LHEScaleWeight, "_LHEScaleWeight[_nLHEScaleWeight]/F");
	outputTree->Branch("nPSWeight", &_nPSWeight, "_nPSWeight/i");
	outputTree->Branch("PSWeight", &_PSWeight, "_PSWeight[_nPSWeight]/F");
	}
    }
    // gen particles
    if( includeGenParticles ){
	if( !containsGenParticles() ){
            std::string msg = "WARNING in TreeReader.setOutputTree:";
            msg.append(" requested to include gen particles in output tree,");
            msg.append(" but there appear to be no gen particle branches in the input tree;");
            msg.append(" will skip writing gen particles to output tree!");
            std::cerr << msg << std::endl;
        }
	else{
	outputTree->Branch("nGenPart", &_nGenPart, "_nGenPart/i");
	outputTree->Branch("GenPart_pt", &_GenPart_pt, "_GenPart_pt[_nGenPart]/F");
	outputTree->Branch("GenPart_eta", &_GenPart_eta, "_GenPart_eta[_nGenPart]/F");
	outputTree->Branch("GenPart_phi", &_GenPart_phi, "_GenPart_phi[_nGenPart]/F");
	outputTree->Branch("GenPart_mass", &_GenPart_mass, "_GenPart_mass[_nGenPart]/F");
	outputTree->Branch("GenPart_genPartIdxMother", &_GenPart_genPartIdxMother, "_GenPart_genPartIdxMother[_nGenPart]/I");
	outputTree->Branch("GenPart_pdgId", &_GenPart_pdgId, "_GenPart_pdgId[_nGenPart]/I");
	outputTree->Branch("GenPart_status", &_GenPart_status, "_GenPart_status[_nGenPart]/i");
	outputTree->Branch("GenPart_statusFlags", &_GenPart_statusFlags, "_GenPart_statusFlags[_nGenPart]/i");
	// gen MET
	outputTree->Branch("GenMET_pt", &_GenMET_pt, "_GenMET_pt/F");
	outputTree->Branch("GenMET_phi", &_GenMET_phi, "_GenMET_phi/F");
	}
    }
    // variables related to electrons
    outputTree->Branch("nElectron", &_nElectron, "_nElectron/i");
    outputTree->Branch("Electron_pt", &_Electron_pt, "_Electron_pt[_nElectron]/F");
    outputTree->Branch("Electron_eta", &_Electron_eta, "_Electron_eta[_nElectron]/F");
    outputTree->Branch("Electron_phi", &_Electron_phi, "_Electron_phi[_nElectron]/F");
    outputTree->Branch("Electron_charge", &_Electron_charge, "_Electron_charge[_nElectron]/I");
    outputTree->Branch("Electron_dxy", &_Electron_dxy, "_Electron_dxy[_nElectron]/F");
    outputTree->Branch("Electron_dz", &_Electron_dz, "_Electron_dz[_nElectron]/F");
    outputTree->Branch("Electron_sip3d", &_Electron_sip3d, "_Electron_sip3d[_nElectron]/F");
    outputTree->Branch("Electron_pfRelIso03_all", &_Electron_pfRelIso03_all, "_Electron_pfRelIso03_all[_nElectron]/F");
    outputTree->Branch("Electron_miniPFRelIso_all", &_Electron_miniPFRelIso_all, "_Electron_miniPFRelIso_all[_nElectron]/F");
    outputTree->Branch("Electron_miniPFRelIso_chg", &_Electron_miniPFRelIso_chg, "_Electron_miniPFRelIso_chg[_nElectron]/F");
    outputTree->Branch("Electron_jetIdx", &_Electron_jetIdx, "_Electron_jetIdx[_nElectron]/I");
    outputTree->Branch("Electron_jetPtRelv2", &_Electron_jetPtRelv2, "_Electron_jetPtRelv2[_nElectron]/F");
    outputTree->Branch("Electron_jetRelIso", &_Electron_jetRelIso, "_Electron_jetRelIso[_nElectron]/F");
    outputTree->Branch("Electron_mvaTTH", &_Electron_mvaTTH, "_Electron_mvaTTH[_nElectron]/F");
    outputTree->Branch("Electron_tightCharge", &_Electron_tightCharge, "_Electron_tightCharge[_nElectron]/I");
    outputTree->Branch("Electron_convVeto", &_Electron_convVeto, "_Electron_convVeto[_nElectron]/B");
    outputTree->Branch("Electron_lostHits", &_Electron_lostHits, "_Electron_lostHits[_nElectron]/b");
    outputTree->Branch("Electron_mvaFall17V2Iso", &_Electron_mvaFall17V2Iso, "_Electron_mvaFall17V2Iso[_nElectron]/F");
    outputTree->Branch("Electron_mvaFall17V2noIso", &_Electron_mvaFall17V2noIso, "_Electron_mvaFall17V2noIso[_nElectron]/F");
    outputTree->Branch("Electron_mvaFall17V2noIso_WPL", &_Electron_mvaFall17V2noIso_WPL, "_Electron_mvaFall17V2noIso_WPL[_nElectron]/O");
    outputTree->Branch("Electron_mvaFall17V2noIso_WP80", &_Electron_mvaFall17V2noIso_WP80, "_Electron_mvaFall17V2noIso_WP80[_nElectron]/O");
    outputTree->Branch("Electron_mvaFall17V2noIso_WP90", &_Electron_mvaFall17V2noIso_WP90, "_Electron_mvaFall17V2noIso_WP90[_nElectron]/O");
    outputTree->Branch("Electron_deltaEtaSC", &_Electron_deltaEtaSC, "_Electron_deltaEtaSC[_nElectron]/F");
    outputTree->Branch("Electron_eInvMinusPInv", &_Electron_eInvMinusPInv, "_Electron_eInvMinusPInv[_nElectron]/F");
    outputTree->Branch("Electron_hoe", &_Electron_hoe, "_Electron_hoe[_nElectron]/F");
    outputTree->Branch("Electron_sieie", &_Electron_sieie, "_Electron_sieie[_nElectron]/F");
    outputTree->Branch("Electron_cutBased", &_Electron_cutBased, "_Electron_cutBased[_nElectron]/I");
    outputTree->Branch("Electron_dEscaleDown", &_Electron_dEscaleDown, "_Electron_dEscaleDown[_nElectron]/F");
    outputTree->Branch("Electron_dEscaleUp", &_Electron_dEscaleUp, "_Electron_dEscaleUp[_nElectron]/F");
    outputTree->Branch("Electron_dEsigmaDown", &_Electron_dEsigmaDown, "_Electron_dEsigmaDown[_nElectron]/F");
    outputTree->Branch("Electron_dEsigmaUp", &_Electron_dEsigmaUp, "_Electron_dEsigmaUp[_nElectron]/F");
    if( includeGenParticles && containsGenParticles() ){
	outputTree->Branch("Electron_genPartFlav", &_Electron_genPartFlav, "_Electron_genPartFlav[_nElectron]/b");
	outputTree->Branch("Electron_genPartIdx", &_Electron_genPartIdx, "_Electron_genPartIdx[_nElectron]/I");
    }
    outputTree->Branch("Electron_isPFcand", &_Electron_isPFCand, "_Electron_isPFCand[_nElectron]/O");
    // variables related to muons
    outputTree->Branch("nMuon", &_nMuon, "_nMuon/i");
    outputTree->Branch("Muon_pt", &_Muon_pt, "_Muon_pt[_nMuon]/F");
    outputTree->Branch("Muon_eta", &_Muon_eta, "_Muon_eta[_nMuon]/F");
    outputTree->Branch("Muon_phi", &_Muon_phi, "_Muon_phi[_nMuon]/F");
    outputTree->Branch("Muon_charge", &_Muon_charge, "_Muon_charge[_nMuon]/I");
    outputTree->Branch("Muon_dxy", &_Muon_dxy, "_Muon_dxy[_nMuon]/F");
    outputTree->Branch("Muon_dz", &_Muon_dz, "_Muon_dz[_nMuon]/F");
    outputTree->Branch("Muon_sip3d", &_Muon_sip3d, "_Muon_sip3d[_nMuon]/F");
    outputTree->Branch("Muon_pfRelIso03_all", &_Muon_pfRelIso03_all, "_Muon_pfRelIso03_all[_nMuon]/F");
    outputTree->Branch("Muon_miniPFRelIso_all", &_Muon_miniPFRelIso_all, "_Muon_miniPFRelIso_all[_nMuon]/F");
    outputTree->Branch("Muon_miniPFRelIso_chg", &_Muon_miniPFRelIso_chg, "_Muon_miniPFRelIso_chg[_nMuon]/F");
    outputTree->Branch("Muon_jetIdx", &_Muon_jetIdx, "_Muon_jetIdx[_nMuon]/I");
    outputTree->Branch("Muon_jetPtRelv2", &_Muon_jetPtRelv2, "_Muon_jetPtRelv2[_nMuon]/F");
    outputTree->Branch("Muon_jetRelIso", &_Muon_jetRelIso, "_Muon_jetRelIso[_nMuon]/F");
    outputTree->Branch("Muon_mvaTTH", &_Muon_mvaTTH, "_Muon_mvaTTH[_nMuon]/F");
    outputTree->Branch("Muon_segmentComp", &_Muon_segmentComp, "_Muon_segmentComp[_nMuon]/F");
    outputTree->Branch("Muon_ptErr", &_Muon_ptErr, "_Muon_ptErr[_nMuon]/F");
    outputTree->Branch("Muon_pfRelIso04_all", &_Muon_pfRelIso04_all, "_Muon_pfRelIso04_all[_nMuon]/F");
    outputTree->Branch("Muon_looseId", &_Muon_looseId, "_Muon_looseId[_nMuon]/O");
    outputTree->Branch("Muon_mediumId", &_Muon_mediumId, "_Muon_mediumId[_nMuon]/O");
    outputTree->Branch("Muon_tightId", &_Muon_tightId, "_Muon_tightId[_nMuon]/O");
    if( includeGenParticles && containsGenParticles() ){
	outputTree->Branch("Muon_genPartFlav", &_Muon_genPartFlav, "_Muon_genPartFlav[_nMuon]/b");
	outputTree->Branch("Muon_genPartIdx", &_Muon_genPartIdx, "_Muon_genPartIdx[_nMuon]/I");
    }
    outputTree->Branch("Muon_isPFcand", &_Muon_isPFCand, "_Muon_isPFCand[_nMuon]/O");
    outputTree->Branch("Muon_isGlobal", &_Muon_isGlobal, "_Muon_isGlobal[_nMuon]/O");
    outputTree->Branch("Muon_isTracker", &_Muon_isTracker, "_Muon_isTracker[_nMuon]/O");
    outputTree->Branch("Muon_isStandalone", &_Muon_isStandalone, "_Muon_isStandalone[_nMuon]/O");
    // variables related to taus
    outputTree->Branch("nTau", &_nTau, "_nTau/i");
    outputTree->Branch("Tau_pt", &_Tau_pt, "_Tau_pt[_nTau]/F");
    outputTree->Branch("Tau_eta", &_Tau_eta, "_Tau_eta[_nTau]/F");
    outputTree->Branch("Tau_phi", &_Tau_phi, "_Tau_phi[_nTau]/F");
    outputTree->Branch("Tau_charge", &_Tau_charge, "_Tau_charge[_nTau]/I");
    outputTree->Branch("Tau_dxy", &_Tau_dxy, "_Tau_dxy[_nTau]/F");
    outputTree->Branch("Tau_dz", &_Tau_dz, "_Tau_dz[_nTau]/F");
    if( includeGenParticles && containsGenParticles() ){
	outputTree->Branch("Tau_genPartFlav", &_Tau_genPartFlav, "_Tau_genPartFlav[_nTau]/b");
	outputTree->Branch("Tau_genPartIdx", &_Tau_genPartIdx, "_Tau_genPartIdx[_nTau]/I");
    }
    // variables related to jets
    outputTree->Branch("nJet", &_nJet, "_nJet/i");
    outputTree->Branch("Jet_pt", &_Jet_pt, "_Jet_pt[_nJet]/F");
    outputTree->Branch("Jet_eta", &_Jet_eta, "_Jet_eta[_nJet]/F");
    outputTree->Branch("Jet_phi", &_Jet_phi, "_Jet_phi[_nJet]/F");
    outputTree->Branch("Jet_btagDeepB", &_Jet_bTagDeepB, "_Jet_bTagDeepB[_nJet]/F");
    outputTree->Branch("Jet_btagDeepFlavB", &_Jet_bTagDeepFlavB, "_Jet_bTagDeepFlavB[_nJet]/F");
    outputTree->Branch("Jet_nConstituents", &_Jet_nConstituents, "_Jet_nConstituents[_nJet]/b");
    if( includeGeneratorInfo && containsGeneratorInfo() ){
	outputTree->Branch("Jet_hadronFlavour", &_Jet_hadronFlavor, "_Jet_hadronFlavor[_nJet]/I");
    }
    outputTree->Branch("Jet_jetId", &_Jet_jetId, "_Jet_jetId[_nJet]/I");
    // variables related to missing transverse energy
    outputTree->Branch("MET_pt", &_MET_pt, "_MET_pt/F");
    outputTree->Branch("MET_phi", &_MET_phi, "_MET_phi/F");
    // write individual trigger decisions to output tree
    setMapOutputBranches( outputTree, _triggerMap, "/O" );
    // write individual MET filters to output tree
    setMapOutputBranches( outputTree, _METFilterMap, "/O" );
}


// get object from current file 
TObject* TreeReader::getFromCurrentFile( const std::string& name ) const{
    checkCurrentFile();
    return _currentFilePtr->Get( name.c_str() );
}


// get list of histograms stored in current file
std::vector< std::shared_ptr< TH1 > > TreeReader::getHistogramsFromCurrentFile() const{
    checkCurrentFile();
    // vector containing all histograms in current file
    std::vector< std::shared_ptr< TH1 > > histogramVector;
    // loop over keys in top directory
    TDirectory* dir = (TDirectory*) _currentFilePtr->Get("");
    TList* keyList = dir->GetListOfKeys();
    for( const auto objectPtr : *keyList ){
	//try if a dynamic_cast to a histogram works to check if object is histogram
	TH1* histPtr = dynamic_cast< TH1* >( dir->Get( objectPtr->GetName() ) );
	if( histPtr ){
            histPtr->SetDirectory( gROOT );
	    histogramVector.emplace_back( histPtr );
	}
    }
    return histogramVector;
}

// lepton MVA reader getters, checkers and setters

bool TreeReader::hasLeptonMVAReader(const std::string& mvaID) const{
    // check if a lepton MVA reader with ID mvaID is available.
    // returns true (if available) or false (otherwise).
    if( _leptonMVAReaderMap.count(mvaID)==0 ) return false;
    return true;
}

void TreeReader::checkLeptonMVAReader(const std::string& mvaID) const{
    // same as hasLeptonMVAReader but throw an error if not available.
    if( !hasLeptonMVAReader(mvaID) ){
	std::string msg = "ERROR in TreeReader::checkLeptonMVAReader:";
	msg.append(" a lepton MVA reader with ID " + mvaID + " was requested but not found.");
	throw std::runtime_error(msg);
    }
}

LeptonMVAReader* TreeReader::leptonMVAReader(const std::string& mvaID) const{
    // returns a lepton MVA reader instance
    // (throws an error if not available)
    checkLeptonMVAReader(mvaID);
    return _leptonMVAReaderMap.at(mvaID).get();
}

std::vector<std::string> TreeReader::leptonMVAReaderIDs() const{
    // return all keys in the lepton MVA reader map
    std::vector<std::string> ids;
    for( const auto& el : _leptonMVAReaderMap ){
	ids.push_back(el.first);
    }
    return ids;
}

void TreeReader::initializeLeptonMVAReader(const std::string& mvaID){
    // initialize a lepton MVA reader with given ID
    if( hasLeptonMVAReader(mvaID) ){
	std::string msg = "ERROR in TreeReader::initializeLeptonMVAReader:";
        msg.append(" a lepton MVA reader with ID " + mvaID + " was requested but already present.");
        throw std::runtime_error(msg);
    }
    _leptonMVAReaderMap[mvaID] = std::make_shared<LeptonMVAReader>(mvaID, getYearString());
}
