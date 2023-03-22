#include "../interface/EventFourT.h"
#include "../interface/MVAHandler.h"
#include "../../histogramSetups/histogramSetup.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

#if JECWRAPPER
#include "../../../CMSSW_imports/interface/JECWrapper.h"
#endif

bool selectLeptonsLooseMVA(const Lepton& lepton) {
    if (! lepton.isLightLepton()) return true;
    const LightLepton& el = (LightLepton&) lepton;
    return (el.leptonMVATOP() > 0.);
}

EventFourT::EventFourT(std::string uncertaintyFile) {
    looseLeps = new LeptonCollection();
    tightLeps = new LeptonCollection();
    foLeps = new LeptonCollection();
    mediumLeps = new LeptonCollection*();
    jets = new JetCollection();
    bTagJets = new JetCollection();

    topReco = new TopReconstructionNew(this);

    #if JECWRAPPER
    std::cout << "reading file" << std::endl;
    std::vector<std::string> jecSourcesFullSplit = {"AbsoluteStat", "AbsoluteScale", "AbsoluteMPFBias", "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "FlavorQCD", "FlavorZJet", "FlavorPhotonJet", "FlavorPureGluon", "FlavorPureQuark", "FlavorPureCharm", "FlavorPureBottom", "TimePtEta", "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeBal", "RelativeSample", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC", "RelativeStatHF", "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", "PileUpPtEC2", "PileUpPtHF", "PileUpMuZero", "PileUpEnvelope", "SubTotalPileUp", "SubTotalRelative", "SubTotalPt", "SubTotalScale", "SubTotalAbsolute", "SubTotalMC", "TotalNoFlavor", "TotalNoTime" ,"TotalNoFlavorNoTime", "Total" };
    jecWrapper = new JECWrapper(uncertaintyFile, jecSourcesFullSplit);
    #endif
}

EventFourT::~EventFourT() {
    cleanSelection();
    delete topReco;
};

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

    *mediumLeps = tightLeps;

    nJets = jets->size();
    nMediumB = event->numberOfMediumBTaggedJets();
    nTightB = event->numberOfTightBTaggedJets();
    nLooseB = event->numberOfLooseBTaggedJets();
    nLooseLep = looseLeps->size();
    nLep = (*mediumLeps)->size();
    ht = jets->scalarPtSum();
    eventMet = event->met();
    //eventMet = event->met().ApplyPhiModulation(event->runNumber(), "2018", true, event->numberOfVertices(), true);

    met = eventMet.pt();
}

