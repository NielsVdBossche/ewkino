#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <sstream>
#include <string>

namespace mvaDataManager {
    TMVA::DataLoader* buildDataLoader(std::string& sampleList, std::string& treeName);

    // All other helpful functions
}

#endif