#include "../../plotting/tdrStyle.h"
#include "../../Tools/interface/Sample.h"
#include "../../Tools/interface/HistInfo.h"
#include "../../Tools/interface/histogramTools.h"

#include "../../TreeReader/interface/TreeReader.h"

// Sets up histograms
std::vector<HistInfo> getHistInfoVec() {
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "leptonPtLeading", "p_{T}(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtSecond", "p_{T}(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtThird", "p_{T}(l3) [GeV]", 18, 25, 205),
        HistInfo( "N_jets", "N_{jets}", 20, -0.5, 19.5),
        HistInfo( "N_B_jets", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_loose_B", "N_{b}^{loose}", 10, -0.5, 9.5),
        HistInfo( "N_tight_B", "N_{b}^{tight}", 10, -0.5, 9.5),
        HistInfo( "Min_dR_Bs", "Min. #Delta R(b,b)", 12, 0, 4.8),
        HistInfo( "dR_lead_leps", "#Delta R(l1,l2)", 12, 0, 4.8),
        HistInfo( "MET", "p_{T}^{miss} [GeV]", 19, 25, 500),
        HistInfo( "HT", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "massToPt", "Max. m/p_{T}(jet)", 20, 0.1, 0.4),
        HistInfo( "minDR_B_lep", "Min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "secMinDR_B_lep", "Sec. min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "ptJetOne", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour", "p_{T}(j4) [GeV]", 7, 25, 200),
        HistInfo( "ptJetFive", "p_{T}(j5) [GeV]", 6, 0, 150),
        HistInfo( "ptJetSix", "p_{T}(j6) [GeV]", 5, 0, 125)
    };

    return histInfoVec;
}

void analyze() {
    // settings nog toevoegen

    // Treereader instance
    TreeReader treeReader;//( "sampleLists/samples_" + modelName + "_" + year + ".txt", sampleDirectoryPath );

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
