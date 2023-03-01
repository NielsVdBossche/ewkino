#ifndef LightLepton_H
#define LightLepton_H

// include other parts of code 
#include "Lepton.h"
#include "../../TreeReader/interface/TreeReader.h"

class LightLepton : public Lepton {
    
    public:
	// constructor from explicitly passed values 
        /*LightLepton( const double pt, const double eta, const double phi, const double e,
                const bool is2016, const bool is2016PreVFP, const bool is2016PostVFP,
                const bool is2017, const bool is2018,
                LeptonSelector* leptonSelector,
                const int charge, const double dxy, const double dz, const double sip3d,
                LeptonGeneratorInfo* leptonGeneratorInfo,
                const double relIso0p3, const double relIso0p4,
                const double miniIso, const double miniIsoCharged,
                const double ptRatio, const double ptRel,
                const double closestJetDeepCSV, const double closestJetDeepFlavor,
                const double closestJetTrackMultiplicity,
                const double leptonMVATOPUL, const double leptonMVATOPv2UL );*/
	// constructor from TreeReader
	LightLepton( const TreeReader& treeReader,
		     const std::string& leptonType,
		     const unsigned leptonIndex,
		     LeptonSelector* leptonSelector );

        LightLepton( const LightLepton& ) = delete;
        LightLepton( LightLepton&& ) = delete;

        LightLepton& operator=( const LightLepton& ) = default;
        LightLepton& operator=( LightLepton&& ) = default;

        //isolation variables 
        double relIso0p3() const{ return _relIso0p3; }
        double relIso0p4() const{ return _relIso0p4; }
        double miniIso() const{ return _miniIso; }
        double miniIsoCharged() const{ return _miniIsoCharged; }
        double miniIsoNeutral() const{ return _miniIso - _miniIsoCharged; }

        //properties of the jet closest to the lepton
        double ptRatio() const{ return _ptRatio; }
        double ptRel() const{ return _ptRel; }
        double closestJetDeepCSV() const{ return _closestJetDeepCSV; }
        double closestJetDeepFlavor() const{ return _closestJetDeepFlavor; }
        unsigned closestJetTrackMultiplicity() const{ return _closestJetTrackMultiplicity; }

        //lepton MVA discriminant
        double leptonMVAtZq() const{ return _leptonMVAtZq; }
        double leptonMVAttH() const{ return _leptonMVAttH; }
	double leptonMVATOP() const{ return _leptonMVATOP; }
	double leptonMVATOPUL() const{ return _leptonMVATOPUL; }
	double leptonMVATOPv2UL() const{ return _leptonMVATOPv2UL; }

        //check lepton type
        virtual bool isLightLepton() const override{ return true; }
        virtual bool isTau() const override{ return false; }

        //destructor
        virtual ~LightLepton(){};

        //print out lepton information
        virtual std::ostream& print( std::ostream& os = std::cout ) const override;

    private:

        //isolation variables 
        double _relIso0p3 = 0;
        double _relIso0p4 = 0;
        double _miniIso = 0;
        double _miniIsoCharged = 0;

        //properties of the jet closest to the lepton
        double _ptRatio = 0;
        double _ptRel = 0;
        double _closestJetDeepCSV = 0;
        double _closestJetDeepFlavor = 0;
        unsigned _closestJetTrackMultiplicity = 0;

        //lepton MVA output 
        double _leptonMVAtZq = 0;
        double _leptonMVAttH = 0;
	double _leptonMVATOP = 0;
	double _leptonMVATOPUL = 0;
	double _leptonMVATOPv2UL = 0;

    protected: 

        LightLepton( const LightLepton&, LeptonSelector* );
        LightLepton( LightLepton&&, LeptonSelector* );
};

#endif
