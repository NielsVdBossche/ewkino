#ifndef MVASETUPS_H
#define MVASETUPS_H

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TMVA/CrossValidation.h>

#include <sstream>
#include <iomanip>

#include "dataManager.h"


namespace mvaSetupManager {
    TMVA::Factory* buildFactory(mvaConfiguration config, TFile* outputFile, unsigned nClasses);
    //TMVA::Factory* buildFactories(mvaConfiguration config, TFile* outputFile);
    void addBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, int cuts, double shrinkage, int minNodeSize, double baggedSampleFraction);
    void addBDT_CV(TMVA::CrossValidation* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, int cuts, double shrinkage, int minNodeSize, double baggedSampleFraction);
    void addBDTs_SplitB(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::vector<std::string>& variables);
    
    void addNN(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup);

    //void searchBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup);

    TMVA::CrossValidation* useCrossValidation(TMVA::DataLoader* dataloader, TFile* outputFile, mvaConfiguration config);

    void evaluateCV(TMVA::CrossValidation* cv);
}


#endif