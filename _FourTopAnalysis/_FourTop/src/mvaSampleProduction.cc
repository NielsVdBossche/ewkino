#include "../interface/FourTop.h"

void FourTop::linkMVAVariables(TTree* tree, bool isML) {

    //std::vector<double>* variableVector = selection->;
    tree->Branch("weight",          &mvaWeight,     "weight/D");
    tree->Branch("N_jets",          &n_jets_f,      "N_jets/D");
    tree->Branch("N_b",             &n_bjets_f,     "N_b/D");
    tree->Branch("N_b_tight",       &n_b_tight,     "N_b_tight/D");
    tree->Branch("N_b_loose",       &n_b_loose,     "N_b_loose/D");
    tree->Branch("MET",             &met,           "MET/D");

    tree->Branch("dr_bJets",        &deltaRBjets,   "dr_bJets/D");

    tree->Branch("bTagLead",        &bTagLead,      "bTagLead/D");
    tree->Branch("bTagSub",         &bTagSub,       "bTagSub/D");
    tree->Branch("bTagThird",       &bTagThird,     "bTagThird/D");
    tree->Branch("bTagFourth",      &bTagFourth,    "bTagFourth/D");

    tree->Branch("bTagPtLead",        &bTagPtLead,      "bTagPtLead/D");
    tree->Branch("bTagPtSub",         &bTagPtSub,       "bTagPtSub/D");
    tree->Branch("bTagPtThird",       &bTagPtThird,     "bTagPtThird/D");
    tree->Branch("bTagPtFourth",      &bTagPtFourth,    "bTagPtFourth/D");

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
    tree->Branch("massBestTop",     &massBestTop,    "massBestTop/D");
    tree->Branch("massBestTopW",    &massBestTopW,   "massBestTopW/D");
    tree->Branch("massSecTop",      &massSecTop,     "massSecTop/D");
    tree->Branch("massSecTopW",     &massSecTopW,    "massSecTopW/D");
    tree->Branch("m2ll",            &m2ll,           "m2ll/D");
    tree->Branch("m2bb",            &m2bb,           "m2bb/D");
    tree->Branch("m2lblb",          &m2lblb,         "m2lblb/D");
    tree->Branch("mtLeadLepMET",    &mtLeadLepMET,   "mtLeadLepMET/D");
    tree->Branch("mtSubLeadLepMET", &mtSubLeadLepMET,"mtSubLeadLepMET/D");

    if (isML) {
        tree->Branch("pt_lep_three",    &ptLepThree,    "pt_lep_three/D");
    }
}

void FourTop::createMVATrainingSamples() {
    std::cout << "Sample loop" << std::endl;

    infuseNonPrompt = true;
    currentEvent = new Event();

    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector() );


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

        TTree* trainingTree_DL_loose = new TTree("DL_tree_loose", "DL_tree_loose");
        linkMVAVariables(trainingTree_DL_loose, false);
        TTree* trainingTree_ML_loose = new TTree("ML_tree_loose", "ML_tree_loose");
        linkMVAVariables(trainingTree_ML_loose, true);

        std::cout << "Event loop" << std::endl;

        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ) {
            //if (entry > 10000) break;
            delete currentEvent;

            currentEvent = treeReader->buildEventPtr( entry );
            selection->addNewEvent(currentEvent);

            // first mass cleaning just to get rid of shit
            if (! selection->passLowMassVeto()) {
                continue;
            } else if (! selection->passZBosonVeto()) {
                continue;
            }

            double weight = currentEvent->weight();
            weight *= reweighter.totalWeight( *currentEvent );
            
            if (selection->passLeanSelection()) {
                if (selection->numberOfLeps() == 2) {
                    fillMVAVariables(false);
                    trainingTree_DL_loose->Fill();
                } else {
                    fillMVAVariables(true);
                    trainingTree_ML_loose->Fill();
                }
            }

            if (infuseNonPrompt && ttgOverlapCheck > 0) {
                if (! selection->passBaselineEventSelectionWithAltLeptons()) {
                    continue;
                }
            } else if (! selection->passBaselineEventSelection())  {
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
            if (selection->numberOfLeps() == 2) {
                fillMVAVariables(false);
                trainingTree_DL->Fill();
            } else {
                fillMVAVariables(true);
                trainingTree_ML->Fill();
            }
        }
        
        std::cout << trainingTree_ML->GetEntries() << std::endl;
        std::cout << trainingTree_DL->GetEntries() << std::endl;

        trainingTree_DL->Write(trainingTree_DL->GetName(), TObject::kOverwrite);
        trainingTree_ML->Write(trainingTree_ML->GetName(), TObject::kOverwrite);
        trainingTree_DL_loose->Write(trainingTree_DL_loose->GetName(), TObject::kOverwrite);
        trainingTree_ML_loose->Write(trainingTree_ML_loose->GetName(), TObject::kOverwrite);

        currentOutputFile->Close();
    }
}
