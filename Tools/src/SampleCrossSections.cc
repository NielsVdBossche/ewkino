#include "../interface/SampleCrossSections.h"

//include c++ library classes
#include <stdexcept>
#include <string>

//include ROOT classes
#include "TH1.h"
#include "TROOT.h"
#include "TTree.h"

SampleCrossSections::SampleCrossSections( const Sample& sample ){

    //open file
    std::shared_ptr< TFile > sampleFile = sample.filePtr();

    // check if nanoAOD file but checking if the Runs Tree exists:
    TTree* runsTree = (TTree*) sampleFile->Get("Runs");
    if (runsTree) {
        initializeAsNanoAOD(sample, runsTree);
    } else if (sampleFile->Get("blackJackAndHookers/hCounter")) {
        initializeAsMiniAOD(sample);
    } else {
        throw std::invalid_argument( "Runs tree is not present in file '" + sample.fileName() + "', neither are HeavyNeutrino histograms." );
        exit(1);
    }
}

void SampleCrossSections::initializeAsNanoAOD(const Sample& sample, TTree* runsTree) {
    // Set branches of RunsTree:
    Double_t tmp_nominalSumOfWeights = 0.;
    UInt_t tmp_nLHEScaleSumw = 0.;
    Double_t tmp_LHEScaleSumw[9];
    UInt_t tmp_nLHEPdfSumw = 0.;
    Double_t tmp_LHEPdfSumw[103];

    runsTree->SetBranchAddress("genEventSumw",  &tmp_nominalSumOfWeights);
    runsTree->SetBranchAddress("nLHEScaleSumw", &tmp_nLHEScaleSumw);
    runsTree->SetBranchAddress("LHEScaleSumw",   tmp_LHEScaleSumw);
    runsTree->SetBranchAddress("nLHEPdfSumw",   &tmp_nLHEPdfSumw);
    runsTree->SetBranchAddress("LHEPdfSumw",     tmp_LHEPdfSumw);
    runsTree->GetEntry(0);
    // Sum of weights equivalent:
    nominalSumOfWeights = tmp_nominalSumOfWeights;

    // LHE Variations:
    lheCrossSectionRatios = std::vector<double>(111, 1.);
    lheCrossSectionRatios[1] = tmp_LHEScaleSumw[5];
    lheCrossSectionRatios[2] = tmp_LHEScaleSumw[3];
    lheCrossSectionRatios[3] = tmp_LHEScaleSumw[7];
    lheCrossSectionRatios[4] = tmp_LHEScaleSumw[8];
    lheCrossSectionRatios[5] = tmp_LHEScaleSumw[6];
    lheCrossSectionRatios[6] = tmp_LHEScaleSumw[1];
    lheCrossSectionRatios[7] = tmp_LHEScaleSumw[2];
    lheCrossSectionRatios[8] = tmp_LHEScaleSumw[0];

    // PDF variations: just vary it all
    for (unsigned i = 9; i < lheCrossSectionRatios.size(); i++){
        lheCrossSectionRatios[i] = tmp_LHEPdfSumw[i-9];
    }

    // PS Weights require the events tree to be read very quickly over all branches, just the PSWeights branch is needed.:
    // Externalize the normalization calculation. Can be done with quick uproot/awkward array script that creates a file with an entry per sample. 
    // Then just need to read out correct entry.
}

void SampleCrossSections::initializeAsMiniAOD(const Sample& sample) {
    std::shared_ptr< TFile > sampleFile = sample.filePtr();

    std::shared_ptr< TH1 > hCounter( dynamic_cast< TH1* >( sampleFile->Get( "blackJackAndHookers/hCounter" ) ) );
    hCounter->SetDirectory( gROOT );
    if( hCounter == nullptr ){
        throw std::invalid_argument( "hCounter is not present in file '" + sample.fileName() + "'." );
    } 

    std::shared_ptr< TH1 > lheCounter( dynamic_cast< TH1* >( sampleFile->Get( "blackJackAndHookers/lheCounter" ) ) );
    lheCounter->SetDirectory( gROOT );
    if( lheCounter == nullptr ){
        throw std::invalid_argument( "lheCounter is not present in file '" + sample.fileName() + "'." );
    } 

    std::shared_ptr< TH1 > psCounter( dynamic_cast< TH1* >( sampleFile->Get( "blackJackAndHookers/psCounter" ) ) );
    psCounter->SetDirectory( gROOT );
    if( psCounter == nullptr ){
        throw std::invalid_argument( "psCounter is not present in file '" + sample.fileName() + "'." );
    }

    nominalSumOfWeights = hCounter->GetBinContent( 1 );
    
    //store all lhe variations
    for( int bin = 1; bin < lheCounter->GetNbinsX() + 1; ++bin ){
        double lheVariedSumOfWeights = lheCounter->GetBinContent( bin );

        //0 entries indicate that a sample didn't have the respective weights
        if( lheVariedSumOfWeights < 1e-6 ) break;
        
        lheCrossSectionRatios.push_back( lheVariedSumOfWeights / nominalSumOfWeights );
    }

    //store all parton shower variations
    for( int bin = 1; bin < lheCounter->GetNbinsX() + 1; ++bin ){
        double psVariedSumOfWeights = psCounter->GetBinContent( bin );

        //0 entries indicate that a sample didn't have the respective weights
        if( psVariedSumOfWeights < 1e-6 ) break;

        psCrossSectionRatios.push_back( psVariedSumOfWeights / nominalSumOfWeights );
    }
}



double SampleCrossSections::crossSectionRatio_lheVar( const size_type index ) const{
    if( index > lheCrossSectionRatios.size() ){
        throw std::out_of_range( "Requesting lhe cross section variation " + std::to_string( index ) + " while only " + std::to_string( lheCrossSectionRatios.size() ) + " are present." );
    }
    return lheCrossSectionRatios[ index ];
}


double SampleCrossSections::crossSectionRatio_pdfVar( const size_type index ) const{

    //the tenth entry in the lhe cross section ratios is the first one that corresponds to pdfs
    return crossSectionRatio_lheVar( index + 9 );
}


double SampleCrossSections::crossSectionRatio_scaleVar( const size_type index ) const{

    //entries up to the ninth one in the lhe cross sectio ratios correspond to scales
    if( index > 8 ){
        throw std::out_of_range( "Requesting scale variation " + std::to_string( index ) + " while only 9 scale variations are defined." );
    }
    return crossSectionRatio_lheVar( index );
}


double SampleCrossSections::crossSectionRatio_psVar( const size_type index ) const{
    if( index > psCrossSectionRatios.size() ){
        throw std::out_of_range( "Requesting ps cross section variation " + std::to_string( index ) + " while only " + std::to_string( psCrossSectionRatios.size() ) + " are present." );
    }
    return psCrossSectionRatios[ index ];
}
