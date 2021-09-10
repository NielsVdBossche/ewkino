#include "histogramSetup.h"

std::vector<double> fourTopHists::fillLepInfoDL(Event* event) {

    std::vector<double> fillVal = {
        event->lightLepton(0).pt(),
        event->lightLepton(1).pt(),
        event->lightLepton(0).eta(),
        event->lightLepton(1).eta(),
        event->lightLepton(0).phi(),
        event->lightLepton(1).phi(),
        event->lightLepton(0).E(),
        event->lightLepton(1).E(),
        event->lightLepton(0)._leptonMVATOP(),
        event->lightLepton(1)._leptonMVATOP()
    };

    return fillVal;
}

std::vector<double> fourTopHists::fillLepInfoML(Event* event) {
    std::vector<double> fillVal = {
        event->lightLepton(0).pt(),
        event->lightLepton(1).pt(),
        event->lightLepton(2).pt(),
        event->lightLepton(0).eta(),
        event->lightLepton(1).eta(),
        event->lightLepton(2).eta(),
        event->lightLepton(0).phi(),
        event->lightLepton(1).phi(),
        event->lightLepton(2).phi(),
        event->lightLepton(0).E(),
        event->lightLepton(1).E(),
        event->lightLepton(2).E(),
        event->lightLepton(0)._leptonMVATOP(),
        event->lightLepton(1)._leptonMVATOP(),
        event->lightLepton(2)._leptonMVATOP()
    };

    return fillVal;
}

