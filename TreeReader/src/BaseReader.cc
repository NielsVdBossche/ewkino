#include "../interface/BaseReader.h"

#include "../../Tools/interface/stringTools.h"
#include "../../Tools/interface/systemTools.h"
#include "../../Tools/interface/analysisTools.h"
#include "../../constants/luminosities.h"

BaseReader::BaseReader(const std::string& samplelistfile, const std::string& sampledirectory) {
    readSamples(samplelistfile, sampledirectory);
}


// Samplelist reading:
void BaseReader::readSamples(const std::string& list, const std::string& directory)
{
    readSamples( list, directory, this->samples );
}

void BaseReader::readSamples(const std::string& list,
                             const std::string& directory,
                             std::vector<Sample>& sampleVector)
{
    //clean current sample list 
    sampleVector.clear();

    //read list of samples from file
    sampleVector = readSampleList(list, directory);

    //print sample information
    for(auto& sample : sampleVector){
        std::cout << "sample: " << sample << std::endl;
    }
}

void BaseReader::readSamples2016PreVFP( const std::string& list, const std::string& directory ){
    std::cout << "########################################" << std::endl;
    std::cout << "         2016 PreVFP samples            " << std::endl;
    std::cout << "########################################" << std::endl;

    readSamples( list, directory, this->samples2016PreVFP );
    //add the 2016 samples to the total sample list 
    this->samples.insert( samples.end(), samples2016PreVFP.begin(), samples2016PreVFP.end() );
    //check for errors
    checkSampleEraConsistency();
}

void BaseReader::readSamples2016PostVFP( const std::string& list, const std::string& directory ){
    std::cout << "########################################" << std::endl;
    std::cout << "         2016 PostVFP samples           " << std::endl;
    std::cout << "########################################" << std::endl;

    readSamples( list, directory, this->samples2016PostVFP );
    //add the 2016 samples to the total sample list 
    this->samples.insert( samples.end(), samples2016PostVFP.begin(), samples2016PostVFP.end() );
    //check for errors
    checkSampleEraConsistency();
}

void BaseReader::readSamples2016( const std::string& list, const std::string& directory ){
    // note: not yet updated to UL files, use with caution
    std::cout << "########################################" << std::endl;
    std::cout << "         2016 samples                   " << std::endl;
    std::cout << "########################################" << std::endl;

    readSamples( list, directory, this->samples2016 );
    //add the 2016 samples to the total sample list 
    this->samples.insert( samples.end(), samples2016.begin(), samples2016.end() );
    //check for errors
    checkSampleEraConsistency();
}


void BaseReader::readSamples2017( const std::string& list, const std::string& directory ){
    // note: not yet updated to UL files, use with caution
    std::cout << "########################################" << std::endl;
    std::cout << "         2017 samples                   " << std::endl;
    std::cout << "########################################" << std::endl;

    readSamples( list, directory, this->samples2017 );
    //add the 2017 samples to the total sample list
    this->samples.insert( samples.end(), samples2017.begin(), samples2017.end() );
    //check for errors 
    checkSampleEraConsistency();
}


void BaseReader::readSamples2018( const std::string& list, const std::string& directory ){
    // note: not yet updated to UL files, use with caution
    std::cout << "########################################" << std::endl;
    std::cout << "         2018 samples                   " << std::endl;
    std::cout << "########################################" << std::endl;

    readSamples( list, directory, this->samples2018 );
    //add the 2018 samples to the total sample list
    this->samples.insert( samples.end(), samples2018.begin(), samples2018.end() );
    //check for errors 
    checkSampleEraConsistency();
}

//initialize the next sample in the list
void BaseReader::initSample(){
    initSample( samples[ ++currentSampleIndex ] );
}

