#ifndef ReweighterPUID_H
#define ReweighterPUID_H

//include c++ library classes
#include <stdexcept>

//include ROOT classes
#include "TH2.h"

//include official b-tag weight reader
#include "../bTagSFCode/BTagCalibrationStandalone.h"

//include other parts of framework
#include "Reweighter.h"
#include "../../Tools/interface/histogramTools.h"
#include "../../Tools/interface/stringTools.h"

class ReweighterPUID : public Reweighter {

    public:
        ReweighterPUID( const std::string& sfFilePath,
                        const std::string& year,
                        const std::string& effFilePath);

        virtual double weight( const Event& ) const override;
        virtual double weightDown( const Event& ) const override;
        virtual double weightUp( const Event& ) const override;

        double weight( const Jet& ) const;
        double weightDown( const Jet& ) const;
        double weightUp( const Jet& ) const;
    private:
        std::shared_ptr< TH2 > puIDEfficiency;
        std::shared_ptr< TH2 > scalefactor;
        std::shared_ptr< TH2 > systunc;

        double efficiencyMC( const Jet& ) const;
        double weight( const Jet&, double (&retrieveValue)( TH2*, const double, const double ) ) const; 
        double weight( const Event&, double (ReweighterPUID::*jetWeight)( const Jet& ) const ) const;
};

#endif
