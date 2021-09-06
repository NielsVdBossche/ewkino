#include "../plotting/tdrStyle.h"
#include "../Tools/interface/Sample.h"
#include "../Tools/interface/HistInfo.h"
#include "../Tools/interface/histogramTools.h"
#include "../TreeReader/interface/TreeReader.h"

#include "histogramSetups/histogramSetup.h"
#include "_FourTop/interface/FourTop.h"

// Sets up histograms
// Split for different setups...

int main(int argc, char* argv[]) {
    setTDRStyle();

    // Set sample directory... Dunno, see hoe te skimmen...

    // zet makkelijk te verwerken string op
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Settings splitsen if necessary

    // main func call
    FourTop analysisObject(argvStr);

    analysisObject.analyze();
    
}
