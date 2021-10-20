#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: analyze() {

    // Loop samples

    // -> Loop entries
    // ->-> Fill histograms
    // Similar to ewkino example but using more of an object-oriented way of working... + more pointers!

    std::cout << "event loop" << std::endl;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        // check if TTbar or TTGamma sample
        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;

            currentEvent = treeReader->buildEventPtr( entry );
            selection->addNewEvent(currentEvent);

            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"
            // apply baseline selection
            // Right now we build CRZ from looser objects.
            // Necessary to account for looser leptons which are otherwise missed in the full lepton selection and could be part of a Z-boson resonance

            if (! infuseNonPrompt && ! selection->passBaselineEventSelection())  {
                delete currentEvent;
                continue;
            }
            if (infuseNonPrompt && ttgOverlapCheck > 0) {
                if (! selection->passBaselineEventSelectionWithAltLeptons()) {
                    delete currentEvent;
                    continue;
                }
            }
            
            // Basic non-prompt handling (using MC to estimate the contribution):
            size_t fillIndex = sampleIndex;
            std::vector<double> fillVec;
            
            // If nonprompt: fillIndex becomes index of nonprompt histograms
            if (selection->isEventNormalSelected()) {
                for (const auto& leptonPtr : *selection->getMediumLepCol()) {
                    if (! leptonPtr->isPrompt()) {
                        fillIndex = treeReader->numberOfSamples();
                        break;
                    }
                }
            } else {
                for (const auto& leptonPtr : *selection->getAltLeptonCol()) {
                    if (! leptonPtr->isPrompt()) {
                        fillIndex = treeReader->numberOfSamples();
                        break;
                    }
                }
            }
            
            // Remove mass resonances

            if (! selection->passZBosonVeto()) {
                // Build CRZ
                fillVec = fourTopHists::fillAllHists(false, selection);
                histHelper::histFiller(fillVec, &(hists_CRZ->at(fillIndex)), currentEvent->weight());
                delete currentEvent;
                continue;
            } else if (! selection->passLowMassVeto()) {
                delete currentEvent;
                continue;
            }

            // Full object selection (only keep the real useful stuff)
            if (! selection->passFullEventSelection()) {
                fillVec = fourTopHists::fillAllHists(false, selection);
                histHelper::histFiller(fillVec, &(hists_Other->at(fillIndex)), currentEvent->weight());
                delete currentEvent;
                continue;
            }


            // Build CRW (might expand these)
            if (selection->numberOfLeps() == 2 && selection->numberOfJets() < 6 && selection->numberOfMediumBJets() == 2) {
                fillVec = fourTopHists::fillAllHists(false, selection);
                histHelper::histFiller(fillVec, &(hists_CRW->at(fillIndex)), currentEvent->weight());
                delete currentEvent;
                continue;
            }

            // Fill histograms
            if (selection->numberOfLeps() == 2) {
                fillVec = fourTopHists::fillAllHists(false, selection);
                histHelper::histFiller(fillVec, &(hists_DL->at(fillIndex)), currentEvent->weight());
                if (histInfoVec_mva_DL) {
                    std::vector<Float_t> scores = mva_DL->scoreEvent();
                    mva_DL->fillHistograms(scores, hists_mva_DL->at(fillIndex), currentEvent->weight());
                    mva_DL->fill2DHistograms(scores, hists2D_mva_DL->at(fillIndex), currentEvent->weight());
                }
            } else {
                fillVec = fourTopHists::fillAllHists(true, selection);
                histHelper::histFiller(fillVec, &(hists_ML->at(fillIndex)), currentEvent->weight());

                if (histInfoVec_mva_ML) {
                    std::vector<Float_t> scores = mva_ML->scoreEvent();
                    mva_ML->fillHistograms(scores, hists_mva_ML->at(fillIndex), currentEvent->weight());
                    mva_ML->fill2DHistograms(scores, hists2D_mva_ML->at(fillIndex), currentEvent->weight());
                }
            }

            // TODO: Systematics

            delete currentEvent;
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

        // works when handling only one sample
        for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
            hists_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_ML->size(); ++dist ) {
            hists_ML->at(sampleIndex)[dist]->Write(TString(histInfoVec_ML->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_CRZ->size(); ++dist ) {
            hists_CRZ->at(sampleIndex)[dist]->Write(TString(histInfoVec_CRZ->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_CRW->size(); ++dist ) {
            hists_CRW->at(sampleIndex)[dist]->Write(TString(histInfoVec_CRW->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_Other->size(); ++dist ) {
            hists_Other->at(sampleIndex)[dist]->Write(TString(histInfoVec_Other->at(dist).name()), TObject::kOverwrite);
        }
        
        if (histInfoVec_mva_DL) {
            for( size_t dist = 0; dist < histInfoVec_mva_DL->size(); ++dist ) {
                hists_mva_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec_mva_DL->at(dist).name()), TObject::kOverwrite);
            }

            for( size_t dist = 0; dist < histInfoVec_mva_ML->size(); ++dist ) {
                hists_mva_ML->at(sampleIndex)[dist]->Write(TString(histInfoVec_mva_ML->at(dist).name()), TObject::kOverwrite);
            }

            for( size_t dist = 0; dist < histInfoVec2D_mva_DL->size(); ++dist ) {
                hists2D_mva_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec2D_mva_DL->at(dist).name()), TObject::kOverwrite);
            }

            for( size_t dist = 0; dist < histInfoVec2D_mva_ML->size(); ++dist ) {
                hists2D_mva_ML->at(sampleIndex)[dist]->Write(TString(histInfoVec2D_mva_ML->at(dist).name()), TObject::kOverwrite);
            }
        }

        // Systematics
        if (currentEvent->isData()) continue;
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
        hists_DL->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_ML->size(); ++dist ) {
        hists_ML->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_ML->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_CRZ->size(); ++dist ) {
        hists_CRZ->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_CRZ->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_CRW->size(); ++dist ) {
        hists_CRW->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_CRW->at(dist).name()), TObject::kOverwrite);
    }

    for( size_t dist = 0; dist < histInfoVec_Other->size(); ++dist ) {
        hists_Other->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_Other->at(dist).name()), TObject::kOverwrite);
    }

    if (histInfoVec_mva_DL) {
        for( size_t dist = 0; dist < histInfoVec_mva_DL->size(); ++dist ) {
            hists_mva_DL->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_mva_DL->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_mva_ML->size(); ++dist ) {
            hists_mva_ML->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec_mva_ML->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec2D_mva_DL->size(); ++dist ) {
            hists2D_mva_DL->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec2D_mva_DL->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec2D_mva_ML->size(); ++dist ) {
            hists2D_mva_ML->at(treeReader->numberOfSamples())[dist]->Write(TString(histInfoVec2D_mva_ML->at(dist).name()), TObject::kOverwrite);
        }
    }

    outfile->Close();
}
