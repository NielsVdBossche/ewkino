#ifndef ReweighterPileup_H
#define ReweighterPileup_H

#include "Reweighter.h"

//include c++ library classes
#include <map>
#include <memory>

//include ROOT classes
#include "TH1.h"

class ReweighterPileup : public Reweighter {

    public:
        ReweighterPileup( const std::vector< Sample >& sampleList, const std::string& weightDirectory ); // DEPRECATED
        ReweighterPileup(std::string& year, const std::string& weightDirectory);

        virtual double weight( const Event& ) const override;
        virtual double weightDown( const Event& ) const override;
        virtual double weightUp( const Event& ) const override;

    private: 
        std::map< std::string, std::shared_ptr< TH1 > > puWeightsCentral; // DEPRECATED
        std::map< std::string, std::shared_ptr< TH1 > > puWeightsDown; // DEPRECATED
        std::map< std::string, std::shared_ptr< TH1 > > puWeightsUp; // DEPRECATED

        std::shared_ptr< TH1 > UL_puWeightsCentral;
        std::shared_ptr< TH1 > UL_puWeightsDown;
        std::shared_ptr< TH1 > UL_puWeightsUp;
        double weight( const Event&, const std::map< std::string, std::shared_ptr< TH1 > >& ) const; // DEPRECATED
        double weight( const Event&, const std::shared_ptr< TH1 >& ) const;

};


#endif
