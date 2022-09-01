#include "../interface/mvaSetups.h"
#include "TMVA/PyMethodBase.h"

TMVA::Factory* mvaSetupManager::buildFactory(mvaConfiguration config, TFile* outputFile) {
    //std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=";
    std::string analysType;
    if (config == NN_DL || config == NN_ML) {
        analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=G:AnalysisType=";
    } else {
        analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=";
    }
    analysType += "Multiclass";

    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification_LeanSelUL", outputFile, analysType.c_str());

    return factory;
}

void mvaSetupManager::addBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, int cuts, double shrinkage, int minNodeSize, double baggedSampleFraction) {
    std::stringstream optionString;
    std::stringstream nameString;

    optionString << "!H:!V:NTrees=" << nTrees << ":MaxDepth=" << depth << ":nCuts=" << cuts;

    nameString << initsetup;
    
    std::cout << optionString.str() << std::endl;
    std::cout << nameString.str() << std::endl;
    

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

void mvaSetupManager::addBDT_CV(TMVA::CrossValidation* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, int cuts, double shrinkage, int minNodeSize, double baggedSampleFraction) {
    std::stringstream optionString;
    std::stringstream nameString;

    optionString << "!H:!V:NTrees=" << nTrees << ":MaxDepth=" << depth << ":nCuts=" << cuts;

    nameString << initsetup;
    
    std::cout << optionString.str() << std::endl;
    std::cout << nameString.str() << std::endl;
    

    optionString << ":BoostType=Grad:Shrinkage=" << std::setprecision(2) << shrinkage;
    nameString << "G_";
    
    optionString << ":UseBaggedBoost:BaggedSampleFraction=" << std::setprecision(2) << baggedSampleFraction;
    nameString << "B_";

    optionString << ":MinNodeSize="   << minNodeSize << "%";

    nameString << nTrees << "_" << depth << "_" << cuts << "_" << std::setprecision(2) << shrinkage << "_" <<  minNodeSize << "_" << std::setprecision(2) << baggedSampleFraction;

    optionString << ":IgnoreNegWeightsInTraining";
    std::cout << optionString.str() << std::endl;
    factory->BookMethod(TMVA::Types::kBDT, nameString.str().c_str(), optionString.str().c_str());

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

TMVA::CrossValidation* mvaSetupManager::useCrossValidation(TMVA::DataLoader* dataloader, TFile* outputFile, mvaConfiguration config) {
    TString optstring = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=Multiclass:NumFolds=5";
    TMVA::CrossValidation* cv = new TMVA::CrossValidation("k-fold_cv_4T", dataloader, outputFile, optstring);

    return cv;
}

void mvaSetupManager::evaluateCV(TMVA::CrossValidation* cv) {
    size_t iMethod = 0;
    for (auto && result : cv->GetResults()) {
        std::cout << "Summary for method "
                    << cv->GetMethods()[iMethod++].GetValue<TString>("MethodName")
                    << std::endl;
        for (UInt_t iFold = 0; iFold<cv->GetNumFolds(); ++iFold) {
            std::cout << "\tFold " << iFold << ": "
                        << "ROC int: " << result.GetROCValues()[iFold]
                        << ", "
                        << "BkgEff@SigEff=0.3: " << result.GetEff30Values()[iFold]
                        << std::endl;
        }
    }
}
