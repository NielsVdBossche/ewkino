#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: analyze() {

    // Loop samples

    // -> Loop entries
    // ->-> Fill histograms
    // Similar to ewkino example but using more of an object-oriented way of working... + more pointers!

    // build histograms for kinematic variables
    // use a fill vector builder and a int or something like that to specify setup?
    // might be more useful... 
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

            // apply baseline selection
            // Adapted to work with increased nonprompt yields
            if (infuseNonPrompt && ttgOverlapCheck > 0) {
                if (! selection->passBaselineEventSelectionWithAltLeptons()) {
                    //delete currentEvent;
                    continue;
                }
            } else if (! selection->passBaselineEventSelection())  {
                //delete currentEvent;
                continue;
            }

            // Apply scale factors
            // TODO
            
            // Basic non-prompt handling (using MC to estimate the contribution):
            std::vector<double> fillVec;
            bool nonPrompt = false;
            // If nonprompt: fillIndex becomes index of nonprompt histograms
            LeptonCollection* lepCol;
            if (selection->isEventNormalSelected()) {
                lepCol = selection->getMediumLepCol();
            } else {
                lepCol = selection->getAltLeptonCol();
            }

            for (const auto& leptonPtr : *lepCol) {
                if (! leptonPtr->isPrompt()) {
                    nonPrompt = true;
                    break;
                }
            }
            
            // Remove mass resonances
            if (! selection->passLowMassVeto()) {

                continue;
            } else if (! selection->passZBosonVeto()) {
                // Build CRZ
                fillVec = fourTopHists::fillAllLean(true, selection);
                CRZManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);

                continue;
            }

            // Full object selection (only keep the real useful stuff and rest is control)
            if (! selection->passFullEventSelection()) {
                fillVec = fourTopHists::fillAllLean(false, selection);
                CROManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);

                continue;
            }

            // Build CRW (might expand these)
            if (selection->numberOfLeps() == 2 && selection->numberOfJets() < 6 && selection->numberOfMediumBJets() == 2) {
                fillVec = fourTopHists::fillAllLean(false, selection);
                CRWManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);
                continue;
            }

            // Fill histograms
            // MVA scores to event handler
            // If assigned, add to fillVec
            // else not
            // from mva handler, then ask a class index + stuff for further calculations, keep these indices somehow managed
            if (selection->numberOfLeps() == 2) {
                std::vector<double> scores = mva_DL->scoreEvent();

                fillVec = fourTopHists::fillAllHists(false, selection);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                DLManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);
                
                std::vector<std::pair<double, double>> fillVec2D = mva_DL->fill2DVector();
                DLManager->fill2DHistograms(fillVec2D, currentEvent->weight(), nonPrompt);

                std::pair<MVAClasses, double> classAndScore = mva_DL->getClassAndScore();                
                DLManager->fillSingleHistogram(dlPosMVA + classAndScore.first, classAndScore.second, currentEvent->weight(), nonPrompt);

            } else if (selection->numberOfLeps() == 3) {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllHists(true, selection);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                TriLManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);

                std::vector<std::pair<double, double>> fillVec2D = mva_ML->fill2DVector();
                TriLManager->fill2DHistograms(fillVec2D, currentEvent->weight(), nonPrompt);

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();                
                TriLManager->fillSingleHistogram(mlPosMVA + classAndScore.first, classAndScore.second, currentEvent->weight(), nonPrompt);
                
            } else {
                std::vector<double> scores = mva_ML->scoreEvent();

                fillVec = fourTopHists::fillAllHists(true, selection, true);
                fillVec.insert(fillVec.end(), scores.begin(), scores.end());

                FourLManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);

                std::vector<std::pair<double, double>> fillVec2D = mva_ML->fill2DVector();
                FourLManager->fill2DHistograms(fillVec2D, currentEvent->weight(), nonPrompt);

                std::pair<MVAClasses, double> classAndScore = mva_ML->getClassAndScore();                
                FourLManager->fillSingleHistogram(fourlPosMVA + classAndScore.first, classAndScore.second, currentEvent->weight(), nonPrompt);
                
            }

            // TODO: Systematics
            if (currentEvent->isData()) {

                continue;
            }

            //// Start filling histograms

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