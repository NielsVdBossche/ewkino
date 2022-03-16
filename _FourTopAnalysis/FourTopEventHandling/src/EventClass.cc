#include "../interface/EventFourT.h"
#include "../interface/MVAHandler.h"
#include "../../histogramSetups/histogramSetup.h"

void EventFourT::classifyEvent() {
    currentClass = eventClass::fail;
    if (! passLeanSelection()) return;
    if (! passLowMassVeto()) return;
    if (! passZBosonVeto()) return;

    if (nLep < 4 && event->numberOfLooseBTaggedJets() < 2) {
        currentClass = eventClass::cro;
    }


    if ((numberOfLeps() == 2 && numberOfJets() < 6 && numberOfLooseBJets() == 2) || (numberOfLeps() == 3 && numberOfJets() < 4 && numberOfLooseBJets() == 2)) {
        currentClass = eventClass::crw;
        return;
    }

    if (numberOfLeps() == 2) {
        currentClass = eventClass::ssdl;
    } else if (numberOfLeps() == 3) {
        currentClass = eventClass::trilep;
    } else if (numberOfLeps() == 4) {
        currentClass = eventClass::fourlep;
    }
    return;
}

std::vector<double> EventFourT::fillVector() {
    std::vector<double> fillVec;
    if (currentClass == eventClass::fail) return fillVec;
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz || currentClass > eventClass::ssdl) currentMVA = ml_MVA;

    std::vector<double> scores = currentMVA->scoreEvent();
    if (currentClass == eventClass::crz) {
        fillVec = fourTopHists::fillAllLean(false, this);
    } else if (currentClass == eventClass::crw || currentClass == eventClass::cro) {
        fillVec = fourTopHists::fillAllLean(false, this);
    } else if (currentClass == eventClass::ssdl) {
        fillVec = fourTopHists::fillAllHists(false, this);
    } else if (currentClass == eventClass::trilep) {
        fillVec = fourTopHists::fillAllHists(true, this);
    } else if (currentClass == eventClass::fourlep) {
        fillVec = fourTopHists::fillAllHists(true, this, true);
    }

    fillVec.insert(fillVec.end(), scores.begin(), scores.end());

    return fillVec;

}

std::vector<std::pair<int, double>> EventFourT::singleFillEntries() {
    std::vector<std::pair<int, double>> singleEntries;
    if (currentClass == eventClass::fail) return singleEntries;
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz || currentClass > eventClass::ssdl) currentMVA = ml_MVA;


    std::pair<MVAClasses, double> classAndScore = currentMVA->getClassAndScore();
    int offset = offsets[currentClass];  
    singleEntries.push_back({offset + classAndScore.first, classAndScore.second});

    return singleEntries;
}

std::vector<std::pair<double, double>> EventFourT::fillVector2D() {
    std::vector<std::pair<double, double>> fillVec2D;
    if (currentClass == eventClass::fail) return fillVec2D;
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz || currentClass > eventClass::ssdl) currentMVA = ml_MVA;
    fillVec2D = currentMVA->fill2DVector();

    return fillVec2D;
}
