#include "../interface/ReweighterTriggerSF.h"
#include "../../Tools/interface/histogramTools.h"

ReweighterTrigger::ReweighterTrigger(const std::shared_ptr<TH2>& scalefactors, EventSelectionHelper* eventSelector) :
    sfMap(scalefactors),
    selector(eventSelector)
{}

double ReweighterTrigger::weight( const Event& event, double (&retrieveValue)( TH2*, const double, const double ) ) const {
    if (selector->passSelection(event)) {
        return retrieveValue(sfMap.get(), event.lepton(0).uncorrectedPt(), event.lepton(1).uncorrectedPt());
    } else {
        return 1.;
    }
}

double ReweighterTrigger::weight( const Event& event ) const{
    return weight( event, histogram::contentAtValues );
}


double ReweighterTrigger::weightDown( const Event& event ) const{
    return weight( event, histogram::contentDownAtValues );
}


double ReweighterTrigger::weightUp( const Event& event ) const{
    return weight( event, histogram::contentUpAtValues );
}