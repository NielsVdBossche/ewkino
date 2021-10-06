#include "../interface/dataManager.h"

#include "../../Tools/interface/parseTools.h"
#include "../../Tools/interface/stringTools.h"

std::pair<Double_t*, std::vector<Double_t>*> mvaDataManager::prepareTChain(TChain* chain, mvaConfiguration config) {
    Double_t* weight = new Double_t;
    *weight = 1.; // unless changed elsewhere

    std::vector<Double_t>* dataVector = nullptr;
    if (config == DL) {
        dataVector = new std::vector<Double_t>(17);
    } else if (config == ML) {
        dataVector = new std::vector<Double_t>(18);
    }

    if (config == DL || config == ML) {
        chain->SetBranchAddress("N_jets",    &dataVector->at(0));
        chain->SetBranchAddress("N_b",       &dataVector->at(1));
        chain->SetBranchAddress("N_b_tight", &dataVector->at(2));
        chain->SetBranchAddress("N_b_loose", &dataVector->at(3));
        chain->SetBranchAddress("dr_bJets",  &dataVector->at(4));
        chain->SetBranchAddress("dr_leps",   &dataVector->at(5));
        chain->SetBranchAddress("aziAngle",  &dataVector->at(6));
        chain->SetBranchAddress("ht",        &dataVector->at(7));
        chain->SetBranchAddress("mToPt",     &dataVector->at(8));
        chain->SetBranchAddress("min_dr_lep_b", &dataVector->at(9));
        chain->SetBranchAddress("sec_min_dr_lep_b", &dataVector->at(10));
        chain->SetBranchAddress("pt_jet_one",  &dataVector->at(11));
        chain->SetBranchAddress("pt_jet_four", &dataVector->at(12));
        chain->SetBranchAddress("pt_jet_five", &dataVector->at(13));
        chain->SetBranchAddress("pt_jet_six",  &dataVector->at(14));
        chain->SetBranchAddress("pt_lep_one",  &dataVector->at(15));
        chain->SetBranchAddress("pt_lep_two",  &dataVector->at(16));
    }

    if (config == ML) {
        chain->SetBranchAddress("pt_lep_three", &dataVector->at(17));
    }

    return {weight, dataVector};
}

void mvaDataManager::prepareLoader(mvaConfiguration config, TMVA::DataLoader* dataloader) {
    dataloader->AddVariable("N_jets", 'F');
    dataloader->AddVariable("N_b", 'F');
    dataloader->AddVariable("N_b_tight", 'F');
    dataloader->AddVariable("N_b_loose", 'F');
    dataloader->AddVariable("dr_bJets", 'F');
    dataloader->AddVariable("dr_leps", 'F');
    dataloader->AddVariable("aziAngle", 'F');
    dataloader->AddVariable("ht",  'F');
    dataloader->AddVariable("mToPt", 'F');
    dataloader->AddVariable("min_dr_lep_b", 'F');
    dataloader->AddVariable("sec_min_dr_lep_b", 'F');
    dataloader->AddVariable("pt_jet_one", 'F');
    dataloader->AddVariable("pt_jet_four", 'F');
    dataloader->AddVariable("pt_jet_five", 'F');
    dataloader->AddVariable("pt_jet_six", 'F');
    dataloader->AddVariable("pt_lep_one", 'F');
    dataloader->AddVariable("pt_lep_two", 'F');

    if (config == ML) {
        dataloader->AddVariable("pt_lep_three", 'F');
    }
}

void mvaDataManager::readChainToLoader(TChain* chain, TString& className, TMVA::DataLoader* dataloader, std::pair<Double_t*, std::vector<Double_t>*>& vars) {
    TRandom3 *r = new TRandom3(777);

    double ptrain = 0.8; // percentage used for training

    int train = 0;
    int test = 0;

    for (int i=0; i < chain->GetEntries(); i++) {
        chain->GetEntry(i);

        float rnd = r->Rndm();
	    if (rnd < ptrain) {
	        dataloader->AddTrainingEvent(className, *vars.second, *vars.first);
            train++;
        } else {
	        dataloader->AddTestEvent(className, *vars.second, *vars.first);
            test++;
        }
    }
}


TMVA::DataLoader* mvaDataManager::buildDataLoader(std::string& sampleList, std::string& treeName, mvaConfiguration config) {
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
    prepareLoader(config, dataloader);

    std::ifstream fileStream(sampleList);
    std::string line;

    std::vector<TChain*> chains;

    while (getline(fileStream, line)) {
        if (parseTools::skipLine(line)) continue;

        std::string className = parseTools::splitNameAndValue(line).second;
        TString classNameAlt = className.c_str();
        TChain* newClass = new TChain(treeName.c_str());
        chains.push_back(newClass);

        std::pair<Double_t*, std::vector<Double_t>*> vars = prepareTChain(newClass, config);

        while (getline(fileStream, line) && ! stringTools::stringStartsWith(line, "End")) {
            if (parseTools::skipLine(line)) continue;

            std::string currentFile = basePath + line;
            newClass->Add(currentFile.c_str());
        }

        readChainToLoader(newClass, classNameAlt, dataloader, vars);
    }
    
    dataloader->PrepareTrainingAndTestTree("","","SplitMode=Random:NormMode=NumEvents:!V");

    return dataloader;
}