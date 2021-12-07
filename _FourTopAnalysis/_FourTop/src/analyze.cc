#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: analyze() {
    ChannelManager* mgrAll = new ChannelManager();

    // reweighter creation
    std::cout << "building reweighter" << std::endl;
    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector() );

    // Histogram creation
    std::string channelDL = "DL";
    std::vector<HistInfo>* infoDL = fourTopHists::allHists(channelDL, false, false);
    HistogramManager* DLManager = new HistogramManager(channelDL, infoDL);
    size_t dlPosMVA = infoDL->size();
    DLManager->extendHistInfo(mva_DL->createHistograms(""));
    DLManager->set2DHistInfo(mva_DL->create2DHistograms(""));

    dlPosMVA += mva_DL->getMaxClass();

    std::string channel3L = "3L";
    std::vector<HistInfo>* info3L = fourTopHists::allHists(channel3L, true, false);
    HistogramManager* TriLManager = new HistogramManager(channel3L, info3L);
    size_t mlPosMVA = info3L->size();
    TriLManager->extendHistInfo(mva_ML->createHistograms(""));
    TriLManager->set2DHistInfo(mva_ML->create2DHistograms(""));

    mlPosMVA += mva_ML->getMaxClass();

    std::string channel4L = "4L";
    std::vector<HistInfo>* info4L = fourTopHists::allHists(channel4L, true, true);
    HistogramManager* FourLManager = new HistogramManager(channel4L, info4L);
    size_t fourlPosMVA = info4L->size();
    FourLManager->extendHistInfo(mva_ML->createHistograms("", true));
    FourLManager->set2DHistInfo(mva_ML->create2DHistograms("", true));

    fourlPosMVA += mva_ML->getMaxClass();

    std::string channelCRZ = "CRZ";
    std::vector<HistInfo>* infoCRZ = fourTopHists::infoLean(channelCRZ, true);
    HistogramManager* CRZManager = new HistogramManager(channelCRZ, infoCRZ);

    std::string channelCRW = "CRW";
    std::vector<HistInfo>* infoCRW = fourTopHists::infoLean(channelCRW, false);
    HistogramManager* CRWManager = new HistogramManager(channelCRW, infoCRW);

    std::string channelCRO = "CRO";
    std::vector<HistInfo>* infoCRO = fourTopHists::infoLean(channelCRO, false);
    HistogramManager* CROManager = new HistogramManager(channelCRO, infoCRO);

    mgrAll->addChannel(eventClass::ssdl, DLManager);
    mgrAll->addChannel(eventClass::trilep, TriLManager);
    mgrAll->addChannel(eventClass::fourlep, FourLManager);
    mgrAll->addChannel(eventClass::crz, CRZManager);
    mgrAll->addChannel(eventClass::crw, CRWManager);
    mgrAll->addChannel(eventClass::cro, CROManager);

    std::cout << "event loop" << std::endl;
    currentEvent = new Event();

    std::vector<Sample> sampleVec = treeReader->sampleVector();

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        DLManager->newSample(uniqueName);
        TriLManager->newSample(uniqueName);
        FourLManager->newSample(uniqueName);
        CRZManager->newSample(uniqueName);
        CRWManager->newSample(uniqueName);
        CROManager->newSample(uniqueName);
        // check if TTbar or TTGamma sample
        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

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
            bool nonPrompt = false;

            for (const auto& leptonPtr : *selection->getMediumLepCol()) {
                if (! leptonPtr->isPrompt()) {
                    nonPrompt = true;
                    break;
                }
            }

            // fill all histograms
            if (selection->getCurrentClass() >= eventClass::crz && selection->getCurrentClass() < eventClass::ssdl) {
                fillVec = fourTopHists::fillAllLean(false, selection); // change falses/trues by eventClass

            } else if (selection->getCurrentClass() == eventClass::ssdl) {   

                std::vector<double> scores = mva_DL->scoreEvent();

                fillVec = fourTopHists::fillAllHists(false, selection);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                fillVec2D = mva_DL->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_DL->getClassAndScore();   
                singleEntries.push_back({dlPosMVA + classAndScore.first, classAndScore.second});

                DLManager->fillHistograms(fillVec, weight, nonPrompt);
                DLManager->fill2DHistograms(fillVec2D, weight, nonPrompt);
                DLManager->fillSingleHistogram(dlPosMVA + classAndScore.first, classAndScore.second, weight, nonPrompt);
            } else if (selection->getCurrentClass() == eventClass::trilep) {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllHists(true, selection);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                fillVec2D = mva_ML->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();   
                singleEntries.push_back({mlPosMVA + classAndScore.first, classAndScore.second});

                TriLManager->fillHistograms(fillVec, weight, nonPrompt);
                TriLManager->fill2DHistograms(fillVec2D, weight, nonPrompt);
                TriLManager->fillSingleHistogram(mlPosMVA + classAndScore.first, classAndScore.second, weight, nonPrompt);
            } else if (selection->getCurrentClass() == eventClass::fourlep) {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllHists(true, selection, true);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                fillVec2D = mva_ML->fill2DVector();

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();  
                singleEntries.push_back({fourlPosMVA + classAndScore.first, classAndScore.second});
                
                FourLManager->fillHistograms(fillVec, weight, nonPrompt);
                FourLManager->fill2DHistograms(fillVec2D, weight, nonPrompt);
                FourLManager->fillSingleHistogram(fourlPosMVA + classAndScore.first, classAndScore.second, weight, nonPrompt);
            }


            // TODO: Systematics
            if (currentEvent->isData()) continue;

            //// Start filling histograms
            // loop uncertainties
            UncertaintyWrapper* uncWrapper = mgrAll->getChannelUncertainties(selection->getCurrentClass());

            unsigned uncID = shapeUncId::pileup;
            while (selection->getCurrentClass() != eventClass::fail && uncID != shapeUncId::end) {
                std::string uncString = "pileup";
                double weightUp = reweighter[ uncString ]->weightUp( *currentEvent ) / reweighter[ uncString ]->weight( *currentEvent );
                double weightDown = reweighter[ uncString ]->weightDown( *currentEvent ) / reweighter[ uncString ]->weight( *currentEvent );

                uncWrapper->fillUncertainty(shapeUncId(uncID), fillVec, weightUp, weightDown, nonPrompt);

                uncID = uncID + 1;
            }

        }
        
        // Output management: save histograms to a ROOT file.
        // Processes were named in samplelist. Should use this to make directory for process and subdir for sample

        // Might interface with Stacker to create desired output plots as well... Or at least already have the stacked process ready instead of individual components. Then a "getDirectory" in stacker could be handy to see if it exists.
        outfile->cd();
        outfile->cd("Nominal");
        const char* processName = treeReader->currentSample().processName().c_str();
        if (! gDirectory->GetDirectory(processName)) {
            gDirectory->mkdir(processName);
        }
        gDirectory->cd(processName);

        std::string outdir = stringTools::fileNameFromPath(treeReader->currentSample().fileName());
        gDirectory->mkdir(outdir.c_str()); // got to switch to gDirectory. Otherwise keeps working as if we're on level of file
        gDirectory->cd(outdir.c_str());

        // Rewrite this to a dedicated function maybe, or something where we don't have to call sampleIndex each time?
        //for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
        //    hists_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        //}
        DLManager->writeCurrentHistograms();
        TriLManager->writeCurrentHistograms();
        FourLManager->writeCurrentHistograms();
        CRZManager->writeCurrentHistograms();
        CRWManager->writeCurrentHistograms();
        CROManager->writeCurrentHistograms();

        outfile->cd();
        outfile->cd("Uncertainties");
        if (! gDirectory->GetDirectory(processName)) {
            gDirectory->mkdir(processName);
        }
        gDirectory->cd(processName);
        gDirectory->mkdir(outdir.c_str());
        gDirectory->cd(outdir.c_str());

        mgrAll->writeCurrentHistograms();
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    DLManager->writeNonpromptHistograms();
    TriLManager->writeNonpromptHistograms();
    FourLManager->writeNonpromptHistograms();
    CRZManager->writeNonpromptHistograms();
    CRWManager->writeNonpromptHistograms();
    CROManager->writeNonpromptHistograms();

    outfile->cd("Uncertainties");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");
    
    mgrAll->writeNonpromptHistograms();


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