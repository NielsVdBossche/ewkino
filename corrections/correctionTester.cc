#include "interface/JetEnergyCorrections.h"

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
    /* code */
    // test a bit correctionlib in here
    std::string corrFile = "correctionFiles/jet_jerc.json.gz";
    std::string year = "2018";
    JetEnergyCorrections* tester = new JetEnergyCorrections(corrFile, year);
    
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    std::cout << argvStr[1] << std::endl;
    std::cout << tester->TestCorrection(argvStr[1], std::stod(argvStr[2]), std::stod(argvStr[3])) << std::endl;
    return 0;
}
