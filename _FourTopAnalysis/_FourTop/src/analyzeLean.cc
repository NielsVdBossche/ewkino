#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: analyzeLean() {

    // Loop samples

    // -> Loop entries
    // ->-> Fill histograms
    // Similar to ewkino example but using more of an object-oriented way of working... + more pointers!

    std::cout << "event loop" << std::endl;
    currentEvent = new Event();

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

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

            // Apply scale factors
            // TODO
            
            // Basic non-prompt handling (using MC to estimate the contribution):
            size_t fillIndex = sampleIndex;
            std::vector<double> fillVec;
            
            // If nonprompt: fillIndex becomes index of nonprompt histograms
            LeptonCollection* lepCol = selection->getMediumLepCol();

            for (const auto& leptonPtr : *lepCol) {
                if (! leptonPtr->isPrompt()) {
                    fillIndex = treeReader->numberOfSamples();
                    break;
                }
            }

            // print mass spectrum 
            // both same-sign as OS


            // Remove mass resonances
            if (! selection->passLowMassVeto()) {
                //continue;
            } else if (! selection->passZBosonVeto()) {
                // Build CRZ
                fillVec = fourTopHists::fillAllHists(false, selection);
                histHelper::histFiller(fillVec, &(hists_CRZ->at(fillIndex)), currentEvent->weight());
                continue;
            }

            // Fill histograms
            if (selection->numberOfLeps() == 2) {
                fillVec = fourTopHists::fillAllHists(false, selection);
                histHelper::histFiller(fillVec, &(hists_DL->at(fillIndex)), currentEvent->weight());
            } else if (selection->numberOfLeps() == 3) {
                fillVec = fourTopHists::fillAllHists(true, selection);
                histHelper::histFiller(fillVec, &(hists_3L->at(fillIndex)), currentEvent->weight());
            } else {
                fillVec = fourTopHists::fillAllHists(true, selection, true);
                histHelper::histFiller(fillVec, &(hists_4L->at(fillIndex)), currentEvent->weight());
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
        for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
            hists_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_3L->size(); ++dist ) {
            hists_3L->at(sampleIndex)[dist]->Write(TString(histInfoVec_3L->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_4L->size(); ++dist ) {
            hists_4L->at(sampleIndex)[dist]->Write(TString(histInfoVec_4L->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_CRZ->size(); ++dist ) {
            hists_CRZ->at(sampleIndex)[dist]->Write(TString(histInfoVec_CRZ->at(dist).name()), TObject::kOverwrite);
        }
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
        hists_DL->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_3L->size(); ++dist ) {
        hists_3L->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_3L->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_4L->size(); ++dist ) {
        hists_4L->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_4L->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_CRZ->size(); ++dist ) {
        hists_CRZ->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_CRZ->at(dist).name()), TObject::kOverwrite);
    }

    outfile->Close();
}
