#include "histogramSetup.h"

std::vector<double> fourTopHists::fillLepInfoDL(Event* event) {

    std::vector<double> fillVal = {
        event->lightLepton(0).pt(),
        event->lightLepton(1).pt(),
        event->lightLepton(0).eta(),
        event->lightLepton(1).eta(),
        event->lightLepton(0).phi(),
        event->lightLepton(1).phi(),
        event->lightLepton(0).energy(),
        event->lightLepton(1).energy(),
        event->lightLepton(0).leptonMVATOP(),
        event->lightLepton(1).leptonMVATOP()
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
        event->lightLepton(0).energy(),
        event->lightLepton(1).energy(),
        event->lightLepton(2).energy(),
        event->lightLepton(0).leptonMVATOP(),
        event->lightLepton(1).leptonMVATOP(),
        event->lightLepton(2).leptonMVATOP()
    };

    return fillVal;
}

