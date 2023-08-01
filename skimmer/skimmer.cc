//include c++ library classes 
#include <string>
#include <vector>
#include <exception>
#include <iostream>

//include ROOT classes 
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

//include other parts of framework
#include "../TreeReader/interface/TreeReader.h"
#include "../Tools/interface/stringTools.h"
#include "../Event/interface/Event.h"
#include "interface/skimSelections.h"


void skimFile(	const std::string& inputFilePath,
		const std::string& outputFilePath,
		const std::string& skimCondition ){

    std::cout << "skimming " << inputFilePath << std::endl;

    // read auxiliary trees
    std::cout << "reading auxiliary trees..." << std::endl;
    TFile* inputFilePtr = TFile::Open( inputFilePath.c_str(), "READ" );
    TTree* LumiTreePtr = (TTree*) inputFilePtr->Get( "LuminosityBlocks" );
    TTree* RunTreePtr = (TTree*) inputFilePtr->Get( "Runs" );
    TTree* MetaTreePtr = (TTree*) inputFilePtr->Get( "MetaData" );
    TTree* ParamTreePtr = (TTree*) inputFilePtr->Get( "ParameterSets" );

    // initialize TreeReader, input files might be corrupt in rare cases
    std::cout << "initializing TreeReader..." << std::endl;
    TreeReader treeReader;
    try{ treeReader.initSampleFromFile( inputFilePath ); }
    catch( std::domain_error& ){
        std::cerr << "Can not read file. Returning." << std::endl;
        return;
    }

    // make output ROOT file
    TFile* outputFilePtr = TFile::Open( outputFilePath.c_str() , "RECREATE" );

    // write auxiliary trees to output file
    std::cout << "writing auxiliary trees to output file..." << std::endl;
    LumiTreePtr->CloneTree()->Write();
    RunTreePtr->CloneTree()->Write();
    MetaTreePtr->CloneTree()->Write();
    ParamTreePtr->CloneTree()->Write();

    // make output tree
    std::cout << "making output tree..." << std::endl;
    std::shared_ptr< TTree > outputTreePtr( std::make_shared< TTree >( "Events", "Events" ) );
    treeReader.setOutputTree( outputTreePtr.get() );

    long unsigned nentries = treeReader.numberOfEntries();
    //long unsigned nentries = 10000; // for testing
    long unsigned npass = 0;
    std::cout << "starting loop over " << nentries << " events..." << std::endl;
    for( long unsigned entry = 0; entry < nentries; ++entry ){
	if(entry%10000 == 0) std::cout<<"processed: "<<entry<<" of "<<nentries<<std::endl;

        // build event
        Event event = treeReader.buildEvent( entry, false, false, false, false );

        // apply event selection
        if( !passSkim( event, skimCondition ) ) continue;

        // fill new tree
	npass++;
        outputTreePtr->Fill();
    }

    // write new tree
    std::cout << "writing output file..." << std::endl;
    outputTreePtr->Write( "",  BIT(2) );

    // close output file
    outputFilePtr->Close();

    // do printouts
    std::cout << "skimmed " << inputFilePath << std::endl;
    std::cout << "number of entries: " << nentries << std::endl;
    std::cout << "number of passing entries: " << npass << std::endl;
}


int main( int argc, char* argv[] ){
    std::cerr << "###starting###" << std::endl;

    if( argc != 4 ){
        std::cerr << "skimmer requires exactly three arguments to run : " << std::endl;
	std::cerr << "  - path to input file" << std::endl;
	std::cerr << "  - path to output file" << std::endl;
	std::cerr << "  - skim condition" << std::endl;
        return -1;
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    std::string& input_file_path = argvStr[1];
    std::string& output_file_path = argvStr[2];
    std::string& skimCondition = argvStr[3];
    skimFile( input_file_path, output_file_path, skimCondition );

    std::cerr << "###done###" << std::endl;
    return 0;
}