void BaseReader::initSample( const Sample& samp ){ 

    //update current sample
    // old comment from Willem:
    // "I wonder if the extra copy can be avoided here, 
    // its however hard if we want to keep the functionality of reading the sample vector, 
    // and also having the function initSampleFromFile. 
    // It's not clear how we can make a new sample in one of them 
    // and refer to an existing one in the other. 
    // It can be done with a static Sample in 'initSampleFromFile', 
    // but this makes the entire TreeReader class unthreadsafe, 
    // so no parallel sample processing in one process can be done"
    _currentSamplePtr = std::make_shared< Sample >( samp );
    _currentFilePtr = samp.filePtr();

    // old comment from Willem:
    // "Warning: this pointer is overwritten, but it is not a memory leak. 
    // ROOT is dirty and deletes the previous tree upon closure of the TFile it belongs to.
    // The previous TFile is closed by the std::shared_ptr destructor, 
    // implicitly called above when opening a new TFile."
    _currentTreePtr = getTreePtr();
    checkCurrentTree();
    initTree();
    if( !samp.isData() ){
        //read sum of simulated event weights
        double sumSimulatedEventWeights = getSumSimulatedEventWeights();

        //event weights set with lumi depending on sample's era 
        double dataLumi;
        if( is2016() && !is2016PreVFP() && !is2016PostVFP()){ dataLumi = lumi::lumi2016; } 
        if( is2016PreVFP() ){ dataLumi = lumi::lumi2016PreVFP; }
        else if( is2016PostVFP() ){ dataLumi = lumi::lumi2016PostVFP; }
        else if( is2017() ){ dataLumi = lumi::lumi2017; } 
        else { dataLumi = lumi::lumi2018; }
        std::cout << "lumi determined to be: " << dataLumi << " with sum of sim weights " << sumSimulatedEventWeights << std::endl;
        scale = samp.xSec() * dataLumi * 1000 / sumSimulatedEventWeights;
    }
}

//initialize the current Sample directly from a root file, this is used when skimming
void BaseReader::initSampleFromFile(const std::string& pathToFile, 
                                    const bool is2016, 
                                    const bool is2016PreVFP,
                                    const bool is2016PostVFP,
                                    const bool is2017, 
                                    const bool is2018, 
                                    const bool resetTriggersAndFilters)
{
    // check if file exists 
    if( !systemTools::fileExists( pathToFile ) ){
        throw std::invalid_argument( "File '" + pathToFile + "' does not exist." );
    }
    _currentFilePtr = std::make_shared< TFile >( pathToFile.c_str(), "read");

    // check year
    if( !(is2016 || is2016PreVFP || is2016PostVFP || is2017 || is2018 ) ){
        std::string msg = "ERROR in TreeReader::initSampleFromFile:";
        msg += " no valid year was given for sample ";
        msg += pathToFile;
        throw std::runtime_error(msg);
    }

    // old comment from Willem:
    // "Warning: this pointer is overwritten, but it is not a memory leak. 
    // ROOT is dirty and deletes the previous tree upon closure of the TFile it belongs to.
    // The previous TFile is closed by the std::shared_ptr destructor, 
    // implicitly called above when opening a new TFile."
    _currentTreePtr = getTreePtr();
    checkCurrentTree();
    // make a new sample, and make sure the pointer remains valid
    // old comment from Willem:
    // "new is no option here since this would also require a destructor for the class, 
    // which does not work for the other initSample case"
    _currentSamplePtr = std::make_shared< Sample >( pathToFile, is2016, is2016PreVFP,
                                                    is2016PostVFP, is2017, is2018, isData() );

    //initialize tree
    initTree( resetTriggersAndFilters );

    //set scale so weights don't become 0 when building the event
    scale = 1.;
}


//automatically determine whether sample is 2017 or 2018 from file name 
void BaseReader::initSampleFromFile(const std::string& pathToFile, 
                                        const bool resetTriggersAndFilters ) {
    bool is2016 = analysisTools::fileIs2016( pathToFile );
    bool is2016PreVFP = analysisTools::fileIs2016PreVFP( pathToFile );
    bool is2016PostVFP = analysisTools::fileIs2016PostVFP( pathToFile );
    bool is2017 = analysisTools::fileIs2017( pathToFile );
    bool is2018 = analysisTools::fileIs2018( pathToFile );
    initSampleFromFile( pathToFile, is2016, is2016PreVFP, is2016PostVFP, is2017, is2018, 
                        resetTriggersAndFilters );
}



void BaseReader::GetEntry( const Sample& samp, long unsigned entry ){
    checkCurrentTree();
    _currentTreePtr->GetEntry( entry );

    //Set up correct event weight
    if( !samp.isData() ){
        _scaledWeight = getWeight() * scale;
    } else {
        _scaledWeight = 1;
    }
}

//use the currently initialized sample when running in serial
void BaseReader::GetEntry(long unsigned entry) {
    GetEntry( *_currentSamplePtr, entry );
}
