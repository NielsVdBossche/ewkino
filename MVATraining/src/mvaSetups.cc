#include "../interface/mvaSetups.h"

TMVA::Factory* mvaSetupManager::buildFactory(mvaConfiguration config, TFile* outputFile) {
    std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D:AnalysisType=";

    analysType += "Multiclass";

    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification", outputFile, analysType.c_str());

    return factory;
}

void mvaSetupManager::addBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, double shrinkage, int cuts, bool baggedBoost) {
    std::stringstream optionString;
    std::stringstream nameString;

    optionString << "!H:!V:NTrees=" << nTrees << ":MaxDepth=" << depth << ":nCuts=" << cuts;

    nameString << initsetup;
    
    std::cout << optionString.str() << std::endl;
    std::cout << nameString.str() << std::endl;
    std::cout << "IDK WHAT IS GOIGN WRONG" << std::endl;
    


    optionString << ":BoostType=Grad:Shrinkage=" << std::setprecision(2) << shrinkage;
    nameString << "G_";
    
    if (baggedBoost) {
        optionString << ":UseBaggedBoost:BaggedSampleFraction=0.50";
        nameString << "B_";
    }

    nameString << nTrees << "_" << depth;

    optionString << ":IgnoreNegWeightsInTraining";
    std::cout << optionString.str() << std::endl;
    factory->BookMethod(dataloader, TMVA::Types::kBDT, nameString.str().c_str(), optionString.str().c_str());

    // optionString << ":SeparationType=GiniIndex";
}

void mvaSetupManager::addNN(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {

}

void mvaSetupManager::searchBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {
    bool baggedBoost = false;
    int nTrees[] = {100, 200, 300, 400, 500, 750, 1000, 1250, 1500, 2000};
    int depths[] = {2, 3, 4, 5};
    double shrinkages[] = {0.05, 0.1, 0.25, 0.5, 0.75, 1., 1.5, 2.};
    int cuts[] = {10, 20, 30, 50, 75, 100, 200, 300};

    for (int i=0; i < 10; i++) { // trees
        for (int j=0; j < 4; j++) { //depth
            for (int k=0; k < 8; k++) { // shrink
                for (int l=0; l < 8; l++) { // cuts
                    addBDT(factory, dataloader, initsetup, nTrees[i], depths[j], shrinkages[k], cuts[l], false);
                    //addBDT(factory, dataloader, initsetup, nTrees[i], depths[j], shrinkages[k], cuts[l], true);
                }
            }
        }
    }
}
