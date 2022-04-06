#include "../interface/EventFourTLoose.h"

void EventFourTLoose::classifyEvent() {
    SetEventClass(eventClass::fail);
    if (! passLowMassVeto()) return;
    if (! passZBosonVeto()) {
        if (passLeanSelection()) {
            SetEventClass(eventClass::crz);
        } else if (! passLeanSelection() && (getHT() > 200 || numberOfLooseBJets() >= 1)) {
            SetEventClass(eventClass::fail);
            return;
        } else if (! passLeanSelection() && getHT() <= 200) {
            SetEventClass(eventClass::crzInvHT);
        } else if (! passLeanSelection() && numberOfLooseBJets() < 1) {
            SetEventClass(eventClass::crzNoB);
        }
        return;
    }
    if (! passLeanSelection()) {
        if (getMediumLepCol()->at(0)->pt() < 25 || getMediumLepCol()->at(1)->pt() < 20) return;

        if (numberOfLooseBJets() < 2) return;
        if (numberOfLeps() == 2 && numberOfJets() < 6 && numberOfJets() >= 4 && numberOfLooseBJets() == 2) {
            SetEventClass(crwInvHT);
        }
        return;
    }
        

    if (numberOfLeps() < 4 && numberOfLooseBJets() < 2) {
        SetEventClass(eventClass::cro);
        return;
    }

    if ((numberOfLeps() == 2 && numberOfJets() < 4) || (numberOfLeps() == 3 && numberOfJets() < 3)) {
        SetEventClass(eventClass::cro);
        return;
    }

    if (numberOfLeps() == 2 && getHT() < 280) {
        SetEventClass(eventClass::cro);
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