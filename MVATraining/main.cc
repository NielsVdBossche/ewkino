#include "interface/dataManager.h"

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>

#include <iostream>

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        std::cerr << "Mvatrainer requires at least one argument: <samplelist.txt>" << std::endl;
        exit(1);
    }
    // manage input
    // Pass files to a class using a samplelist to order these in categories
    std::string sampleList = argv[1];
    std::string tree = argv[2]; // tree should be dynamical, but don't know if it will be used as input. Maybe in some other way... 
    TMVA::DataLoader* data = mvaDataManager::buildDataLoader(sampleList, tree, mvaConfiguration::DL);

    TMVA::Factory* factory = new TMVA::Factory("Factory"); // get type of classificationjob from argv, pass this to function deciding some extra arguments.

    // class manages a dataloader and a factory, as well as settings for the mva's

    // Main part of calling training etc



    return 0;
}
