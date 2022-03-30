#include "../interface/EventFourTLoose.h"

void EventFourTLoose::classifyEvent() {
    SetEventClass(eventClass::fail);
    if (! passLeanSelection()) return;
    if (! passLowMassVeto()) return;
    if (! passZBosonVeto()) return;

    if (numberOfLeps() < 4 && numberOfLooseBJets() < 2) {
        SetEventClass(eventClass::cro);
        return;
    }

    if ((numberOfLeps() == 2 && numberOfJets() < 4) || (numberOfLeps() == 3 && numberOfJets() < 3) || (numberOfLeps() == 4 && numberOfJets() < 2)) {
        SetEventClass(eventClass::cro);
        return;
    }

    if (numberOfLeps() == 2 && getHT() < 280) {
        SetEventClass(eventClass::cro);
        return;
    }
    if (numberOfLeps() == 3 && getHT() < 220) {
        SetEventClass(eventClass::cro);
        return;
    }


    if ((numberOfLeps() == 2 && numberOfJets() < 6 && numberOfLooseBJets() == 2) || (numberOfLeps() == 3 && numberOfJets() < 4 && numberOfLooseBJets() == 2)) {
        SetEventClass(eventClass::crw);
        return;
    }

    if (numberOfLeps() == 2) {
        SetEventClass(eventClass::ssdl);
    } else if (numberOfLeps() == 3) {
        SetEventClass(eventClass::trilep);
    } else if (numberOfLeps() == 4) {
        SetEventClass(eventClass::fourlep);
    }
    return;
}