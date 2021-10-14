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
    BIN_DL,
    BIN_ML,
    BDT_DL,
    BDT_ML,
    NN_DL,
    NN_ML
};

namespace mvaDataManager {

    void prepareLoader(mvaConfiguration config, TMVA::DataLoader* dataloader);
    TMVA::DataLoader* buildDataLoader(std::string& sampleList, std::string& treeName, mvaConfiguration config);

    // All other helpful functions
    std::pair<Double_t*, std::vector<Double_t>*> prepareTChain(TChain* chain, mvaConfiguration config);
    void readChainToLoader(TChain* chain, TString& className, TMVA::DataLoader* dataloader, std::pair<Double_t*, std::vector<Double_t>*>& vars);
}

#endif