#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: analyze() {
    ChannelManager* mgrAll = new ChannelManager(outfile);
    std::shared_ptr< SampleCrossSections > xsecs;

    // reweighter creation
    std::cout << "building reweighter" << std::endl;
    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector() );

    //std::shared_ptr<ReweighterBTagShape> btagReweighterPtr = dynamic_cast<ReweighterBTagShape*>(reweighter["bTag_shape"]);

    size_t crzPosMVA = mgrAll->at(eventClass::crz)->getHistInfo()->size() + mva_ML->getMaxClass();
    size_t croPosMVA = mgrAll->at(eventClass::cro)->getHistInfo()->size() + mva_DL->getMaxClass();
    size_t crwPosMVA = mgrAll->at(eventClass::crw)->getHistInfo()->size() + mva_DL->getMaxClass();
    size_t dlPosMVA = mgrAll->at(eventClass::ssdl)->getHistInfo()->size() + mva_DL->getMaxClass();
    size_t mlPosMVA = mgrAll->at(eventClass::trilep)->getHistInfo()->size() + mva_ML->getMaxClass();
    size_t fourlPosMVA = mgrAll->at(eventClass::fourlep)->getHistInfo()->size() + mva_ML->getMaxClass();


    mgrAll->at(eventClass::crz)->updateHistInfo(mva_ML->createHistograms("_CRZ", true));
    mgrAll->at(eventClass::cro)->updateHistInfo(mva_DL->createHistograms("_CRO", true));
    mgrAll->at(eventClass::crw)->updateHistInfo(mva_DL->createHistograms("_CRW", true));
    mgrAll->at(eventClass::ssdl)->updateHistInfo(mva_DL->createHistograms(""));
    mgrAll->at(eventClass::trilep)->updateHistInfo(mva_ML->createHistograms(""));
    mgrAll->at(eventClass::fourlep)->updateHistInfo(mva_ML->createHistograms("", true));

    mgrAll->at(eventClass::crz)->set2DHistInfo(mva_ML->create2DHistograms("_CRZ", true));
    mgrAll->at(eventClass::cro)->set2DHistInfo(mva_DL->create2DHistograms("_CRO", true));
    mgrAll->at(eventClass::crw)->set2DHistInfo(mva_DL->create2DHistograms("_CRW", true));
    mgrAll->at(eventClass::ssdl)->set2DHistInfo(mva_DL->create2DHistograms(""));
    mgrAll->at(eventClass::trilep)->set2DHistInfo(mva_ML->create2DHistograms(""));
    mgrAll->at(eventClass::fourlep)->set2DHistInfo(mva_ML->create2DHistograms("", true));

    std::vector<std::string> dlSubChannels = {"++", "--", "ee", "em", "mm"};

    mgrAll->at(eventClass::ssdl)->addSubChannels(dlSubChannels);

    std::map<eventClass, int> offsets;
    offsets[eventClass::cro] = croPosMVA;
    offsets[eventClass::crw] = crwPosMVA;
    offsets[eventClass::crz] = crzPosMVA;
    offsets[eventClass::ssdl] = dlPosMVA;
    offsets[eventClass::trilep] = mlPosMVA;
    offsets[eventClass::fourlep] = fourlPosMVA;
    selection->setOffsets(offsets);

    std::map<shapeUncId, std::string> uncTranslateMap = mgrAll->getTranslateUnc();

    std::vector<std::string> processes = {"", "nonPrompt", "ChargeMisID"};
    mgrAll->initHistogramStacks(processes);

    std::cout << "event loop" << std::endl;

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    std::vector<std::string> bTagShapeSystematics;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        currentEvent = treeReader->buildEventPtr(0);

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;

        int numberOfPSVariations = 0;
        int numberOfPdfVariations = 0;
        bool hasValidQcds = false;
        bool hasValidPSs = false;
        bool hasValidPdfs = false;
        bool considerBTagShape = false;
        
        if (! treeReader->isData()) {
            // check if TTbar or TTGamma sample
            ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

            xsecs = std::make_shared<SampleCrossSections>( treeReader->currentSample() );

            std::cout << "finding available PS scale variations...\n";
            Event event = treeReader->buildEvent(0);
            numberOfPSVariations = event.generatorInfo().numberOfPsWeights();
            if(numberOfPSVariations==14) hasValidPSs = true;
            std::cout << "Sample " << treeReader->currentSample().fileName() << " - hasValidPSs: " << hasValidPSs << "\n";

            if(currentEvent->generatorInfo().numberOfScaleVariations() == 9 ) {
                hasValidQcds = true;
            }
            if(numberOfPdfVariations>=100){
	            hasValidPdfs = true;
            }

            considerBTagShape = true;
            
            if (sampleIndex == 0) {
                bTagShapeSystematics = {"jes","hf","lf","hfstats1","hfstats2",
                                        "lfstats1","lfstats2","cferr1","cferr2" };//dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->availableSystematics();
                mgrAll->addSubUncertainties(shapeUncId::bTagShape, bTagShapeSystematics);
            }
        }

        std::string currProcName = sampleVec[sampleIndex].processName();
        mgrAll->changePrimaryProcess(currProcName);
        
        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        mgrAll->newSample(uniqueName);

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );


            // Check triggers here
            if (! eventPassesTriggers()) continue;

            selection->addNewEvent(currentEvent);

            // Apply overlap removal & apply triggers
            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"

            if (! treeReader->isData()) {
                numberOfPdfVariations = currentEvent->generatorInfo().numberOfPdfVariations();
                if(currentEvent->generatorInfo().numberOfScaleVariations() == 9 ) {
                    hasValidQcds = true;
                } else {
                    hasValidQcds = false;
                }

                if (numberOfPdfVariations>=100){
                    hasValidPdfs = true;
                } else {
                    hasValidPdfs = false;
                }
            }
            // Remove mass resonances
            if (! selection->passLowMassVeto()) {
                continue;
            }
            selection->classifyEvent();
            // TEMP! Remove for full stuff
            if (selection->getCurrentClass() == eventClass::fail) continue;


            double weight = currentEvent->weight();
            if( currentEvent->isMC() ){
                weight *= reweighter.totalWeight( *currentEvent );
            }

            // Basic non-prompt handling (using MC to estimate the contribution):
            std::vector<double> fillVec;
            std::vector<std::pair<double, double>> fillVec2D;
            std::vector<std::pair<int, double>> singleEntries;
            std::vector<std::string> subChannels;

            unsigned processNb = 0;

            for (const auto& leptonPtr : *selection->getMediumLepCol()) {
                if (! leptonPtr->isChargeFlip()) {
                    processNb = 2;
                    break;
                }
            }

            for (const auto& leptonPtr : *selection->getMediumLepCol()) {
                if (! leptonPtr->isPrompt()) {
                    processNb = 1;
                    break;
                }
            }

            // fill all histograms
            eventClass nominalClass = selection->getCurrentClass();
            if (nominalClass == eventClass::crz) {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllLean(false, selection); // change falses/trues by eventClass
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());
                fillVec2D = mva_ML->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();   
                singleEntries.push_back({crzPosMVA + classAndScore.first, classAndScore.second});

                mgrAll->at(eventClass::crz)->fillHistograms(processNb, fillVec, weight);
                mgrAll->at(eventClass::crz)->fill2DHistograms(processNb, fillVec2D, weight);
                mgrAll->at(eventClass::crz)->fillSingleHistograms(processNb, singleEntries, weight);

            } else if (nominalClass == eventClass::crw || nominalClass == eventClass::cro) {
                std::vector<double> scores = mva_DL->scoreEvent();

                fillVec = fourTopHists::fillAllLean(false, selection); // change falses/trues by eventClass
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());
                fillVec2D = mva_DL->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_DL->getClassAndScore();   

                if (nominalClass == eventClass::crw) {
                    singleEntries.push_back({crwPosMVA + classAndScore.first, classAndScore.second});
                    mgrAll->at(eventClass::crw)->fillHistograms(processNb, fillVec, weight);
                    mgrAll->at(eventClass::crw)->fill2DHistograms(processNb, fillVec2D, weight);
                    mgrAll->at(eventClass::crw)->fillSingleHistograms(processNb, singleEntries, weight);

                } else {
                    singleEntries.push_back({croPosMVA + classAndScore.first, classAndScore.second});
                    mgrAll->at(eventClass::cro)->fillHistograms(processNb, fillVec, weight);
                    mgrAll->at(eventClass::cro)->fill2DHistograms(processNb, fillVec2D, weight);
                    mgrAll->at(eventClass::cro)->fillSingleHistograms(processNb, singleEntries, weight);
                }

            } else if (nominalClass == eventClass::ssdl) {   

                std::vector<double> scores = mva_DL->scoreEvent();

                fillVec = fourTopHists::fillAllHists(false, selection);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                fillVec2D = mva_DL->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_DL->getClassAndScore();   
                singleEntries.push_back({dlPosMVA + classAndScore.first, classAndScore.second});

                if (selection->getLepton(0)->charge() > 0) {
                    subChannels.push_back("++");
                } else {
                    subChannels.push_back("--");
                }

                if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) {
                    subChannels.push_back("ee");
                } else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) {
                    subChannels.push_back("mm");
                } else {
                    subChannels.push_back("em");
                }

                mgrAll->at(eventClass::ssdl)->fillAllHistograms(subChannels, processNb, fillVec, weight);
                mgrAll->at(eventClass::ssdl)->fillAll2DHistograms(subChannels, processNb, fillVec2D, weight);
                mgrAll->at(eventClass::ssdl)->fillAllSingleHistograms(subChannels, processNb, singleEntries, weight);

            } else if (nominalClass == eventClass::trilep) {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllHists(true, selection);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                fillVec2D = mva_ML->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();   
                singleEntries.push_back({mlPosMVA + classAndScore.first, classAndScore.second});

                mgrAll->at(eventClass::trilep)->fillHistograms(processNb, fillVec, weight);
                mgrAll->at(eventClass::trilep)->fill2DHistograms(processNb, fillVec2D, weight);
                mgrAll->at(eventClass::trilep)->fillSingleHistograms(processNb, singleEntries, weight);
            } else if (nominalClass == eventClass::fourlep) {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllHists(true, selection, true);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                fillVec2D = mva_ML->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();  
                singleEntries.push_back({fourlPosMVA + classAndScore.first, classAndScore.second});
                
                mgrAll->at(eventClass::fourlep)->fillHistograms(processNb, fillVec, weight);
                mgrAll->at(eventClass::fourlep)->fill2DHistograms(processNb, fillVec2D, weight);
                mgrAll->at(eventClass::fourlep)->fillSingleHistograms(processNb, singleEntries, weight);
            }


            // TODO: Systematics
            if (currentEvent->isData()) continue;

            //// Start filling histograms
            // loop uncertainties
            Channel* uncWrapper = mgrAll->at(selection->getCurrentClass());

            std::vector<double> fillVecUp = fillVec;
            std::vector<double> fillVecDown = fillVec;
            std::vector<std::pair<int, double>> singleEntriesUp = singleEntries;
            std::vector<std::pair<int, double>> singleEntriesDown = singleEntries;
            std::vector<std::pair<double, double>> fillVec2DUp = fillVec2D;
            std::vector<std::pair<double, double>> fillVec2DDown = fillVec2D;
            eventClass upClass = eventClass::fail;
            eventClass downClass = eventClass::fail;

            unsigned uncID = 0;
            while (selection->getCurrentClass() != eventClass::fail && uncID < shapeUncId::end) {
                double weightUp = 1.;
                double weightDown = 1.;
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
                } else if (uncID == shapeUncId::pdfShapeVar) {
                    std::vector<double> pdfVariations;
                    if (hasValidPdfs && xsecs.get()->numberOfLheVariations() > 9) {
                        int max = 100;
                        if (numberOfPdfVariations < max) {
                            max = numberOfPdfVariations;
                        }
                        for(int i=0; i<max; ++i){
                            pdfVariations.push_back(weight * currentEvent->generatorInfo().relativeWeightPdfVar(i) / xsecs.get()->crossSectionRatio_pdfVar(i));
                        }
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
                        double nombweight = 1.;//reweighter["bTag_shape"]->weight( *currentEvent );
                        for(std::string btagsys : bTagShapeSystematics){
                            weightUp = 1.;// * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, btagsys ) / nombweight;
                            weightDown = 1.;// * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, btagsys ) / nombweight;

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
                } else if (uncID >= shapeUncId::JER_1p93) {
                    // JER and JEC

                    if( uncID == shapeUncId::JEC && considerBTagShape ) {
                        //weightUp = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, "JECUp" ) 
                        //                    / reweighter["bTag_shape"]->weight( *currentEvent );
                        //weightDown = dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"] )->weightJecVar( *currentEvent, "JECDown" ) 
                        //                    / reweighter["bTag_shape"]->weight( *currentEvent );
                    }

                    upClass = selection->classifyUncertainty(shapeUncId(uncID), true);
                    fillVecUp = selection->fillVector();
                    singleEntriesUp = selection->singleFillEntries();
                    fillVec2DUp = selection->fillVector2D();

                    downClass = selection->classifyUncertainty(shapeUncId(uncID), false);
                    fillVecDown = selection->fillVector();
                    singleEntriesDown = selection->singleFillEntries();
                    fillVec2DDown = selection->fillVector2D();

                    if (upClass == eventClass::ssdl) {
                        if (selection->getLepton(0)->charge() > 0) subChannelsUp.push_back("++");
                        else subChannelsUp.push_back("--");

                        if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) subChannelsUp.push_back("ee");
                        else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) subChannelsUp.push_back("mm");
                        else subChannelsUp.push_back("em");
                    }

                    if (downClass == eventClass::ssdl) {
                        if (selection->getLepton(0)->charge() > 0) subChannelsDown.push_back("++");
                        else subChannelsDown.push_back("--");

                        if (selection->getLepton(0)->isElectron() && selection->getLepton(1)->isElectron()) subChannelsDown.push_back("ee");
                        else if (selection->getLepton(0)->isMuon() && selection->getLepton(1)->isMuon()) subChannelsDown.push_back("mm");
                        else subChannelsDown.push_back("em");
                    }
                }

                if (uncID < shapeUncId::JER_1p93) {
                    uncWrapper->fillAllUncertainties(subChannels, shapeUncId(uncID), processNb, fillVec, weight * weightUp, weight * weightDown);
                    uncWrapper->fillAllSingleUncertainties(subChannels, shapeUncId(uncID), processNb, singleEntries, weight * weightUp, weight * weightDown);
                    uncWrapper->fillAll2DUncertainties(subChannels, shapeUncId(uncID), processNb, fillVec2D, weight * weightUp, weight * weightDown);
                } else {
                    mgrAll->fillAllUpHistograms(subChannelsUp, upClass, shapeUncId(uncID), processNb, fillVecUp, singleEntriesUp, fillVec2DUp, weight * weightUp);
                    mgrAll->fillAllDownHistograms(subChannelsDown, downClass, shapeUncId(uncID), processNb, fillVecDown, singleEntriesDown, fillVec2DDown, weight * weightDown);
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

        std::cout << "writing uncertainties" << std::endl;

        outfile->cd();
        outfile->cd("Uncertainties");

        mgrAll->writeUncertaintyHistograms(outdir);
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
        // TODO: event is data, apply triggers!!
        return true;
    }

    return true;
}