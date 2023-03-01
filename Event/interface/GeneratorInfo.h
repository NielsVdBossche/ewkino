#ifndef GeneratorInfo_H
#define GeneratorInfo_H

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../objects/interface/GenMet.h"

class GeneratorInfo{
    
    public:
        GeneratorInfo( const TreeReader& );

        unsigned numberOfLHEPdfWeights() const{ return _numberOfLHEPdfWeights; }
	unsigned numberOfLHEScaleWeights() const{ return _numberOfLHEScaleWeights; }
	        
	double relativeWeightScaleVar( const unsigned scaleIndex ) const;
        // order of scale weights:
	// see here: https://cms-nanoaod-integration.web.cern.ch/autoDoc/
	double relativeWeight_MuR_1_MuF_1() const{ return relativeWeightScaleVar( 4 ); }
        double relativeWeight_MuR_1_MuF_2() const{ return relativeWeightScaleVar( 5 ); }
        double relativeWeight_MuR_1_MuF_0p5() const{ return relativeWeightScaleVar( 3 ); }
        double relativeWeight_MuR_2_MuF_1() const{ return relativeWeightScaleVar( 7 ); }
        double relativeWeight_MuR_2_MuF_2() const{ return relativeWeightScaleVar( 8 ); }
        double relativeWeight_MuR_2_MuF_0p5() const{ return relativeWeightScaleVar( 6 ); }
        double relativeWeight_MuR_0p5_MuF_1() const{ return relativeWeightScaleVar( 1 ); }
        double relativeWeight_MuR_0p5_MuF_2() const{ return relativeWeightScaleVar( 2 ); }
        double relativeWeight_MuR_0p5_MuF_0p5() const{ return relativeWeightScaleVar( 0 ); }

        double relativeWeightPdfVar( const unsigned pdfIndex ) const;

        unsigned numberOfPsWeights() const{ return _numberOfPsWeights; }
        double relativeWeightPsVar( const unsigned psIndex ) const;
        // order of PS weights:
	// see this line from the official nanoAOD producer,
	// https://github.com/cms-sw/cmssw/blob/3d761d84ee43f5ab61cf104d5081e09b074159b1/
	// PhysicsTools/NanoAOD/plugins/GenWeightsTableProducer.cc#L533
	// (maybe find better source later...)
	// see also here: https://cms-nanoaod-integration.web.cern.ch/autoDoc/
	//double relativeWeight_ISR_InverseSqrt2() const{ return relativeWeightPsVar( 24 ); }
        //double relativeWeight_FSR_InverseSqrt2() const{ return relativeWeightPsVar( 2 ); }
        //double relativeWeight_ISR_Sqrt2() const{ return relativeWeightPsVar( 25 ); }
        //double relativeWeight_FSR_Sqrt2() const{ return relativeWeightPsVar( 3 ); }
        double relativeWeight_ISR_0p5() const{ return relativeWeightPsVar( 2 ); }
        double relativeWeight_FSR_0p5() const{ return relativeWeightPsVar( 3 ); }
        double relativeWeight_ISR_2() const{ return relativeWeightPsVar( 0 ); }
        double relativeWeight_FSR_2() const{ return relativeWeightPsVar( 1 ); }
        //double relativeWeight_ISR_0p25() const{ return relativeWeightPsVar( 28 ); }
        //double relativeWeight_FSR_0p25() const{ return relativeWeightPsVar( 6 ); }
        //double relativeWeight_ISR_4() const{ return relativeWeightPsVar( 29 ); }
        //double relativeWeight_FSR_4() const{ return relativeWeightPsVar( 7 ); }

        float numberOfTrueInteractions() const{ return _numberOfTrueInteractions; }

        double prefireWeight() const{ return _prefireWeight; }
        double prefireWeightDown() const{ return _prefireWeightDown; }
        double prefireWeightUp() const{ return _prefireWeightUp; }
	double prefireWeightMuon() const{ return _prefireWeightMuon; }
        double prefireWeightMuonDown() const{ return _prefireWeightMuonDown; }
        double prefireWeightMuonUp() const{ return _prefireWeightMuonUp; }
	double prefireWeightECAL() const{ return _prefireWeightECAL; }
        double prefireWeightECALDown() const{ return _prefireWeightECALDown; }
        double prefireWeightECALUp() const{ return _prefireWeightECALUp; }

        const GenMet& genMet() const{ return *_genMetPtr; }

    private:
        static constexpr unsigned maxNumberOfLHEPdfWeights = 103;
        unsigned _numberOfLHEPdfWeights;
        double _LHEPdfWeights[maxNumberOfLHEPdfWeights];
	static constexpr unsigned maxNumberOfLHEScaleWeights = 9;
	unsigned _numberOfLHEScaleWeights;
        double _LHEScaleWeights[maxNumberOfLHEScaleWeights];
        static constexpr unsigned maxNumberOfPsWeights = 4;
        unsigned _numberOfPsWeights;
        double _psWeights[maxNumberOfPsWeights];
        double _prefireWeight = 0;
        double _prefireWeightDown = 0;
        double _prefireWeightUp = 0;
	double _prefireWeightMuon = 0;
        double _prefireWeightMuonDown = 0;
        double _prefireWeightMuonUp = 0;
	double _prefireWeightECAL = 0;
        double _prefireWeightECALDown = 0;
        double _prefireWeightECALUp = 0;

        float _numberOfTrueInteractions;

        std::shared_ptr< GenMet > _genMetPtr;
};

#endif 
