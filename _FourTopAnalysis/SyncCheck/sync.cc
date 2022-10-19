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

bool floatEqual(double one, double two) {
    if (fabs(one-two) < 1e-4) return true;
    return false;
}

void raiseError(std::string obj, std::string quant) {
    std::cout << obj << " mismatch for " << quant << std::endl;
}

void syncCheckLoop(std::string& syncfile, std::string& ownSampleList) {
    TreeReader* treeReader = new TreeReader(ownSampleList, "/pnfs/iihe/cms/store/user/nivanden/skims/");

    std::vector<Sample> sampleVec = treeReader->sampleVector();

    SyncTree* syncTreeContent = new SyncTree(syncfile);

    TTree* syncTree = syncTreeContent->GetTree();
    syncTree->BuildIndex("EventNumber"); 
    TTreeIndex *index = (TTreeIndex*)syncTree->GetTreeIndex(); 
    
    // set branches

    int eventsUsed = 0;
    int elTightMismatch = 0;
    int elFOMismatch = 0;
    int muTightMismatch = 0;
    int muFOMismatch = 0;


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

            int retValLoad = syncTree->GetEntryWithIndex(event.eventNumber());
            if (retValLoad <= 0 || event.eventNumber() != syncTreeContent->_eventNumber) {
                //std::cout << "event number " << event.eventNumber() << " not found. Skipping" << std::endl;
                continue;
            }
            eventsUsed++;
            
            
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

            int numberOfLooseElectronsSync = countTrue(*(syncTreeContent->electrons_is_loose));
            int numberOfFOElectronsSync = countTrue(*(syncTreeContent->electrons_is_fakeable));
            int numberOfTightElectronsSync = countTrue(*(syncTreeContent->electrons_is_tight));
            int numberOfLooseMuonsSync = countTrue(*(syncTreeContent->muons_is_loose));
            int numberOfFOMuonsSync = countTrue(*(syncTreeContent->muons_is_fakeable));
            int numberOfTightMuonsSync = countTrue(*(syncTreeContent->muons_is_tight));

            if (numberOfLooseElectronsSync != numberOfLooseElectrons || numberOfFOElectronsSync != numberOfFOElectrons || numberOfTightElectronsSync != numberOfTightElectrons) {
                ElectronCollection electrons = event.electronCollection();
                electrons.sortByPt();
                for (unsigned i=0; i<electrons.size(); i++) {
                    Electron curr = electrons[i];
                    if (curr.isMuon()) continue;
                    if (! floatEqual(curr.eta(), syncTreeContent->electrons_eta->at(i))) raiseError("Electron", "eta");
                    if (! floatEqual(curr.phi(), syncTreeContent->electrons_phi->at(i))) raiseError("Electron", "phi");
                    if (! floatEqual(curr.pt(), syncTreeContent->electrons_pt->at(i))) raiseError("Electron", "pt");
                    if (! floatEqual(curr.closestJetDeepFlavor(), syncTreeContent->electrons_bscore->at(i))) raiseError("Electron", "bscore");
                    if (! floatEqual(curr.leptonMVATOPUL(), syncTreeContent->electrons_extMVAscore->at(i))) raiseError("Electron", "mva");

                }
            }
            if (numberOfLooseMuonsSync != numberOfLooseMuons || numberOfFOMuonsSync != numberOfFOMuons || numberOfTightMuonsSync != numberOfTightMuons) {
                MuonCollection muons = event.muonCollection();
                muons.sortByPt();
                for (unsigned i=0; i<muons.size(); i++) {
                    Muon curr = muons[i];
                    if (curr.isElectron()) continue;
                    if (! floatEqual(curr.eta(), syncTreeContent->muons_eta->at(i))) raiseError("Muon", "eta");
                    if (! floatEqual(curr.phi(), syncTreeContent->muons_phi->at(i))) raiseError("Muon", "phi");
                    if (! floatEqual(curr.pt(), syncTreeContent->muons_pt->at(i))) raiseError("Muon", "pt");
                    if (! floatEqual(curr.closestJetDeepFlavor(), syncTreeContent->muons_bscore->at(i))) raiseError("Muon", "bscore");
                    if (! floatEqual(curr.leptonMVATOPUL(), syncTreeContent->muons_extMVAscore->at(i))) raiseError("Muon", "mva");

                }
            }
        }
    }
    std::cout << "validated " << eventsUsed << std::endl;

    std::cout << elTightMismatch << " tight el mismatched " << std::endl;
    std::cout << elFOMismatch << " FO el mismatched " << std::endl;
    std::cout << muTightMismatch << " tight mu mismatched " << std::endl;
    std::cout << muFOMismatch << " FO mu mismatched " << std::endl;
}

int main(int argc, char const *argv[]) {
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    
    syncCheckLoop(argvStr[1], argvStr[2]);
    return 0;
}
