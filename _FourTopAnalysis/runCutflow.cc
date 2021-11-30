#include "globalSettings.h"

#include "../plotting/tdrStyle.h"

#include "_FourTop/interface/FourTop.h"

// Sets up histograms
// Split for different setups...

#if MEMLEAK
#include "../memleak/debug_new.h" 
#endif

int main(int argc, char* argv[]) {
    setTDRStyle();

    if (argc != 2) {
        std::cout << "One argument is required: <sampleList.txt>" << std::endl;
    }
    // zet makkelijk te verwerken string op
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Settings splitsen if necessary

    // main func call
    FourTop analysisObject(argvStr, 2);

    analysisObject.cutFlow();
}
