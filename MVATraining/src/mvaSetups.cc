#include "../interface/mvaSetups.h"
#include "TMVA/PyMethodBase.h"
#include "TMVA/MethodCategory.h"

#include <ctime>

TMVA::Factory* mvaSetupManager::buildFactory(mvaConfiguration config, TFile* outputFile, unsigned nClasses) {
    //std::string analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=";
    std::string analysType;
    if (config == NN_DL || config == NN_ML) {
        analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=G:AnalysisType=";
    } else {
        analysType = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=";
    }
    if (nClasses > 2) {
        analysType += "Multiclass";
    } else {
        analysType += "Classification";
    }


    std::ostringstream oss;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M");

    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification_UL_" + oss.str() + "_", outputFile, analysType.c_str());

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

void mvaSetupManager::addBDTs_SplitB(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::vector<std::string>& variables) {
    TMVA::IMethod* category = factory->BookMethod(dataloader, TMVA::Types::kCategory, "Category", "<options>" );
    TMVA::MethodCategory* mcategory = dynamic_cast<TMVA::MethodCategory*>(category);
    
    TCut cut1 = "N_b<2";
    TCut cut2 = "N_b>2";
    TCut cut3 = cut1 && cut2;
    std::string opts = "!H:!V:NTrees=1000:MaxDepth=5:nCuts=20:BoostType=Grad:Shrinkage=0.08:UseBaggedBoost:BaggedSampleFraction=0.6:MinNodeSize=1%:IgnoreNegWeightsInTraining";

    std::string variablesStr = "";
    for (auto it : variables) {
        if (it == "N_b") continue;
        variablesStr += it;
        variablesStr += ":";
    }

    std::string variablesStrFin = variablesStr.substr(0, variablesStr.size()-1);

    mcategory->AddMethod(cut1, variablesStrFin, TMVA::Types::kBDT, "lessThan2MedBs", opts.c_str());
    mcategory->AddMethod(cut2, variablesStrFin, TMVA::Types::kBDT, "2MedBs", opts.c_str());
    mcategory->AddMethod(cut3, variablesStrFin, TMVA::Types::kBDT, "moreThan2MedBs", opts.c_str());
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
    TString optstring = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P:AnalysisType=Multiclass:NumFolds=10:FoldFileOutput=True";
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
