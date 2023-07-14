#ifndef ReweighterTriggers_H
#define ReweighterTriggers_H

#include "Reweighter.h"
#include "../../Event/interface/Event.h"
#include "EventSelectionHelper.h"

class ReweighterTrigger : public Reweighter {
    public:
        ReweighterTrigger(const std::shared_ptr<TH2>&, EventSelectionHelper*);
        virtual double weight( const Event& event ) const override;
        virtual double weightDown( const Event& event ) const override;
        virtual double weightUp( const Event& event ) const override;
        
    private:
        std::shared_ptr<TH2> sfMap;
        std::shared_ptr<EventSelectionHelper> selector;

        double weight( const Event& event, double (&retrieveValue)( TH2*, const double, const double ) ) const;
};

#endif