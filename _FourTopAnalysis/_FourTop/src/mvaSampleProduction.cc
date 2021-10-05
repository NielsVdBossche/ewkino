#include "../interface/FourTop.h"

void FourTop::prepareTrainingTrees() {
    trainingTree_DL = new TTree("DL_tree", "DL_tree");
    linkMVAVariables(trainingTree_DL, false);
    trainingTree_ML = new TTree("ML_tree", "ML_tree");
    linkMVAVariables(trainingTree_ML, true);
}

void FourTop::linkMVAVariables(TTree* tree, bool isML) {

    tree->Branch("N_jets",          &n_jets_f,      "N_jets/D");
    tree->Branch("N_b",             &n_bjets_f,     "N_b/D");
    tree->Branch("N_b_tight",       &n_b_tight,     "N_b_tight/D");
    tree->Branch("N_b_loose",       &n_b_loose,     "N_b_loose/D");
    tree->Branch("dr_bJets",        &deltaRBjets,   "dr_bJets/D");

    tree->Branch("dr_leps",         &dRleps,        "dr_leps/D");
    tree->Branch("aziAngle",        &aziAngle,      "aziAngle/D");
    tree->Branch("ht",              &ht,            "ht/D");
    tree->Branch("mToPt",           &massToPt,      "mToPt/D");
    tree->Branch("min_dr_lep_b",    &min_dr_lep_b,  "min_dr_lep_b/D");
    tree->Branch("sec_min_dr_lep_b",    &sec_min_dr_lep_b,  "sec_min_dr_lep_b/D");
    tree->Branch("pt_jet_one",      &ptJetOne,      "pt_jet_one/D");
    tree->Branch("pt_jet_four",     &ptJetFour,     "pt_jet_four/D");
    tree->Branch("pt_jet_five",     &ptJetFive,     "pt_jet_five/D");
    tree->Branch("pt_jet_six",      &ptJetSix,      "pt_jet_six/D");
    tree->Branch("pt_lep_one",      &ptLepOne,      "pt_lep_one/D");
    tree->Branch("pt_lep_two",      &ptLepTwo,      "pt_lep_two/D");

    if (isML) {
        tree->Branch("pt_lep_three",    &ptLepThree,    "pt_lep_three/D");
    }
}

void FourTop::createMVATrainingSamples() {
    std::cout << "event loop" << std::endl;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        // check if TTbar or TTGamma sample
        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap(); // Moet dit?

        std::string outputFileName = "../MVATraining/trainingsamples/" + treeReader->currentSample().fileName();

        TFile* currentOutputFile = new TFile(outputFileName.c_str(), "RECREATE");
        currentOutputFile->cd();

        prepareTrainingTrees();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            if (entry > 10000) break;

            currentEvent = treeReader->buildEventPtr( entry );

            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"
            

            // apply baseline selection
            // Right now we build CRZ from looser objects.
            // Necessary to account for looser leptons which are otherwise missed in the full lepton selection and could be part of a Z-boson resonance

            if (! selection->passBaselineEventSelection())  {
                delete currentEvent;
                continue;
            }

            currentEvent->sortLeptonsByPt();

            // Remove mass resonances
            currentEvent->makeSubLeptonCollections();

            if (! selection->passZBosonVeto()) {
                delete currentEvent;
                continue;
            } else if (! selection->passLowMassVeto()) {
                delete currentEvent;
                continue;
            }

            if (! selection->passFullEventSelection()) {
                delete currentEvent;
                continue;
            }

            currentEvent->makeSubLeptonCollections();

            // Build CRW (might expand these)
            if (currentEvent->numberOfLightLeptons() == 2 && currentEvent->numberOfJets() < 6 && currentEvent->numberOfMediumBTaggedJets() == 2) {
                delete currentEvent;
                continue;
            }

            // Fill histograms
            std::vector<double> fillVec;
            if (currentEvent->numberOfLightLeptons() == 2) {
                fillMVAVariables(false);
                trainingTree_DL->Fill();
            } else {
                fillMVAVariables(true);
                trainingTree_ML->Fill();
            }

            delete currentEvent;
        }
        

        trainingTree_DL->Write(trainingTree_DL->GetName(), TObject::kOverwrite);
        trainingTree_ML->Write(trainingTree_ML->GetName(), TObject::kOverwrite);
        
        currentOutputFile->Close();

        delete trainingTree_ML;
        delete trainingTree_DL;
    }
}