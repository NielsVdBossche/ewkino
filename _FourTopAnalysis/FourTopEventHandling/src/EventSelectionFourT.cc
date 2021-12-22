#include "../interface/EventFourT.h"
#include "../interface/MVAHandler.h"
#include "../../histogramSetups/histogramSetup.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

EventFourT::EventFourT() {
    looseLeps = new LeptonCollection();
    mediumLeps = new LeptonCollection();
    jets = new JetCollection();
    bTagJets = new JetCollection();

    topReco = new TopReconstructionNew(this);
}

void EventFourT::cleanSelection() {
    delete looseLeps;
    delete mediumLeps;
    delete jets;
    delete bTagJets;
    scoresMVA.clear();
}

void EventFourT::addNewEvent(Event* newEvent) {
    cleanSelection();
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
    mediumLeps = new LeptonCollection(event->TightLeptonCollection());
    foLeps = new LeptonCollection(event->FOLeptonCollection());

    jets = new JetCollection(event->jetCollection());
    jets->selectGoodJets();
    bTagJets = new JetCollection(jets->mediumBTagCollection());

    event->selectTightLeptons();

    nJets = jets->size();
    nMediumB = bTagJets->size();
    nTightB = event->numberOfTightBTaggedJets();
    nLooseB = event->numberOfLooseBTaggedJets();
    nLooseLep = looseLeps->size();
    nLep = mediumLeps->size();
    ht = jets->scalarPtSum();
    met = event->met().pt();
}

bool EventFourT::passBaselineEventSelection() {
    // Baseline event selection keeping most leptons in order to correctly veto resonances

    double n_lep = mediumLeps->size();

    if (n_lep < 2) return false; // atm we check our tight leps here, for nonprompt est, this becomes FO
    if (n_lep == 2 && mediumLeps->hasOSPair()) return false;

    if ((*mediumLeps)[0].pt() < 25 || (*mediumLeps)[1].pt() < 20) return false;
    // 2 SS leptons OR 3+ leps
    // check basic nr jets
    // 2 cuts which must be replaced for systematics
    if (jets->size() < 3) return false;

    // 1 bjets
    if (bTagJets->size() < 1) return false;

    if (event->met().pt() < 25) return false;
    if (ht < 300) return false;

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
    if (event->hasOSSFLeptonPair()) {
        double mass = event->bestZBosonCandidateMass();
        //if (mass > 76 && mass < 106) return false;
        if (fabs(mass - particle::mZ) < 7.5) {
            currentClass = eventClass::crz;
            return false;
        }
    }

    return true;
}

bool EventFourT::passLeanSelection() {
    if (nLep < 2) return false; // atm we check our tight leps here, for nonprompt est, this becomes FO
    if (nLep == 2 && mediumLeps->hasOSPair()) return false;

    if ((*mediumLeps)[0].pt() < 25 || (*mediumLeps)[1].pt() < 20) return false;
    if (event->met().pt() < 25) return false;

    if (nJets < 2 || nMediumB < 1) return false;
    if (ht < 100) return false;

    if (nLep == 3) {
        if (ht < 200) return false;
        if (nJets < 3) return false;
    }

    if (nLep == 2) {
        if (ht < 300) return false;
        if (nJets < 4) return false;
    }
    
    return true;
}

bool EventFourT::leptonsArePrompt() {
    for( const auto& leptonPtr : *mediumLeps ){
        if( !leptonPtr->isPrompt() ) return false;
    }
    return true;
}

void EventFourT::classifyEvent() {
    currentClass = eventClass::fail;
    if (! passBaselineEventSelection()) return;
    if (! passLowMassVeto()) return;
    if (! passZBosonVeto()) return;

    if (! passFullEventSelection()) {
        currentClass = eventClass::cro;
        return;
    }

    if (numberOfLeps() == 2 && numberOfJets() < 6 && numberOfMediumBJets() == 2) {
        currentClass = eventClass::crw;
        return;
    }

    if (numberOfLeps() == 2) {
        currentClass = eventClass::ssdl;
    } else if (numberOfLeps() == 3) {
        currentClass = eventClass::trilep;
    } else if (numberOfLeps() == 4) {
        currentClass = eventClass::fourlep;
    }
    return;
}

eventClass EventFourT::classifyUncertainty(shapeUncId id, bool up) {
    //if JER
    if (id == shapeUncId::JER) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JERDownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->mediumBTagCollection());
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JERUpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->mediumBTagCollection());
        }
        met = event->met().pt();

    } else if (id == shapeUncId::JEC) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JECDownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->mediumBTagCollection());
            met = event->met().MetJECDown().pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JECUpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->mediumBTagCollection());
            met = event->met().MetJECUp().pt();
        }
    }

    nJets = jets->size();
    nMediumB = bTagJets->size();
    nTightB = jets->numberOfTightBTaggedJets();
    nLooseB = jets->numberOfLooseBTaggedJets();
    ht = jets->scalarPtSum();
    //event->selectTightLeptons();

    classifyEvent();
    return currentClass;
}

std::vector<double> EventFourT::fillVector() {
    std::vector<double> fillVec;
    if (currentClass == eventClass::fail) return fillVec;
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz || currentClass > eventClass::ssdl) currentMVA = ml_MVA;

    std::vector<double> scores = currentMVA->scoreEvent();
    if (currentClass == eventClass::crz) {
        fillVec = fourTopHists::fillAllLean(false, this);
    } else if (currentClass == eventClass::crw || currentClass == eventClass::cro) {
        fillVec = fourTopHists::fillAllLean(false, this);
    } else if (currentClass == eventClass::ssdl) {
        fillVec = fourTopHists::fillAllHists(false, this);
    } else if (currentClass == eventClass::trilep) {
        fillVec = fourTopHists::fillAllHists(true, this);
    } else if (currentClass == eventClass::fourlep) {
        fillVec = fourTopHists::fillAllHists(true, this, true);
    }

    fillVec.insert(fillVec.end(), scores.begin(), scores.end());

    return fillVec;

}

std::vector<std::pair<int, double>> EventFourT::singleFillEntries() {
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz || currentClass > eventClass::ssdl) currentMVA = ml_MVA;

    std::vector<std::pair<int, double>> singleEntries;

    std::pair<MVAClasses, double> classAndScore = currentMVA->getClassAndScore();
    int offset = offsets[currentClass];  
    singleEntries.push_back({offset + classAndScore.first, classAndScore.second});

    return singleEntries;
}

std::vector<std::pair<double, double>> EventFourT::fillVector2D() {
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz || currentClass > eventClass::ssdl) currentMVA = ml_MVA;
    std::vector<std::pair<double, double>> fillVec2D = currentMVA->fill2DVector();

    return fillVec2D;
}
