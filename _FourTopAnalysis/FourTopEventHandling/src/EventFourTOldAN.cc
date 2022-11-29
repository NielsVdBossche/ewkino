#include "../interface/EventFourTOldAN.h"

void EventFourTOldAN::classifyEvent() {
    SetEventClass(eventClass::fail);
    //if (! passLowMassVeto()) return;
    if (! passPhotonOverlapRemoval()) return;
    
    int njets = 0;
    double ht = 0.;
    // loop jets and add ll with pt larger than 40 gev
    for (const auto jetPtr : *getJetCol()) {
        if (jetPtr->pt() > 40) {
            njets++;
            ht += jetPtr->pt();
        }
    }
    setHT(ht);
    setNumberOfJets(njets);

    if (numberOfJets() < 2) return;
    if (numberOfMediumBJets() < 2) return;
    if (getMET() < 50) return;
    if (getHT() < 300) return;
    if (getMediumLepCol()->at(0)->pt() < 25 || getMediumLepCol()->at(1)->pt() < 20) return;

    if (numberOfLeps() == 3 && getMediumLepCol()->at(2)->pt() < 20) return;
    if (numberOfLeps() == 4 && getMediumLepCol()->sumCharges() != 0) return;
    if (! passZBosonVeto()) {
        if (numberOfLeps() == 3) SetEventClass(eventClass::crz3L);
        if (numberOfLeps() == 4 && passSingleZBosonVeto()) SetEventClass(eventClass::crz3L);
        //if (numberOfLeps() == 4 && !passSingleZBosonVeto()) SetEventClass(eventClass::crzz);
        return;
    }
        
    int trueCountCRO = (numberOfMediumBJets() < 2 ? 1 : 0) + (numberOfJets() < 4 ? 1 : 0) + (getHT() < 280 ? 1 : 0);
    if (numberOfLeps() == 2 && trueCountCRO == 1) {
        //SetEventClass(eventClass::cro); // adapt to other crs potentially
        return;
    } else if (numberOfLeps() == 2 && trueCountCRO > 1) {
        return;
    }
    
    //int trueCountCRO3L = (numberOfLooseBJets() < 2 ? 1 : 0) + (numberOfJets() < 3 ? 1 : 0); // likely want this in future

    if (numberOfLeps() == 3 && (numberOfMediumBJets() < 2 || numberOfJets() < 3)) {
        //SetEventClass(eventClass::cro3L);
        return;
    }

    if (numberOfLeps() == 2 && numberOfJets() < 6 && numberOfMediumBJets() == 2) {
        SetEventClass(eventClass::crw);
        return;
    }

    if (numberOfLeps() == 2) {
        SetEventClass(eventClass::ssdl);
        return;
    } else if (numberOfLeps() == 3) {
        SetEventClass(eventClass::ssdl);
        return;
    } else if (numberOfLeps() == 4) {
        SetEventClass(eventClass::ssdl);
        return;
    }
    return;
}