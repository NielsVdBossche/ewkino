#ifndef ReweighterSample_H
#define ReweighterSample_H

//include c++ library classes
#include <map>
#include <memory>

//include ROOT classes
#include "TH1.h"
#include "../../../Event/interface/Event.h"


class ReweighterSample {
    public:
        ReweighterSample( const std::string& filterName, const std::string& weightDirectory);

        double weight(const Event& event, const int njet) const;
        double weightDown(const Event& event, const int njet) const;
        double weightUp(const Event& event, const int njet) const;
    
    private:
        std::shared_ptr< TH1 > nJetDistribution;
        std::string filterName;
        double weight(const Event& event, const int njet, double (&retrieveValue)( TH1*, const double ) ) const;
};

#endif 
