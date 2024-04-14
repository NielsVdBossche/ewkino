#include "../interface/FourTop.h"

#include "../../OutputTreeHandler/interface/OutputTreeHandler.h"
#include "../../OutputTreeHandler/interface/OutputTreeWeightVar.h"

void FourTop::analyzeToTree(std::string method, std::string uncertaintyflag) {
    std::shared_ptr< SampleCrossSections > xsecs;
    std::vector<std::string> processes = {"", "nonPrompt", "ChargeMisID"};

    if (onlyCR) {
        std::cout << "ANALYZING ONLY CR" << std::endl;
    } else {
        std::cout << "ANALYZING SR + CR" << std::endl;
    }

    double chMisCorr = 0.;
    bool uncertaintySwitch = true;

    // Reweighter creation
    // std::map<shapeUncId, std::string> uncTranslateMap;
    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    ReweighterBTagShape** btagReweighter = nullptr;
    CombinedReweighter reweighter;
    CombinedSampleReweighter* sampleReweighter = nullptr;


    if (testRun) std::cout << "initializing" << std::endl;
    if (! treeReader->sampleVector()[0].isData() && method != "Obs") {
        // still needed?
        // uncTranslateMap = mgrAll->getTranslateUnc(); 

        std::cout << "building reweighter" << std::endl;
        btagReweighter = new ReweighterBTagShape*();
        reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter, testRun );
        if (leanEventSelection && !testRun && (considerRegion == eventClass::ttbar || considerRegion == eventClass::dy)) {
            addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean_OSDL");
        } else if (leanEventSelection && !testRun) {
            addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");
        } else if (!testRun) {
            addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Original");
        }
        sampleReweighter = createSampleReweighter("ANWeights/SampleNJetSF/");
    }

    // default methods
    if (testRun) std::cout << "Setting selection typ" << std::endl;
    if (method == "MCPrompt") {
        processes = {""};

        selection->setSelectionType(selectionType::MCPrompt);
        st = selectionType::MCPrompt;

        std::cout << "Running method " << "MCPrompt" << std::endl;
    } else if (method == "ChargeDD") {
        initDdChargeMisID(&chMisCorr);
        processes = {"ChargeMisID"};
        uncertaintySwitch = false;

        selection->setSelectionType(selectionType::ChargeMisDD);
        st = selectionType::ChargeMisDD;
    } else if (method == "nonPromptDD") {
        initFakerate();
        processes = {"nonPromptElectron", "nonPromptMuon"};
        uncertaintySwitch = false;

        selection->setSelectionType(selectionType::NPDD);
        st = selectionType::NPDD;
    } else if (method == "Obs") {
        processes = {"Data"};
        selection->setSelectionType(selectionType::Data);
        st = selectionType::Data;
        uncertaintySwitch = false;

        std::cout << "Running method " << "Obs" << std::endl;
    }

    // translate to a booltype thing? idk
    std::string outputfile_suffix = "base";
    bool jec_grouped = false;
    bool jec_sources = false;
    bool nominal_run = false;
    std::vector<std::string> uncertaintyNames = {};
    std::vector<shapeUncId> uncertaintyIDs = {};
    if (uncertaintyflag == "nominal") {
        nominal_run = true;
    } else if (uncertaintyflag == "jec_single") {
        uncertaintySwitch = false;
        outputfile_suffix = "JEC";
        uncertaintyNames = {"Unc_JEC_Up", "Unc_JEC_Down"};
        uncertaintyIDs = {shapeUncId::JEC, shapeUncId::JEC};
    } else if (uncertaintyflag == "jec_grouped") {
        uncertaintySwitch = false;
        outputfile_suffix = "GroupedJEC";
        jec_grouped = true;
    } else if (uncertaintyflag == "jec_individual") {
        uncertaintySwitch = false;
        outputfile_suffix = "SourcesJEC";
        jec_sources = true;
    } else if (uncertaintyflag == "jer") {
        uncertaintySwitch = false;
        outputfile_suffix = "JER";
        uncertaintyNames = {"Unc_JER_1p93_Up", "Unc_JER_1p93_Down", "Unc_JER_2p5_Up", "Unc_JER_2p5_Down"};
        uncertaintyIDs = {shapeUncId::JER_1p93, shapeUncId::JER_1p93, shapeUncId::JER_2p5, shapeUncId::JER_2p5};
    } else if (uncertaintyflag == "met") {
        uncertaintySwitch = false;
        outputfile_suffix = "MET";
        uncertaintyNames = {"Unc_METUnclustered_Up", "Unc_METUnclustered_Down"};
        uncertaintyIDs = {shapeUncId::MET, shapeUncId::MET};
    } else if (uncertaintyflag == "hem") {
        if (yearString != "2018") {
            std::cerr << "HEM Issue does not exist in pre-2018. Exiting..." << std::endl;
            exit(1);
        }
        uncertaintySwitch = false;
        outputfile_suffix = "HEMIssue";
        uncertaintyNames = {"Unc_HEMIssue_Up", "Unc_HEMIssue_Down"};
        uncertaintyIDs = {shapeUncId::HEMIssue, shapeUncId::HEMIssue};
    }

    OutputTreeHandler* outputTreeHandler = new OutputTreeHandler(processes, outputSubfolder);

    // tmp for structure purposes 
    bool isNPControl = false;
    bool splitAdditionalBees = false;
    bool uncertaintyExperimentWeight = true & uncertaintySwitch;
    bool uncertaintyTheoryWeight = true & uncertaintySwitch;

    std::vector<std::string> expUncertaintiesSimple = {
        "pileup", "muonIDSyst", "muonIDStat", "electronIDSyst", "electronIDStat", "prefire"
    };
    std::vector<std::string> bTagShapeSystematics;

    
    if (testRun) std::cout << "Starting sample loop" << std::endl;
    for (unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ) {
        treeReader->initSample();
        std::cout << "init sample" << std::endl;
        
        currentEvent = treeReader->buildEventPtr(0, false, false, jec_sources, jec_grouped);

        int numberOfPdfVariations = 0;
        bool hasValidQcds = false;
        bool hasValidPdfs = false;
        bool considerBTagShape = false;
        std::map<std::string, size_t> JECSourcesMapping;
        std::vector<std::string> JECSourcesNames = {};

        if (uncertaintyTheoryWeight && ! treeReader->isData() && st == selectionType::MCPrompt) {
            xsecs = std::make_shared<SampleCrossSections>( treeReader->currentSample() );
        }
        
        if (st == selectionType::MCPrompt) {
            std::string currProcName = treeReader->sampleVector()[sampleIndex].processName();
            outputTreeHandler->ChangeProcess(0, currProcName);

            // need to change processes for the uncertainties
            JetInfo info = currentEvent->jetInfo();
            std::map< std::string, size_t >* a = nullptr;
            if (jec_grouped) {
                // get names
                a = info.groupedJECVariationsMap();
            } else if (jec_sources) {
                a = info.allJECVariationsMap();
            }

            if (a != nullptr) {
                considerBTagShape = ! testRun;
                JECSourcesMapping = *a;
                for (auto var : *a) {
                    if (stringTools::stringContains(var.first, "Total")) continue;
                    JECSourcesNames.push_back(var.first);
                    JECSourcesNames.push_back(var.first);
                    uncertaintyNames.push_back("Unc_" + var.first + "_Up");
                    uncertaintyNames.push_back("Unc_" + var.first + "_Down");
                    uncertaintyIDs.push_back(shapeUncId::JEC);
                    uncertaintyIDs.push_back(shapeUncId::JEC);
                    if (var.first == "FlavorQCD") {
                        JECSourcesNames.push_back(var.first);
                        JECSourcesNames.push_back(var.first);
                        JECSourcesNames.push_back(var.first);
                        JECSourcesNames.push_back(var.first);
                        JECSourcesNames.push_back(var.first);
                        JECSourcesNames.push_back(var.first);
                        uncertaintyIDs.push_back(shapeUncId::JECFlavorQCD);
                        uncertaintyIDs.push_back(shapeUncId::JECFlavorQCD);
                        uncertaintyIDs.push_back(shapeUncId::JECFlavorQCD);
                        uncertaintyIDs.push_back(shapeUncId::JECFlavorQCD);
                        uncertaintyIDs.push_back(shapeUncId::JECFlavorQCD);
                        uncertaintyIDs.push_back(shapeUncId::JECFlavorQCD);
                        uncertaintyNames.push_back("Unc_" + var.first + "_light" + "_Up");
                        uncertaintyNames.push_back("Unc_" + var.first + "_light" + "_Down");
                        uncertaintyNames.push_back("Unc_" + var.first + "_charm" + "_Up");
                        uncertaintyNames.push_back("Unc_" + var.first + "_charm" + "_Down");
                        uncertaintyNames.push_back("Unc_" + var.first + "_bottom" + "_Up");
                        uncertaintyNames.push_back("Unc_" + var.first + "_bottom" + "_Down");
                    }
                }
            }
            if (uncertaintyNames.size() != 0) {
                outputTreeHandler->ChangeAllProcesses(uncertaintyNames);
            }
        }
        // prepare run
        TFile* newOutputFile = outputTreeHandler->InitializeNewSample(treeReader->currentSample(), outputFileTags, method, outputfile_suffix);
        WriteMetadata(newOutputFile);

        if (uncertaintyExperimentWeight && ! treeReader->isData() && st == selectionType::MCPrompt) {
            considerBTagShape = ! testRun;
            if (sampleIndex == 0 && considerBTagShape) {
                bTagShapeSystematics = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->availableSystematics();
            }

            std::vector<std::string> expUncertaintiesAll = expUncertaintiesSimple;
            expUncertaintiesAll.push_back("ElectronReco");
            expUncertaintiesAll.insert(expUncertaintiesAll.end(), bTagShapeSystematics.begin(), bTagShapeSystematics.end());
            outputTreeHandler->WriteExpWeightNaming(expUncertaintiesAll);
        }

        if (testRun) std::cout << "Starting event loop" << std::endl;

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            if (testRun && entry > 1000) {
                std::cout << "Cut at 1000 events" << std::endl;
                break;
            }
            //if (entry % 100000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr(entry, false, false, jec_sources, jec_grouped); // change this last boolean to an option -> basically is this a syst variation run or not

            // Check triggers here
            if (! eventPassesTriggers()) continue;

            selection->addNewEvent(currentEvent);

            // Apply overlap removal & apply triggers
            // if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"
            // if (! currentEvent->passZGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"

            if (! treeReader->isData() && useUncertainties) {
                numberOfPdfVariations = currentEvent->generatorInfo().numberOfPdfVariations();
                if(currentEvent->generatorInfo().numberOfScaleVariations() == 9 ) hasValidQcds = true;
                else hasValidQcds = false;
            
                if (numberOfPdfVariations>=100) hasValidPdfs = true;
                else hasValidPdfs = false;
            }
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
                weight *= reweighter.totalWeight( *currentEvent );

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
            

            if (FillRegion(nominalClass, st) && nominal_run) {
                // check if we need to fill and fill here with correct stuff
                // add some piece in the adding of the event to automatically apply the sys variation we want
                selection->scoreCurrentEvent();
                
                if (uncertaintyTheoryWeight) {               
                    // QCD Variations
                    std::vector<double> qcdvariations;
                    if (hasValidQcds) {
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_1() / xsecs.get()->crossSectionRatio_MuR_2_MuF_1() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_1() / xsecs.get()->crossSectionRatio_MuR_0p5_MuF_1() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_2() / xsecs.get()->crossSectionRatio_MuR_2_MuF_2() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_2() / xsecs.get()->crossSectionRatio_MuR_1_MuF_2() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_0p5() / xsecs.get()->crossSectionRatio_MuR_1_MuF_0p5() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_0p5() / xsecs.get()->crossSectionRatio_MuR_0p5_MuF_0p5() );
                        // Unrenormalized version
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_1());
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_1());
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_2());
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_2());
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_0p5());
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_0p5());
                    } else {
                        qcdvariations = {weight, weight, weight, weight, weight, weight, weight, weight, weight, weight, weight, weight};
                    }
                    ((OutputTreeWeightVar*) outputTreeHandler->GetTree(processNb).get())->SetScaleVariations(qcdvariations);
                    ((OutputTreeWeightVar*) outputTreeHandler->GetTree(processNb).get())->AddPDFVariations(selection, weight, xsecs);

                    std::vector<double> isrfsrVariations;
                    isrfsrVariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_ISR_2() / xsecs.get()->crossSectionRatio_ISR_2());
                    isrfsrVariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_ISR_0p5() / xsecs.get()->crossSectionRatio_ISR_0p5());
                    isrfsrVariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_FSR_2() / xsecs.get()->crossSectionRatio_FSR_2());
                    isrfsrVariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_FSR_0p5() / xsecs.get()->crossSectionRatio_FSR_0p5());
                    ((OutputTreeWeightVar*) outputTreeHandler->GetTree(processNb).get())->SetISRFSRVariations(isrfsrVariations);
                }
                if (uncertaintyExperimentWeight) {
                    // Experimental variations
                    std::vector<double> expUpVar;
                    std::vector<double> expDownVar;

                    // Uncertainties defined earlier: pileup, muon stat, syst, electrons stat, syst, prefire
                    for (auto uncID : expUncertaintiesSimple) {
                        double weightNominalInv = 1. / reweighter[ uncID ]->weight( *currentEvent );
                        expUpVar.push_back(reweighter[ uncID ]->weightUp( *currentEvent ) * weightNominalInv);
                        expDownVar.push_back(reweighter[ uncID ]->weightDown( *currentEvent ) * weightNominalInv);
                    }

                    // electronReco
                    expUpVar.push_back(reweighter[ "electronReco_pTBelow20" ]->weightDown(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weightDown(*currentEvent) 
                        / ( reweighter[ "electronReco_pTBelow20" ]->weight(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weight(*currentEvent) ));
                    expDownVar.push_back(reweighter[ "electronReco_pTBelow20" ]->weightUp(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weightUp(*currentEvent) 
                        / ( reweighter[ "electronReco_pTBelow20" ]->weight(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weight(*currentEvent) ));
                    
                    // bTagging
                    if (considerBTagShape) {
                        double nombweight = reweighter["bTag_shape"]->weight( *currentEvent );
                        for(std::string btagsys : bTagShapeSystematics){
                            expUpVar.push_back(1. * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, btagsys ) / nombweight);
                            expDownVar.push_back(1. * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, btagsys ) / nombweight);
                        }
                    }

                    ((OutputTreeWeightVar*) outputTreeHandler->GetTree(0).get())->SetExperimentalWeightVariations(expUpVar, expDownVar);
                }

                outputTreeHandler->FillAt(processNb, selection, weight);
            } else {
                // Here, variations in variables can be considered explicitely. Loop over what is loaded.
                //std::cout << uncertaintyIDs.size() << std::endl;
                for (unsigned i=0; i < uncertaintyIDs.size(); i++) {
                    shapeUncId uncID = uncertaintyIDs[i];

                    // Even parts should always be up, uneven down
                    bool up = true;
                    if (i % 2 != 0) up = false;
                    double var_weight = 1.;

                    // important when dealing with sources and grouped variation -> 1000 by default
                    unsigned variation_number = 1000;
                    // std::cout << i << std::endl;
                    // std::cout << uncertaintyNames[i] << std::endl;
                    if (uncID == shapeUncId::JEC && (jec_grouped || jec_sources)) {
                        variation_number = JECSourcesMapping[JECSourcesNames[i]];
                        // mif (testRun) std::cout << JECSourcesNames[i] << " " << variation_number << std::endl;
                        if (considerBTagShape) {
                            std::string source = JECSourcesNames[i];
                            if (up) source += "Up";
                            else source += "Down";
                            var_weight = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, source, true, variation_number) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                        }
                    }

                    // arange flavor for JECFlavorQCD
                    unsigned flavor = 0;
                    if (uncID == shapeUncId::JECFlavorQCD) {
                        if (stringTools::stringContains(uncertaintyNames[i], "charm")) flavor = 4;
                        else if (stringTools::stringContains(uncertaintyNames[i], "bottom")) flavor = 5;
                        if (considerBTagShape) {
                            std::string source = "FlavorQCD";
                            if (up) source += "Up";
                            else source += "Down";
                            var_weight = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar_FlavorFilter( *currentEvent, source, flavor) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                        }
                    }
                    eventClass currentClass = selection->classifyUncertainty(shapeUncId(uncID), up, variation_number, flavor);
                    if (FillRegion(currentClass, st)) {
                        // do filling sequence
                        selection->scoreCurrentEvent();
                        outputTreeHandler->FillAt(i, selection, weight * var_weight);
                    }
                }
            }
        }
        outputTreeHandler->FlushTrees();
    }

    delete outputTreeHandler;
    // outfile->Close();
}
