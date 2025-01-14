#include "../interface/FourTop.h"

void FourTop::linkMVAVariables(TTree* tree, bool isML) {

    //std::vector<double>* variableVector = selection->;
    tree->Branch("weight_an",          &anWeight,     "weight_an/D");
    tree->Branch("weight_mc",          &mcWeight,     "weight_mc/D");
    tree->Branch("weight_rew",         &mvaWeight,     "weight_rew/D");
    tree->Branch("weight_non",         &nonWeight,     "weight_non/D");

    tree->Branch("n_mu"      ,         &n_mu     ,    "n_mu/D");
    tree->Branch("chargeSum" ,         &chargeSum,    "chargeSum/D");
    
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
    tree->Branch("massBestTop",     &massBestTop,    "massBestTop/D");
    tree->Branch("massBestTopW",    &massBestTopW,   "massBestTopW/D");
    tree->Branch("massSecTop",      &massSecTop,     "massSecTop/D");
    tree->Branch("massSecTopW",     &massSecTopW,    "massSecTopW/D");
    tree->Branch("m2ll",            &m2ll,           "m2ll/D");
    tree->Branch("m2bb",            &m2bb,           "m2bb/D");
    tree->Branch("m2lblb",          &m2lblb,         "m2lblb/D");
    tree->Branch("mtLeadLepMET",    &mtLeadLepMET,   "mtLeadLepMET/D");
    tree->Branch("mtSubLeadLepMET", &mtSubLeadLepMET,"mtSubLeadLepMET/D");

    tree->Branch("pt_jet_one",          &ptJetOne,      "pt_jet_one/D");
    tree->Branch("phi_jet_one",         &phiJetOne,      "phi_jet_one/D");
    tree->Branch("eta_jet_one",         &etaJetOne,      "eta_jet_one/D");
    tree->Branch("btag_jet_one",        &btagJetOne,      "btag_jet_one/D");
    tree->Branch("pt_jet_two",          &ptJetTwo,      "pt_jet_two/D");
    tree->Branch("phi_jet_two",         &phiJetTwo,      "phi_jet_two/D");
    tree->Branch("eta_jet_two",         &etaJetTwo,      "eta_jet_two/D");
    tree->Branch("btag_jet_two",        &btagJetTwo,      "btag_jet_two/D");
    tree->Branch("pt_jet_three",        &ptJetThree,      "pt_jet_three/D");
    tree->Branch("phi_jet_three",       &phiJetThree,      "phi_jet_three/D");
    tree->Branch("eta_jet_three",       &etaJetThree,      "eta_jet_three/D");
    tree->Branch("btag_jet_three",      &btagJetThree,      "btag_jet_three/D");
    tree->Branch("pt_jet_four",         &ptJetFour,      "pt_jet_four/D");
    tree->Branch("phi_jet_four",        &phiJetFour,      "phi_jet_four/D");
    tree->Branch("eta_jet_four",        &etaJetFour,      "eta_jet_four/D");
    tree->Branch("btag_jet_four",       &btagJetFour,      "btag_jet_four/D");
    tree->Branch("pt_jet_five",         &ptJetFive,      "pt_jet_five/D");
    tree->Branch("phi_jet_five",        &phiJetFive,      "phi_jet_five/D");
    tree->Branch("eta_jet_five",        &etaJetFive,      "eta_jet_five/D");
    tree->Branch("btag_jet_five",       &btagJetFive,      "btag_jet_five/D");
    tree->Branch("pt_jet_six",          &ptJetSix,      "pt_jet_six/D");
    tree->Branch("phi_jet_six",         &phiJetSix,      "phi_jet_six/D");
    tree->Branch("eta_jet_six",         &etaJetSix,      "eta_jet_six/D");
    tree->Branch("btag_jet_six",        &btagJetSix,      "btag_jet_six/D");
    tree->Branch("pt_jet_seven",        &ptJetSeven,      "pt_jet_seven/D");
    tree->Branch("phi_jet_seven",       &phiJetSeven,      "phi_jet_seven/D");
    tree->Branch("eta_jet_seven",       &etaJetSeven,      "eta_jet_seven/D");
    tree->Branch("btag_jet_seven",      &btagJetSeven,      "btag_jet_seven/D");
    tree->Branch("pt_jet_eight",        &ptJetEight,      "pt_jet_eight/D");
    tree->Branch("phi_jet_eight",       &phiJetEight,      "phi_jet_eight/D");
    tree->Branch("eta_jet_eight",       &etaJetEight,      "eta_jet_eight/D");
    tree->Branch("btag_jet_eight",      &btagJetEight,      "btag_jet_eight/D");
    tree->Branch("pt_jet_nine",         &ptJetNine,      "pt_jet_nine/D");
    tree->Branch("phi_jet_nine",        &phiJetNine,      "phi_jet_nine/D");
    tree->Branch("eta_jet_nine",        &etaJetNine,      "eta_jet_nine/D");
    tree->Branch("btag_jet_nine",       &btagJetNine,      "btag_jet_nine/D");
    tree->Branch("pt_jet_ten",          &ptJetTen,      "pt_jet_ten/D");
    tree->Branch("phi_jet_ten",         &phiJetTen,      "phi_jet_ten/D");
    tree->Branch("eta_jet_ten",         &etaJetTen,      "eta_jet_ten/D");
    tree->Branch("btag_jet_ten",        &btagJetTen,      "btag_jet_ten/D");

    tree->Branch("pt_lep_one",          &ptLepOne,      "pt_lep_one/D");
    tree->Branch("phi_lep_one",         &phiLepOne,      "phi_lep_one/D");
    tree->Branch("eta_lep_one",         &etaLepOne,      "eta_lep_one/D");
    tree->Branch("flav_lep_one",        &flavLepOne,      "flav_lep_one/D");
    tree->Branch("pt_lep_two",          &ptLepTwo,      "pt_lep_two/D");
    tree->Branch("phi_lep_two",         &phiLepTwo,      "phi_lep_two/D");
    tree->Branch("eta_lep_two",         &etaLepTwo,      "eta_lep_two/D");
    tree->Branch("flav_lep_two",        &flavLepTwo,      "flav_lep_two/D");

    if (isML) {
        tree->Branch("pt_lep_three",      &ptLepThree,      "pt_lep_three/D");
        tree->Branch("phi_lep_three",     &phiLepThree,      "phi_lep_three/D");
        tree->Branch("eta_lep_three",     &etaLepThree,      "eta_lep_three/D");
        tree->Branch("flav_lep_three",    &flavLepThree,      "flav_lep_three/D");
    }
}

