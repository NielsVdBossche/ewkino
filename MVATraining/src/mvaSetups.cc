#include "../interface/mvaSetups.h"

TMVA::Factory* mvaSetupManager::buildFactory(mvaConfiguration config, TFile* outputFile) {
    std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D:AnalysisType=";
    if (config > 1) {
        analysType += "Multiclass";
    } else {
        analysType += "Classification";
    }

    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification", outputFile, analysType.c_str());

    return factory;
}

void mvaSetupManager::addBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, bool gradientBoost, bool baggedBoost) {
    std::stringstream optionString("!H:!V:NTrees=");
    std::stringstream nameString(initsetup);

    optionString << nTrees << ":MaxDepth=" << depth;
     
    if (gradientBoost) {
        optionString << ":BoostType=Grad:Shrinkage=0.10";
        nameString << "G_";
    }
    if (baggedBoost) {
        optionString << ":UseBaggedBoost:BaggedSampleFraction=0.50";
        nameString << "B_";
    }

    nameString << nTrees << "_" << depth;

    optionString << ":IgnoreNegWeightsInTraining";

    factory->BookMethod(dataloader, TMVA::Types::kBDT, nameString.str().c_str(), optionString.str().c_str());

    // optionString << ":SeparationType=GiniIndex";
}

void mvaSetupManager::addNN(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {

}

void mvaSetupManager::searchBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, bool allVariations) {
    bool gradientBoost = false;
    bool baggedBoost = false;

    for (int nTrees = 100; nTrees < 1001; nTrees += 50) {
        for (int depths = 2; depths < 5; depths++) {
            addBDT(factory, dataloader, initsetup, nTrees, depths, gradientBoost, baggedBoost);
            if (allVariations) {
                gradientBoost = true;
                addBDT(factory, dataloader, initsetup, nTrees, depths, gradientBoost, baggedBoost);
                baggedBoost = true;
                addBDT(factory, dataloader, initsetup, nTrees, depths, gradientBoost, baggedBoost);
                gradientBoost = false;
                addBDT(factory, dataloader, initsetup, nTrees, depths, gradientBoost, baggedBoost);
                baggedBoost = false;
            }
        }
    }
}
