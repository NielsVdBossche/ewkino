
//include c++ library classes
#include <fstream>
#include <memory>
#include <stdexcept>

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Event/interface/Event.h"
#include "../../Tools/interface/analysisTools.h"
#include "../../Tools/interface/histogramTools.h"
#include "../../Tools/interface/systemTools.h"
#include "../../Tools/interface/stringTools.h"


void computeBTagEff( const std::string& year, const std::string& sampleList, const bool cleanJetsFromLooseLeptons, const bool cleanJetsFromFOLeptons ){ //, const bool deepCSV ){

    analysisTools::checkYearString( year ); 

    //make 2D b-tagging efficiency histograms for all jet flavors, and for numerator and denominator

    //assume jets below 20 GeV in pT will not be used for b-tagging
    const std::vector< double > ptBins = {20, 30, 50, 70, 100, 140, 200, 300, 600, 1000};
    const std::vector< double > etaBins = { 0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4 };

    const std::vector< std::string > numeratorOrDenominator = { "numerator", "denominator" };
    const std::vector< std::string > quarkFlavors = { "udsg", "charm", "beauty" };
    const std::vector< std::string > workingPointNames = { "loose", "medium", "tight" };

    //map working points to specific jet selectors
    typedef bool ( Jet::*passBTag )() const;
    const std::vector< passBTag > workingPointFunctions = { &Jet::isBTaggedLoose, &Jet::isBTaggedMedium, &Jet::isBTaggedTight };

    std::vector< std::vector< std::vector< std::shared_ptr< TH2D > > > > bTagEfficiencyMaps(
        numeratorOrDenominator.size(), std::vector< std::vector< std::shared_ptr< TH2D > > >( quarkFlavors.size(), std::vector< std::shared_ptr< TH2D > >( workingPointNames.size() ) ) );

    //initialize histograms
    for( std::vector< std::string >::size_type term = 0; term < numeratorOrDenominator.size(); ++term ){
        for( std::vector< std::string >::size_type flavor = 0; flavor < quarkFlavors.size(); ++flavor ){
            for( std::vector< std::string >::size_type wp = 0; wp < workingPointNames.size(); ++wp ){
                std::string name = "bTagEff_" + workingPointNames[ wp ] + "_" + quarkFlavors[ flavor ] + "_" + numeratorOrDenominator[ term ];
                bTagEfficiencyMaps[ term ][ flavor ][ wp ] = std::make_shared< TH2D >( name.c_str(), ( name + ";p_{T}(jet) (GeV);|#eta|(jet)" ).c_str(), ptBins.size() - 1, &ptBins[0], etaBins.size() - 1, &etaBins[0] );
                bTagEfficiencyMaps[ term ][ flavor ][ wp ]->Sumw2();
            }
        }
    }

    //initialize the TreeReader
    TreeReader treeReader( sampleList, "/pnfs/iihe/cms/store/user/nivanden/skims/" );

    //loop over all samples 
    for( unsigned i = 0; i < treeReader.numberOfSamples(); ++i ){
        treeReader.initSample();

        //loop over events in sample
        for( long unsigned entry = 0; entry < treeReader.numberOfEntries(); ++entry ){
            Event event = treeReader.buildEvent( entry );

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
                if( ! jet.inBTagAcceptance() ) continue;
                    
                size_t flavorIndex = ( 0 + ( jet.hadronFlavor() == 4 ) + 2 * ( jet.hadronFlavor() == 5 ) );
                for( size_t wp = 0; wp < workingPointNames.size(); ++wp ){

                    //check that jet passes specified working point for numerator
                    if( ( jet.*workingPointFunctions[wp] )() ){
                        histogram::fillValues( bTagEfficiencyMaps[ 0 ][ flavorIndex ][ wp ].get(), jet.pt(), jet.absEta(), weight );
                    }

                    //denominator
                    histogram::fillValues( bTagEfficiencyMaps[ 1 ][ flavorIndex ][ wp ].get(), jet.pt(), jet.absEta(), weight );
                }
            }
        }
    }

    //output file path
    const std::string outputDirectory = "../weightFiles/bTagEff";
    systemTools::makeDirectory( outputDirectory );

    std::string cleaningName;
    if( cleanJetsFromLooseLeptons ){
        cleaningName = "looseLeptonCleaned";
    } else if( cleanJetsFromFOLeptons ){
        cleaningName = "FOLeptonCleaned";
    } else {
        cleaningName = "uncleaned";
    }
    const std::string fileName = ( "bTagEff_" + cleaningName + "_" + year + ".root" );
    std::string outputPath = stringTools::formatDirectoryName( outputDirectory ) + fileName;
    
    TFile* outputFilePtr = TFile::Open( outputPath.c_str(), "RECREATE" );
    for( std::vector< std::string >::size_type flavor = 0; flavor < quarkFlavors.size(); ++flavor ){
        for( std::vector< std::string >::size_type wp = 0; wp < workingPointNames.size(); ++wp ){
            double globalEff = bTagEfficiencyMaps[ 0 ][ flavor ][ wp ]->Integral() / bTagEfficiencyMaps[ 1 ][ flavor ][ wp ]->Integral();
            std::cout << "global efficiency at " << wp << flavor << ": " << globalEff << std::endl;
            //divide numerator and denominator and write to file
            bTagEfficiencyMaps[ 0 ][ flavor ][ wp ]->Divide( bTagEfficiencyMaps[ 1 ][ flavor ][ wp ].get() );
            bTagEfficiencyMaps[ 0 ][ flavor ][ wp ]->Write( ( "bTagEff_" + workingPointNames[wp] + "_" + quarkFlavors[ flavor ] ).c_str() );
        }
    }
    outputFilePtr->Close();
}


