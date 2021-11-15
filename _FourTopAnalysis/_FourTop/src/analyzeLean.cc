#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: analyzeLean() {

    // Loop samples
    std::string channelDL = "DL";
    std::vector<HistInfo>* infoDL = fourTopHists::allHists(channelDL, false, false);
    HistogramManager* DLManager = new HistogramManager(channelDL, infoDL);

    std::string channel3L = "3L";
    std::vector<HistInfo>* info3L = fourTopHists::allHists(channel3L, true, false);
    HistogramManager* TriLManager = new HistogramManager(channel3L, info3L);

    std::string channel4L = "4L";
    std::vector<HistInfo>* info4L = fourTopHists::allHists(channel4L, true, true);
    HistogramManager* FourLManager = new HistogramManager(channel4L, info4L);

    std::string channelCRZ = "CRZ";
    std::vector<HistInfo>* infoCRZ = fourTopHists::allHists(channelCRZ, false, false);
    HistogramManager* CRZManager = new HistogramManager(channelCRZ, infoCRZ);

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

        // check if TTbar or TTGamma sample
        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );

            // Check triggers here
            selection->addNewEvent(currentEvent);

            // Apply overlap removal & apply triggers
            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"

            // apply baseline selection
            // Adapted to work with increased nonprompt yields
            if (selection->numberOfLeps() < 2) continue; // atm we check our tight leps here, for nonprompt est, this becomes FO
            if (selection->numberOfLeps() == 2 && selection->getMediumLepCol()->hasOSPair()) continue;

            if (selection->numberOfJets() < 2 || selection->numberOfMediumBJets() < 1) continue;
            if (selection->getJetCol()->scalarPtSum() < 100) continue;

            if (selection->numberOfLeps() == 3 && selection->getJetCol()->scalarPtSum() < 200) continue;
            if (selection->numberOfLeps() == 2 && selection->getJetCol()->scalarPtSum() < 300) continue;
            if (selection->numberOfLeps() < 4 && selection->numberOfJets() < 3) continue;
            if (selection->numberOfLeps() == 2 && selection->numberOfJets() < 4) continue;



            // Apply scale factors
            // TODO
            
            // Basic non-prompt handling (using MC to estimate the contribution):
            size_t fillIndex = sampleIndex;
            std::vector<double> fillVec;
            
            // If nonprompt: fillIndex becomes index of nonprompt histograms
            LeptonCollection* lepCol = selection->getMediumLepCol();
            bool nonPrompt = false;

            for (const auto& leptonPtr : *lepCol) {
                if (! leptonPtr->isPrompt()) {
                    fillIndex = treeReader->numberOfSamples();
                    nonPrompt = true;

                    break;
                }
            }

            // print mass spectrum 
            // both same-sign as OS


            // Remove mass resonances
            if (! selection->passLowMassVeto()) {
                continue;
            } else if (! selection->passZBosonVeto()) {
                // Build CRZ
                fillVec = fourTopHists::fillAllHists(false, selection);
                CRZManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);
                continue;
            }

            // Fill histograms
            if (selection->numberOfLeps() == 2) {
                fillVec = fourTopHists::fillAllHists(false, selection);
                DLManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);
            } else if (selection->numberOfLeps() == 3) {
                fillVec = fourTopHists::fillAllHists(true, selection);
                TriLManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);
            } else {
                fillVec = fourTopHists::fillAllHists(true, selection, true);
                FourLManager->fillHistograms(fillVec, currentEvent->weight(), nonPrompt);
            }

        }
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
        DLManager->writeCurrentHistograms();
        TriLManager->writeCurrentHistograms();
        FourLManager->writeCurrentHistograms();
        CRZManager->writeCurrentHistograms();
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    DLManager->writeNonpromptHistograms();
    TriLManager->writeNonpromptHistograms();
    FourLManager->writeNonpromptHistograms();
    CRZManager->writeNonpromptHistograms();

    outfile->Close();
}
