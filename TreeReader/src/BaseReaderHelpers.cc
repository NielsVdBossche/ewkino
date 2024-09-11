#include "../interface/BaseReader.h"

#include "../../constants/luminosities.h"
#include "../../Tools/interface/stringTools.h"

// Implementation of many small functions in BaseReader that act as helpers or error checks.
// Small helper functions

//get object from current file 
TObject* BaseReader::getFromCurrentFile( const std::string& name ) const{
    checkCurrentFile();
    return _currentFilePtr->Get( name.c_str() );
}

//Get list of histograms stored in current file
std::vector< std::shared_ptr< TH1 > > BaseReader::getHistogramsFromCurrentFile() const{
    checkCurrentFile();
    //vector containing all histograms in current file
    std::vector< std::shared_ptr< TH1 > > histogramVector;

    TDirectory* dir = (TDirectory*) _currentFilePtr->Get("blackJackAndHookers");
    if (! dir) {
        dir = _currentFilePtr.get();
    }
    // old comment from Willem:
    // "this is not a memory leak since this object will implicitly be deleted 
    // when 'dir' gets deleted (DIRTY ROOT!!!)"
    TList* keyList = dir->GetListOfKeys();
    for( const auto objectPtr : *keyList ){
        //try if a dynamic_cast to a histogram works to check if object is histogram
        TH1* histPtr = dynamic_cast< TH1* >( dir->Get( objectPtr->GetName() ) );
        if (histPtr) {
                //make sure histograms don't get deleted by root upon deletion of TDirectory above
                histPtr->SetDirectory( gROOT );
            histogramVector.emplace_back( histPtr );
	    }
    }
    return histogramVector;
}


bool BaseReader::isData() const{
    if( _currentSamplePtr ) return _currentSamplePtr->isData();
    else return ! containsGeneratorInfo();
}

bool BaseReader::isMC() const{
    return !isData();
}

bool BaseReader::is2016PreVFP() const{
    checkCurrentSample();
    return _currentSamplePtr->is2016PreVFP();
}

bool BaseReader::is2016PostVFP() const{
    checkCurrentSample();
    return _currentSamplePtr->is2016PostVFP();
}

bool BaseReader::is2016() const{
    checkCurrentSample();
    return _currentSamplePtr->is2016PreVFP() || _currentSamplePtr->is2016PostVFP() || _currentSamplePtr->is2016();
}

bool BaseReader::is2017() const{
    checkCurrentSample();
    return _currentSamplePtr->is2017();
}

bool BaseReader::is2018() const{
    checkCurrentSample();
    return _currentSamplePtr->is2018();
}

bool BaseReader::isSMSignal() const{
    checkCurrentSample();
    return _currentSamplePtr->isSMSignal();
}

bool BaseReader::isNewPhysicsSignal() const{
    checkCurrentSample();
    return _currentSamplePtr->isNewPhysicsSignal();
}

long unsigned BaseReader::numberOfEntries() const{
    checkCurrentTree();
    return _currentTreePtr->GetEntries();
}

double BaseReader::getIntLumi() const {
    bool y2016Pre = false;
    bool y2016Post = false;
    bool y2017 = false;
    bool y2018 = false;
    for (auto& samp : samples) {
        if (! samp.isMC()) continue;
        if (samp.is2016PostVFP()) {
            y2016Post = true;
        } else if (samp.is2017()) {
            y2017 = true;
        } else if (samp.is2018()) {
            y2018 = true;
        } else {
            y2016Pre = true;
        }
    }
    return y2016Pre * lumi::lumi2016PreVFP + y2016Post * lumi::lumi2016PostVFP + y2017 * lumi::lumi2017 + y2018 * lumi::lumi2018;
}

void BaseReader::removeBSMSignalSamples(){
    // remove some samples from the SampleVector of this TreeReader
    // WARNING: the SampleVector is modified in-place.
    // WARNING: might be analysis-specific (for ewkino), check isNewPhysicsSignal()
    for (auto it = samples.begin(); it != samples.end();) {
        if (it->isNewPhysicsSignal()) {
            it = samples.erase( it );
        } else ++it;
    }
}

void BaseReader::keepOnlySignalsWithName( const std::string& signalName ){
    // remove some samples from the SampleVector of this TreeReader
    // WARNING: the SampleVector is modified in-place.
    // WARNING: might be analysis-specific (for ewkino), check isNewPhysicsSignal()
    for (auto it = samples.begin(); it != samples.end();) {
        if (it->isNewPhysicsSignal() && it->processName() != signalName) {
            it = samples.erase( it );
        } else ++it;
    }
}


// Error checks:
void BaseReader::checkCurrentSample() const{
    if( !_currentSamplePtr ){
        throw std::domain_error( "pointer to current Sample is nullptr." );
    }
}

void BaseReader::checkCurrentTree() const{
    if( !_currentTreePtr ){
        throw std::domain_error( "pointer to current TTree is nullptr." );
    }
}

void BaseReader::checkCurrentFile() const{
    if( !_currentFilePtr ){
        throw std::domain_error( "pointer to current TFile is nullptr." );
    }
}

void BaseReader::checkSampleEraConsistency() const{
    for(auto& sample : samples2016PreVFP){
        if( sample.is2017() || sample.is2016PostVFP()){
            std::cerr << "Error: 2017 or 2016 postvfp sample detected in list of 2016 samples, this will lead to inconsistent lumi-scaling and cuts being applied!" << std::endl;
        }
    }
    for(auto& sample : samples2016PostVFP){
        if( sample.is2017() || sample.is2016PreVFP()){
            std::cerr << "Error: 2017 or 2016 prevfp sample detected in list of 2016 samples, this will lead to inconsistent lumi-scaling and cuts being applied!" << std::endl;
        }
    }
    for(auto& sample : samples2017){
        if( sample.is2016PostVFP() || sample.is2016PreVFP() ){
            std::cerr << "Error: 2016 sample detected in list of 2017 samples, this will lead to inconsistent lumi-scaling and cuts being applied!" << std::endl;
        }
    }
    for(auto& sample : samples2018){
        if( sample.is2016PostVFP() || sample.is2016PreVFP() || sample.is2017() ){
            std::cerr << "Error: 2016 or 2017 sample detected in list of 2018 samples, this will lead to inconsistent lumi-scaling and cuts being applied!" << std::endl;
        }
    }
}

void BaseReader::checkEraOrthogonality() const{
    bool bothTrue = is2017() && is2016();
    if(bothTrue){
        std::cerr << "Error: both is2016 and is2017 flags are returning TRUE for current sample! Sample has to be either one." << std::endl;
    }
    bool bothFalse = !( is2017() || is2016() );
    if(bothFalse){
        std::cerr << "Error: both is2016 and is2017 flags are returning FALSE for current sample! Sample has to be either one." << std::endl;
    }
}

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

template <typename T>
void setMapBranchAddresses(TTree* treePtr,
                           std::map<std::string, T>& variableMap,
                           std::map<std::string, TBranch*> branchMap) {
    for (const auto& variable : variableMap) {
        treePtr->SetBranchAddress(variable.first.c_str(), &variableMap[variable.first], &branchMap[variable.first]);
    }
}

template <typename T>
void setMapBranchAddressesWithNameMap(TTree* treePtr,
                           std::map<std::string, T>& variableMap,
                           std::map<std::string, TBranch*> branchMap,
                           std::map<std::string, std::string> nameMap) {
    for (const auto& variable : nameMap) {
        treePtr->SetBranchAddress(variable.first.c_str(), &variableMap[variable.second], &branchMap[variable.first]);
    }
}