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

    if (! passZBosonVeto()) {
        if (getMediumLepCol()->at(2)->pt() < 20) return;
        SetEventClass(eventClass::crz3L);
        return;
    }
    if (numberOfMediumBJets() == 2 && numberOfJets() < 6 && numberOfLeps() == 2) {
        SetEventClass(eventClass::crw);
        return;
    }

    SetEventClass(eventClass::ssdl);    
    // define crw, crz and signal regions
    return;
}