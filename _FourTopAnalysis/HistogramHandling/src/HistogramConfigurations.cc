#include "../interface/HistogramConfigurations.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* HistogramConfig::getHistInfo(eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
            {fail, "fail"},
            {crwz, "CRWZ"},
            {cr_conv, "CR-Conversion"},
            {crz3L, "CR-3L-Z"},
            {crz4L, "CR-4L-Z"},
            {cro, "CR-2L-23J1B"},
            {cro3L, "CR-3L-2J1B"},
            {crw, "CR-2L-45J2B"},
            {ssdl, "SR-2L"},
            {trilep, "SR-3L"},
            {fourlep, "SR-4L"}
    };

    std::string flag = flagMapping[evClass];

    if (unsigned(evClass) >= eventClass::ssdl) {
        *histInfoVec = {
            HistInfo( "Yield_" + flag, "yield", 1, 0., 1.),
            HistInfo( "N_mu_" + flag, "N_#mu", 5, -0.5, 4.5),
            HistInfo( "N_e_" + flag, "N_e", 5, -0.5, 4.5),
            HistInfo( "leptonPtLeading_" + flag, "p_{T}(l1) [GeV]", 28, 20, 300),
            HistInfo( "leptonPtSecond_" + flag, "p_{T}(l2) [GeV]", 13, 20, 150),
            HistInfo( "leptonEtaLeading_" + flag, "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
            HistInfo( "leptonEtaSecond_" + flag, "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
            HistInfo( "leptonPhiLeading_" + flag, "#phi (l1)", 12, - M_PI, M_PI),
            HistInfo( "leptonPhiSecond_" + flag, "#phi (l2)", 12, - M_PI, M_PI),
            HistInfo( "leptonELeading_" + flag, "E(l1) [GeV]", 28, 20, 300),
            HistInfo( "leptonESecond_" + flag, "E(l2) [GeV]", 23, 20, 250),
            HistInfo( "leptonMvaTOPLeading_" + flag, "score (l1)", 40, -1, 1),
            HistInfo( "leptonMvaTOPSecond_" + flag, "score (l2)", 40, -1, 1),

            HistInfo( "ptJetOne_" + flag, "p_{T}(j1) [GeV]", 19, 25, 500),
            HistInfo( "ptJetTwo_" + flag, "p_{T}(j2) [GeV]", 11, 25, 300),
            HistInfo( "ptJetThree_" + flag, "p_{T}(j3) [GeV]", 20, 0, 200),
            HistInfo( "ptJetFour_" + flag, "p_{T}(j4) [GeV]", 15, 0, 150),
            HistInfo( "N_B_jets_" + flag, "N_{b}", 6, 0.5, 6.5),
            HistInfo( "N_mediumB_jets_" + flag, "N_{b}^{medium}", 6, -0.5, 5.5),
            HistInfo( "N_looseB_jets_" + flag, "N_{b}^{loose}", 6, 0.5, 6.5),
            HistInfo( "N_tightB_jets_" + flag, "N_{b}^{tight}", 4, -0.5, 3.5),
            HistInfo( "N_jets_" + flag, "N_{jets}", 9, 1.5, 10.5),

            HistInfo( "HT_" + flag, "H_{T} [GeV]", 26, 0, 1300),
            HistInfo( "HT_over_NJets_" + flag, "H_{T}/N_{jets} [GeV]", 14, 25, 200),
            HistInfo( "MET_" + flag, "p_{T}^{miss} [GeV]", 30, 0, 300),

            HistInfo( "Min_dR_Bs_" + flag, "Min. #Delta R(b,b)", 24, 0, 4.8),
            //HistInfo( "SecMin_dR_Bs_" + flag, "Sec. min. #Delta R(b,b)", 12, 0, 4.8),

            HistInfo( "minDR_B_lep_" + flag, "Min. #Delta R(l,b)", 24, 0, 4.8),
            HistInfo( "secMinDR_B_lep_" + flag, "Sec. min. #Delta R(l,b)", 24, 0, 4.8),
            HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
            HistInfo( "Nr_of_leps_" + flag, "N_{l}", 6, -0.5, 5.5),

            HistInfo( "B-score_PtLeadJet_" + flag, "DeepFlavor score (j1)", 26, -0.04, 1.),
            HistInfo( "B-score_PtSubLeadJet_" + flag, "DeepFlavor score (j2)", 26, -0.04, 1.),
            HistInfo( "B-score_PtThirdJet_" + flag, "DeepFlavor score (j3)", 26, -0.04, 1.),
            HistInfo( "B-score_PtFourthJet_" + flag, "DeepFlavor score (j4)", 26, -0.04, 1.),
            
            HistInfo( "mtLeadLepMET_" + flag, "M_{T}(W) [GeV]", 13, 40, 300),
            HistInfo( "mtSubLeadLepMET_" + flag, "M_{T}(W) [GeV]", 13, 40, 300),

            //HistInfo( "m2ll_" + flag, "MT", 100, 0, 100),
            HistInfo( "m2ll_" + flag, "MT2LL", 15, 0, 150),
            HistInfo( "m2bb_" + flag, "MT2BB", 12, 80, 200),
            HistInfo( "m2lblb_" + flag, "MT2BLBL", 15, 0, 150),

            HistInfo( "B-score_BTagLeadJet_" + flag, "DeepFlavor score (j1)", 26, -0.04, 1.),
            HistInfo( "B-score_BTagSubLeadJet_" + flag, "DeepFlavor score (j2)", 26, -0.04, 1.),
            HistInfo( "B-score_BTagThirdJet_" + flag, "DeepFlavor score (j3)", 26, -0.04, 1.),
            HistInfo( "B-score_BTagFourthJet_" + flag, "DeepFlavor score (j4)", 26, -0.04, 1.),

            HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120),
            HistInfo( "InvMassSpectrumLowMassEvents_" + flag, "M_{ll}(low) [GeV]", 50, 5, 15),

            //HistInfo( "LepJetPtRatio_LeadLep_" + flag, "p_{T} ratio (l1,j)", 100, 0, 2),
            //HistInfo( "LepJetPtRatio_SubLeadLep_" + flag, "p_{T} ratio (l2,j)", 100, 0, 2),

            HistInfo( "BestTopMass_" + flag, "Best M_{top} [GeV]", 10, 120, 220),
            HistInfo( "BestTopCorrWMass_" + flag, "M_{W} [GeV]", 8, 60, 100),

            HistInfo( "SecondBestTopMass_" + flag, "Sec. best M_{top} [GeV]", 16, 140, 220),
            HistInfo( "SecondBestTopCorrWMass_" + flag, "M_{W} [GeV]", 8, 60, 100),
        };

        if (evClass == eventClass::trilep || evClass == eventClass::fourlep) {
        histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 11, 10, 120) );
        histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
        histInfoVec->push_back( HistInfo( "leptonPhiThird_" + flag, "#phi (l3)", 12, - M_PI, M_PI) );
        histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 18, 25, 205) );
        histInfoVec->push_back( HistInfo( "leptonMvaTOPThird_" + flag, "score (l3)", 40, -1, 1) );
        histInfoVec->push_back(HistInfo( "InvMass3L_" + flag, "M_{3l} [GeV]", 18, 76, 150));

        //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );

            if (evClass == eventClass::fourlep) {
                histInfoVec->push_back( HistInfo( "leptonPtFour_" + flag, "p_{T}(l4) [GeV]", 18, 10, 100) );
                histInfoVec->push_back( HistInfo( "leptonEtaFour_" + flag, "#eta (l4)", 12, -2.4, 2.4) );
                histInfoVec->push_back( HistInfo( "leptonPhiFour_" + flag, "#phi (l4)", 12, - M_PI, M_PI) );
                histInfoVec->push_back( HistInfo( "leptonEFour_" + flag, "E(l4) [GeV]", 15, 0, 150) );
                histInfoVec->push_back( HistInfo( "leptonMvaTOPFour_" + flag, "score (l4)", 40, -1, 1) );
                //histInfoVec->push_back( HistInfo( "LepJetPtRatio_FourthLep_" + flag, "p_{T} ratio (l4,j)", 20, 0, 2) );
            }
        }
    } else {
        *histInfoVec = {
            HistInfo( "Yield_" + flag, "yield", 1, 0., 1.),
            HistInfo( "N_mu_" + flag, "N_#mu", 6, -0.5, 5.5),
            HistInfo( "N_e_" + flag, "N_e", 6, -0.5, 5.5),            
            HistInfo( "leptonPtLeading_" + flag, "p_{T}(l1) [GeV]", 15, 0, 300),
            HistInfo( "leptonPtSecond_" + flag, "p_{T}(l2) [GeV]", 18, 20, 200),
            HistInfo( "leptonEtaLeading_" + flag, "#eta (l1)", 12, -2.4, 2.4), // Might switch to det segmentation
            HistInfo( "leptonEtaSecond_" + flag, "#eta (l2)", 12, -2.4, 2.4), // Might switch to det segmentation
            HistInfo( "leptonELeading_" + flag, "E(l1) [GeV]", 30, 0, 300),
            HistInfo( "leptonESecond_" + flag, "E(l2) [GeV]", 25, 0, 250),

            HistInfo( "N_LooseB_jets_" + flag, "N_{b}", 6, -0.5, 5.5),
            HistInfo( "N_MediumB_jets_" + flag, "N_{b}", 6, -0.5, 5.5),
            HistInfo( "N_TightB_jets_" + flag, "N_{b}", 6, -0.5, 5.5),
            HistInfo( "N_jets_" + flag, "N_{jets}", 7, -0.5, 6.5),

            HistInfo( "HT_" + flag, "H_{T} [GeV]", 16, 0, 800),
            HistInfo( "MET_" + flag, "p_{T}^{miss} [GeV]", 15, 0, 300),

            HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
            HistInfo( "Nr_of_leps_" + flag, "N_{l}", 6, -0.5, 5.5),

            HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120),

        };

        if (evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv) {
            histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 15, 0, 150) );
            histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 22, 0, 220) );
            histInfoVec->push_back(HistInfo( "InvMass3L_" + flag, "M_{3l} [GeV]", 36, 78, 150));
            //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );
        }

        if (evClass == eventClass::crz4L) {
            histInfoVec->push_back( HistInfo("inv_mass_other_leppair", "Mll [GeV]", 20, 0, 200));
        }
        
        //histInfoVec->push_back(HistInfo( "PileupWeight_" + flag, "PUWeight", 100, 0.5, 1.5));
        //histInfoVec->push_back(HistInfo( "electronID_" + flag, "elIDWeight", 100, 0.5, 1.5));
        //histInfoVec->push_back(HistInfo( "muonID_" + flag, "muIDWeight", 100, 0.5, 1.5));
        //histInfoVec->push_back(HistInfo( "electronReco_pTBelow20_" + flag, "electronReco_pTBelow20", 100, 0.5, 1.5));
        //histInfoVec->push_back(HistInfo( "electronReco_pTAbove20_" + flag, "electronReco_pTAbove20", 100, 0.5, 1.5));
        //histInfoVec->push_back(HistInfo( "bTag_shape_" + flag, "bTag_shape", 100, 0.5, 1.5));
        //histInfoVec->push_back(HistInfo( "prefire_" + flag, "prefire", 100, 0.5, 1.5));


    }
    return histInfoVec;
}
