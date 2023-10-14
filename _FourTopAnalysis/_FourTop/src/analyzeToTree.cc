#include "../interface/FourTop.h"

#include "../../OutputTreeHandler/interface/OutputTreeHandler.h"

void FourTop::analyzeToTree(std::string method) {
    std::shared_ptr< SampleCrossSections > xsecs;
    std::vector<std::string> processes = {"", "nonPrompt", "ChargeMisID"};


    if (onlyCR) {
        std::cout << "ANALYZING ONLY CR" << std::endl;
    } else {
        std::cout << "ANALYZING SR + CR" << std::endl;
    }

    // Reweighter creation
    // std::map<shapeUncId, std::string> uncTranslateMap;
    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    ReweighterBTagShape** btagReweighter = nullptr;
    CombinedReweighter reweighter;
    CombinedSampleReweighter* sampleReweighter = nullptr;


    if (testRun) std::cout << "initializing" << std::endl;
    //if (! treeReader->sampleVector()[0].isData() && method != "Obs") {
    //    // still needed?
    //    // uncTranslateMap = mgrAll->getTranslateUnc(); 
//
    //    std::cout << "building reweighter" << std::endl;
    //    btagReweighter = new ReweighterBTagShape*();
    //    reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter, testRun );
    //    if (leanEventSelection && !testRun && (considerRegion == eventClass::ttbar || considerRegion == eventClass::dy)) {
    //        addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean_OSDL");
    //    } else if (leanEventSelection && !testRun) {
    //        addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");
    //    } else if (!testRun) {
    //        addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Original");
    //    }
    //    sampleReweighter = createSampleReweighter("ANWeights/SampleNJetSF/");
    //}

    // default methods
    if (method == "MCPrompt") {
        processes = {""};
        if (testRun) std::cout << "Setting selection time" << std::endl;

        selection->setSelectionType(selectionType::MCPrompt);
        st = selectionType::MCPrompt;

        std::cout << "Running method " << "MCPrompt" << std::endl;
    }

    OutputTreeHandler* outputTreeHandler = new OutputTreeHandler(processes);


    // tmp for structure purposes 
    bool isNPControl = false;
    bool splitAdditionalBees = true;
    double chMisCorr = 1.;
    if (testRun) std::cout << "Starting sample loop" << std::endl;
    
    for (unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ) {
        treeReader->initSample();
        std::cout << "init sample" << std::endl;
        // one tree per sample per process
        // sample decides the filename, process the treename
        // naming structure:
        // PROCESS_central
        // PROCESS_sys_VAR
        // need something to manage these trees.

        // prepare run
        //
        TFile* newOutputFile = outputTreeHandler->InitializeNewSample(treeReader->currentSample(), outputFileTags);
        WriteMetadata(newOutputFile);
        if (testRun) std::cout << "Starting event loop" << std::endl;

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            if (testRun && entry > 1000) {
                std::cout << "Cut at 1000 events" << std::endl;
                break;
            }
            //if (entry % 100000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry, false, false, false, true ); // change this last boolean

            // Check triggers here
            if (! eventPassesTriggers()) continue;

            selection->addNewEvent(currentEvent);

            // Apply overlap removal & apply triggers
            // if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"
            // if (! currentEvent->passZGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"

            // if (! treeReader->isData() && useUncertainties) {
            //     numberOfPdfVariations = currentEvent->generatorInfo().numberOfPdfVariations();
            //     if(currentEvent->generatorInfo().numberOfScaleVariations() == 9 ) hasValidQcds = true;
            //     else hasValidQcds = false;
            // 
            //     if (numberOfPdfVariations>=100) hasValidPdfs = true;
            //     else hasValidPdfs = false;
            // }
            // Remove mass resonances
            if (! selection->passLowMassVeto()) {
                continue;
            }

            // Add lepton selection boolean call here!
            if (! selection->passLeptonSelection()) continue;
            if (testRun) std::cout << "pass lepton selection" << std::endl;
            selection->classifyEvent();
            unsigned processNb = 0;
            if (testRun) std::cout << "process nb " << processNb << std::endl;

            // apply correct weights, change processNb if needed
            double weight = currentEvent->weight();
            if( currentEvent->isMC() && (unsigned(st) <= selectionType::MCNoNP)) {
                //weight *= reweighter.totalWeight( *currentEvent );

                if (st == selectionType::MCPrompt) {
                    if (! selection->leptonsArePrompt()) continue;
                    if (! selection->leptonsAreNotChargeFlip() && selection->numberOfLeps() == 2) continue;

                } else if (st == selectionType::MCNoChargeMisID)  {
                    if (! selection->leptonsAreNotChargeFlip()) continue;
                    if (! selection->leptonsArePrompt()) processNb = 1;

                    // if (selection->numberOfLeps() >= 2) continue;
                } else if (st == selectionType::MCNoNP) {
                    if (! selection->leptonsArePrompt()) continue;
                    if (! selection->leptonsAreNotChargeFlip()) processNb = 1;
                } else if (st == selectionType::MCAll && isNPControl) {
                    if (selection->leptonsArePrompt()) continue;
                    // check if there are additional b quarks somewhere
                    if (selection->NumberOfBFlavorJets() > 2) processNb = 1;
                } else {
                    if (! selection->leptonsAreNotChargeFlip()) processNb = 2; 
                    if (! selection->leptonsArePrompt()) processNb = 1;
                }
            } else if (st == selectionType::NPDD) {
                // apply appropriate weights
                weight *= FakeRateWeight();
                for (auto lepton : *selection->getMediumLepCol()) {
                    if (lepton->isFO() && ! lepton->isTight()) {
                        if (lepton->isMuon()) processNb = 1;
                        break;
                    }
                }

                if (currentEvent->isMC() && ! isNPControl) {
                    weight *= -1;
                    // should all leptons be prompt?
                    if (! selection->leptonsArePrompt()) continue;
                }
                if (currentEvent->isMC()) weight *= reweighter.totalWeight( *currentEvent );
            } else if (st == selectionType::ChargeMisDD) {
                // apply appropriate weights
                if (selection->numberOfLeps() > 2) continue; // seems appropriate
                weight *= ChmisIDWeight();
                weight *= chMisCorr;

                if (weight == 0.) continue; // event only contains muons if this is the case
                if (currentEvent->isMC()) weight *= reweighter.totalWeight( *currentEvent );
            }

            if (splitAdditionalBees && st == selectionType::MCPrompt ) {
                //if (currentEvent->GetPLInfoPtr()->GetParticleLevelBees() > nominalBees) processNb = 1;
                if (selection->HasAdditionalBJets()) {
                    processNb = 1;
                }
            }

            eventClass nominalClass = selection->getCurrentClass();
            //if (sampleReweighter && selection->leptonsArePrompt()) weight *= sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
            if (sampleReweighter && selection->leptonsArePrompt() && nominalClass != eventClass::crzz && nominalClass != eventClass::crwz) weight *= sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
            

            if (FillRegion(nominalClass, st)) {
                // check if we need to fill and fill here with correct stuff
                // add some piece in the adding of the event to automatically apply the sys variation we want
                selection->scoreCurrentEvent();
                outputTreeHandler->FillAt(0, selection, weight);
                                
            }
        }
        outputTreeHandler->FlushTrees();
    }

    delete outputTreeHandler;
    outfile->Close();
}
