// include classes to test
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Event/interface/Event.h"

int main(){
    // arguments to TreeReader constructor:
    // - sample list (see TreeReader constructor for proper formatting)
    // - folder where to find the files mentioned in the sample list

    // settings
    std::string sampleList = "samplelists/samplelist_2018_TTW_nano.txt";
    std::string sampleDirectory = "/pnfs/iihe/cms/store/user/llambrec/nanoaod/sync/";
    // alternative: file
    std::string sampleFile = "/pnfs/iihe/cms/store/user/llambrec/nanoaod/TTWJetsToLNu-RunIISummer20UL18-nanoAOD-fullfile.root";
    unsigned long nEntries = 5000;

    // make the TreeReader for nanoAOD
    std::cout << "initializing TreeReader..." << std::endl;
    //TreeReader treeReader(sampleList, sampleDirectory);
    TreeReader treeReader;
    treeReader.initSampleFromFile(sampleFile);
    /*std::cout << "start loop over samples..." << std::endl;
    for( unsigned sampleIndex = 0; sampleIndex < treeReader.numberOfSamples(); ++sampleIndex ){
	std::cout << "current sample number: " << sampleIndex << std::endl;
	// load next sample
	treeReader.initSample();*/
    if(2>1){
	// loop over events in sample
	if( treeReader.numberOfEntries() < nEntries ) nEntries = treeReader.numberOfEntries();
	std::cout << "start loop over " << nEntries << " events..." << std::endl;
	for( long unsigned entry = 0; entry < nEntries; ++entry ){
            
            // build next event
            Event event = treeReader.buildEvent( entry ); 

	    if( !(event.eventNumber()==13386086 || event.eventNumber()==13372790 || event.eventNumber()==13375211 ) ) continue;
            
	    std::cout << event.numberOfElectrons() << std::endl;
	    for( const auto& leptonPtr : event.electronCollection() ){
                std::cout << *leptonPtr << std::endl;
            }

	    // cleaning and/or selections
	    event.selectLooseLeptons();
            event.cleanElectronsFromLooseMuons();
            event.cleanTausFromLooseLightLeptons();
            event.removeTaus();

	    /*bool doprint = false;
	    for( const auto& leptonPtr : event.electronCollection() ){
                if( !leptonPtr->isPrompt() ) doprint = true;
            }
	    if( !doprint ) continue;*/

	    // printouts for testing
	    std::cout << "=== event ===" << std::endl;
	    //std::cout << "run number: " << event.runNumber() << std::endl;
	    //std::cout << "lumi number: " << event.luminosityBlock() << std::endl;
	    std::cout << "event number: " << event.eventNumber() << std::endl;
	    
	    //std::cout << "gen weight: " << event.genWeight() << std::endl;
	    //std::cout << "scaled weight: " << event.weight() << std::endl;

	    /*for( const auto& leptonPtr : event.leptonCollection() ){
		std::cout << *leptonPtr << std::endl;
	    }*/

	    for( const auto& leptonPtr : event.lightLeptonCollection() ){
		std::cout << leptonPtr->isPrompt() << " " << leptonPtr->leptonMVAttH() << std::endl;
	    }

	    /*for( const auto& jetPtr : event.jetCollection() ){
                std::cout << *jetPtr << std::endl;
            }*/

	    /*std::cout << "pass e: " << event.passTriggers_e() << std::endl;
	    std::cout << "pass m: " << event.passTriggers_m() << std::endl;
	    std::cout << "pass ee: " << event.passTriggers_ee() << std::endl;
	    std::cout << "pass em: " << event.passTriggers_em() << std::endl;
	    std::cout << "pass mm: " << event.passTriggers_mm() << std::endl;
	    std::cout << "pass eee: " << event.passTriggers_eee() << std::endl;
	    std::cout << "pass eem: " << event.passTriggers_eem() << std::endl;
	    std::cout << "pass emm: " << event.passTriggers_emm() << std::endl;
	    std::cout << "pass mmm: " << event.passTriggers_mmm() << std::endl;
	    std::cout << "pass MET: " << event.passMetFilters() << std::endl;*/

	}
    }
    return 0;
}
