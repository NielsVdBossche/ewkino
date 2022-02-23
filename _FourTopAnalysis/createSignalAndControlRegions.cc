#include "globalSettings.h"

#include "../plotting/tdrStyle.h"
#include "../Tools/interface/Sample.h"
#include "../Tools/interface/HistInfo.h"
#include "../Tools/interface/histogramTools.h"
#include "../TreeReader/interface/TreeReader.h"

#include "histogramSetups/histogramSetup.h"
#include "_FourTop/interface/FourTop.h"

// Sets up histograms
// Split for different setups...

#if MEMLEAK
#include "../memleak/debug_new.h" 
#endif

int main(int argc, char* argv[]) {
    setTDRStyle();

    if (argc < 3) {
        std::cout << "One argument is required: <sampleList.txt> <runType>" << std::endl;
    }
    // Set sample directory

    // zet makkelijk te verwerken string op
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Settings splitsen if necessary

    // main func call
    FourTop analysisObject("AnalysisOutput_" + argvStr[2], argvStr, 1);

    if (argvStr[1] != "allSamples.txt") {
        //std::string newYearString = stringTools::fileNameWithoutExtension(stringTools::splitDirectoryFileName(argvStr[1]).second);
        //std::cout << "not running all samples! Year is taken to be: " << newYearString;
        //analysisObject.setYearString(newYearString);
    }

    analysisObject.analyze(argvStr[2]);
}
