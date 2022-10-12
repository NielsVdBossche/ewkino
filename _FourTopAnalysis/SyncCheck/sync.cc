#include <TTreeIndex.h>
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Event/interface/Event.h"

#include "synctree.h"

int countTrue(std::vector<bool>& vec) {
    int ret = 0;
    for (unsigned i=0; i<vec.size(); i++) {
        if (vec[i]) ret++;
    }
    return ret;
}


void syncCheckLoop(std::string& syncfile, std::string& ownSampleList) {
    TreeReader* treeReader = new TreeReader(ownSampleList, "/pnfs/iihe/cms/store/user/nivanden/skims/");

    std::vector<Sample> sampleVec = treeReader->sampleVector();

    SyncTree* syncTreeContent = new SyncTree(syncfile);

    TTree* syncTree = syncTreeContent->GetTree();
    syncTree->BuildIndex("EventNumber"); 
    TTreeIndex *index = (TTreeIndex*)syncTree->GetTreeIndex(); 
    
    // set branches

    for( int i = index->GetN() - 1; i >=0 ; --i ) { 
        Long64_t local = syncTree->LoadTree( index->GetIndex()[i] ); 
        syncTree->GetEntry(local);
    }

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        Event eventZero = treeReader->buildEvent(0);

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string processName = sampleVec[sampleIndex].processName();
        std::string uniqueName = sampleVec[sampleIndex].uniqueName();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            // check if event numbers, 
            Event event = treeReader->buildEvent(entry);

            // Basic lepton selection
            event.removeTaus();
            event.selectLooseLeptons();
            event.cleanElectronsFromLooseMuons();
            event.sortLeptonsByPt();

            // basic jet selection
            event.selectGoodJets();
            // Tight leptons are used for cleaning -> Double check used definitions in objectSelection/ElectronSelector.cc and objectSelection/MuonSelector.cc
            // if FO leptons should also be considered, switch to event.cleanJetsFromFOLeptons();
            event.cleanJetsFromFOLeptons();

            LeptonCollection leptons = event.leptonCollection();
            leptons.selectLooseLeptons();
            int numberOfLooseElectrons = leptons.numberOfElectrons();
            int numberOfLooseMuons = leptons.numberOfMuons();
            leptons.selectFOLeptons();
            int numberOfFOElectrons = leptons.numberOfElectrons();
            int numberOfFOMuons = leptons.numberOfMuons();
            event.selectTightLeptons();
            int numberOfTightElectrons = leptons.numberOfElectrons();
            int numberOfTightMuons = leptons.numberOfMuons();
            
            JetCollection bTaggedJets = event.looseBTagCollection();

            syncTree->GetEntryWithIndex(event.eventNumber());
            
            int numberOfLooseElectronsSync = countTrue(syncTreeContent->electrons_is_loose);
            int numberOfFOElectronsSync = countTrue(syncTreeContent->electrons_is_fakeable);
            int numberOfTightElectronsSync = countTrue(syncTreeContent->electrons_is_tight);
            int numberOfLooseMuonsSync = countTrue(syncTreeContent->muons_is_loose);
            int numberOfFOMuonsSync = countTrue(syncTreeContent->muons_is_fakeable);
            int numberOfTightMuonsSync = countTrue(syncTreeContent->muons_is_tight);

            if (numberOfLooseElectronsSync != numberOfLooseElectrons) {
                std::cout << "EvNb " << event.eventNumber() << " x " << syncTreeContent->_eventNumber;
                std::cout << ":\t mismatch " << "Loose electrons " << numberOfLooseElectrons << " x " << numberOfLooseElectronsSync << std::endl;
             }
            if (numberOfFOElectronsSync != numberOfFOElectrons) {
                std::cout << "EvNb " << event.eventNumber() << " x " << syncTreeContent->_eventNumber;
                std::cout << ":\t mismatch " << "FO electrons " << numberOfFOElectrons << " x " << numberOfFOElectronsSync << std::endl;
             }
            if (numberOfTightElectronsSync != numberOfTightElectrons) {
                std::cout << "EvNb " << event.eventNumber() << " x " << syncTreeContent->_eventNumber;
                std::cout << ":\t mismatch " << "Tight electrons " << numberOfTightElectrons << " x " << numberOfTightElectronsSync << std::endl;
             }
            if (numberOfLooseMuonsSync != numberOfLooseMuons) {
                std::cout << "EvNb " << event.eventNumber() << " x " << syncTreeContent->_eventNumber;
                std::cout << ":\t mismatch " << "Loose muons " << numberOfLooseMuons << " x " << numberOfLooseMuonsSync << std::endl;
             }
            if (numberOfFOMuonsSync != numberOfFOMuons) {
                std::cout << "EvNb " << event.eventNumber() << " x " << syncTreeContent->_eventNumber;
                std::cout << ":\t mismatch " << "FO muons " << numberOfFOMuons << " x " << numberOfFOMuonsSync << std::endl;
             }
            if (numberOfTightMuonsSync != numberOfTightMuons) {
                std::cout << "EvNb " << event.eventNumber() << " x " << syncTreeContent->_eventNumber;
                std::cout << ":\t mismatch " << "Tight muons " << numberOfTightMuons << " x " << numberOfTightMuonsSync << std::endl;
             }
        }
    }
}

int main(int argc, char const *argv[]) {
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    
    syncCheckLoop(argvStr[1], argvStr[2]);
    return 0;
}
