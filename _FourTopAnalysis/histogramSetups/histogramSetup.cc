#include "histogramSetup.h"
#include <math.h>

#if MEMLEAK
#include "../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* fourTopHists::bdtInfoDL() {
    // Prob need if statements to split which histograms we want
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_DL", "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_DL", "p_{T}(l2) [GeV]", 18, 20, 200),
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

std::vector<HistInfo>* fourTopHists::bdtInfoML() {
    // Prob need if statements to split which histograms we want
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_ML", "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_ML", "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtThird_ML", "p_{T}(l3) [GeV]", 18, 20, 200),
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

std::vector<HistInfo>* fourTopHists::lepInfoDL() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_DL", "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_DL", "p_{T}(l2) [GeV]", 18, 20, 200),
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

std::vector<HistInfo>* fourTopHists::lepInfoML() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_ML", "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_ML", "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtThird_ML", "p_{T}(l2) [GeV]", 18, 20, 200),
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

std::vector<HistInfo>* fourTopHists::jetInfoDL() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "ptJetOne_DL", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetTwo_DL", "p_{T}(j2) [GeV]", 19, 25, 500),
        HistInfo( "ptJetThree_DL", "p_{T}(j3) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_DL", "p_{T}(j4) [GeV]", 19, 25, 500),
        HistInfo( "N_B_jets_DL", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "HT_DL", "H_{T} [GeV]", 13, 300, 1600)
    };

    return histInfoVec;

}

std::vector<HistInfo>* fourTopHists::jetInfoML() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "ptJetOne_ML", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetTwo_ML", "p_{T}(j2) [GeV]", 19, 25, 500),
        HistInfo( "ptJetThree_ML", "p_{T}(j3) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_ML", "p_{T}(j4) [GeV]", 19, 25, 500),
        HistInfo( "N_B_jets_ML", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "HT_ML", "H_{T} [GeV]", 13, 300, 1600)
    };

    return histInfoVec;

}

std::vector<HistInfo>* fourTopHists::crzInfo() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "CRZ_N_jets", "N_{jets}", 10, -0.5, 9.5),
        HistInfo( "CRZ_N_b", "N_{b}", 5, -0.5, 4.5),
        HistInfo( "CRZ_HT", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "CRZ_MET", "p_{T}^{miss} [GeV]", 19, 25, 500),
        HistInfo( "CRZ_ptLeadingLep", "p_{T}(l1) [GeV]", 14, 20, 300),
        HistInfo( "CRZ_ptLepTwo", "p_{T}(l2) [GeV]", 7, 20, 160),
        HistInfo( "CRZ_ptLepThree", "p_{T}(l3) [GeV]", 8, 20, 100)
    };

    return histInfoVec;

}

std::vector<HistInfo>* fourTopHists::crwInfo() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "CRW_N_jets", "N_{jets}", 10, -0.5, 9.5),
        HistInfo( "CRW_N_b", "N_{b}", 5, -0.5, 4.5),
        HistInfo( "CRW_HT", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "CRW_MET", "p_{T}^{miss} [GeV]", 19, 25, 500),
        HistInfo( "CRW_ptLeadingLep", "p_{T}(l1) [GeV]", 14, 20, 300),
        HistInfo( "CRW_ptLepTwo", "p_{T}(l2) [GeV]", 7, 20, 160)
    };

    return histInfoVec;

}

