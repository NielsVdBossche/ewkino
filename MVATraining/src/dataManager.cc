#include "../interface/dataManager.h"

#include "../../Tools/interface/parseTools.h"
#include "../../Tools/interface/stringTools.h"

std::pair<Double_t*, std::vector<Double_t>*> mvaDataManager::prepareTTree(TTree* chain, mvaConfiguration config) {
    Double_t* weight = new Double_t;
    *weight = 1.; // unless changed elsewhere

    std::vector<Double_t>* dataVector = nullptr;
    if (config == BDT_DL || config == NN_DL) {
        dataVector = new std::vector<Double_t>(27);
    } else if (config == BDT_ML || config == NN_ML) {
        dataVector = new std::vector<Double_t>(28);
    }

    if ( config == BDT_DL || config == NN_DL || config == BDT_ML || config == NN_ML) {
        chain->SetBranchAddress("N_jets",    &dataVector->at(0));
        //chain->SetBranchAddress("N_b",       &dataVector->at(1));
        //chain->SetBranchAddress("N_b_tight", &dataVector->at(2));
        //chain->SetBranchAddress("N_b_loose", &dataVector->at(3));
        chain->SetBranchAddress("dr_bJets",         &dataVector->at(1));
        chain->SetBranchAddress("dr_leps",          &dataVector->at(2));
        chain->SetBranchAddress("aziAngle",         &dataVector->at(3));
        chain->SetBranchAddress("ht",               &dataVector->at(4));
        chain->SetBranchAddress("mToPt",            &dataVector->at(5));
        chain->SetBranchAddress("min_dr_lep_b",     &dataVector->at(6));
        chain->SetBranchAddress("sec_min_dr_lep_b", &dataVector->at(7));
        chain->SetBranchAddress("pt_jet_one",       &dataVector->at(8));
        chain->SetBranchAddress("pt_jet_four",      &dataVector->at(9));
        chain->SetBranchAddress("pt_jet_five",      &dataVector->at(10));
        //chain->SetBranchAddress("pt_jet_six",  &dataVector->at(14));
        chain->SetBranchAddress("pt_lep_one",       &dataVector->at(11));
        chain->SetBranchAddress("pt_lep_two",       &dataVector->at(12));

        chain->SetBranchAddress("bTagLead",         &dataVector->at(13));
        chain->SetBranchAddress("bTagSub",          &dataVector->at(14));
        chain->SetBranchAddress("bTagThird",        &dataVector->at(15));
        //chain->SetBranchAddress("bTagFourth",       &dataVector->at(20));
        chain->SetBranchAddress("bTagPtLead",       &dataVector->at(16));
        chain->SetBranchAddress("bTagPtSub",        &dataVector->at(17));
        chain->SetBranchAddress("bTagPtThird",      &dataVector->at(18));
        chain->SetBranchAddress("bTagPtFourth",     &dataVector->at(19));
        chain->SetBranchAddress("massBestTop",      &dataVector->at(20));
        chain->SetBranchAddress("massBestTopW",     &dataVector->at(21));
        chain->SetBranchAddress("massSecTop",       &dataVector->at(22));
        chain->SetBranchAddress("massSecTopW",      &dataVector->at(23));
        chain->SetBranchAddress("m2ll",             &dataVector->at(24));
        chain->SetBranchAddress("mtLeadLepMET",     &dataVector->at(25));
        chain->SetBranchAddress("mtSubLeadLepMET",  &dataVector->at(26));

    }

    if (config == BDT_ML || config == NN_ML) {
        chain->SetBranchAddress("pt_lep_three", &dataVector->at(27));
    }

    return {weight, dataVector};
}

void mvaDataManager::prepareLoader(mvaConfiguration config, TMVA::DataLoader* dataloader) {
    dataloader->AddVariable("N_jets", 'F');
    //dataloader->AddVariable("N_b", 'F');
    //dataloader->AddVariable("N_b_tight", 'F');
    //dataloader->AddVariable("N_b_loose", 'F');
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
    //dataloader->AddVariable("pt_jet_six", 'F');
    dataloader->AddVariable("pt_lep_one", 'F');
    dataloader->AddVariable("pt_lep_two", 'F');
    dataloader->AddVariable("bTagLead",       'F');
    dataloader->AddVariable("bTagSub",        'F');
    dataloader->AddVariable("bTagThird",      'F');
    //dataloader->AddVariable("bTagFourth",     'F');
    dataloader->AddVariable("bTagPtLead",     'F');
    dataloader->AddVariable("bTagPtSub",      'F');
    dataloader->AddVariable("bTagPtThird",    'F');
    dataloader->AddVariable("bTagPtFourth",   'F');
    dataloader->AddVariable("massBestTop",    'F');
    dataloader->AddVariable("massBestTopW",   'F');
    dataloader->AddVariable("massSecTop",     'F');
    dataloader->AddVariable("massSecTopW",    'F');
    dataloader->AddVariable("m2ll",           'F');
    dataloader->AddVariable("mtLeadLepMET",   'F');
    dataloader->AddVariable("mtSubLeadLepMET",'F');

    if (config == BDT_ML || config == NN_ML) {
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

    //std::vector<TChain*> chains;
    TRandom3 *r = new TRandom3(777);

    while (getline(fileStream, line)) {
        if (parseTools::skipLine(line)) continue;

        std::string className = parseTools::splitNameAndValue(line).second;
        TString classNameAlt = className.c_str();
        //TTree* newClassElement = new TChain(treeName.c_str());
        //chains.push_back(newClass);

        //std::pair<Double_t*, std::vector<Double_t>*> vars = prepareTTree(newClassElement, config);

        while (getline(fileStream, line) && ! stringTools::stringStartsWith(line, "End")) {
            if (parseTools::skipLine(line)) continue;

            // split line in elements based on spaces
            std::istringstream stream(line);
            std::string fileName, splitFraction;

            stream >> fileName;// >> splitFraction;

            std::string currentFile = basePath + fileName;

            TFile* input = new TFile(currentFile.c_str(), "open");
            TTree* newClassElement  = (TTree*) input->Get(treeName.c_str());

            std::pair<Double_t*, std::vector<Double_t>*> vars = prepareTTree(newClassElement, config);
            
            double ptrain = 0.8;
            if (stream >> splitFraction) {
                ptrain = std::stod(splitFraction);
            }

            for (int i=0; i < newClassElement->GetEntries(); i++) {
                newClassElement->GetEntry(i);

                std::vector<Double_t> variables = *vars.second;
                variables.erase(variables.begin()+20);
                variables.erase(variables.begin()+14);
                variables.erase(variables.begin()+3);
                variables.erase(variables.begin()+2);
                variables.erase(variables.begin()+1);

                float rnd = r->Rndm();
                if (rnd < ptrain) {
                    dataloader->AddTrainingEvent(className, *vars.second, *vars.first);
                } else {
                    dataloader->AddTestEvent(className, *vars.second, *vars.first);
                }
            }

            input->Close();
        }

        //readChainToLoader(newClass, classNameAlt, dataloader, vars);
    }
    
    dataloader->PrepareTrainingAndTestTree("","","SplitMode=Random:NormMode=NumEvents:!V");

    return dataloader;
}