#include "histogramSetup.h"
#include <math.h>

std::vector<HistInfo> fourTopHists::bdtInfoDL() {
    // Prob need if statements to split which histograms we want
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "leptonPtLeading_DL", "p_{T}(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtSecond_DL", "p_{T}(l2) [GeV]", 18, 25, 205),
        HistInfo( "N_jets_DL", "N_{jets}", 20, -0.5, 19.5),
        HistInfo( "N_B_jets_DL", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_loose_B_DL", "N_{b}^{loose}", 10, -0.5, 9.5),
        HistInfo( "N_tight_B_DL", "N_{b}^{tight}", 10, -0.5, 9.5),
        HistInfo( "Min_dR_Bs_DL", "Min. #Delta R(b,b)", 12, 0, 4.8),
        HistInfo( "dR_lead_leps_DL", "#Delta R(l1,l2)", 12, 0, 4.8),
        HistInfo( "MET_DL", "p_{T}^{miss} [GeV]", 19, 25, 500),
        HistInfo( "HT_DL", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "massToPt_DL", "Max. m/p_{T}(jet)", 20, 0.1, 0.4),
        HistInfo( "minDR_B_lep_DL", "Min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "secMinDR_B_lep_DL", "Sec. min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "ptJetOne_DL", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_DL", "p_{T}(j4) [GeV]", 7, 25, 200),
        HistInfo( "ptJetFive_DL", "p_{T}(j5) [GeV]", 6, 0, 150),
        HistInfo( "ptJetSix_DL", "p_{T}(j6) [GeV]", 5, 0, 125)
    };

    return histInfoVec;
}

std::vector<HistInfo> fourTopHists::bdtInfoML() {
    // Prob need if statements to split which histograms we want
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "leptonPtLeading_ML", "p_{T}(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtSecond_ML", "p_{T}(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtThird_ML", "p_{T}(l3) [GeV]", 18, 25, 205),
        HistInfo( "N_jets_ML", "N_{jets}", 20, -0.5, 19.5),
        HistInfo( "N_B_jets_ML", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_loose_B_ML", "N_{b}^{loose}", 10, -0.5, 9.5),
        HistInfo( "N_tight_B_ML", "N_{b}^{tight}", 10, -0.5, 9.5),
        HistInfo( "Min_dR_Bs_ML", "Min. #Delta R(b,b)", 12, 0, 4.8),
        HistInfo( "dR_lead_leps_ML", "#Delta R(l1,l2)", 12, 0, 4.8),
        HistInfo( "MET_ML", "p_{T}^{miss} [GeV]", 19, 25, 500),
        HistInfo( "HT_ML", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "massToPt_ML", "Max. m/p_{T}(jet)", 20, 0.1, 0.4),
        HistInfo( "minDR_B_lep_ML", "Min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "secMinDR_B_lep_ML", "Sec. min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "ptJetOne_ML", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_ML", "p_{T}(j4) [GeV]", 7, 25, 200),
        HistInfo( "ptJetFive_ML", "p_{T}(j5) [GeV]", 6, 0, 150),
        HistInfo( "ptJetSix_ML", "p_{T}(j6) [GeV]", 5, 0, 125)
    };

    return histInfoVec;
}

std::vector<HistInfo> fourTopHists::lepInfoDL() {
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "leptonPtLeading_DL", "p_{T}(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtSecond_DL", "p_{T}(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonEtaLeading_DL", "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonEtaSecond_DL", "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonPhiLeading_DL", "#phi (l1)", 12, - M_PI, M_PI),
        HistInfo( "leptonPhiSecond_DL", "#phi (l2)", 12, - M_PI, M_PI),
        HistInfo( "leptonELeading_DL", "E(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonESecond_DL", "E(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonMvaTOPLeading_DL", "score", 40, -1, 1),
        HistInfo( "leptonMvaTOPSecond_DL", "score", 40, -1, 1),
        
    };

    return histInfoVec;
}

std::vector<HistInfo> fourTopHists::lepInfoML() {
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "leptonPtLeading_ML", "p_{T}(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtSecond_ML", "p_{T}(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonPtThird_ML", "p_{T}(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonEtaLeading_ML", "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonEtaSecond_ML", "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonEtaThird_ML", "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonPhiLeading_ML", "#phi (l1)", 12, - M_PI, M_PI),
        HistInfo( "leptonPhiSecond_ML", "#phi (l2)", 12, - M_PI, M_PI),
        HistInfo( "leptonPhiThird_ML", "#phi (l2)", 12, - M_PI, M_PI),
        HistInfo( "leptonELeading_ML", "E(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonESecond_ML", "E(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonEThird_ML", "E(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonMvaTOPLeading_ML", "score", 40, -1, 1),
        HistInfo( "leptonMvaTOPSecond_ML", "score", 40, -1, 1),
        HistInfo( "leptonMvaTOPThird_ML", "score", 40, -1, 1)

    };

    return histInfoVec;
}

std::vector<HistInfo> fourTopHists::jetInfoDL() {
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "ptJetOne_DL", "p_{T}(j1) [GeV]", 19, 25, 500)
    };

    return histInfoVec;

}

std::vector<HistInfo> fourTopHists::jetInfoML() {
    std::vector< HistInfo > histInfoVec;
    
    histInfoVec = {
        HistInfo( "ptJetOne_DL", "p_{T}(j1) [GeV]", 19, 25, 500)
    };

    return histInfoVec;

}
