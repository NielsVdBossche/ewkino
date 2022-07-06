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

std::string cleanOutputName(std::string path) {
    /// we should just keep date_0000_singlelep_x.root -> last 3 when split
    std::vector<std::string> splitPath = stringTools::split(path, "/");
    std::vector<std::string> subSplitPath(splitPath.end()-3, splitPath.end());
    std::string cleanedName = stringTools::mergeString(subSplitPath, "_");

    return cleanedName;
}

void skimFile( const std::string& pathToFile, const std::string& outputDirectory, const std::string& skimConditionString ){

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
    std::string outputFileName = cleanOutputName(pathToFile);
    std::string outputFilePath = stringTools::formatDirectoryName( outputDirectory ) + outputFileName;
    // Outputpath looks like rawskims/version/samplename/00xx_data_time_singlelep_X.root
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
    treeReader.setOutputTree( outputTreePtr.get() );

    skimCondition currentSkimCondition = giveCondition(skimConditionString);

    long unsigned nentries = treeReader.numberOfEntries();
    for( long unsigned entry = 0; entry < nentries; ++entry ){

        //build event
        Event event = treeReader.buildEvent( entry, true, true, false, false );

        //apply event selection
        if( !passSkim( event, currentSkimCondition ) ) continue;

        //fill new tree
        outputTreePtr->Fill();
    }

    //write new tree
    outputTreePtr->Write( "",  BIT(2) );

    //close output file
    outputFilePtr->Close();

    TreeReader checkOutputTree;
    checkOutputTree.initSampleFromFile(outputFilePath);
    TTree* currentTree = checkOutputTree.GetTree();

    for (unsigned long long i=0; i<currentTree->GetEntries(); i++) {
        try {
            currentTree->GetEntry(i);
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            std::cerr << "outputfile " << outputFilePath << " is corrupted. Exiting..." << std::endl;
            exit(1);
        }   
    }
}

/*
int main( int argc, char* argv[] ){
    if( argc != 4 ){
        std::cerr << "skimmer requires exactly three arguments to run : input_file_path, output_directory, skim_condition" << std::endl;
        return -1;
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    std::string& input_file_path = argvStr[1];
    std::string& output_directory = argvStr[2];
    std::string& skimCondition = argvStr[3];
    skimFile( input_file_path, output_directory, skimCondition );

    return 0;
}*/

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "skimmer requires at least three arguments to run : input_file_path, output_directory, skim_condition" << std::endl;
        return -1;
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    //std::string& input_file_path = argvStr[1];
    std::string& output_directory = argvStr[argc - 2];
    std::string& skimCondition = argvStr[argc - 1];
    
    for (int i = 1; i < argc-2; i++) {
        skimFile( argvStr[i], output_directory, skimCondition );
    }
    
    return 0;
}