std::vector<HistInfo>* fourTopHists::allHistsDL() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_DL", "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_DL", "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "leptonEtaLeading_DL", "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonEtaSecond_DL", "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonPhiLeading_DL", "#phi (l1)", 12, - M_PI, M_PI),
        HistInfo( "leptonPhiSecond_DL", "#phi (l2)", 12, - M_PI, M_PI),
        HistInfo( "leptonELeading_DL", "E(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonESecond_DL", "E(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonMvaTOPLeading_DL", "score", 40, -1, 1),
        HistInfo( "leptonMvaTOPSecond_DL", "score", 40, -1, 1),

        HistInfo( "ptJetOne_DL", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetTwo_DL", "p_{T}(j2) [GeV]", 19, 25, 500),
        HistInfo( "ptJetThree_DL", "p_{T}(j3) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_DL", "p_{T}(j4) [GeV]", 19, 25, 500),
        HistInfo( "N_B_jets_DL", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_looseB_jets_DL", "N_{b}^{loose}", 10, -0.5, 9.5),
        HistInfo( "N_tightB_jets_DL", "N_{b}^{tight}", 10, -0.5, 9.5),
        HistInfo( "N_jets_DL", "N_{jets}", 15, -0.5, 14.5),

        HistInfo( "HT_DL", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "MET_DL", "p_{T}^{miss} [GeV]", 19, 25, 500),

        HistInfo( "Min_dR_Bs_DL", "Min. #Delta R(b,b)", 12, 0, 4.8),
        //HistInfo( "SecMin_dR_Bs_DL", "Sec. min. #Delta R(b,b)", 12, 0, 4.8),

        HistInfo( "minDR_B_lep_DL", "Min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "secMinDR_B_lep_DL", "Sec. min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "LT_DL", "L_{T} [GeV]", 20, 0, 500),

        HistInfo("MissingHitsElsLead", "N_{miss}(l1)", 4, -1.5, 2.5),
        HistInfo("MissingHitsElsSec", "N_{miss}(l2)", 4, -1.5, 2.5)


    };

    return histInfoVec;
}

std::vector<HistInfo>* fourTopHists::allHistsML() {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_ML", "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_ML", "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtThird_ML", "p_{T}(l2) [GeV]", 18, 20, 200),
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
        HistInfo( "leptonMvaTOPThird_ML", "score", 40, -1, 1),

        HistInfo( "ptJetOne_ML", "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetTwo_ML", "p_{T}(j2) [GeV]", 19, 25, 500),
        HistInfo( "ptJetThree_ML", "p_{T}(j3) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_ML", "p_{T}(j4) [GeV]", 19, 25, 500),
        HistInfo( "N_B_jets_ML", "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_looseB_jets_ML", "N_{b}^{loose}", 10, -0.5, 9.5),
        HistInfo( "N_tightB_jets_ML", "N_{b}^{tight}", 10, -0.5, 9.5),
        HistInfo( "N_jets_ML", "N_{jets}", 15, -0.5, 14.5),


        HistInfo( "HT_ML", "H_{T} [GeV]", 13, 300, 1600),
        HistInfo( "MET_ML", "p_{T}^{miss} [GeV]", 19, 25, 500),

        HistInfo( "Min_dR_Bs_ML", "Min. #Delta R(b,b)", 12, 0, 4.8),
        //HistInfo( "SecMin_dR_Bs_ML", "Sec. min. #Delta R(b,b)", 12, 0, 4.8),

        HistInfo( "minDR_B_lep_ML", "Min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "secMinDR_B_lep_ML", "Sec. min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "LT_ML", "L_{T} [GeV]", 20, 0, 500)
    };

    return histInfoVec;
}

