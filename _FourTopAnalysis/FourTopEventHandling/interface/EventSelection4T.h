#ifndef eventselection4T_h
#define eventselection4T_h

#include "../../globalSettings.h"

#include "../../../Event/interface/Event.h"

#include "../../../Event/interface/ElectronCollection.h"
#include "../../../Event/interface/MuonCollection.h"
#include "../../../Event/interface/JetCollection.h"


// Class eventSelection inheriting from Event? Does this make it handy to perform event selection?
// Evt zonder inheritance maar grote call dan... Beter met? buildEvent op treeReader zou dan rechtstreeks hierop feeden...
// Anders 2 klassen voor event sel en event -> evt vervangen door event selection in FourTop klasse rechtstreeks te steken. <- this
// This structure will not work as it requires plugging this in TreeReader (instead of event as return). This makes it quite difficult
// Event provides plug for user specified object selections

enum eventClass {
    SSDL=2,
    TriLep=3,
    FourLep=4
};

class EventSelection4T {
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

        // MVA variable vector
        
    public:

        EventSelection4T();
        ~EventSelection4T() {cleanSelection();};

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

        int numberOfJets() const {return nJets;}
        int numberOfMediumBJets() const {return nMediumB;}
        int numberOfTightBJets() const {return nTightB;}
        int numberOfLooseBJets() const {return nLooseB;}
        int numberOfLooseLeps() const {return nLooseLep;}
        int numberOfLeps() const {return nLep;}

        void objectSelection();
        bool passBaselineEventSelection();
        bool passFullEventSelection();
        bool passZBosonVeto();
        bool passLowMassVeto();

        // altLep event selection and activation functions
        bool passBaselineEventSelectionWithAltLeptons();
        bool passFullEventSelectonWithAltLeptons();
        void altObjectSelection();
        LeptonCollection* getAltLeptonCol() {return altLeps;}
};

bool leptonPtReq(const Lepton& lep);
#endif
