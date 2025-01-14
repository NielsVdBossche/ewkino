#include "../interface/FourTop.h"

#if JECWRAPPER
#include "../../../CMSSW_imports/interface/JECWrapper.h"
#endif

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop::analyze(std::string method) {
    std::map<eventClass, int> offsets;
    std::shared_ptr< SampleCrossSections > xsecs;
    std::vector<std::string> processes = {"", "nonPrompt", "ChargeMisID"};

    //useUncertainties = false;
    useNpNmDistributions = false;
    bool isNPControl = false;
    double chMisCorr = 0.;

    if (onlyCR) {
        std::cout << "ANALYZING ONLY CR" << std::endl;
    } else {
        std::cout << "ANALYZING SR + CR" << std::endl;
    }
    ChannelManager* mgrAll = GenerateChannelManager();
    
    //if (searchRegion == "All") {
    //    mgrAll = new ChannelManager(outfile);
    //} else if (searchRegion == "ConvCR") {
    //    onlyCR = true;
    //    mgrAll = new ChannelManager(outfile, eventClass::cr_conv);
    //    considerRegion = eventClass::cr_conv;
    //    useUncertainties = false;
    //}
    std::map<shapeUncId, std::string> uncTranslateMap; // can we do this so we don't ask it if data

    // reweighter creation

    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    ReweighterBTagShape** btagReweighter = nullptr;
    CombinedReweighter reweighter;
    CombinedSampleReweighter* sampleReweighter = nullptr;
    if (! treeReader->sampleVector()[0].isData() && method != "Obs") {
        uncTranslateMap = mgrAll->getTranslateUnc(); 
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

    offsets = FillHistogramManager(mgrAll);
    selection->setOffsets(offsets);

    if (method == "MCPrompt") {
        processes = {""};
        selection->setSelectionType(selectionType::MCPrompt);
        st = selectionType::MCPrompt;
        // useUncertainties = ! testRun;
        std::cout << "Running method " << "MCPrompt" << std::endl;
    } else if (method == "ChargeDD") {
        initDdChargeMisID(&chMisCorr);
        processes = {"ChargeMisID"};
        selection->setSelectionType(selectionType::ChargeMisDD);
        useUncertainties = false;
        st = selectionType::ChargeMisDD;

        std::cout << "Running method " << "ChargeDD" << std::endl;
    } else if (method == "nonPromptDD") {
        initFakerate();
        processes = {"nonPromptElectron", "nonPromptMuon"};
        selection->setSelectionType(selectionType::NPDD);
        useUncertainties = false;
        st = selectionType::NPDD;

        std::cout << "Running method " << "NP DD" << std::endl;
    } else if (method == "nonPromptDDControl") {
        initFakerate();
        processes = {"nonPromptElectron", "nonPromptMuon"};
        selection->setSelectionType(selectionType::NPDD);
        isNPControl = true;
        useUncertainties = false;
        st = selectionType::NPDD;

        std::cout << "Running method " << "NP DD" << std::endl;
    } else if (method == "Obs") {
        processes = {"Data"};
        selection->setSelectionType(selectionType::Data);
        st = selectionType::Data;
        useUncertainties = false;

        std::cout << "Running method " << "Obs" << std::endl;
    } else if (method == "MCNoChargeMisID") {
        // only run DL channel
        // testmethod for chargeMisID
        processes = {"", "nonPrompt"};
        selection->setSelectionType(selectionType::MCNoChargeMisID);
        st = selectionType::MCNoChargeMisID;
        std::cout << "Running method " << "MCNoChargeMisID" << std::endl;
    } else if (method == "MCNoNP") {
        // testmethod for NP est
        processes = {"", "ChargeMisIDDD"};
        selection->setSelectionType(selectionType::MCNoNP);
        st = selectionType::MCNoNP;
        std::cout << "Running method " << "MCNoNP" << std::endl;
    } else if (method == "MCAllBJetTest") {
        initFakerate();
        processes = {"noAddBs", "AddBs"};
        st = selectionType::MCAll;
        selection->setSelectionType(st);
        useUncertainties = false;
        isNPControl = true;

        std::cout << "Running method " << "MCAllBJetTest" << std::endl;
    } else {
        std::cout << "Running method " << "MCAll" << std::endl;
    }

    if (true) {
        std::cout << "EFT stuff found" << std::endl;
        generateMatrix();

        processes = {"nom", "cQQ8", "cQQ1", "cQt1", "ctt", "cQt8", "ctHRe", "ctHIm", 
                        "cQQ8_cQQ8", "cQQ8_cQQ1", "cQQ8_cQt1", "cQQ8_ctt", "cQQ8_cQt8", "cQQ8_ctHRe", 
                        "cQQ8_ctHIm", "cQQ1_cQQ1", "cQQ1_cQt1", "cQQ1_ctt", "cQQ1_cQt8", "cQQ1_ctHRe", 
                        "cQQ1_ctHIm", "cQt1_cQt1", "cQt1_ctt", "cQt1_cQt8", "cQt1_ctHRe", "cQt1_ctHIm", 
                        "ctt_ctt", "ctt_cQt8", "ctt_ctHRe", "ctt_ctHIm", "cQt8_cQt8", "cQt8_ctHRe", 
                        "cQt8_ctHIm", "ctHRe_ctHRe", "ctHRe_ctHIm", "ctHIm_ctHIm"};
        useUncertainties = false;
        //mgrAll->addSubUncertainties(shapeUncId::eft, eftVariables);
    }
    

    mgrAll->initHistogramStacks(processes, useUncertainties);

    if (st == selectionType::NPDD) {
        std::string frEl = "FRElectron";
        std::string frMu = "FRMuon";

        mgrAll->addUncertainty(frEl, 0);
        mgrAll->addUncertainty(frMu, 1);
        useUncertainties = true;
    }

    std::cout << "event loop" << std::endl;

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    std::vector<std::string> bTagShapeSystematics;
    std::map< std::string, size_t > JECSourcesGrouped;
    std::vector<std::string> JECQCDComponents;
    std::vector<unsigned> JECQCDComponents_flavor;
    std::vector<std::string> wzSFRegions;
    std::vector<std::string> zzSFRegions;
    std::vector<std::string> ttVJetsRegions;
    std::vector<std::string> eftVariables;




    // Rather than histogram manager, set up output tree here based on chosen settings
    // sstd::shared_ptr<OutputTree> outputTree;
    // s// if else: choose which tree to construct. Allow multiple? depending on the settings
    // soutputTree = std::make_shared<OutputTree>();

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        std::cout << "init sample" << std::endl;
        currentEvent = treeReader->buildEventPtr(0, false, false, false, true );

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;

        int numberOfPSVariations = 0;
        int numberOfPdfVariations = 0;
        bool hasValidQcds = false;
        bool hasValidPSs = false;
        bool hasValidPdfs = false;
        bool considerBTagShape = false;
        bool useSplitJEC = false;
        bool splitAdditionalBees = false;
        unsigned nominalBees = 0;
        
        if (! treeReader->isData()) {
            // check if TTbar or TTGamma sample
            ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();
            zgOverlapCheck = treeReader->currentSamplePtr()->zgOverlap();

            std::cout << "ttg sample " << ttgOverlapCheck << std::endl;
            std::cout << "zg sample " << zgOverlapCheck << std::endl;
            if ((st == selectionType::MCAll && !isNPControl) || st == selectionType::MCPrompt || st == selectionType::MCNoNP) {
                std::string currProcName = sampleVec[sampleIndex].processName();
                // if (testRun) std::cout << "changing primary process to " << currProcName << std::endl;
                mgrAll->changePrimaryProcess(currProcName);
                // if (testRun) std::cout << "changed primary process" << std::endl;
                // TODO: hasPL, hasGenLvl flags! -> always true if MC in Nano
                if ((currProcName == "TTZ" || currProcName == "TTW" || currProcName == "TTH") && st == selectionType::MCPrompt && treeReader->hasPL() && treeReader->hasGenLvl()) {
                    std::string bbName = currProcName + "bb";
                    mgrAll->changeProcess(1, bbName);
                    nominalBees = 2;
                    splitAdditionalBees = true;
                }
            }
        }


        // TODO: similar to following code: add variations that we will save in the following steps
        // 
        if (useUncertainties && ! treeReader->isData() && st != selectionType::NPDD) {
            //mgrAll->SetPrintAllUncertaintyVariations(true);
            // MC ONLY (could be changed to MCAll and MCLim options only, but comes down to the same thing)
            // TODO: not compatible with Nano
            xsecs = std::make_shared<SampleCrossSections>( treeReader->currentSample() );

            // std::cout << "finding available PS scale variations...\n";
            Event event = treeReader->buildEvent(0, false, false, false, true);
            // TODO: next line should be fine be test! (get event nb, check if both return same value?)
            numberOfPSVariations = event.generatorInfo().numberOfPsWeights();
            if(numberOfPSVariations>=44) hasValidPSs = true;
            std::cout << "Sample " << treeReader->currentSample().fileName() << " - hasValidPSs: " << hasValidPSs << "\n";

            if(currentEvent->generatorInfo().numberOfScaleVariations() == 9 ) hasValidQcds = true;

            considerBTagShape = ! testRun;
            
            if (sampleIndex == 0 && considerBTagShape) {
                bTagShapeSystematics = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->availableSystematics();
                mgrAll->addSubUncertainties(shapeUncId::bTagShape, bTagShapeSystematics);
            } else if (sampleIndex == 0) {
                bTagShapeSystematics = {"light", "heavy"};
                mgrAll->addSubUncertainties(shapeUncId::bTagShape, bTagShapeSystematics);
            }
            if (considerRegion == eventClass::ttbar || considerRegion == eventClass::dy) {
                useSplitJEC = false;
            }
            if (sampleIndex == 0 && useSplitJEC) {
                std::cout << "split JEC" << std::endl;

                #if JECWRAPPER
                JECWrapper* wrapper = selection->GetJECWrapper();
                std::map< std::string, size_t > a = wrapper->GetUncertaintyMapping();
                std::vector<std::string> inter;

                for (auto& var : a) {
                    inter.push_back(var.first);
                }
                JECSourcesGrouped = a;
                mgrAll->addSubUncertainties(shapeUncId::JEC, inter);
                std::cout << "added variations" << std::endl;

                #else

                JetInfo info = currentEvent->jetInfo();
                std::map< std::string, size_t >* a = info.groupedJECVariationsMap();
                std::vector<std::string> inter;

                for (auto var : *a) {
                    inter.push_back(var.first);
                }

                JECSourcesGrouped = *a;


                mgrAll->addSubUncertainties(shapeUncId::JEC, inter);
                std::cout << "added variations" << std::endl;

                JECQCDComponents = {"light", "charm", "bottom"};
                JECQCDComponents_flavor = {0, 4, 5};
                mgrAll->addSubUncertainties(shapeUncId::JECFlavorQCD, JECQCDComponents);

                #endif

            }
            if (sampleIndex == 0) {
                wzSFRegions = {"0Jet", "1Jet", "2Jet", "3Jet", "4Jet", "5Jet", "6PlusJet"};
                mgrAll->addSubUncertainties(shapeUncId::WZSF, wzSFRegions);
                zzSFRegions = {"0Jet", "1Jet", "2Jet", "3PlusJet"};
                mgrAll->addSubUncertainties(shapeUncId::ZZSF, zzSFRegions);
                ttVJetsRegions = {"AddJets", "AddBJets"};
                mgrAll->addSubUncertainties(shapeUncId::ttvNJetsUnc, ttVJetsRegions);
            }
        }
//        if (treeReader->hasEFT()) {
//            generateMatrix();
//
//            eftVariables = {"nom", "cQQ8", "cQQ1", "cQt1", "ctt", "cQt8", "ctHRe", "ctHIm", 
//                            "cQQ8_cQQ8", "cQQ8_cQQ1", "cQQ8_cQt1", "cQQ8_ctt", "cQQ8_cQt8", "cQQ8_ctHRe", 
//                            "cQQ8_ctHIm", "cQQ1_cQQ1", "cQQ1_cQt1", "cQQ1_ctt", "cQQ1_cQt8", "cQQ1_ctHRe", 
//                            "cQQ1_ctHIm", "cQt1_cQt1", "cQt1_ctt", "cQt1_cQt8", "cQt1_ctHRe", "cQt1_ctHIm", 
//                            "ctt_ctt", "ctt_cQt8", "ctt_ctHRe", "ctt_ctHIm", "cQt8_cQt8", "cQt8_ctHRe", 
//                            "cQt8_ctHIm", "ctHRe_ctHRe", "ctHRe_ctHIm", "ctHIm_ctHIm"};
//
//            mgrAll->addSubUncertainties(shapeUncId::eft, eftVariables);
//        }
//        
        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        if (testRun) std::cout << "new sample " << uniqueName << std::endl;
        mgrAll->newSample(uniqueName);
        if (testRun) std::cout << "new sample done" << std::endl;

        std::ofstream eventTagsOutput;
        if (printEventTags) {
            std::string samplename = treeReader->sampleVector()[0].uniqueName();
            eventTagsOutput.open("Output/EventTags_" + samplename + ".txt");
        }

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            if (testRun && entry > 481) break;
            //if (entry > 10000) break;
            //if (testRun) std::cout << entry << std::endl;
            //if (entry % 100000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry, false, false, false, useSplitJEC );
            if (currentEvent->numberOfTightLeptons() == 2 & testRun) {
                std::cout << entry << std::endl;
                std::cout << currentEvent->eventNumber() << std::endl;
                std::cout << "has 2 tight leptons" << std::endl;
                std::cout << "lepton 1 flav: " << currentEvent->TightLeptonCollection()[0].isElectron() << currentEvent->TightLeptonCollection()[0].isMuon() << std::endl;
                std::cout << "lepton 2 flav: " << currentEvent->TightLeptonCollection()[1].isElectron() << currentEvent->TightLeptonCollection()[1].isMuon() << std::endl;
                std::cout << "lepton 1 charge: " << currentEvent->TightLeptonCollection()[0].charge() << std::endl;
                std::cout << "lepton 2 charge: " << currentEvent->TightLeptonCollection()[1].charge() << std::endl;
                std::cout << "lepton 1 pt: " << currentEvent->TightLeptonCollection()[0].pt() << std::endl;
                std::cout << "lepton 2 pt: " << currentEvent->TightLeptonCollection()[1].pt() << std::endl;
            } else if (testRun) {
                continue;
            }
            // Check triggers here
            if (! eventPassesTriggers()) {
                // std::cout << currentEvent->triggerInfo().passTriggers_e() 
                // << currentEvent->triggerInfo().passTriggers_ee() 
                // << currentEvent->triggerInfo().passTriggers_eee() 
                // << currentEvent->triggerInfo().passTriggers_m() 
                // << currentEvent->triggerInfo().passTriggers_mm() 
                // << currentEvent->triggerInfo().passTriggers_mmm() 
                // << currentEvent->triggerInfo().passTriggers_em() 
                // << currentEvent->triggerInfo().passTriggers_et() 
                // << currentEvent->triggerInfo().passTriggers_mt() 
                // << currentEvent->triggerInfo().passTriggers_eem() 
                // << currentEvent->triggerInfo().passTriggers_emm() 
                // << currentEvent->triggerInfo().passTriggers_FR() 
                // << currentEvent->triggerInfo().passTriggers_FR_iso() << std::endl;
                // if (testRun) std::cerr << "Event does not pass triggers" << std::endl;
                continue;
            }


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
                if (testRun) std::cout << "fail low mass veto" << std::endl;
                continue;
            }

            // Add lepton selection boolean call here!

            if (! selection->passLeptonSelection()) continue;
            if (testRun) std::cout << "pass lepton selection" << std::endl;
            selection->classifyEvent();
            unsigned processNb = 0;
            if (testRun) std::cout << "process nb " << processNb << std::endl;
            if (testRun) std::cout << selection->getCurrentClass() << std::endl;
            double weight = currentEvent->weight();
            if( currentEvent->isMC() && (unsigned(st) <= selectionType::MCNoNP)) {
                weight *= reweighter.totalWeight( *currentEvent );

                if (st == selectionType::MCPrompt) {
                    //std::cout << "prompt check" << std::endl;
                    if (! selection->leptonsArePrompt()) continue;
                    //std::cout << "charge flip check" << std::endl;
                    if (! selection->leptonsAreNotChargeFlip() && selection->numberOfLeps() == 2) continue;
                    //std::cout << "success" << std::endl;
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
                // TODO: matching between PL and Gen lvl -> not checked PL equivalent, so unlikely to work.
                if (selection->HasAdditionalBJets()) {
                    processNb = 1;
                }
            }

            // Basic non-prompt handling (using MC to estimate the contribution):
            std::vector<double> fillVec;
            std::vector<std::pair<double, double>> fillVec2D;
            std::vector<std::pair<int, double>> singleEntries;
            std::vector<std::pair<int, double>> singleEntriesNpNm;
            std::vector<std::string> subChannels;

            // fill all histograms
            // replace with functions in eventHandling?

            eventClass nominalClass = selection->getCurrentClass();
            std::cout << nominalClass << std::endl;
            //if (sampleReweighter && selection->leptonsArePrompt()) weight *= sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
            if (sampleReweighter && selection->leptonsArePrompt() && nominalClass != eventClass::crzz && nominalClass != eventClass::crwz) weight *= sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());

            // make a bool for filling checks. 
            // 

            // if signal region && onlyCR: skip
            // if chargeMisDD && not dilep: skip
            // if region != considerRegion && considerRegion != fail: skip

            if (FillRegion(nominalClass, st)) {
                if (testRun) std::cout << "is fill in " << nominalClass << std::endl;
                if (testRun) std::cout << currentEvent->weight() << std::endl;
                fillVec = selection->fillVector();
                singleEntries = selection->singleFillEntries();
                fillVec2D = selection->fillVector2D();
                subChannels = GetSubClasses(nominalClass);

                // make function bundling this behaviour.
                mgrAll->at(nominalClass)->fillAllHistograms(subChannels, processNb, fillVec, weight);
                mgrAll->at(nominalClass)->fillAll2DHistograms(subChannels, processNb, fillVec2D, weight);
                mgrAll->at(nominalClass)->fillAllSingleHistograms(subChannels, processNb, singleEntries, weight);

                if (useNpNmDistributions) {
                    singleEntriesNpNm = FillNpNmDistributions(nominalClass, offsets);
                    if (selection->getLepton(0)->charge() > 0) {
                        mgrAll->at(nominalClass)->fillAllSingleHistograms(subChannels, processNb, singleEntriesNpNm, weight);
                    } else {
                        mgrAll->at(nominalClass)->fillAllSingleHistograms(subChannels, processNb, singleEntriesNpNm, -1. * weight);
                    }
                }

                if (treeReader->hasEFT()) {
                    // loop over processes and add all weights
                    std::vector<double> weightVar = transformWeights(currentEvent->generatorInfo().getNEFTWeights(), currentEvent->generatorInfo().getEFTWeights());

                    for (unsigned i = 1; i<processes.size(); i++) {
                        mgrAll->at(nominalClass)->fillAllHistograms(subChannels, i, fillVec, weightVar[i-1]);
                        mgrAll->at(nominalClass)->fillAll2DHistograms(subChannels, i, fillVec2D, weightVar[i-1]);
                        mgrAll->at(nominalClass)->fillAllSingleHistograms(subChannels, i, singleEntries, weightVar[i-1]);
                    }
                }

                // Filling step: fill nominal variables here or apply correct variation and fill variables
                // I think ideally we don't want to use the old way anymore... Maybe separate loop to keep it simple?
            }


            // Systematics
            if (! useUncertainties) continue;
            if ((currentEvent->isData() && st != selectionType::NPDD) || (processNb > 0 && (st != selectionType::MCPrompt && st != selectionType::NPDD))) continue;

            if (testRun) std::cout << "uncertainty time" << std::endl;
            //// Start filling histograms
            // loop uncertainties
            Channel* uncWrapper;
            if ((selection->getCurrentClass() != eventClass::fail && considerRegion == eventClass::fail) || (considerRegion != eventClass::fail && selection->getCurrentClass() == considerRegion)) {
                uncWrapper = mgrAll->at(selection->getCurrentClass());
            } else {
                uncWrapper = nullptr;
            }

            std::vector<double> fillVecUp = fillVec;
            std::vector<double> fillVecDown = fillVec;
            std::vector<std::pair<int, double>> singleEntriesUp = singleEntries;
            std::vector<std::pair<int, double>> singleEntriesDown = singleEntries;
            std::vector<std::pair<double, double>> fillVec2DUp = fillVec2D;
            std::vector<std::pair<double, double>> fillVec2DDown = fillVec2D;
            std::vector<std::pair<int, double>> singleEntriesNpNmUp = singleEntriesNpNm;
            std::vector<std::pair<int, double>> singleEntriesNpNmDown = singleEntriesNpNm;


            unsigned uncID = 0;
            while (uncID < shapeUncId::end) {
                if (( !uncWrapper || (onlyCR && unsigned(nominalClass) >= eventClass::ssdl) || (considerRegion != eventClass::fail && nominalClass != considerRegion)) && uncID < shapeUncId::JER_1p93) {
                    uncID++;
                    continue;
                }
                if (st == selectionType::NPDD && uncID > 1) break; 
                double weightUp = 1.;
                double weightDown = 1.;
                eventClass upClass = eventClass::fail;
                eventClass downClass = eventClass::fail;
                std::vector<std::string> subChannelsUp;
                std::vector<std::string> subChannelsDown;

                //if (testRun) std::cout << uncID << std::endl;

                if (uncID == 0 && st == selectionType::NPDD) {
                    weightUp = FakeRateWeightVariation(true, true) / FakeRateWeight();
                    weightDown = FakeRateWeightVariation(false, true) / FakeRateWeight();
                } else if (uncID == 1 && st == selectionType::NPDD) {
                    weightUp = FakeRateWeightVariation(true, false) / FakeRateWeight();
                    weightDown = FakeRateWeightVariation(false, false) / FakeRateWeight();
                } else if (uncID <= shapeUncId::pileup) {
                    // all uncertainties with simple reweighting
                    std::string id = uncTranslateMap[shapeUncId(uncID)];
                    double weightNominalInv = 1. / reweighter[ id ]->weight( *currentEvent );
                    weightUp = reweighter[ id ]->weightUp( *currentEvent ) * weightNominalInv;
                    weightDown = reweighter[ id ]->weightDown( *currentEvent ) * weightNominalInv;
                } else if (uncID == shapeUncId::qcdScale) {
                    std::vector<double> qcdvariations;
                    if (hasValidQcds) {
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_1() / xsecs.get()->crossSectionRatio_MuR_2_MuF_1() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_1() / xsecs.get()->crossSectionRatio_MuR_0p5_MuF_1() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_2() / xsecs.get()->crossSectionRatio_MuR_2_MuF_2() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_2() / xsecs.get()->crossSectionRatio_MuR_1_MuF_2() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_0p5() / xsecs.get()->crossSectionRatio_MuR_1_MuF_0p5() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_0p5() / xsecs.get()->crossSectionRatio_MuR_0p5_MuF_0p5() );
                    } else {
                        qcdvariations = {weight, weight, weight, weight, weight, weight};
                    }

                    uncWrapper->fillAllEnvelope(subChannels, shapeUncId::qcdScale, processNb, fillVec, qcdvariations);
                    uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::qcdScale, processNb, singleEntries, qcdvariations);
                    uncWrapper->fillAllEnvelope2Ds(subChannels, shapeUncId::qcdScale, processNb, fillVec2D, qcdvariations);
                    if (useNpNmDistributions) {
                        if (selection->getLepton(0)->charge() > 0) {
                            uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::qcdScale, processNb, singleEntriesNpNm, qcdvariations);
                        } else {
                            std::vector<double> qcdVariationsNegs;
                            for (auto qcdVar : qcdvariations) {
                                qcdVariationsNegs.push_back(-1. * qcdVar);
                            }
                            uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::qcdScale, processNb, singleEntriesNpNm, qcdVariationsNegs);
                        }
                    }
                } else if (uncID == shapeUncId::pdfShapeVar) {
                    std::vector<double> pdfVariations;
                    if (hasValidPdfs && xsecs.get()->numberOfLheVariations() > 9) {
                        int max = 100;
                        if (numberOfPdfVariations < max) {
                            max = numberOfPdfVariations;
                        }
                        //if (isSignalSample) {
                        for(int i=1; i<max+1; ++i){
                            pdfVariations.push_back(weight * currentEvent->generatorInfo().relativeWeightPdfVar(i) / xsecs.get()->crossSectionRatio_pdfVar(i));
                        }
                        //} else {
                        //    for(int i=1; i<max+1; ++i){
                        //        pdfVariations.push_back(weight * currentEvent->generatorInfo().relativeWeightPdfVar(i));
                        //    }
                        //}
                    } else {
                        for (unsigned i = pdfVariations.size(); i<100; i++) {
                            pdfVariations.push_back(weight);
                        }
                    }

                    uncWrapper->fillAllEnvelope(subChannels, shapeUncId::pdfShapeVar, processNb, fillVec, pdfVariations);
                    uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::pdfShapeVar, processNb, singleEntries, pdfVariations);
                    uncWrapper->fillAllEnvelope2Ds(subChannels, shapeUncId::pdfShapeVar, processNb, fillVec2D, pdfVariations);
                    if (useNpNmDistributions) {
                        if (selection->getLepton(0)->charge() > 0) {
                            uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::pdfShapeVar, processNb, singleEntriesNpNm, pdfVariations);
                        } else {
                            std::vector<double> pdfVariationsNegs;
                            for (auto pdfVar : pdfVariations) {
                                pdfVariationsNegs.push_back(-1. * pdfVar);
                            }
                            uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::pdfShapeVar, processNb, singleEntriesNpNm, pdfVariationsNegs);
                        }
                    }
                } else if (uncID == shapeUncId::bTagShape) {
                    if (considerBTagShape) {
                        double nombweight = reweighter["bTag_shape"]->weight( *currentEvent );
                        for(std::string btagsys : bTagShapeSystematics){
                            weightUp = 1. * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, btagsys ) / nombweight;
                            weightDown = 1. * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, btagsys ) / nombweight;

                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, fillVec, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntries, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, fillVec2D, weight * weightUp, weight * weightDown);
                            if (useNpNmDistributions) {
                                if (selection->getLepton(0)->charge() > 0) {
                                    uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntriesNpNm, weight * weightUp, weight * weightDown);
                                } else {
                                    uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntriesNpNm, -1. * weight * weightUp, -1. * weight * weightDown);
                                }
                            }
                        }

                        weightUp = 1.;
                        weightDown = 1.;
                    } //else {
                    //    for(std::string btagsys : bTagShapeSystematics){
                    //        std::string btagString = "bTag_tight_WP_" + btagsys;
                    //        double nombweight = reweighter[btagString]->weight( *currentEvent );
                    //        weightUp = 1. * reweighter[btagString]->weightUp( *currentEvent) / nombweight;
                    //        weightDown = 1. * reweighter[btagString]->weightDown( *currentEvent) / nombweight;
                    //
                    //        uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, fillVec, weight * weightUp, weight * weightDown);
                    //        uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntries, weight * weightUp, weight * weightDown);
                    //        uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, fillVec2D, weight * weightUp, weight * weightDown);
                    //        if (useNpNmDistributions) {
                    //            if (selection->getLepton(0)->charge() > 0) {
                    //                uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntriesNpNm, weight * weightUp, weight * weightDown);
                    //            } else {
                    //                uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntriesNpNm, -1. * weight * weightUp, -1. * weight * weightDown);
                    //            }
                    //        }
                    //    }
                    //
                    //    weightUp = 1.;
                    //    weightDown = 1.;
                    //}
                } else if (uncID == shapeUncId::isrShape && hasValidPSs) {                  
                    weightUp = currentEvent->generatorInfo().relativeWeight_ISR_2() / xsecs.get()->crossSectionRatio_ISR_2();
                    weightDown = currentEvent->generatorInfo().relativeWeight_ISR_0p5() / xsecs.get()->crossSectionRatio_ISR_0p5();
                } else if (uncID == shapeUncId::isrNorm && hasValidPSs) {
                    weightUp = xsecs.get()->crossSectionRatio_ISR_2();
                    weightDown = xsecs.get()->crossSectionRatio_ISR_0p5();
                } else if (uncID == shapeUncId::fsrShape && hasValidPSs) {
                    weightUp = currentEvent->generatorInfo().relativeWeight_FSR_2() / xsecs.get()->crossSectionRatio_FSR_2();
                    weightDown = currentEvent->generatorInfo().relativeWeight_FSR_0p5() / xsecs.get()->crossSectionRatio_FSR_0p5();
                } else if (uncID == shapeUncId::fsrNorm && hasValidPSs) {
                    weightUp = xsecs.get()->crossSectionRatio_FSR_2();
                    weightDown = xsecs.get()->crossSectionRatio_FSR_0p5();
                } else if (uncID == shapeUncId::electronReco) {
                    weightDown *= reweighter[ "electronReco_pTBelow20" ]->weightDown(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weightDown(*currentEvent) 
                        / ( reweighter[ "electronReco_pTBelow20" ]->weight(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weight(*currentEvent) );
                    weightUp *= reweighter[ "electronReco_pTBelow20" ]->weightUp(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weightUp(*currentEvent) 
                        / ( reweighter[ "electronReco_pTBelow20" ]->weight(*currentEvent) * reweighter[ "electronReco_pTAbove20" ]->weight(*currentEvent) );
                } else if (uncID == shapeUncId::WZSF) {
                    if (sampleReweighter) {
                        for (int i=0; i < 7; i++) {
                            weightUp = 1.;
                            weightDown = 1.;
                            if (selection->numberOfJets() == i || (i == 6 && selection->numberOfJets() >= 6)) {
                                weightUp = 1. * sampleReweighter->totalWeightUp(*currentEvent, selection->numberOfJets()) / sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
                                weightDown = 1. * sampleReweighter->totalWeightDown(*currentEvent, selection->numberOfJets()) / sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
                            }
                            std::string wzSFRegion = wzSFRegions[i];
                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntries, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec2D, weight * weightUp, weight * weightDown);
                            if (useNpNmDistributions) {
                                if (selection->getLepton(0)->charge() > 0) {
                                    uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntriesNpNm, weight * weightUp, weight * weightDown);
                                } else {
                                    uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntriesNpNm, -1. * weight * weightUp, -1. * weight * weightDown);
                                }
                            }
                        }
                    } else {
                        for (std::string wzSFRegion : wzSFRegions) {
                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec, weight, weight);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntries, weight, weight);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec2D, weight, weight);
                            if (useNpNmDistributions) {
                                if (selection->getLepton(0)->charge() > 0) {
                                    uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntriesNpNm, weight, weight);
                                } else {
                                    uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntriesNpNm, -1. * weight, -1. * weight);
                                }
                            }
                        }
                    }
                } else if (uncID == shapeUncId::ZZSF) {
                    //if (testRun) std::cout << "ZZSF unc" << std::endl;
                    if (sampleReweighter) {
                       // if (testRun) std::cout << "has rew" << std::endl;
                        for (int i = 0; i < 4; i++) {

                            weightUp = 1.;
                            weightDown = 1.;
                            if (selection->numberOfJets() == i || (i == 3 && selection->numberOfJets() >= 3)) {
                                weightUp = 1. * sampleReweighter->totalWeightUp(*currentEvent, selection->numberOfJets()) / sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
                                weightDown = 1. * sampleReweighter->totalWeightDown(*currentEvent, selection->numberOfJets()) / sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
                            }
                            std::string zzSFRegion = zzSFRegions[i];
                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, zzSFRegion, fillVec, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, zzSFRegion, singleEntries, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, zzSFRegion, fillVec2D, weight * weightUp, weight * weightDown);
                        }
                    } else {
                        //if (testRun) std::cout << "has no rew" << std::endl;

                        for (std::string zzSFRegion : zzSFRegions) {
                            //if (testRun) std::cout << "fill " << zzSFRegion << std::endl;

                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, zzSFRegion, fillVec, weight, weight);
                            //if (testRun) std::cout << "fill all" << zzSFRegion << std::endl;

                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, zzSFRegion, singleEntries, weight, weight);
                            //if (testRun) std::cout << "fill single" << zzSFRegion << std::endl;

                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, zzSFRegion, fillVec2D, weight, weight);
                            //if (testRun) std::cout << "fill 2d" << zzSFRegion << std::endl;

                        }
                        //if (testRun) std::cout << "done fill" << std::endl;

                    }
                } else if (uncID == eft && treeReader->hasEFT()) {
                    std::vector<double> weightVar = transformWeights(currentEvent->generatorInfo().getNEFTWeights(), currentEvent->generatorInfo().getEFTWeights());
                    //for (unsigned eftID = 0; eftID < weightVar.size(); eftID++) {
                    //        uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, eftVariables[eftID], fillVec, weightVar[eftID], 0.);
                    //        uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, eftVariables[eftID], singleEntries, weightVar[eftID], 0.);
                    //        uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, eftVariables[eftID], fillVec2D, weightVar[eftID], 0.);
                    //}
                } else if (uncID == ttvNJetsUnc) {
                    for (int i=0; i < 2; i++) {
                        weightUp = 1.;
                        weightDown = 1.;
                        if (i == 0) {
                            if (selection->numberOfLeps() == 2) {
                                if (selection->numberOfJets() <= 2) {
                                    weightUp = 0.99;//0.92;
                                } else if (selection->numberOfJets() == 3) {
                                    weightUp = 0.99;//0.95;
                                } else if (selection->numberOfJets() == 4) {
                                    weightUp = 1.07;//1.03;
                                } else if (selection->numberOfJets() == 5) {
                                    weightUp = 1.18;//1.20;
                                } else if (selection->numberOfJets() == 6) {
                                    weightUp = 1.30;//1.42;
                                } else if (selection->numberOfJets() >= 7) {
                                    weightUp = 1.32;//1.55;
                                } 
                            }
                            if (selection->numberOfLeps() >= 3) {
                                if (selection->numberOfJets() <= 2) {
                                    weightUp = 1.1;//0.96;
                                } else if (selection->numberOfJets() == 3) {
                                    weightUp = 1.17;//1.16;
                                } else if (selection->numberOfJets() == 4) {
                                    weightUp = 1.22;//1.28;
                                } else if (selection->numberOfJets() == 5) {
                                    weightUp = 1.28;//1.46;
                                } else if (selection->numberOfJets() == 6) {
                                    weightUp = 1.22;//1.44;
                                } else if (selection->numberOfJets() >= 7) {
                                    weightUp = 1.18;//1.44;
                                } 
                            }
                            weightDown = 1.;
                        } else if (i==1) {
                            if (splitAdditionalBees && st == selectionType::MCPrompt ) {
                                if (selection->HasAdditionalBJets()) {
                                    weightUp = 1.4;
                                    weightDown = 0.6;
                                }
                            }
                        }
                        std::string ttVJetsRegion = ttVJetsRegions[i];
                        uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, ttVJetsRegion, fillVec, weight * weightUp, weight * weightDown);
                        uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, ttVJetsRegion, singleEntries, weight * weightUp, weight * weightDown);
                        uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, ttVJetsRegion, fillVec2D, weight * weightUp, weight * weightDown);
                        if (useNpNmDistributions) {
                            if (selection->getLepton(0)->charge() > 0) {
                                uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, ttVJetsRegion, singleEntriesNpNm, weight * weightUp, weight * weightDown);
                            } else {
                                uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, ttVJetsRegion, singleEntriesNpNm, -1. * weight * weightUp, -1. * weight * weightDown);
                            }
                        }
                    }
                    weightUp = 1.;
                    weightDown = 1.;
                    
                } else if ((uncID >= shapeUncId::JER_1p93 && (uncID != shapeUncId::JEC && uncID != shapeUncId::JECFlavorQCD)) || (uncID == shapeUncId::JEC && !useSplitJEC)) {
                    // JER and JEC
                    if( uncID == shapeUncId::JEC && considerBTagShape ) {
                        //weightUp = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, "JECUp" ) 
                        //                    / reweighter["bTag_shape"]->weight( *currentEvent );
                        //weightDown = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, "JECDown" ) 
                        //                    / reweighter["bTag_shape"]->weight( *currentEvent );
                    }
                    std::string empty = "";

                    upClass = selection->classifyUncertainty(shapeUncId(uncID), true, 1000);
                    fillVecUp = selection->fillVector();
                    singleEntriesUp = selection->singleFillEntries();
                    fillVec2DUp = selection->fillVector2D();
                    if (useNpNmDistributions) singleEntriesNpNmUp = FillNpNmDistributions(upClass, offsets);

                    downClass = selection->classifyUncertainty(shapeUncId(uncID), false, 1000);
                    fillVecDown = selection->fillVector();
                    singleEntriesDown = selection->singleFillEntries();
                    fillVec2DDown = selection->fillVector2D();
                    if (useNpNmDistributions) singleEntriesNpNmDown = FillNpNmDistributions(downClass, offsets);

                    subChannelsUp = GetSubClasses(upClass);
                    subChannelsDown = GetSubClasses(downClass);

                    if (FillRegion(upClass, st)) mgrAll->fillAllUpHistograms(subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecUp, singleEntriesUp, fillVec2DUp, weight * weightUp);
                    if (FillRegion(downClass, st)) mgrAll->fillAllDownHistograms(subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecDown, singleEntriesDown, fillVec2DDown, weight * weightDown);
                    
                    if (useNpNmDistributions) {
                        double mod = 1.;
                        std::vector<double> fillVecEmpty = {};
                        std::vector<std::pair<double, double>> fillVec2DEmpty = {};


                        if (selection->getLepton(0)->charge() > 0) mod = -1.;
                        if (FillRegion(upClass, st)) mgrAll->fillAllUpHistograms(subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecEmpty, singleEntriesNpNmUp, fillVec2DEmpty, mod * weight * weightUp);
                        if (FillRegion(downClass, st)) mgrAll->fillAllDownHistograms(subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecEmpty, singleEntriesNpNmDown, fillVec2DEmpty, mod * weight * weightDown);
                    }
                } else if (uncID == shapeUncId::JEC && useSplitJEC) {
                    //if (testRun) std::cout << "prestart? " << std::endl;

                    for (auto jecSource : JECSourcesGrouped) {
                        //std::cout << "start? " << std::endl;

                        if (stringTools::stringContains(jecSource.first, "Total")) continue;
                        std::string jecSourceStr = jecSource.first;
                        if (considerBTagShape) {
                            #if JECWRAPPER
                            weightUp = 1.;
                            weightDown = 1.;
                            #else
                            std::string sourceUp = jecSourceStr + "Up";
                            std::string sourceDown = jecSourceStr + "Down";

                            weightUp = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, sourceUp, true, jecSource.second) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                            weightDown = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, sourceDown, true, jecSource.second) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                            #endif
                        }
                        //std::cout << "classify? " << std::endl;


                        upClass = selection->classifyUncertainty(shapeUncId(uncID), true, jecSource.second);
                        //std::cout << "classify unc " << std::endl;

                        fillVecUp = selection->fillVector();
                        //std::cout << "fillvec " << std::endl;

                        singleEntriesUp = selection->singleFillEntries();
                       // std::cout << "singles " << std::endl;

                        fillVec2DUp = selection->fillVector2D();
                        //std::cout << "try first? " << std::endl;

                        if (useNpNmDistributions) singleEntriesNpNmUp = FillNpNmDistributions(upClass, offsets);
                        //std::cout << "first done? " << std::endl;


                        downClass = selection->classifyUncertainty(shapeUncId(uncID), false, jecSource.second);
                        fillVecDown = selection->fillVector();
                        singleEntriesDown = selection->singleFillEntries();
                        fillVec2DDown = selection->fillVector2D();
                        if (useNpNmDistributions) singleEntriesNpNmDown = FillNpNmDistributions(downClass, offsets);

                        subChannelsUp = GetSubClasses(upClass);
                        subChannelsDown = GetSubClasses(downClass);

                        if (FillRegion(upClass, st)) mgrAll->fillAllSubUpHistograms(jecSourceStr, subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecUp, singleEntriesUp, fillVec2DUp, weight * weightUp);
                        if (FillRegion(downClass, st)) mgrAll->fillAllSubDownHistograms(jecSourceStr, subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecDown, singleEntriesDown, fillVec2DDown, weight * weightDown);
                        if (useNpNmDistributions) {
                            //std::cout << "problem? " << std::endl;

                            double mod = 1.;
                            std::vector<double> fillVecEmpty = {};
                            std::vector<std::pair<double, double>> fillVec2DEmpty = {};
                            //std::cout << "set? " << std::endl;

                            if (selection->getLepton(0)->charge() > 0) mod = -1.;
                            //std::cout << "got mod? " << std::endl;

                            if (FillRegion(upClass, st)) {
                                //std::cout << "fill up? " << std::endl;
                             
                                mgrAll->fillAllSubUpHistograms(jecSourceStr, subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecEmpty, singleEntriesNpNmUp, fillVec2DEmpty, mod * weight * weightUp);
                            }
                            if (FillRegion(downClass, st)) {
                                //std::cout << "fill down? " << std::endl;
                                mgrAll->fillAllSubDownHistograms(jecSourceStr, subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecEmpty, singleEntriesNpNmDown, fillVec2DEmpty, mod * weight * weightDown);
                            }
                        }
                    }
                } else if (uncID == shapeUncId::JECFlavorQCD && useSplitJEC) {
                    #if ! JECWRAPPER
                    for (int i = 0; i < 3; i++) {
                        std::string source = JECQCDComponents[i];
                        std::string jecVar = "FlavorQCD";
                        unsigned flavor = JECQCDComponents_flavor[i];
                        if (considerBTagShape) {
                            std::string sourceUp = "FlavorQCDUp";
                            std::string sourceDown = "FlavorQCDDown";
                            
                            weightUp = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar_FlavorFilter( *currentEvent, sourceUp, flavor) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                            weightDown = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar_FlavorFilter( *currentEvent, sourceDown, flavor) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                        }

                        upClass = selection->classifyUncertainty(shapeUncId(uncID), true, 1000, flavor);
                        fillVecUp = selection->fillVector();
                        singleEntriesUp = selection->singleFillEntries();
                        fillVec2DUp = selection->fillVector2D();
                        if (useNpNmDistributions) singleEntriesNpNmUp = FillNpNmDistributions(upClass, offsets);

                        downClass = selection->classifyUncertainty(shapeUncId(uncID), false, 1000, flavor);
                        fillVecDown = selection->fillVector();
                        singleEntriesDown = selection->singleFillEntries();
                        fillVec2DDown = selection->fillVector2D();
                        if (useNpNmDistributions) singleEntriesNpNmDown = FillNpNmDistributions(downClass, offsets);

                        subChannelsUp = GetSubClasses(upClass);
                        subChannelsDown = GetSubClasses(downClass);

                        if (FillRegion(upClass, st)) mgrAll->fillAllSubUpHistograms(source, subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecUp, singleEntriesUp, fillVec2DUp, weight * weightUp);
                        if (FillRegion(downClass, st)) mgrAll->fillAllSubDownHistograms(source, subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecDown, singleEntriesDown, fillVec2DDown, weight * weightDown);
                        if (useNpNmDistributions) {
                            double mod = 1.;
                            std::vector<double> fillVecEmpty = {};
                            std::vector<std::pair<double, double>> fillVec2DEmpty = {};

                            if (selection->getLepton(0)->charge() > 0) mod = -1.;
                            if (FillRegion(upClass, st)) mgrAll->fillAllSubUpHistograms(source, subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecEmpty, singleEntriesNpNmUp, fillVec2DEmpty, mod * weight * weightUp);
                            if (FillRegion(downClass, st)) mgrAll->fillAllSubDownHistograms(source, subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecEmpty, singleEntriesNpNmDown, fillVec2DEmpty, mod * weight * weightDown);
                        }
                    }
                    #endif
                }

                if (uncID < shapeUncId::JER_1p93) {
                    uncWrapper->fillAllUncertainties(subChannels, shapeUncId(uncID), processNb, fillVec, weight * weightUp, weight * weightDown);
                    uncWrapper->fillAllSingleUncertainties(subChannels, shapeUncId(uncID), processNb, singleEntries, weight * weightUp, weight * weightDown);
                    uncWrapper->fillAll2DUncertainties(subChannels, shapeUncId(uncID), processNb, fillVec2D, weight * weightUp, weight * weightDown);
                    
                    if (useNpNmDistributions) {
                        if (selection->getLepton(0)->charge() > 0) {
                            uncWrapper->fillAllSingleUncertainties(subChannels, shapeUncId(uncID), processNb, singleEntriesNpNm, weight * weightUp, weight * weightDown);
                        } else {
                            uncWrapper->fillAllSingleUncertainties(subChannels, shapeUncId(uncID), processNb, singleEntriesNpNm, -1. * weight * weightUp, -1. * weight * weightDown);
                        }
                    }
                }

                uncID = uncID + 1;

            }
        }
        
        // Output management: save histograms to a ROOT file.
        // Processes were named in samplelist. Should use this to make directory for process and subdir for sample

        // Might interface with Stacker to create desired output plots as well... Or at least already have the stacked process ready instead of individual components. Then a "getDirectory" in stacker could be handy to see if it exists.
        std::cout << "output" << std::endl;
        
        outfile->cd();
        outfile->cd("Nominal");
    
        std::string outdir = stringTools::fileNameWithoutExtension(stringTools::fileNameFromPath(treeReader->currentSample().fileName()));

        mgrAll->writeNominalHistograms(outdir);

        //gDirectory->mkdir("analytics");
        //gDirectory->cd("analytics");
        if (useUncertainties) {
            //std::cout << "writing uncertainties" << std::endl;

            outfile->cd();
            outfile->cd("Uncertainties");

            mgrAll->writeUncertaintyHistograms(outdir);
            //std::cout << "done writing uncertainties" << std::endl;
        }

        if (printEventTags) {
            eventTagsOutput.close();
        }

        if (splitAdditionalBees) {
            std::string anotherName = "somethingbb";
            std::cout << "change proc 1" << std::endl;

            mgrAll->changeProcess(1, anotherName);
        }

    }
    // std::cout << "change proc " << std::endl;
    std::string anotherName = "something";
    mgrAll->changePrimaryProcess(anotherName); // workaround so that we would print histograms of last process
    // std::cout << "Done " << std::endl;

    //delete mgrAll;
    outfile->Close();

    //delete btagReweighter;
}

