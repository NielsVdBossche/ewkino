#include "histogramSetup.h"
#include <math.h>

#if MEMLEAK
#include "../../memleak/debug_new.h" 
#endif

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
        HistInfo( "N_mu_" + flag, "N_#mu", 6, -0.5, 5.5),
        HistInfo( "N_e_" + flag, "N_e", 6, -0.5, 5.5),
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
        HistInfo( "HT_over_NJets_" + flag, "H_{T}/N_{jets} [GeV]", 30, 0, 300),
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
        HistInfo( "m2llAlt_" + flag, "MT2LL", 10, 0, 100),
        HistInfo( "m2bb_" + flag, "MT2BB", 10, 0, 100),
        HistInfo( "m2lblb_" + flag, "MT2BLBL", 10, 0, 100),

        HistInfo( "B-score_BTagLeadJet_" + flag, "DeepFlavor score (j1)", 20 , -1., 1.),
        HistInfo( "B-score_BTagSubLeadJet_" + flag, "DeepFlavor score (j2)", 20 , -1., 1.),
        HistInfo( "B-score_BTagThirdJet_" + flag, "DeepFlavor score (j3)", 20 , -1., 1.),
        HistInfo( "B-score_BTagFourthJet_" + flag, "DeepFlavor score (j4)", 20 , -1., 1.),

        HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 15, 50, 125),
        HistInfo( "InvMassSpectrumLowMassEvents_" + flag, "M_{ll}(low) [GeV]", 10, 5, 15),

        //HistInfo( "LepJetPtRatio_LeadLep_" + flag, "p_{T} ratio (l1,j)", 20, 0, 2),
        //HistInfo( "LepJetPtRatio_SubLeadLep_" + flag, "p_{T} ratio (l2,j)", 20, 0, 2),

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
        //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );

        if (fourLep) {
            histInfoVec->push_back( HistInfo( "leptonPtFour_" + flag, "p_{T}(l4) [GeV]", 20, 0, 200) );
            histInfoVec->push_back( HistInfo( "leptonEtaFour_" + flag, "#eta (l4)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonPhiFour_" + flag, "#phi (l4)", 12, - M_PI, M_PI) );
            histInfoVec->push_back( HistInfo( "leptonEFour_" + flag, "E(l4) [GeV]", 18, 25, 205) );
            histInfoVec->push_back( HistInfo( "leptonMvaTOPFour_" + flag, "score (l4)", 40, -1, 1) );
            //histInfoVec->push_back( HistInfo( "LepJetPtRatio_FourthLep_" + flag, "p_{T} ratio (l4,j)", 20, 0, 2) );
        }
    }

    return histInfoVec;
}

std::vector<HistInfo>* fourTopHists::infoLean(std::string flag, bool multilep) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "N_mu_" + flag, "N_#mu", 6, -0.5, 5.5),
        HistInfo( "N_e_" + flag, "N_e", 6, -0.5, 5.5),
        HistInfo( "leptonPtLeading_" + flag, "p_{T}(l1) [GeV]", 18, 20, 200),
        HistInfo( "leptonPtSecond_" + flag, "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "leptonEtaLeading_" + flag, "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonEtaSecond_" + flag, "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
        HistInfo( "leptonELeading_" + flag, "E(l1) [GeV]", 18, 25, 205),
        HistInfo( "leptonESecond_" + flag, "E(l2) [GeV]", 18, 25, 205),

        HistInfo( "N_B_jets_" + flag, "N_{b}", 10, -0.5, 9.5),
        HistInfo( "N_jets_" + flag, "N_{jets}", 15, -0.5, 14.5),

        HistInfo( "HT_" + flag, "H_{T} [GeV]", 16, 0, 1600),
        HistInfo( "MET_" + flag, "p_{T}^{miss} [GeV]", 20, 0, 500),

        HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
        HistInfo( "Nr_of_leps_" + flag, "N_{l}", 5, 0, 5),

        HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 15, 50, 125),
    };

    if (multilep) {
        histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 20, 0, 200) );
        histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
        histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 18, 25, 205) );
        //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );

    }

    return histInfoVec;
}

std::vector<HistInfo>* fourTopHists::testHists(eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
            {fail, "fail"},
            {crz3L, "CRZ"},
            {crz4L, "CRZ-4L"},
            {cro, "CRO"},
            {cro3L, "CRO-3L"},
            {crw, "CRW"},
            {ssdl, "DL"},
            {trilep, "3L"},
            {fourlep, "4L"}
    };
    std::string flag = flagMapping[evClass];

    *histInfoVec = {
        HistInfo("NLeps_After_TightCharge" + flag, "N_{L}", 6, -0.5, 5.5),
    };

    return histInfoVec;
}
