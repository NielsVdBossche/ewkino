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
    if (fabs(one-two) < 1e-3) return true;
    return false;
}

void raiseError(std::string obj, std::string quant, double diff) {
    std::cout << obj << " mismatch for " << quant << " with diff " << diff << std::endl;
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
            if (entry % 10000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;

            int retValLoad = syncTree->GetEntryWithIndex(event.eventNumber());
            if (retValLoad <= 0 || event.eventNumber() != syncTreeContent->_eventNumber) {
                //std::cout << "event number " << event.eventNumber() << " not found. Skipping" << std::endl;
                continue;
            }

            eventsUsed++;
            std::cout << "use event " << event.eventNumber() << std::endl;
            
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

            LeptonCollection leptons = LeptonCollection(event.leptonCollection());
            leptons.selectLooseLeptons();
            int numberOfLooseElectrons = leptons.numberOfElectrons();
            int numberOfLooseMuons = leptons.numberOfMuons();
            leptons.selectFOLeptons();
            int numberOfFOElectrons = leptons.numberOfElectrons();
            int numberOfFOMuons = leptons.numberOfMuons();
            leptons.selectTightLeptons();
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
                std::cout << "mismatch in ";
                if (numberOfLooseElectronsSync != numberOfLooseElectrons) std::cout << "number of loose electrons " << numberOfLooseElectronsSync << " " << numberOfLooseElectrons;
                if (numberOfFOElectronsSync != numberOfFOElectrons) std::cout << "number of FO electrons " << numberOfFOElectronsSync << " " << numberOfFOElectrons;
                if (numberOfTightElectronsSync != numberOfTightElectrons) std::cout << "number of Tight electrons " << numberOfTightElectronsSync << " " << numberOfTightElectrons;
                std::cout << std::endl;
                unsigned looseCounter = 0;
                
                for (unsigned i=0; i<electrons.size(); i++) {
                    if (! syncTreeContent->electrons_is_loose->at(i)) continue;

                    Electron curr = electrons[looseCounter];
                    looseCounter++;
                    
                    if (curr.isMuon()) continue;
                    std::cout << "new electron" << std::endl;

                    if (! floatEqual(curr.eta(), syncTreeContent->electrons_eta->at(i))) raiseError("Electron", "eta", (curr.eta() - syncTreeContent->electrons_eta->at(i)));
                    if (! floatEqual(curr.phi(), syncTreeContent->electrons_phi->at(i))) raiseError("Electron", "phi", (curr.phi() - syncTreeContent->electrons_phi->at(i)));
                    if (! floatEqual(curr.pt(), syncTreeContent->electrons_pt->at(i))) raiseError("Electron", "pt", (curr.pt() - syncTreeContent->electrons_pt->at(i)));
                    if (! floatEqual(curr.closestJetDeepFlavor(), syncTreeContent->electrons_bscore->at(i))) raiseError("Electron", "bscore", (curr.closestJetDeepFlavor() - syncTreeContent->electrons_bscore->at(i)));
                    if (! floatEqual(curr.leptonMVATOPUL(), syncTreeContent->electrons_extMVAscore->at(i))) raiseError("Electron", "mva", (curr.leptonMVATOPUL() - syncTreeContent->electrons_extMVAscore->at(i)));
                    
                    if (! floatEqual(curr.relIso0p3(), syncTreeContent->electrons_pfRelIso03_all->at(i))) raiseError("Electron", "relIso0p3", (curr.relIso0p3() - syncTreeContent->electrons_pfRelIso03_all->at(i)));
                    if (! floatEqual(curr.miniIso(), syncTreeContent->electrons_miniPFRelIso_all->at(i))) raiseError("Electron", "miniIso", (curr.miniIso() - syncTreeContent->electrons_miniPFRelIso_all->at(i)));
                    if (! floatEqual(curr.miniIsoCharged(), syncTreeContent->electrons_miniPFRelIso_chg->at(i))) raiseError("Electron ", "miniIsoCharged", (curr.miniIsoCharged() - syncTreeContent->electrons_miniPFRelIso_chg->at(i)));
                    if (! floatEqual(curr.ptRel(), syncTreeContent->electrons_ptrel_final->at(i))) raiseError("Electron", "ptRel", (curr.ptRel() - syncTreeContent->electrons_ptrel_final->at(i)));
                    if (! floatEqual(curr.ptRatio(), syncTreeContent->electrons_ptratio_final->at(i))) raiseError("Electron", "ptRatio", (curr.ptRatio() - syncTreeContent->electrons_ptratio_final->at(i)));
                    if (! floatEqual(curr.sip3d(), syncTreeContent->electrons_sip3d->at(i))) raiseError("Electron ", "sip3d", (curr.sip3d() - syncTreeContent->electrons_sip3d->at(i)));
                }
            }
            if (numberOfLooseMuonsSync != numberOfLooseMuons || numberOfFOMuonsSync != numberOfFOMuons || numberOfTightMuonsSync != numberOfTightMuons) {
                MuonCollection muons = event.muonCollection();
                muons.sortByPt();
                std::cout << "mismatch in ";
                if (numberOfLooseMuonsSync != numberOfLooseMuons) std::cout << "number of loose Muons " << numberOfLooseMuonsSync << " " << numberOfLooseMuons;;
                if (numberOfFOMuonsSync != numberOfFOMuons) std::cout << "number of FO Muons " << numberOfFOMuonsSync << " " << numberOfFOMuons;;
                if (numberOfTightMuonsSync != numberOfTightMuons) std::cout << "number of Tight Muons " << numberOfTightMuonsSync << " " << numberOfTightMuons;;
                std::cout << std::endl;
                unsigned looseCounter = 0;
                for (unsigned i=0; i<syncTreeContent->muons_is_loose->size(); i++) {
                    if (! syncTreeContent->muons_is_loose->at(i)) continue;
                    
                    Muon curr = muons[looseCounter];
                    looseCounter++;
                    if (curr.isElectron()) continue;

                    std::cout << "new muon" << std::endl;

                    if (! floatEqual(curr.eta(), syncTreeContent->muons_eta->at(i))) raiseError("Muon", "eta", (curr.eta() - syncTreeContent->muons_eta->at(i)));
                    if (! floatEqual(curr.phi(), syncTreeContent->muons_phi->at(i))) raiseError("Muon", "phi", (curr.phi() - syncTreeContent->muons_phi->at(i)));
                    if (! floatEqual(curr.pt(), syncTreeContent->muons_pt->at(i))) raiseError("Muon", "pt", (curr.pt() - syncTreeContent->muons_pt->at(i)));
                    if (! floatEqual(curr.closestJetDeepFlavor(), syncTreeContent->muons_bscore->at(i))) raiseError("Muon", "bscore", (curr.closestJetDeepFlavor() - syncTreeContent->muons_bscore->at(i)));
                    if (! floatEqual(curr.leptonMVATOPUL(), syncTreeContent->muons_extMVAscore->at(i))) raiseError("Muon", "mva", (curr.leptonMVATOPUL() - syncTreeContent->muons_extMVAscore->at(i)));

                    if (! floatEqual(curr.relIso0p3(), syncTreeContent->muons_pfRelIso03_all->at(i))) raiseError("Muon", "relIso0p3", (curr.relIso0p3() - syncTreeContent->muons_pfRelIso03_all->at(i)));
                    if (! floatEqual(curr.miniIso(), syncTreeContent->muons_miniPFRelIso_all->at(i))) raiseError("Muon", "miniIso", (curr.miniIso() - syncTreeContent->muons_miniPFRelIso_all->at(i)));
                    if (! floatEqual(curr.miniIsoCharged(), syncTreeContent->muons_miniPFRelIso_chg->at(i))) raiseError("Muon ", "miniIsoCharged", (curr.miniIsoCharged() - syncTreeContent->muons_miniPFRelIso_chg->at(i)));
                    if (! floatEqual(curr.ptRel(), syncTreeContent->muons_ptrel_final->at(i))) raiseError("Muon", "ptRel", (curr.ptRel() - syncTreeContent->muons_ptrel_final->at(i)));
                    if (! floatEqual(curr.ptRatio(), syncTreeContent->muons_ptratio_final->at(i))) raiseError("Muon", "ptRatio", (curr.ptRatio() - syncTreeContent->muons_ptratio_final->at(i)));
                    if (! floatEqual(curr.sip3d(), syncTreeContent->muons_sip3d->at(i))) raiseError("Muon ", "sip3d", (curr.sip3d() - syncTreeContent->muons_sip3d->at(i)));

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