//void FourTop::BuildVariableMapAlt(TTree* tree, )

void FourTop::createMVATrainingSamples() {
    std::cout << "Sample loop" << std::endl;

    infuseNonPrompt = true;
    currentEvent = new Event();

    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector() );
    //ReweighterBTagShape** btagReweighter = new ReweighterBTagShape*();
//
    //CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter, false );
    //addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");

    delete selection;
    selection = new EventFourTLoose("");

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string outputFileName = "../MVATraining/trainingsamples/" + stringTools::fileNameFromPath(treeReader->currentSample().fileName());

        TFile* currentOutputFile = new TFile(outputFileName.c_str(), "RECREATE");
        currentOutputFile->cd();

        TTree* trainingTree_DL_loose = new TTree("DL_tree_loose", "DL_tree_loose");
        linkMVAVariables(trainingTree_DL_loose, false);
        TTree* trainingTree_ML_loose = new TTree("ML_tree_loose", "ML_tree_loose");
        linkMVAVariables(trainingTree_ML_loose, true);

        std::cout << "Event loop" << std::endl;

        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();
        zgOverlapCheck = treeReader->currentSamplePtr()->zgOverlap();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ) {
            //if (entry > 10000) break;
            delete currentEvent;

            currentEvent = treeReader->buildEventPtr( entry );
            selection->addNewEvent(currentEvent);

            // first mass cleaning just to get rid of shit
            if (! selection->passLowMassVeto()) {
                continue;
            }
            if (! selection->passZBosonVeto()) {
                continue;
            }

            // if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue;
            // if (! currentEvent->passZGOverlap(ttgOverlapCheck)) continue;

            if (! selection->passLeptonSelection()) continue;

            // mvaWeight = currentEvent->weight();
            // mvaWeight *= reweighter.totalWeight( *currentEvent );

            anWeight = currentEvent->weight();
            anWeight *= reweighter.totalWeight( *currentEvent );
            // TODO: fix this if this is ever used
            mcWeight = ((TreeReader*) treeReader)->_weight;
            mvaWeight = 1. * reweighter.totalWeight( *currentEvent );
            if (mcWeight < 0.) mvaWeight *= -1.;

            nonWeight = 1.;

            selection->classifyEvent();
            eventClass currClass = selection->getCurrentClass();

            if (currClass == eventClass::ssdl) {
                fillMVAVariables(false);
                trainingTree_DL_loose->Fill();
            } else if (currClass == eventClass::trilep) {
                fillMVAVariables(true);
                trainingTree_ML_loose->Fill();
                //fillMVAVariablesNormalized(false);
                //trainingTree_ML_loose_Norm->Fill();
            } else if (currClass == eventClass::fourlep) {
                fillMVAVariables(true);
                trainingTree_ML_loose->Fill();
                //fillMVAVariablesNormalized(true);
                //trainingTree_ML_loose_Norm->Fill();
            }
        }
        
        std::cout << trainingTree_ML_loose->GetEntries() << std::endl;
        std::cout << trainingTree_DL_loose->GetEntries() << std::endl;
        trainingTree_DL_loose->Write(trainingTree_DL_loose->GetName(), TObject::kOverwrite);
        trainingTree_ML_loose->Write(trainingTree_ML_loose->GetName(), TObject::kOverwrite);

        currentOutputFile->Close();
    }
}