bool FourTop::eventPassesTriggers() {
    if (!currentEvent->passMetFilters()) return false;
    if (currentEvent->isMC()) {
        return (currentEvent->passTriggers_e()   || currentEvent->passTriggers_m()   || 
                currentEvent->passTriggers_em()  || currentEvent->passTriggers_mm()  || currentEvent->passTriggers_ee()  || 
                currentEvent->passTriggers_eee() || currentEvent->passTriggers_eem() || currentEvent->passTriggers_emm() || currentEvent->passTriggers_mmm());
    } else {
        return (currentEvent->passTriggers_e()   || currentEvent->passTriggers_m()   || 
                currentEvent->passTriggers_em()  || currentEvent->passTriggers_mm()  || currentEvent->passTriggers_ee()  || 
                currentEvent->passTriggers_eee() || currentEvent->passTriggers_eem() || currentEvent->passTriggers_emm() || currentEvent->passTriggers_mmm());
    }

    return true;
}

std::vector<std::string> FourTop::GetSubClasses(eventClass currClass) {
    std::vector<std::string> subClasses;

    if (currClass == eventClass::ssdl || (currClass == eventClass::cro && onlyCR) || (currClass == eventClass::crw)) {
        if (currClass == eventClass::ssdl && selection->getLepton(0)->charge() > 0) subClasses.push_back("++");
        else if (currClass == eventClass::ssdl) subClasses.push_back("--");

        if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) subClasses.push_back("ee");
        else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) subClasses.push_back("mm");
        else subClasses.push_back("em");
        //std::cout << "is dl " << std::endl;
        if (bdtOutput && currClass == eventClass::ssdl && (selection->GetDLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTTT) {
            if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) subClasses.push_back("pureSig_ee");
            else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) subClasses.push_back("pureSig_mm");
            else subClasses.push_back("pureSig_em");
        } else if (bdtOutput && currClass == eventClass::ssdl && (selection->GetDLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTW) {
            if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) subClasses.push_back("pureTTV_ee");
            else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) subClasses.push_back("pureTTV_mm");
            else subClasses.push_back("pureTTV_em");
        } else if (bdtOutput && currClass == eventClass::ssdl && (selection->GetDLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTBar) subClasses.push_back("pureTTB");

        if (bdtOutput && currClass == eventClass::ssdl && (selection->GetDLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTTT) {
            double score;
            for (auto it : selection->GetDLMVA()->getClassAndScore()) {
                if (it.first > 3) continue;
                score = it.second;
                break;
            }
            //if (score > 0.81) {
            //    std::cout << selection->getEvent()->eventTags() << std::endl;
            //    std::cout << "leptons:" << std::endl;
            //    for (auto& lep : *(selection->getMediumLepCol())) {
            //        std::cout << lep->isMuon() << " " << lep->pt() << " " << lep->eta() << std::endl;
            //    }
            //    std::cout << "jets:" << std::endl;
            //    for (auto& jet : *(selection->getJetCol())) {
            //        std::cout << jet->isBTaggedLoose() << " " << jet->isBTaggedMedium() << " " << jet->isBTaggedTight() << " " << jet->pt() << " " << jet->eta() << std::endl;
            //    }
            //}
            //if (score > 0.81) subClasses.push_back("score15");
            //if (score > 0.928) subClasses.push_back("score2");
        }
    } else if (currClass == eventClass::trilep) {
        if (selection->getMediumLepCol()->hasOSSFPair()) {
            subClasses.push_back("OSSF");
        } else {
            subClasses.push_back("noOSSF");
        }

        //std::cout << "is trilep " << std::endl;

        if (bdtOutput && (selection->GetMLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTTT) subClasses.push_back("pureSig");
        else if (bdtOutput && (selection->GetMLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTW) subClasses.push_back("pureTTV");
        else if (bdtOutput && (selection->GetMLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTBar) subClasses.push_back("pureTTB");

        if (bdtOutput && currClass == eventClass::trilep && (selection->GetMLMVA()->getClassAndScore().begin()->first) % 3 == MVAClasses::TTTT) {
            double score;
            for (auto it : selection->GetMLMVA()->getClassAndScore()) {
                if (it.first > 3) continue;
                score = it.second;
                break;
            }
            //if (score > 0.81) {
            //    std::cout << selection->getEvent()->eventTags() << std::endl;
            //    std::cout << "leptons:" << std::endl;
            //    for (auto& lep : *(selection->getMediumLepCol())) {
            //        std::cout << lep->isMuon() << " " << lep->pt() << " " << lep->eta() << std::endl;
            //    }
            //    std::cout << "jets:" << std::endl;
            //    for (auto& jet : *(selection->getJetCol())) {
            //        std::cout << jet->isBTaggedLoose() << " " << jet->isBTaggedMedium() << " " << jet->isBTaggedTight() << " " << jet->pt() << " " << jet->eta() << std::endl;
            //    }
            //}
        }
    } else if (currClass == eventClass::crz3L) {
        if (selection->numberOfLooseBJets() >= 2 && selection->numberOfJets() >= 3) {
            subClasses.push_back("SigZVeto");
        }
        if (selection->numberOfMediumBJets() >= 1) subClasses.push_back("OneMedB");
        if (selection->numberOfMediumBJets() > 1) subClasses.push_back("TwoMedB");
    }

    return subClasses;
}

CombinedSampleReweighter* FourTop::createSampleReweighter(std::string dir) {
    CombinedSampleReweighter* sampleReweighter = new CombinedSampleReweighter();

    for (auto& samp : treeReader->sampleVector()) {
        std::string sampleName = samp.fileName();
        if (stringTools::stringStartsWith(sampleName, "WZTo") || stringTools::stringStartsWith(sampleName, "WZ_")) {
            sampleReweighter->addReweighter("WZ", std::make_shared< ReweighterSample >("WZ", dir));
        } else if (stringTools::stringStartsWith(sampleName, "ZZTo4L")) {
            sampleReweighter->addReweighter("ZZ", std::make_shared< ReweighterSample >("ZZTo4L", dir));
        }
        
        //else if (stringTools::stringStartsWith(sampleName, "ZG")) {
          //  sampleReweighter->addReweighter("ZG", std::make_shared< ReweighterSample >("ZG", dir));
        //}
    }

    if (sampleReweighter->IsEmpty()) {
        delete sampleReweighter;
        sampleReweighter = nullptr;
    }

    return sampleReweighter;
}

ChannelManager* FourTop::GenerateChannelManager() {
    // rules for generation of channelmanager.

    std::map<std::string, eventClass> regionMap = {
        {"DY", eventClass::dy},
        {"TTBar", eventClass::ttbar},
        {"CR-2L-23J1B", eventClass::cro},
        {"CR-2L-45J2B", eventClass::crw},
        {"CR-3L-2J1B", eventClass::cro3L},
        {"CR-3L-Z", eventClass::crz3L},
        {"CR-4L-Z", eventClass::crz4L},
        {"CR-Conversion", eventClass::cr_conv},
        {"CRWZ", eventClass::crwz},
        {"SR-2L", eventClass::ssdl},
        {"SR-3L", eventClass::trilep},
        {"SR-4L", eventClass::fourlep},
    };
    if (searchRegion != "All") {
        considerRegion = regionMap[searchRegion];
        selection->SetRelRegion(considerRegion);
    }

    ChannelManager* ret = nullptr;

    if (stringTools::stringContains(plotString, "NoBDTOutput")) {
        bdtOutput = false;
        selection->setPrintBDTOutput(false);
    }

    std::function<std::vector<HistInfo>*(const eventClass)> histGenerator;
    std::function<std::vector<double>(const eventClass, EventFourT*)> histFiller;
    if (considerRegion == eventClass::fail) {
        if (stringTools::stringContains(plotString, "Nominal")) {
            histGenerator = HistogramConfig::getNominalHists;
            ret = new ChannelManager(outfile, HistogramConfig::getNominalHists);
            histFiller = HistogramConfig::fillNominalHists;
        } else if (stringTools::stringContains(plotString, "Minimal")) {
            ret = new ChannelManager(outfile, HistogramConfig::getMinimalHists);
            histGenerator = HistogramConfig::getMinimalHists;
            histFiller = HistogramConfig::fillMinimalHists;
        } else if (stringTools::stringContains(plotString, "OnlyBDTVariables")) {
            ret = new ChannelManager(outfile, HistogramConfig::getAllBDTVarsHists);
            histGenerator = HistogramConfig::getAllBDTVarsHists;
            histFiller = HistogramConfig::fillAllBDTVarsHists;
        }
    } else {
        if (stringTools::stringContains(plotString, "Nominal")) {
            histGenerator = HistogramConfig::getNominalHists;
            ret = new ChannelManager(outfile, considerRegion, HistogramConfig::getNominalHists);
            histFiller = HistogramConfig::fillNominalHists;
        } else if (stringTools::stringContains(plotString, "Minimal")) {
            ret = new ChannelManager(outfile, considerRegion, HistogramConfig::getMinimalHists);
            histGenerator = HistogramConfig::getMinimalHists;
            histFiller = HistogramConfig::fillMinimalHists;
        } else if (stringTools::stringContains(plotString, "OnlyBDTVariables")) {
            ret = new ChannelManager(outfile, considerRegion, HistogramConfig::getAllBDTVarsHists);
            histGenerator = HistogramConfig::getAllBDTVarsHists;
            histFiller = HistogramConfig::fillAllBDTVarsHists;
        }
    }
    selection->setFillerFunction(histFiller);

    return ret;
}


std::map<eventClass, int> FourTop::FillHistogramManager(ChannelManager* mgrAll) {
    std::map<eventClass, int> offsets;

    std::map<eventClass, std::string> namingScheme = {
            {fail, "fail"},
            {dy, "_DY"},
            {ttbar, "_TTBar"},
            {crwz, "CRWZ"},
            {crzz, "CRZZ"},
            {cr_conv, "CR-Conversion"},
            {crz3L, "_CR-3L-Z"},
            {crz4L, "_CR-4L-Z"},
            {cro, "_CR-2L-23J1B"},
            {cro3L, "_CR-3L-2J1B"},
            {crw, "_CR-2L-45J2B"},
            {ssdl, ""},
            {trilep, ""},
            {fourlep, ""}};

    std::vector<std::string> dlSubChannels = {"++", "--", "ee", "em", "mm", "pureSig_mm", "pureSig_ee", "pureSig_em", "pureTTV_mm", "pureTTV_ee", "pureTTV_em", "pureTTB"};
    std::vector<std::string> croSubChannels = {"++", "--", "ee", "em", "mm"};
    std::vector<std::string> crwSubChannels = {"ee", "em", "mm"};
    //std::vector<std::string> crwSubChannels = {"++", "--", "ee", "em", "mm"};
    std::vector<std::string> trilepSubChannels = {"OSSF", "noOSSF", "pureSig", "pureTTV", "pureTTB"};
    std::vector<std::string> crzSubChannels = {"SigZVeto", "OneMedB", "TwoMedB"};

    if (considerRegion == eventClass::fail) {
        if (bdtOutput) {
            offsets[eventClass::dy] = mgrAll->at(eventClass::dy)->getHistInfo()->size() + mva_DL->getMaxClass();
            offsets[eventClass::ttbar] = mgrAll->at(eventClass::ttbar)->getHistInfo()->size() + mva_DL->getMaxClass();
            offsets[eventClass::crz3L] = mgrAll->at(eventClass::crz3L)->getHistInfo()->size() + mva_ML->getMaxClass();
            offsets[eventClass::crz4L] = mgrAll->at(eventClass::crz4L)->getHistInfo()->size() + mva_ML->getMaxClass();
            offsets[eventClass::cro3L] = mgrAll->at(eventClass::cro3L)->getHistInfo()->size() + mva_ML->getMaxClass();  
            offsets[eventClass::cro] = mgrAll->at(eventClass::cro)->getHistInfo()->size() + mva_DL->getMaxClass();
            offsets[eventClass::crw] = mgrAll->at(eventClass::crw)->getHistInfo()->size() + mva_DL->getMaxClass();

            mgrAll->at(eventClass::dy)->updateHistInfo(mva_DL->createHistograms("_DY", true));
            mgrAll->at(eventClass::ttbar)->updateHistInfo(mva_DL->createHistograms("_TTBar", true));
            mgrAll->at(eventClass::crz3L)->updateHistInfo(mva_ML->createHistograms("_CR-3L-Z", true));
            mgrAll->at(eventClass::crz4L)->updateHistInfo(mva_ML->createHistograms("_CR-4L-Z", true));
            mgrAll->at(eventClass::cro3L)->updateHistInfo(mva_ML->createHistograms("_CR-3L-2J1B", true));
            mgrAll->at(eventClass::cro)->updateHistInfo(mva_DL->createHistograms("_CR-2L-23J1B", true));
            mgrAll->at(eventClass::crw)->updateHistInfo(mva_DL->createHistograms("_CR-2L-45J2B", true));

            mgrAll->at(eventClass::dy)->set2DHistInfo(mva_DL->create2DHistograms("_DY", true));
            mgrAll->at(eventClass::ttbar)->set2DHistInfo(mva_DL->create2DHistograms("_TTBar", true));
            mgrAll->at(eventClass::crz3L)->set2DHistInfo(mva_ML->create2DHistograms("_CR-3L-Z", true));
            mgrAll->at(eventClass::crz4L)->set2DHistInfo(mva_ML->create2DHistograms("_CR-4L-Z", true));
            mgrAll->at(eventClass::cro3L)->set2DHistInfo(mva_ML->create2DHistograms("_CR-3L-2J1B", true));
            mgrAll->at(eventClass::cro)->set2DHistInfo(mva_DL->create2DHistograms("_CR-2L-23J1B", true));
            mgrAll->at(eventClass::crw)->set2DHistInfo(mva_DL->create2DHistograms("_CR-2L-45J2B", true));
        }
        if (useNpNmDistributions) {
            mgrAll->at(eventClass::cro)->updateHistInfo(HistogramConfig::createNpNmHistograms(eventClass::cro));
            mgrAll->at(eventClass::crw)->updateHistInfo(HistogramConfig::createNpNmHistograms(eventClass::crw));
        }
        
        mgrAll->at(eventClass::crz3L)->addSubChannels(crzSubChannels);
        mgrAll->at(eventClass::crw)->addSubChannels(crwSubChannels);
        if (onlyCR) mgrAll->at(eventClass::cro)->addSubChannels(croSubChannels);
        //if (onlyCR) mgrAll->at(eventClass::crw)->addSubChannels(crwSubChannels);

        if (! onlyCR) {
            std::cout << "SRs are considered" << std::endl;
            if (bdtOutput) { 
                offsets[eventClass::ssdl] = mgrAll->at(eventClass::ssdl)->getHistInfo()->size() + mva_DL->getMaxClass();
                offsets[eventClass::trilep] = mgrAll->at(eventClass::trilep)->getHistInfo()->size() + mva_ML->getMaxClass();
                offsets[eventClass::fourlep] = mgrAll->at(eventClass::fourlep)->getHistInfo()->size() + mva_ML->getMaxClass();

                mgrAll->at(eventClass::ssdl)->updateHistInfo(mva_DL->createHistograms(""));
                mgrAll->at(eventClass::trilep)->updateHistInfo(mva_ML->createHistograms(""));
                mgrAll->at(eventClass::fourlep)->updateHistInfo(mva_ML->createHistograms("", true));
                mgrAll->at(eventClass::ssdl)->set2DHistInfo(mva_DL->create2DHistograms(""));
                mgrAll->at(eventClass::trilep)->set2DHistInfo(mva_ML->create2DHistograms(""));
                mgrAll->at(eventClass::fourlep)->set2DHistInfo(mva_ML->create2DHistograms("", true));
            }

            if (useNpNmDistributions) {
                mgrAll->at(eventClass::ssdl)->updateHistInfo(HistogramConfig::createNpNmHistograms(eventClass::ssdl));
            }

            mgrAll->at(eventClass::ssdl)->addSubChannels(dlSubChannels);
            mgrAll->at(eventClass::trilep)->addSubChannels(trilepSubChannels);
        }
    } else {
        if (bdtOutput && (considerRegion >= unsigned(eventClass::crz3L) || considerRegion == unsigned(eventClass::dy) || considerRegion == unsigned(eventClass::ttbar))) {
            //std::cout << "bdt output" << std::endl;
            if (considerRegion == eventClass::cro || considerRegion == eventClass::crw || considerRegion == eventClass::ssdl || considerRegion == eventClass::dy || considerRegion == eventClass::ttbar) {
                offsets[considerRegion] = mgrAll->at(considerRegion)->getHistInfo()->size() + mva_DL->getMaxClass();
                mgrAll->at(considerRegion)->updateHistInfo(mva_DL->createHistograms(namingScheme[considerRegion], true));
                mgrAll->at(considerRegion)->set2DHistInfo(mva_DL->create2DHistograms(namingScheme[considerRegion], true));
            } else {
                offsets[considerRegion] = mgrAll->at(considerRegion)->getHistInfo()->size() + mva_ML->getMaxClass();
                mgrAll->at(considerRegion)->updateHistInfo(mva_ML->createHistograms(namingScheme[considerRegion], true));
                mgrAll->at(considerRegion)->set2DHistInfo(mva_ML->create2DHistograms(namingScheme[considerRegion], true));
            }
        }

        if (considerRegion == eventClass::cro) mgrAll->at(considerRegion)->addSubChannels(croSubChannels);
        else if (considerRegion == eventClass::crw) mgrAll->at(considerRegion)->addSubChannels(crwSubChannels);
        else if (considerRegion == eventClass::ssdl) mgrAll->at(considerRegion)->addSubChannels(dlSubChannels);
        else if (considerRegion == eventClass::trilep) mgrAll->at(considerRegion)->addSubChannels(trilepSubChannels);
    }
    if (! bdtOutput) {
        offsets = {
            {eventClass::dy, 0},
            {eventClass::ttbar, 0},
            {eventClass::crwz, 0},
            {eventClass::cr_conv, 0},
            {eventClass::crzz, 0},
            {eventClass::crz3L, 0},
            {eventClass::crz4L, 0},
            {eventClass::cro, 0},
            {eventClass::cro3L, 0},
            {eventClass::crw, 0},
            {eventClass::ssdl, 0},
            {eventClass::trilep, 0},
            {eventClass::fourlep, 0}
        };
    }

    return offsets;
}

bool FourTop::FillRegion(eventClass nominalClass, selectionType st) {
    if (nominalClass == eventClass::fail) return false;
    if (st == selectionType::ChargeMisDD && !(nominalClass == eventClass::ssdl || nominalClass == eventClass::cro || nominalClass == eventClass::crw)) return false;
    if (onlyCR && unsigned(nominalClass) >= eventClass::ssdl) return false;
    if (considerRegion != eventClass::fail && nominalClass != considerRegion) return false;

    return true;
}

std::vector<std::pair<int, double>> FourTop::FillNpNmDistributions(eventClass currentClass, std::map<eventClass,int>& offsets) {
    std::vector<std::pair<int, double>> singleEntriesNpNm;

    if (currentClass != eventClass::ssdl && currentClass != eventClass::crw && currentClass != eventClass::cro) return singleEntriesNpNm;
    
    std::vector<double> singleEntriesVector = HistogramConfig::fillNpNmHistograms(currentClass, selection);
    int minOffset = offsets[currentClass] + 6;

    for (unsigned i=0; i<singleEntriesVector.size(); i++) {
        singleEntriesNpNm.push_back({minOffset+i, singleEntriesVector[i]});
    }
    return singleEntriesNpNm;
}
