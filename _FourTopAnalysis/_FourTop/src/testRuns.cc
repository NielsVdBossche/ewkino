#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: testRuns() {
    ChannelManager* mgrAll = new ChannelManager(outfile, fourTopHists::testHists);
    std::shared_ptr< SampleCrossSections > xsecs;

    // reweighter creation
    std::cout << "building reweighter" << std::endl;
    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    ReweighterBTagShape** btagReweighter = new ReweighterBTagShape*();
    CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter );

    addBTaggingNormFactors(*btagReweighter, "bTagNorms/");
    //std::shared_ptr<ReweighterBTagShape> btagReweighterPtr = dynamic_cast<ReweighterBTagShape*>(reweighter["bTag_shape"]);

    std::vector<std::string> processes = {"", "nonPrompt", "ChargeMisID"};
    mgrAll->initHistogramStacks(processes, false);

    std::cout << "event loop" << std::endl;

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    std::vector<std::string> bTagShapeSystematics;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        currentEvent = treeReader->buildEventPtr(0);

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;

        if (! treeReader->isData()) {
            // check if TTbar or TTGamma sample
            ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

            Event event = treeReader->buildEvent(0);
        }

        std::string currProcName = sampleVec[sampleIndex].processName();
        mgrAll->changePrimaryProcess(currProcName);
        
        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        mgrAll->newSample(uniqueName);

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            //if (entry % 1000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );


            // Check triggers here
            if (! eventPassesTriggers()) continue;

            selection->addNewEvent(currentEvent);

            // Apply overlap removal & apply triggers
            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"

            // Remove mass resonances
            if (! selection->passLowMassVeto()) {
                continue;
            }
            selection->classifyEvent();
            // TEMP! Remove for full stuff
            //if (selection->getCurrentClass() == eventClass::fail) continue;

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
                if (leptonPtr->isChargeFlip()) {
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
            fillVec = fourTopHists::fillTestHists(nominalClass, selection);
            if (nominalClass == eventClass::crz3L || nominalClass == eventClass::crz4L || nominalClass == eventClass::cro3L) {
                mgrAll->at(nominalClass)->fillHistograms(processNb, fillVec, weight);
            } else if (nominalClass == eventClass::crw || nominalClass == eventClass::cro) {
                if (nominalClass == eventClass::crw) {
                    mgrAll->at(eventClass::crw)->fillHistograms(processNb, fillVec, weight);
                } else {
                    mgrAll->at(eventClass::cro)->fillHistograms(processNb, fillVec, weight);
                }
            } else if (nominalClass == eventClass::ssdl) {   
                mgrAll->at(eventClass::ssdl)->fillHistograms(processNb, fillVec, weight);
            } else if (nominalClass == eventClass::trilep) {
                mgrAll->at(eventClass::trilep)->fillHistograms(processNb, fillVec, weight);
            } else if (nominalClass == eventClass::fourlep) {
                mgrAll->at(eventClass::fourlep)->fillHistograms(processNb, fillVec, weight);
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
