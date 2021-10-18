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
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Tools/interface/stringTools.h"
#include "../../Event/interface/Event.h"

void skimFile( const std::string& pathToFile, const std::string& outputDirectory){

    std::cout << "skimming " << pathToFile << std::endl;

    //initialize TreeReader, input files might be corrupt in rare cases
    TreeReader treeReader;
    try{
        treeReader.initSampleFromFile( pathToFile );
    } catch( std::domain_error& ){
        std::cerr << "Can not read file. Returning." << std::endl;
        return;
    }

    //make output ROOT file
    //make file names unique by modifying the full path, but shorten slightly to avoid errors with too long file names for the OS
    std::string outputFileName = stringTools::split( pathToFile, "/heavyNeutrino/" ).back();
    outputFileName = stringTools::removeOccurencesOf( outputFileName, "/" );
    std::string outputFilePath = stringTools::formatDirectoryName( "$TMPDIR/" ) + outputFileName;//stringTools::formatDirectoryName( outputDirectory ) + outputFileName;
    TFile* outputFilePtr = TFile::Open( outputFilePath.c_str() , "RECREATE" );
    outputFilePtr->mkdir( "blackJackAndHookers" );
    outputFilePtr->cd( "blackJackAndHookers" );

    //read histograms from input file and write them to the new file
    std::vector< std::shared_ptr< TH1 > > histVector = treeReader.getHistogramsFromCurrentFile();
    for( const auto& histPtr : histVector ){
        histPtr->Write();
    }

    //make output tree
    std::shared_ptr< TTree > outputTreePtr( std::make_shared< TTree >( "blackJackAndHookersTree","blackJackAndHookersTree" ) );
    treeReader.setLeanOutputTree( outputTreePtr.get() );

    long unsigned nentries = treeReader.numberOfEntries();
    for( long unsigned entry = 0; entry < nentries; ++entry ){

        //build event
        Event event = treeReader.buildEvent( entry, false, false, false, false );

        //fill new tree
        outputTreePtr->Fill();
    }

    //write new tree
    outputTreePtr->Write( "",  BIT(2) );

    //close output file
    outputFilePtr->Close();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "skimmer requires at least three arguments to run : input_file_path, output_directory, skim_condition" << std::endl;
        return -1;
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    //std::string& input_file_path = argvStr[1];
    std::string& output_directory = argvStr[argc - 1];
    
    for (int i = 1; i < argc-2; i++) {
        skimFile( argvStr[i], output_directory);
    }
    
    return 0;
}