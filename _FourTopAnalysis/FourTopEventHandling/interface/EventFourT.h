#ifndef EVENTFOURT_H
#define EVENTFOURT_H

#include "../../globalSettings.h"

#include "../../../Event/interface/Event.h"

#include "../../../Event/interface/ElectronCollection.h"
#include "../../../Event/interface/MuonCollection.h"
#include "../../../Event/interface/JetCollection.h"

#include "../../../constants/particleMasses.h"
#include "../../TopQuarkReconstruction/interface/TopReconstructionNew.h"

class TopReconstructionNew;

enum eventClass {
    SSDL=2,
    TriLep=3,
    FourLep=4
};

class EventFourT {
    private:
        Event* event = nullptr;

        LeptonCollection* looseLeps;
        LeptonCollection* mediumLeps;

        // These should only be used to increase nonprompt yield. Should be triggered by other functions and only done when ttbar sample used
        // Additionally, the event selection based on these should take into account the full event selection but allow one of the leptons to be loose
        LeptonCollection* altLeps; 

        JetCollection* jets;
        JetCollection* bTagJets;

        int nJets, nMediumB, nTightB, nLooseB, nLooseLep, nLep;
        double ht;

        bool isNormalSelected;

        TopReconstructionNew* topReco;
        
    public:

        EventFourT();
        ~EventFourT() {cleanSelection();};

        void cleanSelection();
        void addNewEvent(Event* newEvent);

        Event* getEvent() {return event;};
        Lepton* getLepton(size_t index) {return mediumLeps->at(index);}
        Jet* getJet(size_t index) {return jets->at(index);}
        Jet* getBtagJet(size_t index) {return bTagJets->at(index);}

        LeptonCollection* getLooseLepCol() {return looseLeps;}
        LeptonCollection* getMediumLepCol() {return mediumLeps;}
        JetCollection* getJetCol() {return jets;}
        JetCollection* getBtagJetCol() {return bTagJets;}

        TopReconstructionNew* getTopReco() {return topReco;}

        int numberOfJets() const {return nJets;}
        int numberOfMediumBJets() const {return nMediumB;}
        int numberOfTightBJets() const {return nTightB;}
        int numberOfLooseBJets() const {return nLooseB;}
        int numberOfLooseLeps() const {return nLooseLep;}
        int numberOfLeps() const {return nLep;}
        bool isEventNormalSelected() const {return isNormalSelected;}

        void objectSelection();
        bool passBaselineEventSelection();
        bool passFullEventSelection();
        bool passLeanSelection();
        bool passZBosonVeto();
        bool passLowMassVeto();

        // altLep event selection and activation functions
        bool passBaselineEventSelectionWithAltLeptons();
        bool passFullEventSelectonWithAltLeptons();
        void altObjectSelection();
        LeptonCollection* getAltLeptonCol() {return altLeps;}

        // HistogramFillers
        //std::vector<double> fillAllHists(bool multilep, EventFourT* selec);
};

bool leptonPtReq(const Lepton& lep);
#endif
