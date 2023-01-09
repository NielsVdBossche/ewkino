#include "../interface/EventFourT.h"
#include "../interface/MVAHandler.h"
#include "../../histogramSetups/histogramSetup.h"

void EventFourT::classifyEvent() {
    currentClass = eventClass::fail;
    if (! passBaselineEventSelection()) return;
    if (! passLowMassVeto()) return;
    if (! passZBosonVeto()) {
        currentClass = eventClass::crz3L;
        return;
    }

    if (! passFullEventSelection()) {
        currentClass = eventClass::cro;
        return;
    }

    if (numberOfLeps() == 2 && numberOfJets() < 6 && numberOfMediumBJets() == 2) {
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
    if (currentClass == eventClass::crz3L || currentClass == eventClass::crz4L || currentClass == eventClass::cro3L || currentClass > eventClass::ssdl) currentMVA = ml_MVA;
    bool useMVA = true;
    scoresMVA = currentMVA->scoreEvent();
    
    if (currentClass == eventClass::crz3L || currentClass == eventClass::crz4L || currentClass == eventClass::cro3L || currentClass == eventClass::crwz) {
        fillVec = histFiller(currentClass, this);
    } else if (currentClass == eventClass::crw || currentClass == eventClass::cro) {
        fillVec = histFiller(currentClass, this);
    } else if (currentClass == eventClass::ssdl) {
        fillVec = histFiller(currentClass, this);
    } else if (currentClass == eventClass::trilep) {
        fillVec = histFiller(currentClass, this);
    } else if (currentClass == eventClass::fourlep) {
        fillVec = histFiller(currentClass, this);
    } else if (currentClass == eventClass::dy || currentClass == eventClass::ttbar) {
        fillVec = histFiller(currentClass, this);
    } else if (currentClass != eventClass::fail) {
        useMVA = false;
        fillVec = histFiller(currentClass, this);
    }

    if (currentClass == eventClass::crwz) useMVA = false;

    if (useMVA && bdtOutput) {
        fillVec.insert(fillVec.end(), scoresMVA.begin(), scoresMVA.end());
    }

    return fillVec;
}

std::vector<std::pair<int, double>> EventFourT::singleFillEntries() {
    std::vector<std::pair<int, double>> singleEntries;
    if (currentClass == eventClass::fail || (currentClass < eventClass::crz3L && (currentClass != eventClass::dy || currentClass != eventClass::ttbar))) return singleEntries;
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz3L || currentClass == eventClass::crz4L || currentClass == eventClass::cro3L || currentClass > eventClass::ssdl) currentMVA = ml_MVA;

    if (bdtOutput) {
        std::map<int, double> classAndScore = currentMVA->getClassAndScore();
        int offset = offsets[currentClass];
        for (auto& el : classAndScore) {
            singleEntries.push_back({offset + el.first, el.second});
            if (currentClass == eventClass::fourlep && el.first == int(MVAClasses::TTBar)) {
                singleEntries.push_back({offset + MVAClasses::TTW, scoresMVA[2]}); // good enough
            }
        }
    }
    return singleEntries;
}

std::vector<std::pair<double, double>> EventFourT::fillVector2D() {
    std::vector<std::pair<double, double>> fillVec2D;
    if (currentClass == eventClass::fail || (currentClass < eventClass::crz3L && (currentClass != eventClass::dy || currentClass != eventClass::ttbar))) return fillVec2D;
    MVAHandler_4T* currentMVA = dl_MVA;
    //if (currentClass == eventClass::cro || currentClass == eventClass::crw || currentClass == eventClass::ssdl) 
    if (currentClass == eventClass::crz3L || currentClass == eventClass::crz4L || currentClass == eventClass::cro3L || currentClass > eventClass::ssdl) currentMVA = ml_MVA;
    if (bdtOutput) {
        fillVec2D = currentMVA->fill2DVector();
    }
    return fillVec2D;
}
