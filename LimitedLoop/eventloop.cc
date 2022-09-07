#include "../TreeReader/interface/TreeReader.h"
#include "../Event/interface/Event.h"

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
            Event event = treeReader->buildEvent(entry);

            // Alternative with pointers:
            // Event* event = treeReader->buildEventPtr( entry );

            // Basic lepton selection
            event.removeTaus();
            event.selectLooseLeptons();
            event.cleanElectronsFromLooseMuons();
            event.sortLeptonsByPt();

            // basic jet selection
            event.selectGoodJets();
            // Tight leptons are used for cleaning -> Double check used definitions in objectSelection/ElectronSelector.cc and objectSelection/MuonSelector.cc
            // if FO leptons should also be considered, switch to event.cleanJetsFromFOLeptons();
            event.cleanJetsFromTightLeptons();


            // if only tight leptons are desired:
            event.selectTightLeptons();
            // pt tresholds for triggers
            if (event.lepton(0).pt() < 25 || event.lepton(0).pt() < 20) continue;

            JetCollection bTaggedJets = event.looseBTagCollection();

            /*
            Event contains a number of objects:
            - JetCollection: a collection of "Jet" objects
            - LeptonCollection: a collection of "Lepton" objects
            ---> Both collections overloaded objects: e.g. Leptoncollection == LeptonCollection : PhysicsObjectCollection
            -> Full definitions found in Event/interface/
            - LightLeptonCollection: very similar to LeptonCollection but mostly separate from LeptonCollection
            - Collections for all other types of objects or subsets. Also includes a MET object
            */
        }
    }
}


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cout << "One argument is required: <sampleList.txt>" << std::endl;
    }
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Samplelist should be structured as:
    // Process name, path to sample, cross section in pb. e.g.:
    // TTTT SomeFourTopSample.root 0.012
    // path to root file should be relative to path supplied on next line 
    TreeReader* treeReader = new TreeReader(argvStr[1], "/pnfs/iihe/cms/store/user/");
    eventLoop(treeReader);
    return 0;
}