std::vector<HistInfo>* fourTopHists::allHists(std::string flag, bool multilep, bool fourLep) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_" + flag, "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_" + flag, "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "leptonEtaLeading_" + flag, "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonEtaSecond_" + flag, "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonPhiLeading_" + flag, "#phi (l1)", 12, - M_PI, M_PI),
        HistInfo( "leptonPhiSecond_" + flag, "#phi (l2)", 12, - M_PI, M_PI),
        HistInfo( "leptonELeading_" + flag, "E(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonESecond_" + flag, "E(l2) [GeV]", 18, 25, 205),
        HistInfo( "leptonMvaTOPLeading_" + flag, "score (l1)", 40, -1, 1),
        HistInfo( "leptonMvaTOPSecond_" + flag, "score (l2)", 40, -1, 1),

        HistInfo( "ptJetOne_" + flag, "p_{T}(j1) [GeV]", 19, 25, 500),
        HistInfo( "ptJetTwo_" + flag, "p_{T}(j2) [GeV]", 19, 25, 500),
        HistInfo( "ptJetThree_" + flag, "p_{T}(j3) [GeV]", 19, 25, 500),
        HistInfo( "ptJetFour_" + flag, "p_{T}(j4) [GeV]", 20, 0, 500),
        HistInfo( "N_B_jets_" + flag, "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_looseB_jets_" + flag, "N_{b}^{loose}", 10, -0.5, 9.5),
        HistInfo( "N_tightB_jets_" + flag, "N_{b}^{tight}", 10, -0.5, 9.5),
        HistInfo( "N_jets_" + flag, "N_{jets}", 15, -0.5, 14.5),

        HistInfo( "HT_" + flag, "H_{T} [GeV]", 16, 0, 1600),
        HistInfo( "MET_" + flag, "p_{T}^{miss} [GeV]", 20, 0, 500),

        HistInfo( "Min_dR_Bs_" + flag, "Min. #Delta R(b,b)", 12, 0, 4.8),
        //HistInfo( "SecMin_dR_Bs_" + flag, "Sec. min. #Delta R(b,b)", 12, 0, 4.8),

        HistInfo( "minDR_B_lep_" + flag, "Min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "secMinDR_B_lep_" + flag, "Sec. min. #Delta R(l,b)", 12, 0, 4.8),
        HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
        HistInfo( "Nr_of_leps_" + flag, "N_{l}", 5, 0, 5),

        HistInfo( "B-score_PtLeadJet_" + flag, "DeepFlavor score (j1)", 20 , -1., 1.),
        HistInfo( "B-score_PtSubLeadJet_" + flag, "DeepFlavor score (j2)", 20 , -1., 1.),
        HistInfo( "B-score_PtThirdJet_" + flag, "DeepFlavor score (j3)", 20 , -1., 1.),
        HistInfo( "B-score_PtFourthJet_" + flag, "DeepFlavor score (j4)", 20 , -1., 1.),
        
        HistInfo( "mtLeadLepMET_" + flag, "M_{T}(W) [GeV]", 20, 0, 200),
        HistInfo( "mtSubLeadLepMET_" + flag, "M_{T}(W) [GeV]", 20, 0, 200),

        //HistInfo( "m2ll_" + flag, "MT", 10, 0, 100),
        HistInfo( "m2llAlt_" + flag, "MT", 10, 0, 100),

        HistInfo( "B-score_BTagLeadJet_" + flag, "DeepFlavor score (j1)", 20 , -1., 1.),
        HistInfo( "B-score_BTagSubLeadJet_" + flag, "DeepFlavor score (j2)", 20 , -1., 1.),
        HistInfo( "B-score_BTagThirdJet_" + flag, "DeepFlavor score (j3)", 20 , -1., 1.),
        HistInfo( "B-score_BTagFourthJet_" + flag, "DeepFlavor score (j4)", 20 , -1., 1.),

        //HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 15, 50, 125),
        //HistInfo( "InvMassSpectrumLowMassEvents_" + flag, "M_{ll}(low) [GeV]", 10, 5, 15),

        HistInfo( "LepJetPtRatio_LeadLep_" + flag, "p_{T} ratio (l1,j)", 20, 0, 2),
        HistInfo( "LepJetPtRatio_SubLeadLep_" + flag, "p_{T} ratio (l2,j)", 20, 0, 2),

        HistInfo( "BestTopMass_" + flag, "Best M_{top} [GeV]", 20, 100, 300),
        HistInfo( "BestTopCorrWMass_" + flag, "M_{W} [GeV]", 10, 50, 150),

        HistInfo( "SecondBestTopMass_" + flag, "Sec. best M_{top} [GeV]", 20, 100, 300),
        HistInfo( "SecondBestTopCorrWMass_" + flag, "M_{W} [GeV]", 10, 50, 150),
    };

    if (multilep) {
        histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 20, 0, 200) );
        histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
        histInfoVec->push_back( HistInfo( "leptonPhiThird_" + flag, "#phi (l3)", 12, - M_PI, M_PI) );
        histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 18, 25, 205) );
        histInfoVec->push_back( HistInfo( "leptonMvaTOPThird_" + flag, "score (l3)", 40, -1, 1) );
        histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );

        if (fourLep) {
            histInfoVec->push_back( HistInfo( "leptonPtFour_" + flag, "p_{T}(l4) [GeV]", 20, 0, 200) );
            histInfoVec->push_back( HistInfo( "leptonEtaFour_" + flag, "#eta (l4)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonPhiFour_" + flag, "#phi (l4)", 12, - M_PI, M_PI) );
            histInfoVec->push_back( HistInfo( "leptonEFour_" + flag, "E(l4) [GeV]", 18, 25, 205) );
            histInfoVec->push_back( HistInfo( "leptonMvaTOPFour_" + flag, "score (l4)", 40, -1, 1) );
            histInfoVec->push_back( HistInfo( "LepJetPtRatio_FourthLep_" + flag, "p_{T} ratio (l4,j)", 20, 0, 2) );
        }
    }

    return histInfoVec;
}