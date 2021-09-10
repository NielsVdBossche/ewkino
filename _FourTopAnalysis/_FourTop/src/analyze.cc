#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop::analyze() {

    // Loop samples

    // -> Loop entries
    // ->-> Fill histograms
    // Similar to ewkino example but using more of an object-oriented way of working... + more pointers!

    std::cout << "event loop" << std::endl;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            if (entry > 10000) break;
            
            Event* event = new Event(treeReader->buildEvent( entry ));

            // apply baseline selection
            // Right now we build CRZ from looser objects.
            // Necessary to account for looser leptons which are otherwise missed in the full lepton selection and could be part of a Z-boson resonance
            EventSelection4T::applyBaselineObjectSelection(event);

            if (! EventSelection4T::passBaselineEventSelection(event)) continue;

            event->sortLeptonsByPt();

            if (! EventSelection4T::passZBosonVeto(event)) {
                // TODO: CRZ handling
                continue;
            } else if (! EventSelection4T::passLowMassVeto(event)) {
                continue;
            }
            // Z-res: hasOSLeptonPair()

            // Full object selection (only keep the real useful stuff)
            EventSelection4T::applyFullObjectSelection(event);

            if (! EventSelection4T::passFullEventSelection(event)) continue;

            // Fill histograms
            std::vector<double> fillVec;
            size_t fillIndex = sampleIndex;
            if (event->numberOfLightLeptons() == 2) {
                fillVec = fourTopHists::fillLepInfoDL(event);
                for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ){
                    histogram::fillValue( (*hists_DL)[ dist ][ fillIndex ].get(), fillVec[ dist ], event->weight() );
                }

            } else {
                fillVec = fourTopHists::fillLepInfoML(event);
                for( size_t dist = 0; dist < histInfoVec_ML->size(); ++dist ){
                    histogram::fillValue( (*hists_ML)[ dist ][ fillIndex ].get(), fillVec[ dist ], event->weight() );
                }

            }

            delete event;

            
        }
         
        // works when handling only one sample
        for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
            hists_DL->at(dist)[sampleIndex]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
            hists_DL->at(dist)[sampleIndex]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        }
    }

  
}
