#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop::analyze(std::string method) {
    std::map<eventClass, int> offsets;
    std::shared_ptr< SampleCrossSections > xsecs;
    std::vector<std::string> processes = {"", "nonPrompt", "ChargeMisID"};

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
    ReweighterBTagShape** btagReweighter;
    CombinedReweighter reweighter;
    CombinedSampleReweighter* sampleReweighter = nullptr;
    if (! treeReader->sampleVector()[0].isData() && method != "Obs") {
        uncTranslateMap = mgrAll->getTranslateUnc(); 
        std::cout << "building reweighter" << std::endl;
        btagReweighter = new ReweighterBTagShape*();
        reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter, testRun );
        if (leanEventSelection && !testRun) {
            addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");
        } else if (!testRun) {
            addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Original");
        }
        sampleReweighter = createSampleReweighter("ANWeights/SampleNJetSF/");
    }

    offsets = FillHistogramManager(mgrAll);
    selection->setOffsets(offsets);

    if (method == "MCPrompt") {
        processes = {"", "ChargeMisID"};
        selection->setSelectionType(selectionType::MCPrompt);
        st = selectionType::MCPrompt;

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
        processes = {"nonPromptDD"};
        selection->setSelectionType(selectionType::NPDD);
        useUncertainties = false;
        st = selectionType::NPDD;

        std::cout << "Running method " << "NP DD" << std::endl;
    } else if (method == "nonPromptDDControl") {
        initFakerate();
        processes = {"nonPrompt"};
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

    mgrAll->initHistogramStacks(processes, useUncertainties);

    std::cout << "event loop" << std::endl;

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    std::vector<std::string> bTagShapeSystematics;
    std::vector<std::string> JECSourcesGrouped;
    std::vector<std::string> wzSFRegions;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        currentEvent = treeReader->buildEventPtr(0, false, false, false, true );

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;

        int numberOfPSVariations = 0;
        int numberOfPdfVariations = 0;
        bool hasValidQcds = false;
        bool hasValidPSs = false;
        bool hasValidPdfs = false;
        bool considerBTagShape = false;
        bool useSplitJEC = true;
        bool isSignalSample = false;
        
        if (! treeReader->isData()) {
            // check if TTbar or TTGamma sample
            ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();
            zgOverlapCheck = treeReader->currentSamplePtr()->zgOverlap();

            std::cout << "ttg sample " << ttgOverlapCheck << std::endl;
            std::cout << "zg sample " << zgOverlapCheck << std::endl;
            if ((st == selectionType::MCAll && !isNPControl) || st == selectionType::MCPrompt || st == selectionType::MCNoNP) {
                std::string currProcName = sampleVec[sampleIndex].processName();
                mgrAll->changePrimaryProcess(currProcName);
            }
        }

        if (useUncertainties && ! treeReader->isData()) {
            //mgrAll->SetPrintAllUncertaintyVariations(true);
            // MC ONLY (could be changed to MCAll and MCLim options only, but comes down to the same thing)
            xsecs = std::make_shared<SampleCrossSections>( treeReader->currentSample() );

            std::cout << "finding available PS scale variations...\n";
            Event event = treeReader->buildEvent(0);
            numberOfPSVariations = event.generatorInfo().numberOfPsWeights();
            if(numberOfPSVariations>=44) hasValidPSs = true;
            std::cout << "Sample " << treeReader->currentSample().fileName() << " - hasValidPSs: " << hasValidPSs << "\n";

            if(currentEvent->generatorInfo().numberOfScaleVariations() == 9 ) hasValidQcds = true;

            considerBTagShape = ! testRun;
            
            if (sampleIndex == 0 && considerBTagShape) {
                bTagShapeSystematics = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->availableSystematics();
                mgrAll->addSubUncertainties(shapeUncId::bTagShape, bTagShapeSystematics);
            }
            if (sampleIndex == 0 && useSplitJEC) {
                std::cout << "split JEC" << std::endl;
                JECSourcesGrouped = currentEvent->jetInfo().groupedJECVariations();

                mgrAll->addSubUncertainties(shapeUncId::JEC, JECSourcesGrouped);
            }
            if (sampleIndex == 0) {
                wzSFRegions = {"0Jet", "1Jet", "2Jet", "3Jet", "4Jet", "5Jet", "6PlusJet"};
                mgrAll->addSubUncertainties(shapeUncId::WZSF, wzSFRegions);
            }

            if (stringTools::stringContains(treeReader->currentSample().processName(), "TTTT")) {
                isSignalSample = true;
            }
        }
        
        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        mgrAll->newSample(uniqueName);

        std::ofstream eventTagsOutput;
        if (printEventTags) {
            std::string samplename = treeReader->sampleVector()[0].uniqueName();
            eventTagsOutput.open("Output/EventTags_" + samplename + ".txt");
        }

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            if (testRun && entry > 10000) break;
            //if (entry % 100000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry, false, false, false, true );

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

            double weight = currentEvent->weight();
            if( currentEvent->isMC() && (unsigned(st) <= selectionType::MCNoNP)) {
                weight *= reweighter.totalWeight( *currentEvent );

                if (st == selectionType::MCPrompt) {
                    if (! selection->leptonsArePrompt()) continue;
                    if (! selection->leptonsAreNotChargeFlip()) processNb = 1;
                    if (processNb == 1 && selection->numberOfLeps() == 2) continue;

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

            // Basic non-prompt handling (using MC to estimate the contribution):
            std::vector<double> fillVec;
            std::vector<std::pair<double, double>> fillVec2D;
            std::vector<std::pair<int, double>> singleEntries;
            std::vector<std::string> subChannels;

            // fill all histograms
            // replace with functions in eventHandling?

            eventClass nominalClass = selection->getCurrentClass();
            if (sampleReweighter && selection->leptonsArePrompt() && nominalClass > eventClass::crwz) weight *= sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());

            // make a bool for filling checks. 
            // 

            // if signal region && onlyCR: skip
            // if chargeMisDD && not dilep: skip
            // if region != considerRegion && considerRegion != fail: skip

            if (FillRegion(nominalClass, st)) {
                if (testRun) std::cout << "is fill " << std::endl;

                fillVec = selection->fillVector();
                singleEntries = selection->singleFillEntries();
                fillVec2D = selection->fillVector2D();

                subChannels = GetSubClasses(nominalClass);

                // make function bundling this behaviour.
                mgrAll->at(nominalClass)->fillAllHistograms(subChannels, processNb, fillVec, weight);
                mgrAll->at(nominalClass)->fillAll2DHistograms(subChannels, processNb, fillVec2D, weight);
                mgrAll->at(nominalClass)->fillAllSingleHistograms(subChannels, processNb, singleEntries, weight);
            }

            // Systematics
            if (currentEvent->isData() || ! useUncertainties || processNb > 0) continue;

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

            unsigned uncID = 0;
            while (uncID < shapeUncId::end) {
                if (( !uncWrapper || (onlyCR && unsigned(nominalClass) >= eventClass::ssdl) || (considerRegion != eventClass::fail && nominalClass != considerRegion)) && uncID < shapeUncId::JER_1p93) {
                    uncID++;
                    continue;
                }
                double weightUp = 1.;
                double weightDown = 1.;
                eventClass upClass = eventClass::fail;
                eventClass downClass = eventClass::fail;
                std::vector<std::string> subChannelsUp;
                std::vector<std::string> subChannelsDown;

                //std::cout << uncID << std::endl;

                if (uncID <= shapeUncId::pileup) {
                    // all uncertainties with simple reweighting
                    std::string id = uncTranslateMap[shapeUncId(uncID)];
                    double weightNominalInv = 1. / reweighter[ id ]->weight( *currentEvent );
                    weightUp = reweighter[ id ]->weightUp( *currentEvent ) * weightNominalInv;
                    weightDown = reweighter[ id ]->weightDown( *currentEvent ) * weightNominalInv;
                } else if (uncID == shapeUncId::qcdScale) {
                    std::vector<double> qcdvariations;
                    if (hasValidQcds) {
                        //if (isSignalSample) {
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_1() / xsecs.get()->crossSectionRatio_MuR_2_MuF_1() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_1() / xsecs.get()->crossSectionRatio_MuR_0p5_MuF_1() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_2() / xsecs.get()->crossSectionRatio_MuR_2_MuF_2() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_2() / xsecs.get()->crossSectionRatio_MuR_1_MuF_2() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_0p5() / xsecs.get()->crossSectionRatio_MuR_1_MuF_0p5() );
                        qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_0p5() / xsecs.get()->crossSectionRatio_MuR_0p5_MuF_0p5() );
                        //} else {
                        //    qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_1());
                        //    qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_1());
                        //    qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_2_MuF_2());
                        //    qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_2());
                        //    qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_1_MuF_0p5());
                        //    qcdvariations.push_back(weight * currentEvent->generatorInfo().relativeWeight_MuR_0p5_MuF_0p5());
                        //}
                    } else {
                        qcdvariations = {weight, weight, weight, weight, weight, weight};
                    }

                    uncWrapper->fillAllEnvelope(subChannels, shapeUncId::qcdScale, processNb, fillVec, qcdvariations);
                    uncWrapper->fillAllEnvelopeSingles(subChannels, shapeUncId::qcdScale, processNb, singleEntries, qcdvariations);
                    uncWrapper->fillAllEnvelope2Ds(subChannels, shapeUncId::qcdScale, processNb, fillVec2D, qcdvariations);
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

                } else if (uncID == shapeUncId::bTagShape) {
                    if (considerBTagShape) {
                        double nombweight = reweighter["bTag_shape"]->weight( *currentEvent );
                        for(std::string btagsys : bTagShapeSystematics){
                            weightUp = 1. * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, btagsys ) / nombweight;
                            weightDown = 1. * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, btagsys ) / nombweight;

                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, fillVec, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, singleEntries, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, btagsys, fillVec2D, weight * weightUp, weight * weightDown);
                        }

                        weightUp = 1.;
                        weightDown = 1.;
                    }
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
                            if (selection->numberOfJets() == i || (i == 6 && selection->numberOfJets() >= 6)) {
                                weightUp = 1. * sampleReweighter->totalWeightUp(*currentEvent, selection->numberOfJets()) / sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
                                weightDown = 1. * sampleReweighter->totalWeightDown(*currentEvent, selection->numberOfJets()) / sampleReweighter->totalWeight(*currentEvent, selection->numberOfJets());
                            }
                            std::string wzSFRegion = wzSFRegions[i];
                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntries, weight * weightUp, weight * weightDown);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec2D, weight * weightUp, weight * weightDown);
                        }
                        weightUp = 1.;
                        weightDown = 1.;
                    } else {
                        for (std::string wzSFRegion : wzSFRegions) {
                            uncWrapper->fillAllSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec, weight, weight);
                            uncWrapper->fillAllSingleSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, singleEntries, weight, weight);
                            uncWrapper->fillAll2DSubUncertainty(subChannels, shapeUncId(uncID), processNb, wzSFRegion, fillVec2D, weight, weight);
                        }
                    }
                } else if ((uncID >= shapeUncId::JER_1p93 && uncID != shapeUncId::JEC) || (uncID == shapeUncId::JEC && !useSplitJEC)) {
                    // JER and JEC

                    if( uncID == shapeUncId::JEC && considerBTagShape ) {
                        //weightUp = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, "JECUp" ) 
                        //                    / reweighter["bTag_shape"]->weight( *currentEvent );
                        //weightDown = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, "JECDown" ) 
                        //                    / reweighter["bTag_shape"]->weight( *currentEvent );
                    }
                    std::string empty = "";

                    upClass = selection->classifyUncertainty(shapeUncId(uncID), true, empty);
                    fillVecUp = selection->fillVector();
                    singleEntriesUp = selection->singleFillEntries();
                    fillVec2DUp = selection->fillVector2D();

                    downClass = selection->classifyUncertainty(shapeUncId(uncID), false, empty);
                    fillVecDown = selection->fillVector();
                    singleEntriesDown = selection->singleFillEntries();
                    fillVec2DDown = selection->fillVector2D();

                    subChannelsUp = GetSubClasses(upClass);
                    subChannelsDown = GetSubClasses(downClass);

                    if (FillRegion(upClass, st)) mgrAll->fillAllUpHistograms(subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecUp, singleEntriesUp, fillVec2DUp, weight * weightUp);
                    if (FillRegion(downClass, st)) mgrAll->fillAllDownHistograms(subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecDown, singleEntriesDown, fillVec2DDown, weight * weightDown);
                } else if (uncID == shapeUncId::JEC && useSplitJEC) {
                    for (std::string jecSource : JECSourcesGrouped) {
                        if (stringTools::stringContains(jecSource, "Total")) continue;
                        if (considerBTagShape) {
                            std::string sourceUp = jecSource + "Up";
                            std::string sourceDown = jecSource + "Down";

                            weightUp = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, sourceUp) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                            weightDown = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, sourceDown) 
                                                / reweighter["bTag_shape"]->weight( *currentEvent );
                        }

                        upClass = selection->classifyUncertainty(shapeUncId(uncID), true, jecSource);
                        fillVecUp = selection->fillVector();
                        singleEntriesUp = selection->singleFillEntries();
                        fillVec2DUp = selection->fillVector2D();

                        downClass = selection->classifyUncertainty(shapeUncId(uncID), false, jecSource);
                        fillVecDown = selection->fillVector();
                        singleEntriesDown = selection->singleFillEntries();
                        fillVec2DDown = selection->fillVector2D();

                        subChannelsUp = GetSubClasses(upClass);
                        subChannelsDown = GetSubClasses(downClass);

                        if (FillRegion(upClass, st)) mgrAll->fillAllSubUpHistograms(jecSource, subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecUp, singleEntriesUp, fillVec2DUp, weight * weightUp);
                        if (FillRegion(downClass, st)) mgrAll->fillAllSubDownHistograms(jecSource, subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecDown, singleEntriesDown, fillVec2DDown, weight * weightDown);
                    }
                }

                if (uncID < shapeUncId::JER_1p93) {
                    uncWrapper->fillAllUncertainties(subChannels, shapeUncId(uncID), processNb, fillVec, weight * weightUp, weight * weightDown);
                    uncWrapper->fillAllSingleUncertainties(subChannels, shapeUncId(uncID), processNb, singleEntries, weight * weightUp, weight * weightDown);
                    uncWrapper->fillAll2DUncertainties(subChannels, shapeUncId(uncID), processNb, fillVec2D, weight * weightUp, weight * weightDown);
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
            std::cout << "writing uncertainties" << std::endl;

            outfile->cd();
            outfile->cd("Uncertainties");

            mgrAll->writeUncertaintyHistograms(outdir);
        }

        if (printEventTags) {
            eventTagsOutput.close();
        }
    }
    std::string anotherName = "something";
    mgrAll->changePrimaryProcess(anotherName); // workaround so that we would print histograms of last process

    outfile->Close();
}

