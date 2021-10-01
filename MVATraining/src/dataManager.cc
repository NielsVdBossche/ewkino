#include "../interface/dataManager.h"

#include "../../Tools/interface/parseTools.h"
#include "../../Tools/interface/stringTools.h"


TMVA::DataLoader* mvaDataManager::buildDataLoader(std::string& sampleList, std::string& treeName) {
    // Open samplelist,
    // Line by line read samples, ordered per class
    // Structure of samplelist:
    /*
    Class=<ClassName>
    Sample_1.root
    Sample_2.root
    End

    Class=<ClassName>
    Sample_3.root
    End
    */
    // Add each sample to TChain for each class
    
    std::string basePath = "/user/nivanden/ewkino/MVATraining/trainingsamples/";

    TMVA::DataLoader* dataloader = new TMVA::DataLoader("VeryEpicLoader");

    //dataloader->AddVariable(



    std::ifstream fileStream(sampleList);
    std::string line;

    std::vector<TChain*> chains;

    while (getline(fileStream, line)) {
        if (parseTools::skipLine(line)) continue;

        std::string className = parseTools::splitNameAndValue(line).second;
        TChain* newClass = new TChain(treeName.c_str());
        chains.push_back(newClass);

        while (getline(fileStream, line) && ! stringTools::stringStartsWith(line, "End")) {
            if (parseTools::skipLine(line)) continue;

            std::string currentFile = basePath + line;
            newClass->Add(currentFile.c_str());
        }
        // probably redo this with manual reading of ttrees because tmva does not seem to provide an option to specify how the random 
        // selection betw training and testing tree occurs. Can't define percentage so won't use it. Stupid but ok.
        dataloader->AddTree(newClass, className); // considers all components the same weight, should be fine?
        // might be useful to add a weight specifier when reading samples so one class can be boosted
    }




    
}