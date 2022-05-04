#ifndef MVASETUPS_H
#define MVASETUPS_H

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

#include <sstream>
#include <iomanip>

#include "dataManager.h"

namespace mvaSetupManager {
    TMVA::Factory* buildFactory(mvaConfiguration config, TFile* outputFile);
    void addBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup, int nTrees, int depth, int cuts, double shrinkage, int minNodeSize, double baggedSampleFraction);
    void addNN(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup);

    void searchBDT(TMVA::Factory* factory, TMVA::DataLoader* dataloader, std::string& initsetup);
}


#endif