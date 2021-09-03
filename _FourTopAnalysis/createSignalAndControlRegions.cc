#include "../plotting/tdrStyle.h"
#include "../Tools/interface/Sample.h"
#include "../Tools/interface/HistInfo.h"
#include "../Tools/interface/histogramTools.h"

#include "../TreeReader/interface/TreeReader.h"

// Sets up histograms
std::vector<HistInfo> getHistInfoVec() {
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "leptonPtLeading", "p_{T}^{leading lepton} [GeV]", 18, 25, 205)
    };

    return histInfoVec;
}

void analyze() {
    // settings nog toevoegen

    // Treereader instance
    TreeReader treeReader( "sampleLists/samples_" + modelName + "_" + year + ".txt", sampleDirectoryPath );

    // I guess stuff voor preppen van systematics

    // histograms
    std::vector<HistInfo> histInfoVector = getHistInfoVec();

    std::vector<Sample> sampleVec = treeReader.sampleVector();
    

}

int main(int argc, char* argv[]) {
    setTDRStyle();

    // Set sample directory... Dunno, see hoe te skimmen...

    // zet makkelijk te verwerken string op
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Settings splitsen if necessary

    // main func call

    analyze();
}
