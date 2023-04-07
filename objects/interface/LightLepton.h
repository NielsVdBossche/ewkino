#ifndef LightLepton_H
#define LightLepton_H

// include other parts of code 
#include "Lepton.h"
#include "../../TreeReader/interface/TreeReader.h"

class LightLepton : public Lepton {
    
    public:
	// constructor from TreeReader
	LightLepton( const TreeReader& treeReader,
		     const std::string& leptonType,
		     const unsigned leptonIndex,
		     LeptonSelector* leptonSelector );

        LightLepton( const LightLepton& ) = delete;
        LightLepton( LightLepton&& ) = delete;

        LightLepton& operator=( const LightLepton& ) = default;
        LightLepton& operator=( LightLepton&& ) = default;

        // isolation variables 
        double relIso0p3() const{ return _relIso0p3; }
        double relIso0p4() const{ return _relIso0p4; }
        double miniIso() const{ return _miniIso; }
        double miniIsoCharged() const{ return _miniIsoCharged; }
        double miniIsoNeutral() const{ return _miniIso - _miniIsoCharged; }

        // properties of the jet closest to the lepton
        double ptRatio() const{ return _ptRatio; }
        double ptRel() const{ return _ptRel; }
        double closestJetDeepCSV() const{ return _closestJetDeepCSV; }
        double closestJetDeepFlavor() const{ return _closestJetDeepFlavor; }
        unsigned closestJetTrackMultiplicity() const{ return _closestJetTrackMultiplicity; }
	unsigned closestJetNumberOfChargedDaughters() const{ return _closestJetNumberOfChargedDaughters; }

	// other properties
	double sip3d() const{ return _sip3d; }

        // lepton MVA discriminant
        double leptonMVAtZq() const{ return _leptonMVAtZq; }
        double leptonMVAttH() const{ return _leptonMVAttH; }
	double leptonMVATOP() const{ return _leptonMVATOP; }
	double leptonMVATOPUL() const{ return _leptonMVATOPUL; }
	double leptonMVATOPv2UL() const{ return _leptonMVATOPv2UL; }

        // check lepton type
        virtual bool isLightLepton() const override{ return true; }
        virtual bool isTau() const override{ return false; }

        // destructor
        virtual ~LightLepton(){};

        // print out lepton information
        virtual std::ostream& print( std::ostream& os = std::cout ) const override;

    private:

        // isolation variables 
        double _relIso0p3 = 0;
        double _relIso0p4 = 0;
        double _miniIso = 0;
        double _miniIsoCharged = 0;

        // properties of the jet closest to the lepton
        double _ptRatio = 0;
        double _ptRel = 0;
        double _closestJetDeepCSV = 0;
        double _closestJetDeepFlavor = 0;
        unsigned _closestJetTrackMultiplicity = 0;
	unsigned _closestJetNumberOfChargedDaughters = 0;

        // other properties
	double _sip3d = 0;

        // lepton MVA output 
        double _leptonMVAtZq = 0;
        double _leptonMVAttH = 0;
	double _leptonMVATOP = 0;

    protected: 

        LightLepton( const LightLepton&, LeptonSelector* );
        LightLepton( LightLepton&&, LeptonSelector* );

	// lepton MVA scores
	// note: must be protected instead of private,
	// as this is needed for setting them on the fly
	// in derived Electron and Muon classes.
	double _leptonMVATOPUL = 0;
        double _leptonMVATOPv2UL = 0;
};

#endif