int main(int argc, char* argv[]){

    //convert all input to std::string format for easier handling
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    //either take 3 command line arguments ( sampleDirectory, year, cleaning ) or 1 ( sampleDirectory ) in which case everything will be run
    if( !( argvStr.size() == 4 || argvStr.size() == 2 ) ){
        std::cerr << argc - 1 << " command line arguments given, while 3 or 1 are expected." << std::endl;
        std::cerr << "Usage ( to determine the efficiencies for all years and cleaning options ): ./computeBTagEfficienciesMC sampleDirectory" << std::endl;
        std::cerr << "Or ( to determine the efficiencies for one year and cleaning option ): ./computeBTagEfficienciesMC sampleDirectory year cleaningSchem ( = looseLeptons, FOLeptons, uncleaned )" << std::endl;
        return 1;
    } else if( argvStr.size() == 2 ){
        std::string sampleDirectory = argvStr[1];

		//submit job for each configuration
       	for( const auto & year : { "2016", "2017", "2018" } ){
            for( const auto& cleaning : { "looseLeptons", "FOLeptons", "uncleaned" } ){
	
                std::string command = ( "./computeBTagEfficienciesMC " + sampleDirectory + " " + year + " " + cleaning );
                std::string scriptName = ( std::string( "bTagEff_" ) + year + "_" + cleaning + ".sh" );
                systemTools::submitCommandAsJob( command, scriptName, "169:00:00" );
            }
        }
    } else {
        std::string sampleList = argvStr[1];
        std::string year = argvStr[2];
        std::string cleaningOption = argvStr[3];
        analysisTools::checkYearString( year );
        if( ! ( cleaningOption == "looseLeptons" || cleaningOption == "FOLeptons" || cleaningOption == "uncleaned" ) ){
            throw std::invalid_argument( "cleaningOption should be either 'looseLeptons', 'FOLeptons' or 'uncleaned'." );
        }
        bool cleanJetsFromLooseLeptons = ( cleaningOption == "looseLeptons" );
        bool cleanJetsFromFOLeptons = ( cleaningOption == "FOLeptons" );
        computeBTagEff( year, sampleList, cleanJetsFromLooseLeptons, cleanJetsFromFOLeptons );
    }
	return 0;
}
