#include "../interface/OutputTreeHandler.h"
#include "../interface/OutputTreeSysVar.h"
#include "../interface/OutputTreeWeightVar.h"

#include <iostream>
#include "../../../Tools/interface/Sample.h"
#include "../../../Tools/interface/stringTools.h"

#include "../../FourTopEventHandling/interface/EventFourT.h"

OutputTreeHandler::OutputTreeHandler(std::vector<std::string>& processes, std::string& outputSubfolder) {
    for (auto entry : processes) {
        processNames.push_back(entry);
        //std::shared_ptr<OutputTree> tmp = std::make_shared<OutputTreeVariables>()
    }
    outputfolder = outputSubfolder;    
}

OutputTreeHandler::~OutputTreeHandler() {

}

void OutputTreeHandler::ChangeProcess(unsigned processNumber, std::string& newProcess) {
    if (processNumber > processNames.size()+1) {
        std::cerr << "Error: Processnumber " << processNumber 
                << " is way larger than the current number of stored processes ("
                << processNames.size() << ")." << std::endl;
        exit(1);
    } else if (processNumber == processNames.size()) {
        processNames.push_back(newProcess);
    } else {
        processNames[processNumber] = newProcess;
    }
}

void OutputTreeHandler::ChangeAllProcesses(std::vector<std::string>& processes) {
    for (unsigned i=0; i < processes.size(); i++) {
        ChangeProcess(i, processes[i]);
    }
}


TFile* OutputTreeHandler::InitializeNewSample(const Sample& sample, std::string& outputFileTags, std::string runtype, std::string name) {
    // use Sample class?
    // anyway
    // change maintree to JEC_VAR_tree or JER_VAR_tree?
    std::string outputFilename = outputfolder + "/Tree_";

    // what does this do that nothing else can do? Like what if I want to do just a more extreme skim?
    // then again need the weight variations explicitely... in python

    // if MC: split unique name at _crab_
    // if data, split at _singlelepton
    std::string uniquename = sample.uniqueName();
    if (sample.isMC()) {
        uniquename = stringTools::split(uniquename, "_crab_")[0];
    } else {
        uniquename = stringTools::split(uniquename, "_singlelepton")[0];
    }
    outputFilename += uniquename + "_";
    
    outputFilename += outputFileTags + "_" + name + ".root";
    // add identifier of sample
    // also in file
    currentFile = new TFile(outputFilename.c_str(), "recreate");

    // also keep metadata somewhere in a class -> done, kinda
    // maybe fourtop class can have a "write metadata" thing -> exactly
    
    // create output tree per process with correct tags, tags can be passed in setup? -> todo

    // next step: create trees
    currentFile->cd();

    // depends on set up, for now: create a OutputTree object, or a deriv
    // for now:
    mapping.clear();
    std::string test = "tree";
    for (auto process : processNames) {
        if (runtype == "MCPrompt" && name == "base") {
            std::shared_ptr<OutputTree> new_outtree = std::make_shared<OutputTreeWeightVar>(currentFile, process, test);
            mapping.push_back(new_outtree);
        } else if (runtype == "nonPromptDD" || runtype == "ChargeDD" || runtype == "Obs" || name != "base") {
            std::shared_ptr<OutputTree> new_outtree = std::make_shared<OutputTreeVariables>(currentFile, process, test);
            mapping.push_back(new_outtree);
        }
    }

    return currentFile;
}

void OutputTreeHandler::FlushTrees() {
    for (auto tree : mapping) {
        tree->SaveToFile(currentFile);
    }
    
    mapping.clear();
    
}

void OutputTreeHandler::WriteExpWeightNaming(std::vector<std::string>& weightNames) {
    std::shared_ptr<TTree> tree = std::make_shared<TTree>("Namingscheme", "Namingscheme");
    std::vector<std::string> tmp;
    tree->Branch("ExperimentalWeightNames", &tmp);
    tmp = weightNames;
    tree->Fill();
    currentFile->cd();
    tree->Write(tree->GetName(), TObject::kOverwrite);
}


void OutputTreeHandler::FillAt(unsigned pNb, EventFourT* ftEvent, double wgt) {
    mapping[pNb]->FillTree(ftEvent, wgt);
    mapping[pNb]->AddEntry();
}

