#include "../interface/EventFourT.h"
#include "../interface/MVAHandler.h"
#include "../../histogramSetups/histogramSetup.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

bool selectLeptonsLooseMVA(const Lepton& lepton) {
    if (! lepton.isLightLepton()) return true;
    const LightLepton& el = (LightLepton&) lepton;
    return (el.leptonMVATOP() > 0.);
}

EventFourT::EventFourT() {
    looseLeps = new LeptonCollection();
    tightLeps = new LeptonCollection();
    foLeps = new LeptonCollection();
    mediumLeps = new LeptonCollection*();
    jets = new JetCollection();
    bTagJets = new JetCollection();

    topReco = new TopReconstructionNew(this);
}

void EventFourT::cleanSelection() {
    delete mediumLeps;
    delete looseLeps;
    delete tightLeps;
    delete foLeps;
    delete jets;
    delete bTagJets;
    scoresMVA.clear();
}

void EventFourT::addNewEvent(Event* newEvent) {
    cleanSelection();
    mediumLeps = new LeptonCollection*();
    event = newEvent;
    isNormalSelected = true;
    currentClass = eventClass::fail;
    
    objectSelection();
}

void EventFourT::objectSelection() {
    event->removeTaus();
    event->selectLooseLeptons();
    event->cleanElectronsFromLooseMuons(); // consider making loose lep sel the original one

    //event->selectGoodJets();
    event->cleanJetsFromFOLeptons(); // Clean jets from leps: is loose leps good or again like earlier

    event->sortLeptonsByPt();

    looseLeps = new LeptonCollection(event->looseLeptonCollection());
    tightLeps = new LeptonCollection(event->TightLeptonCollection());
    foLeps = new LeptonCollection(event->FOLeptonCollection());

    jets = new JetCollection(event->jetCollection());
    jets->selectGoodJets();
    bTagJets = new JetCollection(jets->looseBTagCollection());

    event->selectFOLeptons();

    if (tightLeps->size() == 3) {
    //    LeptonCollection looseWPLeptons(*looseLeps);
    //    looseWPLeptons.selectObjects(selectLeptonsLooseMVA);
    //    if (looseWPLeptons.size() - 3 == 1) {
    //        delete tightLeps;
    //        tightLeps = new LeptonCollection(looseWPLeptons); 
    //    }
    }

    *mediumLeps = tightLeps;

    nJets = jets->size();
    nMediumB = event->numberOfMediumBTaggedJets();
    nTightB = event->numberOfTightBTaggedJets();
    nLooseB = event->numberOfLooseBTaggedJets();
    nLooseLep = looseLeps->size();
    nLep = (*mediumLeps)->size();
    ht = jets->scalarPtSum();
    met = event->met().pt();
}

bool EventFourT::passBaselineEventSelection() {
    // Baseline event selection keeping most leptons in order to correctly veto resonances

    //double n_lep = mediumLeps->size();

    //if (n_lep < 2) return false; // atm we check our tight leps here, for nonprompt est, this becomes FO
    //if (n_lep == 2 && mediumLeps->hasOSPair()) return false;

    //if (! passLeptonSelection()) return false;
    //std::cout << "pqss lep sel" << std::endl;
    //if (mediumLeps->size() < 2) return false;
    //if (mediumLeps->size() == 2 && mediumLeps->hasOSPair()) return false;

    if ((**mediumLeps)[0].pt() < 25 || (**mediumLeps)[1].pt() < 20) return false;

    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    // 2 cuts which must be replaced for systematics
    if (jets->size() < 3) return false;


    // 1 bjets
    if (nMediumB < 1) return false;


    if (nLep < 4 && ht < 300) return false;

    return true;
}

bool EventFourT::passLeptonSelection() {
    // place here the different set of requirements for leptons (e.g. all tight, all FO, prompt or not prompt, depending on the selection criteria)

    if (selType == MCAll) {
        // normal tight lepton selection, no prompt or charge requirements
        if (relevantRegion == eventClass::ttbar || relevantRegion == eventClass::dy) {
            if (tightLeps->size() != 2) return false;
            if (! tightLeps->hasOSPair()) return false;
            nLep = (*mediumLeps)->size();

            return true;
        }
        if (tightLeps->size() < 2) return false;
        if (tightLeps->size() == 2 && tightLeps->hasOSPair()) return false;

        if (foLeps->size() != tightLeps->size()) return false;
        event->selectTightLeptons();
    } else if (selType == MCPrompt) {
        // tight and prompt and no charge misID
        // or also FO and prompt with negative weight?
        if (tightLeps->size() < 2) return false;
        if (tightLeps->size() == 2 && tightLeps->hasOSPair()) return false;

        if (foLeps->size() != tightLeps->size()) return false;
        if (! leptonsArePrompt()) return false;
        event->selectTightLeptons();

        // check if any lepton is nonprompt or charge misIDd
    } else if (selType == ChargeMisDD) {
        // tight but OS events
        if (tightLeps->size() != 2) return false;
        if (! tightLeps->hasOSPair()) return false;
        event->selectTightLeptons();

    } else if (selType == NPDD) {
        // FO  with at least 1 non tight
        if (foLeps->size() < 2) return false;
        if (foLeps->size() == 2 && foLeps->hasOSPair()) return false;
        if (tightLeps->size() == foLeps->size()) return false;

        *mediumLeps = foLeps;
        nLep = (*mediumLeps)->size();
    } else if (selType == Data) {
        if (relevantRegion == eventClass::ttbar || relevantRegion == eventClass::dy) {
            event->selectTightLeptons();
            if (tightLeps->size() != 2) return false;
            if (! tightLeps->hasOSPair()) return false;
            nLep = (*mediumLeps)->size();

            return true;
        }
        if (tightLeps->size() < 2) return false;
        if (tightLeps->size() == 2 && tightLeps->hasOSPair()) return false;

        event->selectTightLeptons();
        if (foLeps->size() != tightLeps->size()) return false;
    } else if (selType == MCNoChargeMisID) {
        if (tightLeps->size() < 2) return false;
        if (tightLeps->size() == 2 && tightLeps->hasOSPair()) return false;

        if (foLeps->size() != tightLeps->size()) return false;
    } else if (selType == MCNoNP) {
        if (tightLeps->size() < 2) return false;
        if (tightLeps->size() == 2 && tightLeps->hasOSPair()) return false;

        if (foLeps->size() != tightLeps->size()) return false;
    }
    
    nLep = (*mediumLeps)->size();
    return true;
}


