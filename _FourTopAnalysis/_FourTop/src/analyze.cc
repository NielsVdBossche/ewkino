#include "../interface/FourTop.h"

void FourTop::analyze() {

    // Loop samples

    // -> Loop entries
    // ->-> Fill histograms
    // Similar to ewkino example but using more of an object-oriented way of working... + more pointers!

    std::cout << "event loop" << std::endl;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        if( treeReader->isSusy() ) continue;

        std::cout << treeReader->currentSample().fileName() << std::endl;

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            Event* event = &(treeReader->buildEvent( entry ));

            //apply baseline selection
            EventSelection4T::applyBaselineObjectSelection(event);

            if (! EventSelection4T::passBaselineEventSelection(event)) continue;

            // TODO: Resonance cleaning
            if (! EventSelection4T::passZBosonVeto(event)) {
                // CRZ handling
                continue;
            } else if (! EventSelection4T::passLowMassVeto(event)) {
                continue;
            }
            // Z-res: hasOSLeptonPair()

            // Full object selection (only keep the real useful stuff)
            EventSelection4T::applyFullObjectSelection(event);

            if (! EventSelection4T::passFullEventSelection(event)) continue;



            
        }
    }
}
