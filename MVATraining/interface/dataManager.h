#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>

#include <fstream>
#include <sstream>
#include <string>

enum mvaConfiguration {
    BDT_DL,
    BDT_ML,
    BDT_VAR_DL,
    BDT_VAR_ML,
    NN_DL,
    NN_ML
};

namespace mvaDataManager {

    void prepareLoader(mvaConfiguration config, TMVA::DataLoader* dataloader, std::vector<std::string>& variables);
    TMVA::DataLoader* buildDataLoader(std::string& sampleList, std::string& treeName, mvaConfiguration config, std::string configFile="");

    // All other helpful functions
    std::pair<Double_t*, std::vector<Double_t>*> prepareTTree(TTree* chain, mvaConfiguration config, std::vector<std::string>& variables);
    void readChainToLoader(TChain* chain, TString& className, TMVA::DataLoader* dataloader, std::pair<Double_t*, std::vector<Double_t>*>& vars);
    std::vector<std::string> readConfigFile(std::string& configFile);
}

#endif