#include "../interface/HistogramConfigurations.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* HistogramConfig::getHistInfo(eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
    {fail, "fail"},
    {crz, "CRZ"},
    {cro, "CRO"},
    {crw, "CRW"},
    {ssdl, "DL"},
    {trilep, "3L"},
    {fourlep, "4L"}};

    std::string flag = flagMapping[evClass];

    if (unsigned(evClass) >= eventClass::ssdl) {
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

        if (evClass == eventClass::trilep || evClass == eventClass::fourlep) {
        histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 20, 0, 200) );
        histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
        histInfoVec->push_back( HistInfo( "leptonPhiThird_" + flag, "#phi (l3)", 12, - M_PI, M_PI) );
        histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 18, 25, 205) );
        histInfoVec->push_back( HistInfo( "leptonMvaTOPThird_" + flag, "score (l3)", 40, -1, 1) );
        //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );

        if (evClass == eventClass::fourlep) {
            histInfoVec->push_back( HistInfo( "leptonPtFour_" + flag, "p_{T}(l4) [GeV]", 20, 0, 200) );
            histInfoVec->push_back( HistInfo( "leptonEtaFour_" + flag, "#eta (l4)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonPhiFour_" + flag, "#phi (l4)", 12, - M_PI, M_PI) );
            histInfoVec->push_back( HistInfo( "leptonEFour_" + flag, "E(l4) [GeV]", 18, 25, 205) );
            histInfoVec->push_back( HistInfo( "leptonMvaTOPFour_" + flag, "score (l4)", 40, -1, 1) );
            //histInfoVec->push_back( HistInfo( "LepJetPtRatio_FourthLep_" + flag, "p_{T} ratio (l4,j)", 20, 0, 2) );
        }
    }
    } else {
        *histInfoVec = {
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

        if (evClass == eventClass::crz) {
            histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 20, 0, 200) );
            histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 18, 25, 205) );
            //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );

        }
    }
    return histInfoVec;
}
