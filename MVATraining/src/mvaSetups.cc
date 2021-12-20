#include "../interface/mvaSetups.h"

TMVA::Factory* mvaSetupManager::buildFactory(mvaConfiguration config, TFile* outputFile) {
    std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=";

    analysType += "Multiclass";

    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification_Temp", outputFile, analysType.c_str());

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

    nameString << nTrees << "_" << depth << "_" << std::setprecision(2) << shrinkage << "_" << cuts;

    optionString << ":IgnoreNegWeightsInTraining";
    std::cout << optionString.str() << std::endl;
    factory->BookMethod(dataloader, TMVA::Types::kBDT, nameString.str().c_str(), optionString.str().c_str());

    // optionString << ":SeparationType=GiniIndex";
}

void mvaSetupManager::addNN(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {

}

void mvaSetupManager::searchBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {
    bool baggedBoost = false;
    int nTrees[] = {200, 500, 1000, 1500, 2000};
    int depths[] = {2, 3, 4, 5};
    double shrinkages[] = {0.1, 0.5, 1., 2.};
    int cuts[] = {20, 50, 100, 200};

    for (int i=0; i < 5; i++) { // trees
        for (int j=0; j < 4; j++) { //depth
            for (int k=0; k < 4; k++) { // shrink
                for (int l=0; l < 4; l++) { // cuts
                    addBDT(factory, dataloader, initsetup, nTrees[i], depths[j], shrinkages[k], cuts[l], false);
                    //addBDT(factory, dataloader, initsetup, nTrees[i], depths[j], shrinkages[k], cuts[l], true);
                }
            }
        }
    }
}
