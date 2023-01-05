#ifndef CombinedSampleReweighter_H
#define CombinedSampleReweighter_H

//include c++ library classes
#include <map>
#include <memory>
#include <string>

//include other parts of framework
//#include "Reweighter.h"
#include "ReweighterSample.h"

class CombinedSampleReweighter {

    public:
        CombinedSampleReweighter() = default;
        
        void addReweighter( const std::string&, const std::shared_ptr< ReweighterSample >& );

        double totalWeight( const Event&, const int ) const;
        double totalWeightUp( const Event&, const int ) const;
        double totalWeightDown( const Event&, const int ) const;

        bool hasReweighter(std::string rew);

        bool IsEmpty() const;
    private:
        double totalWeight( double (ReweighterSample::*weightFunction)(const Event&, const int) const, const Event&, const int ) const;
        
        std::map< std::string, std::shared_ptr< ReweighterSample > > reweighterMap;
        std::vector< std::shared_ptr< ReweighterSample > > reweighterVector;
};


#endif
