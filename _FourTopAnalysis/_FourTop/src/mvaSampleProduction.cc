#include "../interface/FourTop.h"

void FourTop::linkMVAVariables(TTree* tree, bool isML) {

    //std::vector<double>* variableVector = selection->;
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
    std::cout << "Sample loop" << std::endl;

    infuseNonPrompt = true;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string outputFileName = "../MVATraining/trainingsamples/" + stringTools::fileNameFromPath(treeReader->currentSample().fileName());

        TFile* currentOutputFile = new TFile(outputFileName.c_str(), "RECREATE");
        currentOutputFile->cd();

        TTree* trainingTree_DL = new TTree("DL_tree", "DL_tree");
        linkMVAVariables(trainingTree_DL, false);
        TTree* trainingTree_ML = new TTree("ML_tree", "ML_tree");
        linkMVAVariables(trainingTree_ML, true);

        std::cout << "Event loop" << std::endl;

        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        currentEvent = new Event();


        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ) {
            //if (entry > 10000) break;
            delete currentEvent;

            currentEvent = treeReader->buildEventPtr( entry );
            selection->addNewEvent(currentEvent);

            if (infuseNonPrompt && ttgOverlapCheck > 0) {
                if (! selection->passBaselineEventSelectionWithAltLeptons()) {
                    continue;
                }
            } else if (! selection->passBaselineEventSelection())  {
                continue;
            }

            if (! selection->passZBosonVeto()) {
                continue;
            } else if (! selection->passLowMassVeto()) {
                continue;
            }

            if (! selection->passFullEventSelection()) {
                // Full event selection should be the same as normal event selection.
                continue;
            }

            // Build CRW (might expand these)
            if (selection->numberOfLeps() == 2 && selection->numberOfJets() < 6 && selection->numberOfMediumBJets() == 2) {
                continue;
            }

            // Fill histograms
            std::vector<double> fillVec;
            if (selection->numberOfLeps() == 2) {
                fillMVAVariables(false);
                trainingTree_DL->Fill();
            } else {
                fillMVAVariables(true);
                trainingTree_ML->Fill();
            }

            delete currentEvent;
        }
        
        std::cout << trainingTree_ML->GetEntries() << std::endl;
        std::cout << trainingTree_DL->GetEntries() << std::endl;

        trainingTree_DL->Write(trainingTree_DL->GetName(), TObject::kOverwrite);
        trainingTree_ML->Write(trainingTree_ML->GetName(), TObject::kOverwrite);


        currentOutputFile->Close();
    }
}
