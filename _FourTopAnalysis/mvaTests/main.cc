#include "../globalSettings.h"

#include "../../plotting/tdrStyle.h"
#include "../../Tools/interface/Sample.h"
#include "../../Tools/interface/HistInfo.h"
#include "../../Tools/interface/histogramTools.h"
#include "../../TreeReader/interface/TreeReader.h"

#include "../histogramSetups/histogramSetup.h"
#include "../_FourTop/interface/FourTop.h"

// Sets up histograms
// Split for different setups...

#if MEMLEAK
#include "../memleak/debug_new.h" 
#endif

void eventLoop(TreeReader* treeReader) {
    std::vector<Sample> sampleVec = treeReader->sampleVector();

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        Event eventZero = treeReader->buildEvent(0);

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string processName = sampleVec[sampleIndex].processName();
        std::string uniqueName = sampleVec[sampleIndex].uniqueName();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            // Alternative with pointers:
            Event* event = treeReader->buildEventPtr( entry );

            // Basic lepton selection
            event->removeTaus();
            event->selectLooseLeptons();
            event->cleanElectronsFromLooseMuons();
            event->sortLeptonsByPt();

            // basic jet selection
            event->selectGoodJets();
            event->cleanJetsFromTightLeptons();


            // if only tight leptons are desired:
            event->selectTightLeptons();
            // pt tresholds for triggers
            if (event->lepton(0).pt() < 25 || event->lepton(0).pt() < 20) continue;

            JetCollection bTaggedJets = event->looseBTagCollection();

        }
    }
}


int main(int argc, char const *argv[])
{
    setTDRStyle();

    if (argc < 3) {
        std::cout << "One argument is required: <sampleList.txt> <runType>" << std::endl;
    }
    // Set sample directory

    // zet makkelijk te verwerken string op
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Settings splitsen if necessary

    // main func call
    FourTop analysisObject("MVATest_" + argvStr[2], argvStr, 1);

}
