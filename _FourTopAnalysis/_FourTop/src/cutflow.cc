#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* getCutflowHist(std::string flag) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    *histInfoVec = {HistInfo("Cutflow_" + flag, "", 11, -.5, 10.5)};
    return histInfoVec;
}

void FourTop::cutFlow() {

    std::string channelDL = "DL";
    std::vector<HistInfo>* infoDL = getCutflowHist(channelDL);
    HistogramManager* DLManager = new HistogramManager(channelDL, infoDL);

    std::string channel3L = "3L";
    std::vector<HistInfo>* info3L = getCutflowHist(channel3L);
    HistogramManager* TriLManager = new HistogramManager(channel3L, info3L);

    std::string channel4L = "4L";
    std::vector<HistInfo>* info4L = getCutflowHist(channel4L);
    HistogramManager* FourLManager = new HistogramManager(channel4L, info4L);
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
        // check if TTbar or TTGamma sample
        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        std::shared_ptr<TH1D> dlHist = DLManager->getHistograms(false)->at(0);
        std::shared_ptr<TH1D> trilepHist = TriLManager->getHistograms(false)->at(0);
        std::shared_ptr<TH1D> fourlepHist = FourLManager->getHistograms(false)->at(0);
        
        int dlSteps = 0;
        int trilepSteps = 0;
        int fourlepSteps = 0;
        // at end of sel, count extra if it flows to other channels (3l and then ssdl?)
        double weight = 0;

        int steps = 0;
        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){

            if (entry != 0) {
                if (selection->numberOfLooseLeps() == 2) dlSteps = steps;
                else if (selection->numberOfLooseLeps() == 3) trilepSteps = steps;
                else if (selection->numberOfLooseLeps() == 4) fourlepSteps = steps;

                for (int i = 0; i < dlSteps; i++) {
                    dlHist->Fill(i - 1, weight);
                }
                for (int i = 0; i < trilepSteps; i++) {
                    trilepHist->Fill(i - 1, weight);
                }
                for (int i = 0; i < fourlepSteps; i++) {
                    fourlepHist->Fill(i - 1, weight);
                }
            }

            dlSteps = 0;
            trilepSteps = 0;
            fourlepSteps = 0;

            steps = 0;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );
            
            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"
            
            selection->addNewEvent(currentEvent);
            weight = currentEvent->weight();

            if (selection->numberOfLooseLeps() < 2) continue;
            if (selection->numberOfLooseLeps() == 2 && selection->getLooseLepCol()->hasOSPair()) continue;

            steps++;

            if (selection->numberOfLeps() < 2) continue;
            if (selection->numberOfLeps() == 2 && selection->getMediumLepCol()->hasOSPair()) continue;

            steps++;

            if (selection->getMediumLepCol()->at(0)->pt() < 25 || selection->getMediumLepCol()->at(1)->pt() < 20) continue;
            
            steps++;

            // Remove mass resonances
            if (! selection->passLowMassVeto()) {
                continue;
            }
            steps++;
            
            if (! selection->passZBosonVeto()) {
                continue;
            }
            steps++;

            if (selection->numberOfJets() < 4) continue;
            steps++;

            if (selection->numberOfMediumBJets() < 2) continue;
            steps++; 

            if (selection->getHT() < 300) continue;
            steps++;

            if (selection->getMET() < 25) continue;
            steps++;

            if (selection->numberOfLeps() == 3) steps++;
            if (selection->numberOfLeps() == 2) steps++;

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
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    DLManager->writeNonpromptHistograms();
    TriLManager->writeNonpromptHistograms();
    FourLManager->writeNonpromptHistograms();

    outfile->Close();
}