bool EventFourT::passBaselineEventSelection() {
    // Baseline event selection keeping most leptons in order to correctly veto resonances

    //double n_lep = mediumLeps->size();

    //if (n_lep < 2) return false; // atm we check our tight leps here, for nonprompt est, this becomes FO
    //if (n_lep == 2 && mediumLeps->hasOSPair()) return false;

    //if (! passLeptonSelection()) return false;
    //std::cout << "pass lep sel" << std::endl;
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
    (*mediumLeps)->sortByPt();
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
    double masscut = 12.;
    if (foLeps->size() == 2) masscut = 20.;

    //std::cout << "Nleps == "<< foLeps->size() << "\t";
    for( const auto& leptonPtrPair : looseLeps->pairCollection() ){

        //veto SF pairs of low mass
        Lepton& lepton1 = *( leptonPtrPair.first );
        Lepton& lepton2 = *( leptonPtrPair.second );
        //std::cout << "l1 isEl == "<< lepton1.isElectron() << " isFO "<< lepton1.isFO() << "\t" << "l2 isEl == "<< lepton2.isElectron() << " isFO "<< lepton2.isFO()<< "\t";

        //if(! sameFlavor( lepton1, lepton2 ) ){
        //    continue;
        //}
        //std::cout << "mass == " << ( lepton1 + lepton2 ).mass() << "\t";

        if (( lepton1 + lepton2 ).mass() < masscut){
            return false;
        }
    }
    //std::cout << std::endl;
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

    if (nLooseB < 1) return false;

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

bool EventFourT::HasAdditionalBJets() {
    std::vector<std::shared_ptr<LorentzVector>> plBs, genLvlBs;
    if (! event->GetPLInfoPtr() || ! event->GetGenLevelPtr()) return false;

    plBs = event->GetPLInfoPtr()->GetParticleLevelBeeVectors();
    genLvlBs = event->GetGenLevelPtr()->GetBottomquarkCollection();

    for (auto& plB : plBs) {
        bool hasMatch = false;
        for (auto& genB : genLvlBs) {
            if (deltaR(*plB, *genB) > 0.4) continue;
            hasMatch = true;
            break;
        }
        if (! hasMatch) return true;
    }

    return false;
}

eventClass EventFourT::classifyUncertainty(shapeUncId id, bool up, unsigned variation, unsigned flavor) {
    //if JER

    delete jets;
    delete bTagJets;
    
    if (id == shapeUncId::JER_1p93) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_UpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetVariation(event->jetCollection(), &Jet::JetJER_1p93_Up);
            met = eventMet.pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_DownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetVariation(event->jetCollection(), &Jet::JetJER_1p93_Down);
            met = eventMet.pt();
        }
    } else if (id == shapeUncId::JER_2p5) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_To_2p5_UpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetVariation(event->jetCollection(), &Jet::JetJER_1p93_To_2p5_Up);
            met = eventMet.pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JER_1p93_To_2p5_DownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetVariation(event->jetCollection(), &Jet::JetJER_1p93_To_2p5_Down);
            met = eventMet.pt();
        }
    } else if (id == shapeUncId::JEC && variation == 1000) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JECUpCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetJECUp();
            met = eventMet.pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JECDownCollection());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetJECDown();
            met = eventMet.pt();
        }
    } else if (id == shapeUncId::JEC && variation != 1000) {
        //std::cout << "in if" << std::endl; 
        if (up) {
            #if JECWRAPPER

            std::pair<JetCollection, Met> variedStuff = jecWrapper->VaryJetsAndMet(*event, variation, true);
            jets = new JetCollection(variedStuff.first);
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = variedStuff.second;
            met = eventMet.pt();

            #else

            jets = new JetCollection(event->getJetCollectionPtr()->JECGroupedUpCollection(variation));
            //std::cout << "got jet col " << std::endl;
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            //std::cout << "loose bs " << std::endl;

            eventMet = event->met().MetJECGroupedUp(variation);
            met = eventMet.pt();
            #endif
        } else {
            #if JECWRAPPER

            std::pair<JetCollection, Met> variedStuff = jecWrapper->VaryJetsAndMet(*event, variation, false);
            jets = new JetCollection(variedStuff.first);
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = variedStuff.second;
            met = eventMet.pt();

            #else

            jets = new JetCollection(event->getJetCollectionPtr()->JECGroupedDownCollection(variation));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetJECGroupedDown(variation);
            met = eventMet.pt();
            
            #endif
        }
    } else if (id == shapeUncId::MET) {
        if (up) {
            jets = new JetCollection(*(event->getJetCollectionPtr()));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetUnclusteredUp();
            met = eventMet.pt();
        } else {
            jets = new JetCollection(*(event->getJetCollectionPtr()));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().MetUnclusteredDown();
            met = eventMet.pt();
        }
    } else if (id == shapeUncId::JECFlavorQCD) {
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->JECUpGroupedFlavorQCD(flavor));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met(); // event->met().MetJECUp(variation);
            met = eventMet.pt();
        } else {
            jets = new JetCollection(event->getJetCollectionPtr()->JECDownGroupedFlavorQCD(flavor));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met(); // event->met().MetJECDown(variation);
            met = eventMet.pt();
        }
    } else if (id == shapeUncId::HEMIssue) { 
        if (up) {
            jets = new JetCollection(event->getJetCollectionPtr()->HEMIssue());
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met().HEMIssue(*jets);
            met = eventMet.pt();
        } else {
            jets = new JetCollection(*(event->getJetCollectionPtr()));
            jets->selectGoodJets();
            bTagJets = new JetCollection(jets->looseBTagCollection());
            eventMet = event->met(); // event->met().MetJECDown(variation);
            met = eventMet.pt();
        }
    }
    //std::cout << "vars " << std::endl;


    nJets = jets->size();
    nMediumB = jets->numberOfMediumBTaggedJets();
    nTightB = jets->numberOfTightBTaggedJets();
    nLooseB = jets->numberOfLooseBTaggedJets();
    ht = jets->scalarPtSum();
    //event->selectTightLeptons();

    classifyEvent();
    return currentClass;
}
