#include "../interface/Uncertainty.h"

Uncertainty::Uncertainty(std::string& channel, std::vector<HistInfo>* histInfo, int uncNumber) : HistogramManager(channel, histInfo) {
    id = (shapeUncertaintyIdentifier) uncNumber;

    if (id == shapeUncertaintyIdentifier::end - 1) return;

    next = new Uncertainty(channel, histInfo, uncNumber + 1);
}

void Uncertainty::applyUncertainty(EventFourT* event, bool nonPrompt, bool trilep, bool fourlep) {
    // do stuff

    std::vector<double> fillVector = fourTopHists::fillAllHists(trilep, event, fourlep); // get this
    
    fillHistograms(fillVector, event->getEvent()->weight(), nonPrompt);

    if (next) {
        next->applyUncertainty(event, nonPrompt, trilep, fourlep);
    }
}