#include "../interface/mvaSetups.h"
#include "TMVA/PyMethodBase.h"

TMVA::Factory* mvaSetupManager::buildFactory(mvaConfiguration config, TFile* outputFile) {
    //std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=";
    std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=G:AnalysisType=";

    analysType += "Multiclass";

    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification_LeanSel", outputFile, analysType.c_str());

    return factory;
}

void mvaSetupManager::addBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, int cuts, double shrinkage, int minNodeSize, double baggedSampleFraction) {
    std::stringstream optionString;
    std::stringstream nameString;

    optionString << "!H:!V:NTrees=" << nTrees << ":MaxDepth=" << depth << ":nCuts=" << cuts;

    nameString << initsetup;
    
    std::cout << optionString.str() << std::endl;
    std::cout << nameString.str() << std::endl;
    std::cout << "IDK WHAT IS GOIGN WRONG" << std::endl;
    


    optionString << ":BoostType=Grad:Shrinkage=" << std::setprecision(2) << shrinkage;
    nameString << "G_";
    
    optionString << ":UseBaggedBoost:BaggedSampleFraction=" << std::setprecision(2) << baggedSampleFraction;
    nameString << "B_";

    optionString << ":MinNodeSize="   << minNodeSize << "%";

    nameString << nTrees << "_" << depth << "_" << cuts << "_" << std::setprecision(2) << shrinkage << "_" <<  minNodeSize << "_" << std::setprecision(2) << baggedSampleFraction;

    optionString << ":IgnoreNegWeightsInTraining";
    std::cout << optionString.str() << std::endl;
    factory->BookMethod(dataloader, TMVA::Types::kBDT, nameString.str().c_str(), optionString.str().c_str());

    // optionString << ":SeparationType=GiniIndex";
}

void mvaSetupManager::addNN(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {
    if (initsetup == "DL_NN") {
        //factory->BookMethod(dataloader, TMVA::Types::kPyKeras, "PyKeras_DL_NN_3x256", "H:!V:VarTransform=D,G:FilenameModel=KerasModelTrainer/Models/keras_48_3_3_256.h5:NumEpochs=20:BatchSize=256");
        factory->BookMethod(dataloader, TMVA::Types::kPyKeras, "PyKeras_DL_NN_4x256", "H:!V:VarTransform=D,G:FilenameModel=KerasModelTrainer/Models/keras_46_3_4_256.h5:NumEpochs=40:BatchSize=128");
    } else {
        //factory->BookMethod(dataloader, TMVA::Types::kPyKeras, "PyKeras_ML_NN_3x256", "H:!V:VarTransform=D,G:FilenameModel=KerasModelTrainer/Models/keras_52_3_3_256.h5:NumEpochs=20:BatchSize=256");
        factory->BookMethod(dataloader, TMVA::Types::kPyKeras, "PyKeras_ML_NN_4x256", "H:!V:VarTransform=D,G:FilenameModel=KerasModelTrainer/Models/keras_49_3_4_256.h5:NumEpochs=40:BatchSize=128");
    }
}

void mvaSetupManager::searchBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup) {
    bool baggedBoost = false;
    int nTrees[] = {200, 500, 1000, 2000};
    int depths[] = {2, 3, 4, 5};
    double shrinkages[] = {0.1, 0.5, 1.};
    int cuts[] = {20, 50};

    for (int i=0; i < 4; i++) { // trees
        for (int j=0; j < 4; j++) { //depth
            for (int k=0; k < 3; k++) { // shrink
                for (int l=0; l < 2; l++) { // cuts
                    //addBDT(factory, dataloader, initsetup, nTrees[i], depths[j], shrinkages[k], cuts[l], false);
                    //addBDT(factory, dataloader, initsetup, nTrees[i], depths[j], shrinkages[k], cuts[l], true);
                }
            }
        }
    }
}