bool FourTop::eventPassesTriggers() {
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

    if (currClass == eventClass::ssdl || (currClass == eventClass::cro && onlyCR) || (currClass == eventClass::crw && onlyCR)) {
        if (selection->getLepton(0)->charge() > 0) subClasses.push_back("++");
        else subClasses.push_back("--");

        if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) subClasses.push_back("ee");
        else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) subClasses.push_back("mm");
        else subClasses.push_back("em");
    } else if (currClass == eventClass::trilep) {
        if (selection->getMediumLepCol()->hasOSSFPair()) {
            subClasses.push_back("OSSF");
        } else {
            subClasses.push_back("noOSSF");
        }
    }

    return subClasses;
}

CombinedSampleReweighter* FourTop::createSampleReweighter(std::string dir) {
    CombinedSampleReweighter* sampleReweighter = new CombinedSampleReweighter();

    for (auto& samp : treeReader->sampleVector()) {
        std::string sampleName = samp.fileName();
        if (stringTools::stringStartsWith(sampleName, "WZTo") || stringTools::stringStartsWith(sampleName, "WZ_")) {
            sampleReweighter->addReweighter("WZ", std::make_shared< ReweighterSample >("WZ", dir));
        } //else if (stringTools::stringStartsWith(sampleName, "ZG")) {
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
            {dy, "DY"},
            {ttbar, "TTBar"},
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

    std::vector<std::string> dlSubChannels = {"++", "--", "ee", "em", "mm"};
    std::vector<std::string> croSubChannels = {"++", "--", "ee", "em", "mm"};
    std::vector<std::string> crwSubChannels = {"++", "--", "ee", "em", "mm"};
    std::vector<std::string> trilepSubChannels = {"OSSF", "noOSSF"};

    if (considerRegion == eventClass::fail) {
        if (bdtOutput) {
            offsets[eventClass::crz3L] = mgrAll->at(eventClass::crz3L)->getHistInfo()->size() + mva_ML->getMaxClass();
            offsets[eventClass::crz4L] = mgrAll->at(eventClass::crz4L)->getHistInfo()->size() + mva_ML->getMaxClass();
            offsets[eventClass::cro3L] = mgrAll->at(eventClass::cro3L)->getHistInfo()->size() + mva_ML->getMaxClass();  
            offsets[eventClass::cro] = mgrAll->at(eventClass::cro)->getHistInfo()->size() + mva_DL->getMaxClass();
            offsets[eventClass::crw] = mgrAll->at(eventClass::crw)->getHistInfo()->size() + mva_DL->getMaxClass();

            mgrAll->at(eventClass::crz3L)->updateHistInfo(mva_ML->createHistograms("_CR-3L-Z", true));
            mgrAll->at(eventClass::crz4L)->updateHistInfo(mva_ML->createHistograms("_CR-4L-Z", true));
            mgrAll->at(eventClass::cro3L)->updateHistInfo(mva_ML->createHistograms("_CR-3L-2J1B", true));
            mgrAll->at(eventClass::cro)->updateHistInfo(mva_DL->createHistograms("_CR-2L-23J1B", true));
            mgrAll->at(eventClass::crw)->updateHistInfo(mva_DL->createHistograms("_CR-2L-45J2B", true));

            mgrAll->at(eventClass::crz3L)->set2DHistInfo(mva_ML->create2DHistograms("_CR-3L-Z", true));
            mgrAll->at(eventClass::crz4L)->set2DHistInfo(mva_ML->create2DHistograms("_CR-4L-Z", true));
            mgrAll->at(eventClass::cro3L)->set2DHistInfo(mva_ML->create2DHistograms("_CR-3L-2J1B", true));
            mgrAll->at(eventClass::cro)->set2DHistInfo(mva_DL->create2DHistograms("_CR-2L-23J1B", true));
            mgrAll->at(eventClass::crw)->set2DHistInfo(mva_DL->create2DHistograms("_CR-2L-45J2B", true));
        }
        
        if (onlyCR) mgrAll->at(eventClass::cro)->addSubChannels(croSubChannels);
        if (onlyCR) mgrAll->at(eventClass::crw)->addSubChannels(crwSubChannels);

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

            mgrAll->at(eventClass::ssdl)->addSubChannels(dlSubChannels);
            mgrAll->at(eventClass::trilep)->addSubChannels(trilepSubChannels);
        }
    } else {
        if (bdtOutput && considerRegion >= unsigned(eventClass::crz3L)) {
            //std::cout << "bdt output" << std::endl;
            if (considerRegion == eventClass::cro || considerRegion == eventClass::crw || considerRegion == eventClass::ssdl) {
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
