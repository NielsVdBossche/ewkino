#ifndef EVENTFOURT_H
#define EVENTFOURT_H

#include "../../globalSettings.h"

#include "../../../Event/interface/Event.h"

#include "../../../Event/interface/ElectronCollection.h"
#include "../../../Event/interface/MuonCollection.h"
#include "../../../Event/interface/JetCollection.h"

#include "../../../constants/particleMasses.h"
#include "../../TopQuarkReconstruction/interface/TopReconstructionNew.h"
#include "../../HistogramHandling/interface/ChannelManager.h"
//#include "MVAHandler.h"

class TopReconstructionNew;
class MVAHandler_4T;
///enum shapeUncId;

class EventFourT {
    private:
        Event* event = nullptr;

        LeptonCollection* looseLeps;
        LeptonCollection* foLeps;
        LeptonCollection* mediumLeps;

        // These should only be used to increase nonprompt yield. Should be triggered by other functions and only done when ttbar sample used
        // Additionally, the event selection based on these should take into account the full event selection but allow one of the leptons to be loose
        LeptonCollection* altLeps; 

        eventClass currentClass = fail;

        JetCollection* jets;
        JetCollection* bTagJets;

        int nJets, nMediumB, nTightB, nLooseB, nLooseLep, nLep;
        double ht, met;

        bool isNormalSelected;

        TopReconstructionNew* topReco;
        std::vector<double> scoresMVA;
        
        MVAHandler_4T* dl_MVA, *ml_MVA;

        std::map<eventClass, int> offsets;

    public:

        EventFourT();
        ~EventFourT() {cleanSelection();};

        void setDLMVA(MVAHandler_4T* dl_new) {dl_MVA = dl_new;}
        void setMLMVA(MVAHandler_4T* ml_new) {ml_MVA = ml_new;}
        void setOffsets(std::map<eventClass, int> newOffsets) {offsets = newOffsets;}

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
        double getHT() const {return ht;}
        double getMET() const {return met;}
        eventClass getCurrentClass() const {return currentClass;}
        bool isEventNormalSelected() const {return isNormalSelected;}

        void objectSelection();
        bool passBaselineEventSelection();
        bool passFullEventSelection();
        bool passLeanSelection();
        bool passZBosonVeto();
        bool passLowMassVeto();

        bool leptonsArePrompt();
        bool leptonsAreTight();

        void classifyEvent();

        eventClass classifyUncertainty(shapeUncId id, bool up);
        std::vector<double> fillVector();
        std::vector<std::pair<int, double>> singleFillEntries();
        std::vector<std::pair<double, double>> fillVector2D();


        // altLep event selection and activation functions
        bool passBaselineEventSelectionWithAltLeptons();
        bool passFullEventSelectonWithAltLeptons();
        void altObjectSelection();
        LeptonCollection* getAltLeptonCol() {return altLeps;}

        // MVA related
        void fillMVAScores(std::vector<double>& scoresMVANew) {scoresMVA = scoresMVANew;}
        bool isMVARated() const {return scoresMVA.size() != 0;}
        std::vector<double>& getMVAScores() {return scoresMVA;}
        
        // HistogramFillers
        //std::vector<double> fillAllHists(bool multilep, EventFourT* selec);
};

#endif
