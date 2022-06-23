#include "../interface/EventFourTLoose.h"

void EventFourTLoose::classifyEvent() {
    SetEventClass(eventClass::fail);
    //if (! passLowMassVeto()) return;
    if (! passPhotonOverlapRemoval()) return;
    if (! passLeanSelection()) {
        if (numberOfLeps() != 3) return;
        if (getMediumLepCol()->at(0)->pt() < 25 || getMediumLepCol()->at(1)->pt() < 15) return;
        //if (getMET() < 50) return;

        if (numberOfLooseBJets() != 0) return;
        if (! getMediumLepCol()->hasOSSFPair()) return;
        double m3l = (*getLepton(0) + *getLepton(1) + *getLepton(2)).mass();
        double m2l = getMediumLepCol()->bestZBosonCandidateMass();
        
        if (! passZBosonVeto() && fabs(m3l - particle::mZ) > 15. && getMET() > 50) {
            SetEventClass(eventClass::crwz);
        } else if (fabs(m3l - particle::mZ) < 15. && m2l < 75) {
            SetEventClass(eventClass::cr_conv);
        }
        return;
    }
    if (numberOfLeps() == 4 && getMediumLepCol()->sumCharges() != 0) return;
    if (! passZBosonVeto()) {
        if (numberOfLeps() == 3) SetEventClass(eventClass::crz3L);
        if (numberOfLeps() == 4 && passSingleZBosonVeto()) SetEventClass(eventClass::crz4L);
        return;
    }
        
    int trueCountCRO = (numberOfLooseBJets() < 2 ? 1 : 0) + (numberOfJets() < 4 ? 1 : 0) + (getHT() < 280 ? 1 : 0);
    if (numberOfLeps() == 2 && trueCountCRO == 1) {
        SetEventClass(eventClass::cro); // adapt to other crs potentially
        return;
    } else if (numberOfLeps() == 2 && trueCountCRO > 1) {
        return;
    }
    
    //int trueCountCRO3L = (numberOfLooseBJets() < 2 ? 1 : 0) + (numberOfJets() < 3 ? 1 : 0); // likely want this in future

    if (numberOfLeps() == 3 && (numberOfLooseBJets() < 2 || numberOfJets() < 3)) {
        SetEventClass(eventClass::cro3L);
        return;
    }

    if (numberOfLeps() == 2 && numberOfJets() < 6 && numberOfLooseBJets() == 2) {
        SetEventClass(eventClass::crw);
        return;
    }

    if (numberOfLeps() == 2) {
        SetEventClass(eventClass::ssdl);
        return;
    } else if (numberOfLeps() == 3) {
        SetEventClass(eventClass::trilep);
        return;
    } else if (numberOfLeps() == 4) {
        SetEventClass(eventClass::fourlep);
        return;
    }
    return;
}