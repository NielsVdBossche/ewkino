// include classes to test
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Event/interface/Event.h"

int main(){
    // arguments to TreeReader constructor:
    // - sample list (see TreeReader constructor for proper formatting)
    // - folder where to find the files mentioned in the sample list

    // settings
    std::string sampleList = "samplelists/samplelist_2018_TTW_nano.txt";
    std::string sampleDirectory = "samplelists";
    unsigned long nEntries = 100;

    // make the TreeReader for nanoAOD
    std::cout << "initializing TreeReader..." << std::endl;
    TreeReader treeReader(sampleList, sampleDirectory);
    std::cout << "start loop over samples..." << std::endl;
    for( unsigned sampleIndex = 0; sampleIndex < treeReader.numberOfSamples(); ++sampleIndex ){
	std::cout << "current sample number: " << sampleIndex << std::endl;
	// load next sample
	treeReader.initSample();
	// loop over events in sample
	if( treeReader.numberOfEntries() < nEntries ) nEntries = treeReader.numberOfEntries();
	for( long unsigned entry = 0; entry < nEntries; ++entry ){
            
            // build next event
            Event event = treeReader.buildEvent( entry ); 

	    // printouts for testing
	    std::cout << "=== event ===" << std::endl;
	    std::cout << "run number: " << event.runNumber() << std::endl;
	    std::cout << "lumi number: " << event.luminosityBlock() << std::endl;
	    std::cout << "event number: " << event.eventNumber() << std::endl;
	    std::cout << "gen weight: " << event.genWeight() << std::endl;
	    std::cout << "scaled weight: " << event.weight() << std::endl;

	    for( const auto& leptonPtr : event.leptonCollection() ){
		std::cout << *leptonPtr << std::endl;
	    }
	}
    }
    return 0;
}
