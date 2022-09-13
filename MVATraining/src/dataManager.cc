#include "../interface/dataManager.h"

#include "../../Tools/interface/parseTools.h"
#include "../../Tools/interface/stringTools.h"

std::pair<Double_t*, std::vector<Double_t>*> mvaDataManager::prepareTTree(TTree* chain, mvaConfiguration config, std::vector<std::string>& variables) {
    Double_t* weight = new Double_t;
    //*weight = 1.; // unless changed elsewhere
    chain->SetBranchAddress("weight_rew", weight);


    std::vector<Double_t>* dataVector = nullptr;
    if (config == BDT_DL) {
        dataVector = new std::vector<Double_t>(35);
    } else if (config == BDT_ML) {
        dataVector = new std::vector<Double_t>(36);
    } else if (config == NN_DL) {
        dataVector = new std::vector<Double_t>(46);
    } else if (config == NN_ML) {
        dataVector = new std::vector<Double_t>(49);
    }

    if (config == BDT_VAR_DL || config == BDT_VAR_ML) {
        dataVector = new std::vector<Double_t>(variables.size());
        int pos = 0;
        for (auto var : variables) {
            chain->SetBranchAddress(var.c_str(), &dataVector->at(pos));
            pos++;
        }
    }

    if (config == BDT_DL) {
        chain->SetBranchAddress("N_jets",           &dataVector->at(0));
        chain->SetBranchAddress("N_b",              &dataVector->at(1));
        chain->SetBranchAddress("N_b_tight",        &dataVector->at(2));
        chain->SetBranchAddress("N_b_loose",        &dataVector->at(3));
        chain->SetBranchAddress("dr_bJets",         &dataVector->at(4));
        chain->SetBranchAddress("dr_leps",          &dataVector->at(5));
        chain->SetBranchAddress("aziAngle",         &dataVector->at(6));
        chain->SetBranchAddress("ht",               &dataVector->at(7));
        chain->SetBranchAddress("mToPt",            &dataVector->at(8));
        chain->SetBranchAddress("min_dr_lep_b",     &dataVector->at(9));
        chain->SetBranchAddress("sec_min_dr_lep_b", &dataVector->at(10));
        chain->SetBranchAddress("pt_jet_one",       &dataVector->at(11));
        chain->SetBranchAddress("pt_jet_four",      &dataVector->at(12));
        chain->SetBranchAddress("pt_jet_five",      &dataVector->at(13));
        chain->SetBranchAddress("pt_jet_six",       &dataVector->at(14));
        chain->SetBranchAddress("pt_lep_one",       &dataVector->at(15));
        chain->SetBranchAddress("pt_lep_two",       &dataVector->at(16));
        chain->SetBranchAddress("bTagLead",         &dataVector->at(17));
        chain->SetBranchAddress("bTagSub",          &dataVector->at(18));
        chain->SetBranchAddress("bTagThird",        &dataVector->at(19));
        chain->SetBranchAddress("bTagFourth",       &dataVector->at(20));
        chain->SetBranchAddress("bTagPtLead",       &dataVector->at(21));
        chain->SetBranchAddress("bTagPtSub",        &dataVector->at(22));
        chain->SetBranchAddress("bTagPtThird",      &dataVector->at(23));
        chain->SetBranchAddress("bTagPtFourth",     &dataVector->at(24));
        chain->SetBranchAddress("m2ll",             &dataVector->at(25));
        chain->SetBranchAddress("m2bb",             &dataVector->at(26));
        chain->SetBranchAddress("m2lblb",           &dataVector->at(27));
        chain->SetBranchAddress("mtLeadLepMET",     &dataVector->at(28));
        chain->SetBranchAddress("mtSubLeadLepMET",  &dataVector->at(29));
        chain->SetBranchAddress("massBestTop",      &dataVector->at(30));
        chain->SetBranchAddress("massBestTopW",     &dataVector->at(31));
        chain->SetBranchAddress("massSecTop",       &dataVector->at(32));
        chain->SetBranchAddress("massSecTopW",      &dataVector->at(33));
        chain->SetBranchAddress("MET",              &dataVector->at(34));
    }

    if (config == BDT_ML) {
        chain->SetBranchAddress("N_jets",           &dataVector->at(0));
        chain->SetBranchAddress("N_b",              &dataVector->at(1));
        chain->SetBranchAddress("N_b_tight",        &dataVector->at(2));
        chain->SetBranchAddress("N_b_loose",        &dataVector->at(3));
        chain->SetBranchAddress("dr_bJets",         &dataVector->at(4));
        chain->SetBranchAddress("dr_leps",          &dataVector->at(5));
        chain->SetBranchAddress("aziAngle",         &dataVector->at(6));
        chain->SetBranchAddress("ht",               &dataVector->at(7));
        chain->SetBranchAddress("mToPt",            &dataVector->at(8));
        chain->SetBranchAddress("min_dr_lep_b",     &dataVector->at(9));
        chain->SetBranchAddress("sec_min_dr_lep_b", &dataVector->at(10));
        chain->SetBranchAddress("pt_jet_one",       &dataVector->at(11));
        chain->SetBranchAddress("pt_jet_four",      &dataVector->at(12));
        chain->SetBranchAddress("pt_jet_five",      &dataVector->at(13));
        chain->SetBranchAddress("pt_jet_six",       &dataVector->at(14));
        chain->SetBranchAddress("pt_lep_one",       &dataVector->at(15));
        chain->SetBranchAddress("pt_lep_two",       &dataVector->at(16));
        chain->SetBranchAddress("bTagLead",         &dataVector->at(17));
        chain->SetBranchAddress("bTagSub",          &dataVector->at(18));
        chain->SetBranchAddress("bTagThird",        &dataVector->at(19));
        chain->SetBranchAddress("bTagFourth",       &dataVector->at(20));
        chain->SetBranchAddress("bTagPtLead",       &dataVector->at(21));
        chain->SetBranchAddress("bTagPtSub",        &dataVector->at(22));
        chain->SetBranchAddress("bTagPtThird",      &dataVector->at(23));
        chain->SetBranchAddress("bTagPtFourth",     &dataVector->at(24));
        chain->SetBranchAddress("m2ll",             &dataVector->at(25));
        chain->SetBranchAddress("m2bb",             &dataVector->at(26));
        chain->SetBranchAddress("m2lblb",           &dataVector->at(27));
        chain->SetBranchAddress("mtLeadLepMET",     &dataVector->at(28));
        chain->SetBranchAddress("mtSubLeadLepMET",  &dataVector->at(29));
        chain->SetBranchAddress("massBestTop",      &dataVector->at(30));
        chain->SetBranchAddress("massBestTopW",     &dataVector->at(31));
        chain->SetBranchAddress("massSecTop",       &dataVector->at(32));
        chain->SetBranchAddress("massSecTopW",      &dataVector->at(33));
        chain->SetBranchAddress("MET",              &dataVector->at(34));
        chain->SetBranchAddress("pt_lep_three",     &dataVector->at(35));
    }

    if (config == NN_DL) {
        chain->SetBranchAddress("pt_jet_one",          &dataVector->at(0));
        chain->SetBranchAddress("phi_jet_one",         &dataVector->at(1));
        chain->SetBranchAddress("eta_jet_one",         &dataVector->at(2));
        chain->SetBranchAddress("btag_jet_one",        &dataVector->at(3));
        chain->SetBranchAddress("pt_jet_two",          &dataVector->at(4));
        chain->SetBranchAddress("phi_jet_two",         &dataVector->at(5));
        chain->SetBranchAddress("eta_jet_two",         &dataVector->at(6));
        chain->SetBranchAddress("btag_jet_two",        &dataVector->at(7));
        chain->SetBranchAddress("pt_jet_three",        &dataVector->at(8));
        chain->SetBranchAddress("phi_jet_three",       &dataVector->at(9));
        chain->SetBranchAddress("eta_jet_three",       &dataVector->at(10));
        chain->SetBranchAddress("btag_jet_three",      &dataVector->at(11));
        chain->SetBranchAddress("pt_jet_four",         &dataVector->at(12));
        chain->SetBranchAddress("phi_jet_four",        &dataVector->at(13));
        chain->SetBranchAddress("eta_jet_four",        &dataVector->at(14));
        chain->SetBranchAddress("btag_jet_four",       &dataVector->at(15));
        chain->SetBranchAddress("pt_jet_five",         &dataVector->at(16));
        chain->SetBranchAddress("phi_jet_five",        &dataVector->at(17));
        chain->SetBranchAddress("eta_jet_five",        &dataVector->at(18));
        chain->SetBranchAddress("btag_jet_five",       &dataVector->at(19));
        chain->SetBranchAddress("pt_jet_six",          &dataVector->at(20));
        chain->SetBranchAddress("phi_jet_six",         &dataVector->at(21));
        chain->SetBranchAddress("eta_jet_six",         &dataVector->at(22));
        chain->SetBranchAddress("btag_jet_six",        &dataVector->at(23));
        chain->SetBranchAddress("pt_jet_seven",        &dataVector->at(24));
        chain->SetBranchAddress("phi_jet_seven",       &dataVector->at(25));
        chain->SetBranchAddress("eta_jet_seven",       &dataVector->at(26));
        chain->SetBranchAddress("btag_jet_seven",      &dataVector->at(27));
        chain->SetBranchAddress("pt_jet_eight",        &dataVector->at(28));
        chain->SetBranchAddress("phi_jet_eight",       &dataVector->at(29));
        chain->SetBranchAddress("eta_jet_eight",       &dataVector->at(30));
        chain->SetBranchAddress("btag_jet_eight",      &dataVector->at(31));
        chain->SetBranchAddress("pt_jet_nine",         &dataVector->at(32));
        chain->SetBranchAddress("phi_jet_nine",        &dataVector->at(33));
        chain->SetBranchAddress("eta_jet_nine",        &dataVector->at(34));
        chain->SetBranchAddress("btag_jet_nine",       &dataVector->at(35));
        chain->SetBranchAddress("pt_jet_ten",          &dataVector->at(36));
        chain->SetBranchAddress("phi_jet_ten",         &dataVector->at(37));
        chain->SetBranchAddress("eta_jet_ten",         &dataVector->at(38));
        chain->SetBranchAddress("btag_jet_ten",        &dataVector->at(39));
        chain->SetBranchAddress("pt_lep_one",          &dataVector->at(40));
        chain->SetBranchAddress("phi_lep_one",         &dataVector->at(41));
        chain->SetBranchAddress("eta_lep_one",         &dataVector->at(42));
        //chain->SetBranchAddress("flav_lep_one",        &dataVector->at(43));
        chain->SetBranchAddress("pt_lep_two",          &dataVector->at(43));
        chain->SetBranchAddress("phi_lep_two",         &dataVector->at(44));
        chain->SetBranchAddress("eta_lep_two",         &dataVector->at(45));
        //chain->SetBranchAddress("flav_lep_two",        &dataVector->at(47));
    }

    if (config == NN_ML) {
        chain->SetBranchAddress("pt_jet_one",          &dataVector->at(0));
        chain->SetBranchAddress("phi_jet_one",         &dataVector->at(1));
        chain->SetBranchAddress("eta_jet_one",         &dataVector->at(2));
        chain->SetBranchAddress("btag_jet_one",        &dataVector->at(3));
        chain->SetBranchAddress("pt_jet_two",          &dataVector->at(4));
        chain->SetBranchAddress("phi_jet_two",         &dataVector->at(5));
        chain->SetBranchAddress("eta_jet_two",         &dataVector->at(6));
        chain->SetBranchAddress("btag_jet_two",        &dataVector->at(7));
        chain->SetBranchAddress("pt_jet_three",        &dataVector->at(8));
        chain->SetBranchAddress("phi_jet_three",       &dataVector->at(9));
        chain->SetBranchAddress("eta_jet_three",       &dataVector->at(10));
        chain->SetBranchAddress("btag_jet_three",      &dataVector->at(11));
        chain->SetBranchAddress("pt_jet_four",         &dataVector->at(12));
        chain->SetBranchAddress("phi_jet_four",        &dataVector->at(13));
        chain->SetBranchAddress("eta_jet_four",        &dataVector->at(14));
        chain->SetBranchAddress("btag_jet_four",       &dataVector->at(15));
        chain->SetBranchAddress("pt_jet_five",         &dataVector->at(16));
        chain->SetBranchAddress("phi_jet_five",        &dataVector->at(17));
        chain->SetBranchAddress("eta_jet_five",        &dataVector->at(18));
        chain->SetBranchAddress("btag_jet_five",       &dataVector->at(19));
        chain->SetBranchAddress("pt_jet_six",          &dataVector->at(20));
        chain->SetBranchAddress("phi_jet_six",         &dataVector->at(21));
        chain->SetBranchAddress("eta_jet_six",         &dataVector->at(22));
        chain->SetBranchAddress("btag_jet_six",        &dataVector->at(23));
        chain->SetBranchAddress("pt_jet_seven",        &dataVector->at(24));
        chain->SetBranchAddress("phi_jet_seven",       &dataVector->at(25));
        chain->SetBranchAddress("eta_jet_seven",       &dataVector->at(26));
        chain->SetBranchAddress("btag_jet_seven",      &dataVector->at(27));
        chain->SetBranchAddress("pt_jet_eight",        &dataVector->at(28));
        chain->SetBranchAddress("phi_jet_eight",       &dataVector->at(29));
        chain->SetBranchAddress("eta_jet_eight",       &dataVector->at(30));
        chain->SetBranchAddress("btag_jet_eight",      &dataVector->at(31));
        chain->SetBranchAddress("pt_jet_nine",         &dataVector->at(32));
        chain->SetBranchAddress("phi_jet_nine",        &dataVector->at(33));
        chain->SetBranchAddress("eta_jet_nine",        &dataVector->at(34));
        chain->SetBranchAddress("btag_jet_nine",       &dataVector->at(35));
        chain->SetBranchAddress("pt_jet_ten",          &dataVector->at(36));
        chain->SetBranchAddress("phi_jet_ten",         &dataVector->at(37));
        chain->SetBranchAddress("eta_jet_ten",         &dataVector->at(38));
        chain->SetBranchAddress("btag_jet_ten",        &dataVector->at(39));
        chain->SetBranchAddress("pt_lep_one",          &dataVector->at(40));
        chain->SetBranchAddress("phi_lep_one",         &dataVector->at(41));
        chain->SetBranchAddress("eta_lep_one",         &dataVector->at(42));
        //chain->SetBranchAddress("flav_lep_one",        &dataVector->at(43));
        chain->SetBranchAddress("pt_lep_two",          &dataVector->at(43));
        chain->SetBranchAddress("phi_lep_two",         &dataVector->at(44));
        chain->SetBranchAddress("eta_lep_two",         &dataVector->at(45));
        //chain->SetBranchAddress("flav_lep_two",        &dataVector->at(47));

        chain->SetBranchAddress("pt_lep_three",        &dataVector->at(46));
        chain->SetBranchAddress("phi_lep_three",       &dataVector->at(47));
        chain->SetBranchAddress("eta_lep_three",       &dataVector->at(48));
        //chain->SetBranchAddress("flav_lep_three",      &dataVector->at(51));
    }
    return {weight, dataVector};
}

