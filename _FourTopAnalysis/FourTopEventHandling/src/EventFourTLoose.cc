#include "../interface/EventFourTLoose.h"

void EventFourTLoose::classifyEvent() {
    SetEventClass(eventClass::fail);
    if (! passLeanSelection()) return;
    if (numberOfLeps() == 4 && getMediumLepCol()->sumCharges() != 0) return;
    if (! passLowMassVeto()) return;
    if (! passZBosonVeto()) {
        if (numberOfLeps() == 3) SetEventClass(eventClass::crz3L);
        if (numberOfLeps() == 4 && passSingleZBosonVeto()) SetEventClass(eventClass::crz4L);
        return;
    }
        

    if (numberOfLeps() == 2 && (numberOfLooseBJets() < 2 || numberOfJets() < 4 || getHT() < 280)) {
        SetEventClass(eventClass::cro); // adapt to other crs potentially
        return;
    }
    

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