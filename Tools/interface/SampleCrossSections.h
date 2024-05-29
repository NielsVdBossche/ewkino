#ifndef SampleCrossSections_H
#define SampleCrossSections_H

#include "Sample.h"


class SampleCrossSections{

	public:
		using size_type = std::vector< double >::size_type;
		// TODO: also use Nano: Check if Runs tree exists..
        SampleCrossSections() = default;
		SampleCrossSections( const Sample& );

		void initializeAsNanoAOD(const Sample&, TTree* runsTree);
		void initializeAsMiniAOD(const Sample&);

        size_type numberOfLheVariations() const{ return lheCrossSectionRatios.size(); }
		double crossSectionRatio_pdfVar( const size_type ) const;

		double crossSectionRatio_scaleVar( const size_type ) const;
		double crossSectionRatio_MuR_1_MuF_2() const{ return crossSectionRatio_scaleVar( 1 ); }
		double crossSectionRatio_MuR_1_MuF_0p5() const{ return crossSectionRatio_scaleVar( 2 ); }
		double crossSectionRatio_MuR_2_MuF_1() const{ return crossSectionRatio_scaleVar( 3 ); }
		double crossSectionRatio_MuR_2_MuF_2() const{ return crossSectionRatio_scaleVar( 4 ); }
		double crossSectionRatio_MuR_2_MuF_0p5() const{ return crossSectionRatio_scaleVar( 5 ); }
		double crossSectionRatio_MuR_0p5_MuF_1() const{ return crossSectionRatio_scaleVar( 6 ); }
		double crossSectionRatio_MuR_0p5_MuF_2() const{ return crossSectionRatio_scaleVar( 7 ); }
		double crossSectionRatio_MuR_0p5_MuF_0p5() const{ return crossSectionRatio_scaleVar( 8 ); }


        size_type numberOfPsVariations() const{ return psCrossSectionRatios.size(); }
		double crossSectionRatio_psVar( const size_type ) const;
		double crossSectionRatio_FSR_InverseSqrt2() const{ return crossSectionRatio_psVar( 2 ); }
		double crossSectionRatio_FSR_Sqrt2() const{ return crossSectionRatio_psVar( 3 ); }
		double crossSectionRatio_FSR_0p5() const{ return crossSectionRatio_psVar( 4 ); }
		double crossSectionRatio_FSR_2() const{ return crossSectionRatio_psVar( 5 ); }
		double crossSectionRatio_FSR_0p25() const{ return crossSectionRatio_psVar( 6 ); }
		double crossSectionRatio_FSR_4() const{ return crossSectionRatio_psVar( 7 ); }
		double crossSectionRatio_ISR_InverseSqrt2() const{ return crossSectionRatio_psVar( 24 ); }
		double crossSectionRatio_ISR_Sqrt2() const{ return crossSectionRatio_psVar( 25 ); }
		double crossSectionRatio_ISR_0p5() const{ return crossSectionRatio_psVar( 26 ); }
		double crossSectionRatio_ISR_2() const{ return crossSectionRatio_psVar( 27 ); }
		double crossSectionRatio_ISR_0p25() const{ return crossSectionRatio_psVar( 28 ); }
		double crossSectionRatio_ISR_4() const{ return crossSectionRatio_psVar( 29 ); }

	private:
		std::vector< double > lheCrossSectionRatios;
        std::vector< double > psCrossSectionRatios;
		double nominalSumOfWeights;

        double crossSectionRatio_lheVar( const size_type ) const;
};


#endif
