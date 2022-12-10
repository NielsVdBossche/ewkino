#include "interface/dataManager.h"
#include "interface/mvaSetups.h"

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TMVA/PyMethodBase.h>
#include <ctime>

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>

#include <iostream>

#include "../Tools/interface/stringTools.h"

int main(int argc, char const *argv[]) {
    ///TMVA::PyMethodBase::PyInitialize();

    if (argc < 5) {
        std::cerr << "Mvatrainer requires at least four arguments: <samplelist.txt> treename setup searchOrNot" << std::endl;
        exit(1);
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    for (auto el : argvStr) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    // manage input
    // Pass files to a class using a samplelist to order these in categories
    std::string sampleList = argv[1];
    std::string tree = argv[2]; // tree should be dynamical, but don't know if it will be used as input. Maybe in some other way... 
    std::string setup = argv[3];
    std::string searchSetup = argv[4];
    std::string variables = ""; // just make variables the last thing added.
    std::string variables_short = "";
    bool useCV = false;

    int el = 0;
    for (auto arg : argvStr) {
        el++;
        if (el < 6) continue;
        if (stringTools::stringContains(arg, ".txt")) {
            variables = arg;
            variables_short = stringTools::fileNameWithoutExtension(stringTools::fileNameFromPath(variables));
        } else if (stringTools::stringContains(arg, "CV")) {
            useCV = true;
        }
    }

    std::map<std::string, mvaConfiguration> translator = {
        {"BDT_VAR_DL", mvaConfiguration::BDT_VAR_DL},
        {"BDT_VAR_ML", mvaConfiguration::BDT_VAR_ML},
        {"DL_BDT", mvaConfiguration::BDT_DL},
        {"ML_BDT", mvaConfiguration::BDT_ML},
        {"DL_NN", mvaConfiguration::NN_DL},
        {"ML_NN", mvaConfiguration::NN_ML},
        {"BDT_DL_SPLIT", mvaConfiguration::BDT_DL_SPLIT}
    };

    mvaConfiguration conf = translator[setup];

    TMVA::DataLoader* data = mvaDataManager::buildDataLoader(sampleList, tree, conf, variables);

    TFile* outfile;
    std::ostringstream oss;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M");

    if (searchSetup == "search") {
        outfile = new TFile(("Classifiers/FourTopClassificationUL_LeanSel_Search_" + setup + "_nTrees_" + std::string(argv[5]) + "_Depth_" + std::string(argv[6]) + "_nCuts_" + std::string(argv[7]) + "_shrink_" + std::string(argv[8]) + "_minNodeSize" + std::string(argv[9]) + "_baggedFraction_" + std::string(argv[10]) + ".root").c_str() ,"RECREATE");
    } else {
        outfile = new TFile(("Classifiers/FourTopClassificationUL_LeanSel_" + oss.str() + "_" + variables_short + "_" + setup + ".root").c_str() ,"RECREATE");
    }
    TMVA::Factory* factory;
    TMVA::CrossValidation* cv;
    unsigned nClasses = data->GetDataSetInfo().GetNClasses();

    if (!useCV) factory = mvaSetupManager::buildFactory(conf, outfile, nClasses);
    else cv = mvaSetupManager::useCrossValidation(data, outfile, conf);

    // class manages a dataloader and a factory, as well as settings for the mva's

    // Main part of calling training etc
    std::vector<std::string> variableVector;
    if (variables != "") {
        variableVector = mvaDataManager::readConfigFile(variables);
    }
    
    if (conf < mvaConfiguration::NN_DL) {
        if (searchSetup == "search") {
            int ntrees = std::stoi(argv[5]);
            int maxDepth = std::stoi(argv[6]);
            int ncuts = std::stoi(argv[7]);
            double shrinkage = std::stod(argv[8]);
            int minNodeSize = std::stoi(argv[9]);
            double baggedSampleFraction = std::stod(argv[10]);
            if (!useCV) mvaSetupManager::addBDT(factory, data, setup, ntrees, maxDepth, ncuts, shrinkage, minNodeSize, baggedSampleFraction);
            else mvaSetupManager::addBDT_CV(cv, data, setup, ntrees, maxDepth, ncuts, shrinkage, minNodeSize, baggedSampleFraction);
        } else {
            std::string tmpname = variables_short + "_" + setup;
            if (!useCV && (conf == BDT_VAR_DL || conf == BDT_DL)) mvaSetupManager::addBDT(factory, data, tmpname, 2000, 4, 20, 0.10, 1, 1.);
            else if (!useCV && (conf == BDT_VAR_ML || conf == BDT_ML)) mvaSetupManager::addBDT(factory, data, tmpname, 1500, 4, 20, 0.08, 1, 0.6);
            else if (useCV && (conf == BDT_VAR_DL || conf == BDT_DL)) mvaSetupManager::addBDT_CV(cv, data, tmpname, 2000, 6, 20, 0.10, 1, 1.);
            else if (useCV) mvaSetupManager::addBDT_CV(cv, data, tmpname, 1500, 5, 20, 0.08, 1, 0.6);
            else if (conf == BDT_DL_SPLIT) mvaSetupManager::addBDTs_SplitB(factory, data, variableVector);

        }
        //factory->OptimizeAllMethods("ROCIntegral","FitGA");
        
    } else {
        mvaSetupManager::addNN(factory, data, setup);
    }

    std::cout << "Methods booked!!" << std::endl;
    if (useCV) {
        cv->Evaluate();
        mvaSetupManager::evaluateCV(cv);
    } else {
        factory->TrainAllMethods();
        factory->TestAllMethods();
        factory->EvaluateAllMethods();
        TCanvas* rocPlot = factory->GetROCCurve(data);
        rocPlot->Print(("rocCurve_" + setup + ".png").c_str());
        //factory->EvaluateAllVariables(data);
    }

    outfile->Write();
    outfile->Close();

    return 0;
}
