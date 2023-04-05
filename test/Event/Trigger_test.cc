
//include class to test 
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Event/interface/TriggerInfo.h"
#include "../copyMoveTest.h"

//include c++ library classes
#include <iostream>
#include <chrono>


int main( int argc, char* argv[] ){

    int nargs = 1;
    if( argc != nargs+1 ){
        std::cerr << "ERROR: Trigger_test.cc requires " << nargs;
        std::cerr << " arguments to run:" << std::endl;
        std::cerr << "- path to input file (.root)" << std::endl;
        return -1;
    }

    // parse arguments
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    std::string& inputFilePath = argvStr[1];

    // read the input file
    TreeReader treeReader;
    treeReader.initSampleFromFile( inputFilePath );

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long numberOfEntries = treeReader.numberOfEntries();
    numberOfEntries = 100; // for limiting running time
    for(unsigned i = 0; i < numberOfEntries; ++i){

        treeReader.GetEntry(i);
        
        TriggerInfo triggerInfo( treeReader, true, true );
        triggerInfo.passTriggers_e();
        triggerInfo.passTriggers_m();
        triggerInfo.passTriggers_ee();
        triggerInfo.passTriggers_em();
        triggerInfo.passTriggers_et();
        triggerInfo.passTriggers_mm();
        triggerInfo.passTriggers_mt();
        triggerInfo.passTriggers_eee();
        triggerInfo.passTriggers_eem();
        triggerInfo.passTriggers_emm();
        triggerInfo.passTriggers_mmm();
        triggerInfo.passMetFilters();

        if( i == 0 ){
            triggerInfo.printAvailableIndividualTriggers();
            triggerInfo.printAvailableMetFilters();
        }

        copyMoveTest( triggerInfo );
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time for looping over sample : " << elapsed.count() << " s\n";

    return 0;
}
