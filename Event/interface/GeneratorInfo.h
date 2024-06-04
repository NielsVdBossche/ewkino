#ifndef GeneratorInfo_H
#define GeneratorInfo_H

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"
#include "../../objects/interface/GenMet.h"
#include "LheCollection.h"

class GeneratorInfo{
    
    public:
        GeneratorInfo( const TreeReader& );
        GeneratorInfo( const NanoReader& );
        ~GeneratorInfo();

        unsigned numberOfLheWeights() const{ return _numberOfLheWeights; }

        unsigned firstScaleIndex() const{ return _firstScaleIndex; }
        unsigned numberOfScaleVariations() const{ return _numberOfScaleVariations; }
        unsigned firstPdfIndex() const{ return _firstPdfIndex; }
        unsigned numberOfPdfVariations() const{ return _numberOfPdfVariations; }

        // order of scale weights in nano:
        // see here: https://cms-nanoaod-integration.web.cern.ch/autoDoc/

        // MiniAOD implementation
        double relativeWeightScaleVar( const unsigned scaleIndex ) const;
        double relativeWeight_MuR_1_MuF_1() const{ return (miniAODSetup ? relativeWeightScaleVar( 0 ) : relativeWeightScaleVar(4)); }
        double relativeWeight_MuR_1_MuF_2() const{ return (miniAODSetup ? relativeWeightScaleVar( 1 ) : relativeWeightScaleVar(5)); }
        double relativeWeight_MuR_1_MuF_0p5() const{ return (miniAODSetup ? relativeWeightScaleVar( 2 ) : relativeWeightScaleVar(3)); }
        double relativeWeight_MuR_2_MuF_1() const{ return (miniAODSetup ? relativeWeightScaleVar( 3 ) : relativeWeightScaleVar(7)); }
        double relativeWeight_MuR_2_MuF_2() const{ return (miniAODSetup ? relativeWeightScaleVar( 4 ) : relativeWeightScaleVar(8)); }
        double relativeWeight_MuR_2_MuF_0p5() const{ return (miniAODSetup ? relativeWeightScaleVar( 5 ) : relativeWeightScaleVar(6)); }
        double relativeWeight_MuR_0p5_MuF_1() const{ return (miniAODSetup ? relativeWeightScaleVar( 6 ) : relativeWeightScaleVar(1)); }
        double relativeWeight_MuR_0p5_MuF_2() const{ return (miniAODSetup ? relativeWeightScaleVar( 7 ) : relativeWeightScaleVar(2)); }
        double relativeWeight_MuR_0p5_MuF_0p5() const{ return (miniAODSetup ? relativeWeightScaleVar( 8 ) : relativeWeightScaleVar(0)); }

        double relativeWeightPdfVar( const unsigned pdfIndex ) const;
        unsigned numberOfPsWeights() const{ return _numberOfPsWeights; }

        // order of PS weights:
        // see this line from the official nanoAOD producer,
        // https://github.com/cms-sw/cmssw/blob/3d761d84ee43f5ab61cf104d5081e09b074159b1/
        // PhysicsTools/NanoAOD/plugins/GenWeightsTableProducer.cc#L533
        // (maybe find better source later...)
        // see also here: https://cms-nanoaod-integration.web.cern.ch/autoDoc/
        double relativeWeightPsVar( const unsigned psIndex ) const;
        double relativeWeight_FSR_InverseSqrt2() const{ return relativeWeightPsVar( 2 ); }
        double relativeWeight_FSR_Sqrt2() const{ return relativeWeightPsVar( 3 ); }
        double relativeWeight_FSR_0p5() const{ return (miniAODSetup ? relativeWeightPsVar( 4 ) : relativeWeightPsVar(3)); }
        double relativeWeight_FSR_2() const{ return (miniAODSetup ? relativeWeightPsVar( 5 ) : relativeWeightPsVar(1)); }
        double relativeWeight_FSR_0p25() const{ return relativeWeightPsVar( 6 ); }
        double relativeWeight_FSR_4() const{ return relativeWeightPsVar( 7 ); }
        double relativeWeight_ISR_InverseSqrt2() const{ return relativeWeightPsVar( 24 ); }
        double relativeWeight_ISR_Sqrt2() const{ return relativeWeightPsVar( 25 ); }
        double relativeWeight_ISR_0p5() const{ return (miniAODSetup ? relativeWeightPsVar( 26 ) : relativeWeightPsVar(2)); }
        double relativeWeight_ISR_2() const{ return (miniAODSetup ? relativeWeightPsVar( 27 ) : relativeWeightPsVar(0)); }
        double relativeWeight_ISR_0p25() const{ return relativeWeightPsVar( 28 ); }
        double relativeWeight_ISR_4() const{ return relativeWeightPsVar( 29 ); }

        unsigned ttgEventType() const{ return _ttgEventType; }
        unsigned zgEventType() const{ return _zgEventType; }
        float numberOfTrueInteractions() const{ return _numberOfTrueInteractions; }

        double prefireWeight() const{ return _prefireWeight; }
        double prefireWeightDown() const{ return _prefireWeightDown; }
        double prefireWeightUp() const{ return _prefireWeightUp; }

        const GenMet& genMet() const{ return *_genMetPtr; }

        LheCollection* getLheCollection() const {return lheCollectionPtr;}

        unsigned getNEFTWeights() const {return _nEFTWeights;}
        double*  getEFTWeights() {return _eftWeight;}
    private:
        bool miniAODSetup = true;

        // nanoAOD LHE Weights
        unsigned _numberOfLHEPdfWeights = 0;
        unsigned _numberOfLHEScaleWeights = 0;
        static constexpr unsigned maxNumberOfLHEPdfWeights = 103;
        static constexpr unsigned maxNumberOfLHEScaleWeights = 9;
        double _LHEPdfWeights[maxNumberOfLHEPdfWeights];
        double _LHEScaleWeights[maxNumberOfLHEScaleWeights];

        // MiniAOD LHE Weights
        static constexpr unsigned maxNumberOfLheWeights = 148;
        unsigned _numberOfLheWeights = 0;
        double _lheWeights[maxNumberOfLheWeights];

        // PS Weights
        static constexpr unsigned maxNumberOfPsWeights = 46;
        static constexpr unsigned maxNumberOfPsWeightsNano = 4;
        unsigned _numberOfPsWeights = 0;
        double _psWeights[maxNumberOfPsWeights];

        double _prefireWeight = 0;
        double _prefireWeightDown = 0;
        double _prefireWeightUp = 0;

        unsigned _ttgEventType = 0;
        unsigned _zgEventType = 0;
        float _numberOfTrueInteractions = 0;

        unsigned _firstScaleIndex;
        unsigned _numberOfScaleVariations;
        unsigned _firstPdfIndex;
        unsigned _numberOfPdfVariations;

        // Relevant when performing EFT interpretation studies
        static constexpr unsigned maxNumberOfDynScaleWeights = 38;
        unsigned _nDynScaleWeights = 0;
        double _dynScaleWeight[maxNumberOfDynScaleWeights];
        static constexpr unsigned maxNumberOfEFTWeights = 37;
        unsigned _nEFTWeights = 0;
        double _eftWeight[maxNumberOfEFTWeights];

        std::shared_ptr< GenMet > _genMetPtr;

        // likely remove in future
        LheCollection* lheCollectionPtr = nullptr;
};

#endif 