bool EventFourT::passFullEventSelection() {

    if (nLep >= 4) return true;
    if (jets->size() < 4) return false;

    if (bTagJets->size() < 2) return false;


    return true;
}

bool EventFourT::passLowMassVeto() {  
    // Reject same flavor lepton pairs (indep of charge) w inv mass below 12 gev

    for( const auto& leptonPtrPair : looseLeps->pairCollection() ){

        //veto SF pairs of low mass
        Lepton& lepton1 = *( leptonPtrPair.first );
        Lepton& lepton2 = *( leptonPtrPair.second );
        if(! sameFlavor( lepton1, lepton2 ) ){
            continue;
        }
        if(( lepton1 + lepton2 ).mass() < 12){
            return false;
        }
    }
    return true;
}

bool EventFourT::passZBosonVeto() {
    // Reject OSSF lepton pairs with inv mass close to Z boson mass
    if (numberOfLeps() < 3) return true;
    if ((*mediumLeps)->hasOSSFPair()) {
        double mass = (*mediumLeps)->bestZBosonCandidateMass();
        if (fabs(mass - particle::mZ) < 15.) {
            return false;
        }
    }

    return true;
}

bool EventFourT::passSingleZBosonVeto() {
    if (numberOfLeps() != 4) return true;
    std::pair<std::size_t, std::size_t> indices = getMediumLepCol()->bestZBosonCandidateIndices();

    std::vector<size_t> relIndices;
    for (size_t i=0; i<4; i++) {
        if (i == indices.first || i == indices.second) continue;
        relIndices.push_back(i);
    }
    
    Lepton* l1New = getLepton(relIndices[0]);
    Lepton* l2New = getLepton(relIndices[1]);

    if ((l1New->isElectron() && !l2New->isElectron()) || (l1New->isMuon() && !l2New->isMuon())) return true;
    
    double twoMass = (*l1New + *l2New).mass();
    if (fabs(twoMass - particle::mZ) < 15.) {
        return false;
    }
    return true;
}


bool EventFourT::passLeanSelection() {
    if ((**mediumLeps)[0].pt() < 25 || (**mediumLeps)[1].pt() < 20) return false;

    if (nJets < 2) return false;

    if (event->numberOfLooseBTaggedJets() < 1) return false;

    if (nLep < 4 && ht < 200) return false;
    
    return true;
}

bool EventFourT::leptonsArePrompt() {
    for( const auto& leptonPtr : **mediumLeps ){
        if( !leptonPtr->isPrompt() ) return false;
    }
    return true;
}

bool EventFourT::leptonsAreNotChargeFlip() {
    for( const auto& leptonPtr : **mediumLeps ){
        if(leptonPtr->isChargeFlip()) return false;
    }
    return true;
}

bool EventFourT::leptonsAreNotChargeMisMatch() {
    for( const auto& leptonPtr : **mediumLeps ){
        if(leptonPtr->isChargeMisMatch()) return false;
    }
    return true;
}

int EventFourT::NumberOfBFlavorJets() {
    int count = 0;
    for (const auto& jetPtr : *event->getJetCollectionPtr()) {
        if (fabs(jetPtr->jetHadronFlavor()) == 5) count++;
    }
    return count;
}

eventClass EventFourT::classifyUncertainty(shapeUncId id, bool up, std::string& variation, unsigned flavor) {
    //if JER

    delete jets;
    delete bTagJets;
    
    if (id == shapeUncId::JER_1p93) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_UpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_DownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
        }
        met = event->met().pt();

    } else if (id == shapeUncId::JER_2p5) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_To_2p5_UpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_To_2p5_DownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
        }
        met = event->met().pt();
    } else if (id == shapeUncId::JEC && variation == "") {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JECUpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().MetJECUp().pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JECDownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().MetJECDown().pt();
        }
    } else if (id == shapeUncId::JEC && variation != "") {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JECUpCollection(variation));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().MetJECUp(variation).pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JECDownCollection(variation));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().MetJECDown(variation).pt();
        }
    } else if (id == shapeUncId::MET) {
        if (up) {
            jets = new JetCollection(*(event->getJetCollectionPtr()));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().MetUnclusteredUp().pt();
        } else {
            jets = new JetCollection(*(event->getJetCollectionPtr()));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().MetUnclusteredDown().pt();
        }
    } else if (id == shapeUncId::JECFlavorQCD) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JECUpGroupedFlavorQCD(flavor));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().pt(); // event->met().MetJECUp(variation).pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JECDownGroupedFlavorQCD(flavor));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            met = event->met().pt(); // event->met().MetJECDown(variation).pt();
        }
    }

    nJets = jets->size();
    nMediumB = jets->numberOfMediumBTaggedJets();
    nTightB = jets->numberOfTightBTaggedJets();
    nLooseB = jets->numberOfLooseBTaggedJets();
    ht = jets->scalarPtSum();
    //event->selectTightLeptons();

    classifyEvent();
    return currentClass;
}
