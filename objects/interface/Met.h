#ifndef Met_H
#define Met_H

// include c++ libraries
#include <math.h>

//include other parts of code 
#include "PhysicsObject.h"
#include "../../Event/interface/JetCollection.h"
#include "../../TreeReader/interface/TreeReader.h"

enum TheRunEra {
        y2016B,
        y2016C,
        y2016D,
        y2016E,
        y2016F,
        y2016G,
        y2016H,
        y2017B,
        y2017C,
        y2017D,
        y2017E,
        y2017F,
        y2018A,
        y2018B,
        y2018C,
        y2018D,
        y2016MC,
        y2017MC,
        y2018MC,
        yUL2016B,
        yUL2016C,
        yUL2016D,
        yUL2016E,
        yUL2016F,
        yUL2016Flate,
        yUL2016G,
        yUL2016H,
        yUL2017B,
        yUL2017C,
        yUL2017D,
        yUL2017E,
        yUL2017F,
        yUL2018A,
        yUL2018B,
        yUL2018C,
        yUL2018D,
        yUL2016MCAPV,
        yUL2016MCnonAPV,
        yUL2017MC,
        yUL2018MC
};

class Met : public PhysicsObject {

    public:
        Met() = default;
        Met( const TreeReader&, bool, bool );
        Met( const NanoReader&, bool, bool);

        virtual bool isGood() const override{ return true; }
        bool IsMetVariationsInitialized() const {return _metVariationsInitialized;}
        bool IsMetJERVariationsInitialized() const {return _metJERVariationsInitialized;}

        //met objects varied by respective uncertainties
        Met MetJECDown() const;
        Met MetJECUp() const;
        Met MetUnclusteredDown() const;
        Met MetUnclusteredUp() const;
        Met MetJER1p93Down() const;
        Met MetJER1p93Up() const;
        Met MetJER2p5Down() const;
        Met MetJER2p5Up() const;
        Met MetJECDown( const std::string& variation ) const;
        Met MetJECUp( const std::string& variation) const;

	// met objects varied by split JEC uncertainties

        Met MetJECGroupedDown( const std::string& variation ) const;
	Met MetJECGroupedUp( const std::string& variation) const;
        Met MetJECSourcesDown( const std::string& variation ) const;
        Met MetJECSourcesUp( const std::string& variation) const;
        Met MetVariation(JetCollection& nominalJets, Jet (Jet::*jetVariation)() const) const;

	Met getVariedMet( const std::string& variation ) const;
        Met getVariedMet(JetCollection& nomJets, const std::string& variation, unsigned flavor, bool up) const;
        Met HEMIssue(JetCollection&) const;
        //maximum variations of met pT
        double maxPtAnyVariation() const;
        double minPtAnyVariation() const;

        //print met information
        virtual std::ostream& print( std::ostream& ) const override;
        void setLorentzVectorExplicit(double pt, double eta, double phi, double e) {setLorentzVector(pt, eta, phi, e);}

        Met ApplyPhiModulation(int runnb, std::string year, bool isMC, int npv, bool isUL = true);
    private:
        // control var:
        bool isNano = false;
        //JEC uncertainties
        double _pt_JECDown = 0;
        double _phi_JECDown = 0;
        double _pt_JECUp = 0;
        double _phi_JECUp = 0;

        std::vector< std::string > _JECSources;
        std::map< std::string, std::pair<double,double> > _pxy_JECSourcesUp;
        std::map< std::string, std::pair<double,double> > _pxy_JECSourcesDown;

        std::vector< std::string > _JECGrouped;
        std::map< std::string, std::pair<double,double> > _pxy_JECGroupedUp;
        std::map< std::string, std::pair<double,double> > _pxy_JECGroupedDown;

        std::map< std::string, std::pair<double,double> > _pt_phi_JECGroupedUp;
        std::map< std::string, std::pair<double,double> > _pt_phi_JECGroupedDown;
        
        //unclustered energy uncertainties
        double _pt_UnclDown = 0;
        double _phi_UnclDown = 0;
        double _pt_UnclUp = 0;
        double _phi_UnclUp = 0;

        // JER variations eta < 1p93:
        double _pt_JER1p93Down = 0;
        double _phi_JER1p93Down = 0;
        double _pt_JER1p93Up = 0;
        double _phi_JER1p93Up = 0;
        // JER Variations 1p93 < eta < 2p5:
        double _pt_JER2p5Down = 0;
        double _phi_JER2p5Down = 0;
        double _pt_JER2p5Up = 0;
        double _phi_JER2p5Up = 0;

        // Control variable, equivalent to checking if we're running on mini or nano
        bool _metVariationsInitialized = false;
        bool _metJERVariationsInitialized = false;

        Met variedMet( const double pt, const double phi ) const;
	Met variedMetPxPy( const double px, const double py ) const;

        std::vector< double > metPtVariations() const;        
};
#endif 
