#ifndef GeneratorInfo_H
#define GeneratorInfo_H

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../objects/interface/GenMet.h"
#include "LheCollection.h"

class GeneratorInfo{
    
    public:
        GeneratorInfo( const TreeReader& );
        ~GeneratorInfo();

        unsigned numberOfLheWeights() const{ return _numberOfLheWeights; }

        unsigned firstScaleIndex() const{ return _firstScaleIndex; }
        unsigned numberOfScaleVariations() const{ return _numberOfScaleVariations; }
        unsigned firstPdfIndex() const{ return _firstPdfIndex; }
        unsigned numberOfPdfVariations() const{ return _numberOfPdfVariations; }

        double relativeWeightScaleVar( const unsigned scaleIndex ) const;
        double relativeWeight_MuR_1_MuF_1() const{ return relativeWeightScaleVar( 0 ); }
        double relativeWeight_MuR_1_MuF_2() const{ return relativeWeightScaleVar( 1 ); }
        double relativeWeight_MuR_1_MuF_0p5() const{ return relativeWeightScaleVar( 2 ); }
        double relativeWeight_MuR_2_MuF_1() const{ return relativeWeightScaleVar( 3 ); }
        double relativeWeight_MuR_2_MuF_2() const{ return relativeWeightScaleVar( 4 ); }
        double relativeWeight_MuR_2_MuF_0p5() const{ return relativeWeightScaleVar( 5 ); }
        double relativeWeight_MuR_0p5_MuF_1() const{ return relativeWeightScaleVar( 6 ); }
        double relativeWeight_MuR_0p5_MuF_2() const{ return relativeWeightScaleVar( 7 ); }
        double relativeWeight_MuR_0p5_MuF_0p5() const{ return relativeWeightScaleVar( 8 ); }

        double relativeWeightPdfVar( const unsigned pdfIndex ) const;

        unsigned numberOfPsWeights() const{ return _numberOfPsWeights; }
        double relativeWeightPsVar( const unsigned psIndex ) const;
        double relativeWeight_FSR_InverseSqrt2() const{ return relativeWeightPsVar( 2 ); }
        double relativeWeight_FSR_Sqrt2() const{ return relativeWeightPsVar( 3 ); }
        double relativeWeight_FSR_0p5() const{ return relativeWeightPsVar( 4 ); }
        double relativeWeight_FSR_2() const{ return relativeWeightPsVar( 5 ); }
        double relativeWeight_FSR_0p25() const{ return relativeWeightPsVar( 6 ); }
        double relativeWeight_FSR_4() const{ return relativeWeightPsVar( 7 ); }
        double relativeWeight_ISR_InverseSqrt2() const{ return relativeWeightPsVar( 24 ); }
        double relativeWeight_ISR_Sqrt2() const{ return relativeWeightPsVar( 25 ); }
        double relativeWeight_ISR_0p5() const{ return relativeWeightPsVar( 26 ); }
        double relativeWeight_ISR_2() const{ return relativeWeightPsVar( 27 ); }
        double relativeWeight_ISR_0p25() const{ return relativeWeightPsVar( 28 ); }
        double relativeWeight_ISR_4() const{ return relativeWeightPsVar( 29 ); }

        unsigned ttgEventType() const{ return _ttgEventType; }
        unsigned zgEventType() const{ return _zgEventType; }
        double partonLevelHT() const{ return _partonLevelHT; }
        float numberOfTrueInteractions() const{ return _numberOfTrueInteractions; }

        double prefireWeight() const{ return _prefireWeight; }
        double prefireWeightDown() const{ return _prefireWeightDown; }
        double prefireWeightUp() const{ return _prefireWeightUp; }

        const GenMet& genMet() const{ return *_genMetPtr; }

        LheCollection* getLheCollection() const {return lheCollectionPtr;}

        unsigned getNEFTWeights() const {return _nEFTWeights;}
        double*  getEFTWeights() {return _eftWeight;}
    private:
        static constexpr unsigned maxNumberOfLheWeights = 148;
        unsigned _numberOfLheWeights;
        double _lheWeights[maxNumberOfLheWeights];
        static constexpr unsigned maxNumberOfPsWeights = 46;
        unsigned _numberOfPsWeights;
        double _psWeights[maxNumberOfPsWeights];
        double _prefireWeight;
        double _prefireWeightDown;
        double _prefireWeightUp;

        unsigned _ttgEventType;
        unsigned _zgEventType;
        double _partonLevelHT;
        float _numberOfTrueInteractions;

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

        LheCollection* lheCollectionPtr = nullptr;
};

#endif 
