#include "interface/dataManager.h"
#include "interface/mvaSetups.h"

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>

#include <iostream>

int main(int argc, char const *argv[]) {

    if (argc < 5) {
        std::cerr << "Mvatrainer requires at least four arguments: <samplelist.txt> treename setup searchOrNot" << std::endl;
        exit(1);
    }
    // manage input
    // Pass files to a class using a samplelist to order these in categories
    std::string sampleList = argv[1];
    std::string tree = argv[2]; // tree should be dynamical, but don't know if it will be used as input. Maybe in some other way... 
    std::string setup = argv[3];
    std::string searchSetup = argv[4];
    // extra variable for training true or false?

    std::map<std::string, mvaConfiguration> translator = {
        {"DL_BDT", mvaConfiguration::BDT_DL},
        {"ML_BDT", mvaConfiguration::BDT_ML},
        {"DL_NN", mvaConfiguration::NN_DL},
        {"ML_NN", mvaConfiguration::NN_ML}
    };
    mvaConfiguration conf = translator[setup];

    TMVA::DataLoader* data = mvaDataManager::buildDataLoader(sampleList, tree, conf);

    TFile* outfile = new TFile(("Classifiers/FourTopClassification_" + setup + ".root").c_str() ,"RECREATE");
    TMVA::Factory* factory = mvaSetupManager::buildFactory(conf, outfile);

    // class manages a dataloader and a factory, as well as settings for the mva's

    // Main part of calling training etc

    std::cout << "READY FOR ADDING METHODS" << std::endl;
    if (conf < mvaConfiguration::NN_DL) {
        std::cout << "DOING ADDITION OF BDTS" << std::endl;

        if (searchSetup == "search") mvaSetupManager::searchBDT(factory, data, setup);
        else mvaSetupManager::addBDT(factory, data, setup, 1000, 3, true, true);
    } else {
        mvaSetupManager::addNN(factory, data, setup);
    }

    std::cout << "Methods booked!!" << std::endl;

    
    factory->TrainAllMethods();
	
	factory->TestAllMethods();
	
	factory->EvaluateAllMethods();

    TCanvas* rocPlot = factory->GetROCCurve(data);
    rocPlot->Print(("rocCurve_" + setup + ".png").c_str());

	outfile->Write();
	outfile->Close();

    return 0;
}
