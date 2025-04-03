//include c++ library classes
#include <fstream>
#include <memory>
#include <stdexcept>

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"
#include "../../Event/interface/Event.h"
#include "../../Tools/interface/analysisTools.h"
#include "../../Tools/interface/histogramTools.h"
#include "../../Tools/interface/systemTools.h"
#include "../../Tools/interface/stringTools.h"


void computePUIDEff( const std::string& year, const std::string& sampleList, const bool cleanJetsFromLooseLeptons, const bool cleanJetsFromFOLeptons, const bool isNanoAOD ){ //, const bool deepCSV ){

    analysisTools::checkYearString( year ); 

    //make 2D b-tagging efficiency histograms for all jet flavors, and for numerator and denominator

    //assume jets below 20 GeV in pT will not be used for b-tagging
    // Only use jets between 25 and 50 GeV
    const std::vector< double > ptBins = {25, 35, 50};
    const std::vector< double > etaBins = { 0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4 };

    const std::vector< std::string > numeratorOrDenominator = { "numerator", "denominator" };

    std::vector< std::shared_ptr< TH2D > > puidEfficiencyMaps(numeratorOrDenominator.size());

    //initialize histograms
    for( std::vector< std::string >::size_type term = 0; term < numeratorOrDenominator.size(); ++term ){
        std::string name = "puIDEff_LoosWP_" + numeratorOrDenominator[ term ];
        puidEfficiencyMaps[ term ] = std::make_shared< TH2D >( name.c_str(), ( name + ";p_{T}(jet) (GeV);|#eta|(jet)" ).c_str(), ptBins.size() - 1, &ptBins[0], etaBins.size() - 1, &etaBins[0] );
        puidEfficiencyMaps[ term ]->Sumw2();
    }

    //initialize the TreeReader
    // TODO: Mini VS Nano
    BaseReader* treeReader;
    if (isNanoAOD) {
        treeReader = new NanoReader( sampleList, "/pnfs/iihe/cms/store/user/nivanden/skims/" );
    } else {
        treeReader = new TreeReader( sampleList, "/pnfs/iihe/cms/store/user/nivanden/skims/" );
    }

    //loop over all samples 
    for( unsigned i = 0; i < treeReader->numberOfSamples(); ++i ){
        treeReader->initSample();

        //loop over events in sample
        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            Event event = treeReader->buildEvent( entry );

            //ignore weight differences between samples for better statistics
            double weight = event.weight();
            if( weight > 0. ){
                weight = 1.;
            } else if( weight < 0. ){
                weight = -1.;
            } else {
                throw std::runtime_error( "Weight of event is zero." );
            }

            //apply selection to jets 
            event.selectLooseLeptons();
            event.cleanElectronsFromLooseMuons();
            event.removeTaus();
            event.selectGoodJets();
            if( cleanJetsFromLooseLeptons && !cleanJetsFromFOLeptons ){
                event.cleanJetsFromLooseLeptons();
            } else if( !cleanJetsFromLooseLeptons && cleanJetsFromFOLeptons ){
                event.cleanJetsFromFOLeptons();
            } else if( !( cleanJetsFromLooseLeptons || cleanJetsFromFOLeptons ) ){

                //no cleaning to do
            } else {
                throw std::invalid_argument( "Arguments 'cleanJetsFromLooseLeptons' and 'cleanJetsFromFOLeptons' should not both be true." );
            }

            if (event.numberOfTightLeptons() < 2) continue;
            if (event.numberOfTightLeptons() == 2 && event.TightLeptonCollection()[0].charge() != event.TightLeptonCollection()[1].charge()) continue;
            if (event.numberOfGoodJets() < 2) continue;
            if (event.numberOfTightLeptons() < 4 && event.HT() < 200) continue;
            
            //loop over jets 
            for( const auto& jetPtr : event.jetCollection() ){

                const Jet& jet = *jetPtr;

                //jet must pass additional requirements for b tagging
                if( ! jet.isGood() ) continue;
                if (jet.pt() < 25 || jet.pt() > 50) continue;
                if (jet.genJetIdx() == -1) continue;  

                //check that jet passes specified working point for numerator
                // TODO: check that jet passes PUID
                if( jet.inBTagAcceptance() ){
                    histogram::fillValues( puidEfficiencyMaps[ 0 ].get(), jet.pt(), jet.absEta(), weight );
                }

                //denominator
                histogram::fillValues( puidEfficiencyMaps[ 1 ].get(), jet.pt(), jet.absEta(), weight );
                break;
            }
        }
    }

    //output file path
    const std::string outputDirectory = "../weightFiles/puIDEff";
    systemTools::makeDirectory( outputDirectory );

    std::string cleaningName;
    if( cleanJetsFromLooseLeptons ){
        cleaningName = "looseLeptonCleaned";
    } else if( cleanJetsFromFOLeptons ){
        cleaningName = "FOLeptonCleaned";
    } else {
        cleaningName = "uncleaned";
    }
    const std::string fileName = ( "puIDEff_HSonly_" + cleaningName + "_" + year + ".root" );
    std::string outputPath = stringTools::formatDirectoryName( outputDirectory ) + fileName;
    std::cout << "Creating file at " << outputPath << std::endl;
    
    TFile* outputFilePtr = TFile::Open( outputPath.c_str(), "RECREATE" );
    double globalEff = puidEfficiencyMaps[ 0 ]->Integral() / puidEfficiencyMaps[ 1 ]->Integral();
    std::cout << "global efficiency at " << ": " << globalEff << std::endl;
    //divide numerator and denominator and write to file
    puidEfficiencyMaps[ 0 ]->Divide( puidEfficiencyMaps[ 1 ].get() );
    puidEfficiencyMaps[ 0 ]->Write("puIDEff_LoosWP");

    outputFilePtr->Close();
}


int main(int argc, char* argv[]){

    //convert all input to std::string format for easier handling
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    //either take 3 command line arguments ( sampleDirectory, year, cleaning ) or 1 ( sampleDirectory ) in which case everything will be run
    if( !( argvStr.size() == 5) ){
        std::cerr << argc - 1 << " command line arguments given, while 3 or 1 are expected." << std::endl;
        std::cerr << "Usage ( to determine the efficiencies for all years and cleaning options ): ./computeBTagEfficienciesMC sampleDirectory" << std::endl;
        std::cerr << "Or ( to determine the efficiencies for one year and cleaning option ): ./computeBTagEfficienciesMC sampleDirectory year cleaningSchem ( = looseLeptons, FOLeptons, uncleaned )" << std::endl;
        return 1;
    } else {
        std::string sampleList = argvStr[1];
        std::string year = argvStr[2];
        std::string cleaningOption = argvStr[3];
        std::string nanoOrMini = argvStr[4];
        bool isNanoAOD = ( nanoOrMini == "nanoAOD" );
        analysisTools::checkYearString( year );

        if (! ( cleaningOption == "looseLeptons" || cleaningOption == "FOLeptons" || cleaningOption == "uncleaned" )) {
            throw std::invalid_argument( "cleaningOption should be either 'looseLeptons', 'FOLeptons' or 'uncleaned'." );
        }
        bool cleanJetsFromLooseLeptons = ( cleaningOption == "looseLeptons" );
        bool cleanJetsFromFOLeptons = ( cleaningOption == "FOLeptons" );
        computePUIDEff(year, sampleList, cleanJetsFromLooseLeptons, cleanJetsFromFOLeptons, isNanoAOD);
    }
	return 0;
}