void mvaDataManager::prepareLoader(mvaConfiguration config, TMVA::DataLoader* dataloader, std::vector<std::string>& variables) {

    if (config == BDT_VAR_DL || config == BDT_VAR_ML) {
        for (auto var : variables) {
            dataloader->AddVariable(var, 'F');
        }
    }

    if (config == BDT_DL) {
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
        dataloader->AddVariable("bTagLead",       'F');
        dataloader->AddVariable("bTagSub",        'F');
        dataloader->AddVariable("bTagThird",      'F');
        dataloader->AddVariable("bTagFourth",     'F');
        dataloader->AddVariable("bTagPtLead",     'F');
        dataloader->AddVariable("bTagPtSub",      'F');
        dataloader->AddVariable("bTagPtThird",    'F');
        dataloader->AddVariable("bTagPtFourth",   'F');
        dataloader->AddVariable("m2ll",           'F');
        dataloader->AddVariable("m2bb",           'F');
        dataloader->AddVariable("m2lblb",         'F');
        dataloader->AddVariable("mtLeadLepMET",   'F');
        dataloader->AddVariable("mtSubLeadLepMET",'F');
        dataloader->AddVariable("massBestTop",    'F');
        dataloader->AddVariable("massBestTopW",   'F');
        dataloader->AddVariable("massSecTop",     'F');
        dataloader->AddVariable("massSecTopW",    'F');
        dataloader->AddVariable("MET",            'F');
    }

    if (config == BDT_ML) {    
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
        dataloader->AddVariable("pt_jet_one",     'F');
        dataloader->AddVariable("pt_jet_four",    'F');
        dataloader->AddVariable("pt_jet_five",    'F');
        dataloader->AddVariable("pt_jet_six",     'F');
        dataloader->AddVariable("pt_lep_one",     'F');
        dataloader->AddVariable("pt_lep_two",     'F');
        dataloader->AddVariable("bTagLead",       'F');
        dataloader->AddVariable("bTagSub",        'F');
        dataloader->AddVariable("bTagThird",      'F');
        dataloader->AddVariable("bTagFourth",     'F');
        dataloader->AddVariable("bTagPtLead",     'F');
        dataloader->AddVariable("bTagPtSub",      'F');
        dataloader->AddVariable("bTagPtThird",    'F');
        dataloader->AddVariable("bTagPtFourth",   'F');
        dataloader->AddVariable("m2ll",           'F');
        dataloader->AddVariable("m2bb",           'F');
        dataloader->AddVariable("m2lblb",         'F');
        dataloader->AddVariable("mtLeadLepMET",   'F');
        dataloader->AddVariable("mtSubLeadLepMET",'F');
        dataloader->AddVariable("massBestTop",    'F');
        dataloader->AddVariable("massBestTopW",   'F');
        dataloader->AddVariable("massSecTop",     'F');
        dataloader->AddVariable("massSecTopW",    'F');
        dataloader->AddVariable("MET",            'F');
        dataloader->AddVariable("pt_lep_three",   'F');
    }

    if (config == NN_DL) {
        dataloader->AddVariable("pt_jet_one",     'F');
        dataloader->AddVariable("phi_jet_one",    'F');
        dataloader->AddVariable("eta_jet_one",    'F');
        dataloader->AddVariable("btag_jet_one",   'F');
        dataloader->AddVariable("pt_jet_two",     'F');
        dataloader->AddVariable("phi_jet_two",    'F');
        dataloader->AddVariable("eta_jet_two",    'F');
        dataloader->AddVariable("btag_jet_two",   'F');
        dataloader->AddVariable("pt_jet_three",   'F');
        dataloader->AddVariable("phi_jet_three",  'F');
        dataloader->AddVariable("eta_jet_three",  'F');
        dataloader->AddVariable("btag_jet_three", 'F');
        dataloader->AddVariable("pt_jet_four",    'F');
        dataloader->AddVariable("phi_jet_four",   'F');
        dataloader->AddVariable("eta_jet_four",   'F');
        dataloader->AddVariable("btag_jet_four",  'F');
        dataloader->AddVariable("pt_jet_five",    'F');
        dataloader->AddVariable("phi_jet_five",   'F');
        dataloader->AddVariable("eta_jet_five",   'F');
        dataloader->AddVariable("btag_jet_five",  'F');
        dataloader->AddVariable("pt_jet_six",     'F');
        dataloader->AddVariable("phi_jet_six",    'F');
        dataloader->AddVariable("eta_jet_six",    'F');
        dataloader->AddVariable("btag_jet_six",   'F');
        dataloader->AddVariable("pt_jet_seven",   'F');
        dataloader->AddVariable("phi_jet_seven",  'F');
        dataloader->AddVariable("eta_jet_seven",  'F');
        dataloader->AddVariable("btag_jet_seven", 'F');
        dataloader->AddVariable("pt_jet_eight",   'F');
        dataloader->AddVariable("phi_jet_eight",  'F');
        dataloader->AddVariable("eta_jet_eight",  'F');
        dataloader->AddVariable("btag_jet_eight", 'F');
        dataloader->AddVariable("pt_jet_nine",    'F');
        dataloader->AddVariable("phi_jet_nine",   'F');
        dataloader->AddVariable("eta_jet_nine",   'F');
        dataloader->AddVariable("btag_jet_nine",  'F');
        dataloader->AddVariable("pt_jet_ten",     'F');
        dataloader->AddVariable("phi_jet_ten",    'F');
        dataloader->AddVariable("eta_jet_ten",    'F');
        dataloader->AddVariable("btag_jet_ten",   'F');
        dataloader->AddVariable("pt_lep_one",     'F');
        dataloader->AddVariable("phi_lep_one",    'F');
        dataloader->AddVariable("eta_lep_one",    'F');
        //dataloader->AddVariable("flav_lep_one",   'F');
        dataloader->AddVariable("pt_lep_two",     'F');
        dataloader->AddVariable("phi_lep_two",    'F');
        dataloader->AddVariable("eta_lep_two",    'F');
        //dataloader->AddVariable("flav_lep_two",   'F');
    }

    if (config == NN_ML) {
        dataloader->AddVariable("pt_jet_one",     'F');
        dataloader->AddVariable("phi_jet_one",    'F');
        dataloader->AddVariable("eta_jet_one",    'F');
        dataloader->AddVariable("btag_jet_one",   'F');
        dataloader->AddVariable("pt_jet_two",     'F');
        dataloader->AddVariable("phi_jet_two",    'F');
        dataloader->AddVariable("eta_jet_two",    'F');
        dataloader->AddVariable("btag_jet_two",   'F');
        dataloader->AddVariable("pt_jet_three",   'F');
        dataloader->AddVariable("phi_jet_three",  'F');
        dataloader->AddVariable("eta_jet_three",  'F');
        dataloader->AddVariable("btag_jet_three", 'F');
        dataloader->AddVariable("pt_jet_four",    'F');
        dataloader->AddVariable("phi_jet_four",   'F');
        dataloader->AddVariable("eta_jet_four",   'F');
        dataloader->AddVariable("btag_jet_four",  'F');
        dataloader->AddVariable("pt_jet_five",    'F');
        dataloader->AddVariable("phi_jet_five",   'F');
        dataloader->AddVariable("eta_jet_five",   'F');
        dataloader->AddVariable("btag_jet_five",  'F');
        dataloader->AddVariable("pt_jet_six",     'F');
        dataloader->AddVariable("phi_jet_six",    'F');
        dataloader->AddVariable("eta_jet_six",    'F');
        dataloader->AddVariable("btag_jet_six",   'F');
        dataloader->AddVariable("pt_jet_seven",   'F');
        dataloader->AddVariable("phi_jet_seven",  'F');
        dataloader->AddVariable("eta_jet_seven",  'F');
        dataloader->AddVariable("btag_jet_seven", 'F');
        dataloader->AddVariable("pt_jet_eight",   'F');
        dataloader->AddVariable("phi_jet_eight",  'F');
        dataloader->AddVariable("eta_jet_eight",  'F');
        dataloader->AddVariable("btag_jet_eight", 'F');
        dataloader->AddVariable("pt_jet_nine",    'F');
        dataloader->AddVariable("phi_jet_nine",   'F');
        dataloader->AddVariable("eta_jet_nine",   'F');
        dataloader->AddVariable("btag_jet_nine",  'F');
        dataloader->AddVariable("pt_jet_ten",     'F');
        dataloader->AddVariable("phi_jet_ten",    'F');
        dataloader->AddVariable("eta_jet_ten",    'F');
        dataloader->AddVariable("btag_jet_ten",   'F');
        dataloader->AddVariable("pt_lep_one",     'F');
        dataloader->AddVariable("phi_lep_one",    'F');
        dataloader->AddVariable("eta_lep_one",    'F');
       // dataloader->AddVariable("flav_lep_one",   'F');
        dataloader->AddVariable("pt_lep_two",     'F');
        dataloader->AddVariable("phi_lep_two",    'F');
        dataloader->AddVariable("eta_lep_two",    'F');
        //dataloader->AddVariable("flav_lep_two",   'F');

        dataloader->AddVariable("pt_lep_three",   'F');
        dataloader->AddVariable("phi_lep_three",  'F');
        dataloader->AddVariable("eta_lep_three",  'F');
        //dataloader->AddVariable("flav_lep_three", 'F');
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

std::vector<std::string> mvaDataManager::readConfigFile(std::string& configFile) {
    std::vector<std::string> ret;

    std::ifstream fileStream(configFile);
    std::string line;
    std::string mvaName; 
    
    while (getline(fileStream, line)) {
        if (parseTools::skipLine(line)) continue;
        
        // potentially also extract a name here. Although rather avoid it

        ret.push_back(line);
    }
    return ret;
}



TMVA::DataLoader* mvaDataManager::buildDataLoader(std::string& sampleList, std::string& treeName, mvaConfiguration config, std::string configFile, bool useCV) {
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
    
    std::vector<std::string> variables;
    if (configFile != "") {
        variables = readConfigFile(configFile);
    }

    prepareLoader(config, dataloader, variables);

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

            std::pair<Double_t*, std::vector<Double_t>*> vars = prepareTTree(newClassElement, config, variables);
            
            double ptrain = 0.8;
            if (stream >> splitFraction) {
                ptrain = std::stod(splitFraction);
            }

            for (int i=0; i < newClassElement->GetEntries(); i++) {
                newClassElement->GetEntry(i);
                //std::cout << *vars.first << std::endl;
                if (useCV) dataloader->AddTrainingEvent(className, *vars.second, *vars.first);
                else {
                    float rnd = r->Rndm();
                    if (rnd < ptrain) {
                        dataloader->AddTrainingEvent(className, *vars.second, *vars.first);
                    } else {
                        dataloader->AddTestEvent(className, *vars.second, *vars.first);
                    }
                }
            }

            input->Close();
            delete vars.second;
        }

        //readChainToLoader(newClass, classNameAlt, dataloader, vars);
    }
    
    dataloader->PrepareTrainingAndTestTree("","","SplitMode=Random:NormMode=EqualNumEvents:!V");

    return dataloader;
}

