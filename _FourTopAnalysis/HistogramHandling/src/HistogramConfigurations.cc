#include "../interface/HistogramConfigurations.h"
#include "../../FourTopEventHandling/interface/EventFourT.h"
#include "../../FourTopEventHandling/interface/MVAHandler.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* HistogramConfig::getNominalHists(const eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
            {fail, "fail"},
            {dy, "DY"},
            {ttbar, "TTBar"},
            {crwz, "CRWZ"},
            {crzz, "CRZZ"},
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
            HistInfo( "N_mu_" + flag, "N_{#mu}", 5, -0.5, 4.5),
            HistInfo( "N_e_" + flag, "N_{e}", 5, -0.5, 4.5),
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

            HistInfo( "Min_dR_Bs_" + flag, "min #Delta R(b,b)", 24, 0, 4.8),
            //HistInfo( "SecMin_dR_Bs_" + flag, "Sec. min #Delta R(b,b)", 12, 0, 4.8),

            HistInfo( "minDR_B_lep_" + flag, "min #Delta R(l,b)", 24, 0, 4.8),
            HistInfo( "secMinDR_B_lep_" + flag, "min_{2} #Delta R(l,b)", 24, 0, 4.8),
            HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
            HistInfo( "Nr_of_leps_" + flag, "N_{l}", 6, -0.5, 5.5),

            HistInfo( "B-score_PtLeadJet_" + flag, "DF(j_{1})", 26, -0.04, 1.),
            HistInfo( "B-score_PtSubLeadJet_" + flag, "DF(j_{2})", 26, -0.04, 1.),
            HistInfo( "B-score_PtThirdJet_" + flag, "DF(j_{3})", 26, -0.04, 1.),
            HistInfo( "B-score_PtFourthJet_" + flag, "DF(j_{4})", 26, -0.04, 1.),
            
            HistInfo( "mtLeadLepMET_" + flag, "m_{T}(l_{1},#vec{p}_{T}^{miss}) [GeV]", 13, 40, 300),
            HistInfo( "mtSubLeadLepMET_" + flag, "m_{T}(l_{2}},#vec{p}_{T}^{miss}) [GeV]", 13, 40, 300),

            //HistInfo( "m2ll_" + flag, "MT", 100, 0, 100),
            HistInfo( "m2ll_" + flag, "m_{T2}(l1,l2) [GeV]", 15, 0, 150),
            HistInfo( "m2bb_" + flag, "m_{T2}(b1,b2) [GeV]", 12, 80, 200),
            HistInfo( "m2lblb_" + flag, "m_{T2}(lb,lb) [GeV]", 15, 0, 150),

            HistInfo( "DR_Leps_" + flag, "#Delta R(l1,l2)", 24, 0, 4.8),
            HistInfo( "dPhi_Leps_" + flag, "#Phi (l1,l2)", 12, - M_PI, M_PI),
            HistInfo( "ptJetFive_" + flag, "p_{T}(j5) [GeV]", 15, 0, 150),
            HistInfo( "ptJetSix_" + flag, "p_{T}(j6) [GeV]", 15, 0, 150),
            HistInfo( "MToPT_" + flag, "#frac{M}{p_{T}}", 15, 0, 150),

            HistInfo( "B-score_BTagLeadJet_" + flag, "max_{1} DF", 26, -0.04, 1.),
            HistInfo( "B-score_BTagSubLeadJet_" + flag, "max_{2} DF", 26, -0.04, 1.),
            HistInfo( "B-score_BTagThirdJet_" + flag, "max_{3} DF", 26, -0.04, 1.),
            HistInfo( "B-score_BTagFourthJet_" + flag, "max_{4} DF", 26, -0.04, 1.),

            HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120),
            HistInfo( "InvMassSpectrumLowMassEvents_" + flag, "M_{ll}(low) [GeV]", 50, 5, 15),

            //HistInfo( "LepJetPtRatio_LeadLep_" + flag, "p_{T} ratio (l1,j)", 100, 0, 2),
            //HistInfo( "LepJetPtRatio_SubLeadLep_" + flag, "p_{T} ratio (l2,j)", 100, 0, 2),

            HistInfo( "BestTopMass_" + flag, "m(t_{1}) [GeV]", 10, 120, 220),
            HistInfo( "BestTopCorrWMass_" + flag, "m(W_{1}) [GeV]", 8, 60, 100),

            HistInfo( "SecondBestTopMass_" + flag, "m(t_{2}) [GeV]", 16, 140, 220),
            HistInfo( "SecondBestTopCorrWMass_" + flag, "m(W_{2}) [GeV]", 8, 60, 100),
        };

        if (evClass == eventClass::trilep || evClass == eventClass::fourlep) {
            histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 11, 10, 120) );
            histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonPhiThird_" + flag, "#phi (l3)", 12, - M_PI, M_PI) );
            histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 18, 25, 205) );
            histInfoVec->push_back( HistInfo( "leptonMvaTOPThird_" + flag, "score (l3)", 40, -1, 1) );
            histInfoVec->push_back( HistInfo( "InvMass3L_" + flag, "M_{3l} [GeV]", 18, 76, 150));

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
            HistInfo( "N_mu_" + flag, "N_{#mu}", 6, -0.5, 5.5),
            HistInfo( "N_e_" + flag, "N_{e}", 6, -0.5, 5.5),            
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
            
            HistInfo( "Min_dR_Bs_" + flag, "min #Delta R(b,b)", 24, 0, 4.8),
            HistInfo( "minDR_B_lep_" + flag, "min #Delta R(l,b)", 24, 0, 4.8),
            HistInfo( "secMinDR_B_lep_" + flag, "min_{2} #Delta R(l,b)", 24, 0, 4.8),

            HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120),
            HistInfo( "InvMassSpectrumSSEvents_" + flag, "M_{ll}(SS) [GeV]", 30, 60, 120),

        };

        if (evClass == eventClass::cro) {
            histInfoVec->push_back(HistInfo( "EventFailsCut_" + flag, "", 7, 0.5, 7.5));
        } else if (evClass == eventClass::cro3L) {
            histInfoVec->push_back(HistInfo( "EventFailsCut_" + flag, "", 3, 0.5, 3.5));
        }

        if (evClass != eventClass::crwz && evClass != eventClass::cr_conv) {
            histInfoVec->push_back(HistInfo( "m2ll_" + flag, "MT2LL", 15, 0, 150));
            histInfoVec->push_back(HistInfo( "m2bb_" + flag, "MT2BB", 12, 80, 200));
            histInfoVec->push_back(HistInfo( "m2lblb_" + flag, "MT2BLBL", 15, 0, 150));
        }
        
        if (evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv || evClass == eventClass::crzz) {
            histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 15, 0, 150) );
            histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 12, -2.4, 2.4) );
            histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 22, 0, 220) );
            histInfoVec->push_back(HistInfo( "InvMass3L_" + flag, "M_{3l} [GeV]", 36, 78, 150));
            //histInfoVec->push_back( HistInfo( "LepJetPtRatio_ThirdLep_" + flag, "p_{T} ratio (l3,j)", 20, 0, 2) );
        }

        if (evClass == eventClass::crz4L || evClass == eventClass::crzz) {
            histInfoVec->push_back( HistInfo("inv_mass_other_leppair_" + flag, "Mll [GeV]", 20, 0, 200));
        }

        if (evClass == eventClass::crwz) {
            histInfoVec->push_back( HistInfo( "mtLepNotInZMET_" + flag, "M_{T}(W) [GeV]", 16, 40, 120));
        }
    }
    return histInfoVec;
}

std::vector<HistInfo>* HistogramConfig::getMinimalHists(const eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
        {fail, "fail"},
        {dy, "DY"},
        {ttbar, "TTBar"},
        {crwz, "CRWZ"},
        {crzz, "CRZZ"},
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

    std::pair<double, double> minMaxNjets = {-0.5, 6.5};
    std::pair<double, double> minMaxNBjets = {-0.5, 4.5};
    std::pair<double, double> minMaxHT = {0, 900};
    std::pair<double, double> minMaxNMu = {-0.5, 5.5};
    if (evClass == eventClass::crw) {
        minMaxNjets = {3.5,5.5};
        minMaxNBjets = {1.5,2.5};
        minMaxHT = {200,1100};
        minMaxNMu = {-0.5, 2.5};
    } else if (evClass == eventClass::cro) {
        minMaxNjets = {1.5,6.5};
        minMaxNBjets = {0.5,4.5};
        minMaxHT = {250,1150};
        minMaxNMu = {-0.5, 2.5};
    } else if (evClass == eventClass::crz3L) {
        minMaxNjets = {1.5, 8.5};
        minMaxNBjets = {0.5, 6.5};
        minMaxHT = {200, 1100};
        minMaxNMu = {-0.5, 3.5};
    } else if (evClass == eventClass::cro3L) {
        minMaxNjets = {1.5, 6.5};
        minMaxNBjets = {0.5, 3.5};
        minMaxHT = {200, 1100};
        minMaxNMu = {-0.5, 3.5};
    } else if (evClass == eventClass::crz4L) {
        minMaxNjets = {1.5, 8.5};
        minMaxNBjets = {0.5, 4.5};
        minMaxHT = {0, 900};
        minMaxNMu = {-0.5, 4.5};
    } else if (evClass == eventClass::ttbar) {
        minMaxNjets = {2.5, 10.5};
        minMaxNBjets = {1.5, 5.5};
    } else if (evClass == eventClass::dy) {
        minMaxNjets = {1.5, 10.5};
        minMaxNBjets = {0.5, 5.5};
    } else if (evClass == eventClass::crzz) {
        minMaxNjets = {-0.5, 4.5};
    } else if (evClass == eventClass::ssdl || evClass == eventClass::trilep) {
        minMaxNjets = {2.5, 10.5};
        minMaxNBjets = {1.5, 6.5};
        minMaxHT = {0, 1400};
    } else if (evClass == eventClass::fourlep) {
        minMaxNjets = {1.5, 6.5};
        minMaxNBjets = {0.5, 4.5};    
    }

    *histInfoVec = {
        HistInfo( "Yield_" + flag, "yield", 1, 0., 1.),
        HistInfo( "leptonPtLeading_" + flag, "p_{T}(l1) [GeV]", 15, 0, 300),
        HistInfo( "leptonPtSecond_" + flag, "p_{T}(l2) [GeV]", 18, 20, 200),
        HistInfo( "N_jets_" + flag, "N_{jets}", int(minMaxNjets.second -  minMaxNjets.first), minMaxNjets.first, minMaxNjets.second),
        HistInfo( "N_Bjets_" + flag, "N_{b}", int(minMaxNBjets.second -  minMaxNBjets.first), minMaxNBjets.first, minMaxNBjets.second),
        HistInfo( "N_MediumBjets_" + flag, "N_{b} (medium WP)", 6, -0.5, 5.5),
        HistInfo( "N_TightBjets_" + flag, "N_{b} (medium WP)", 6, -0.5, 5.5),
        HistInfo( "HT_" + flag, "H_{T} [GeV]", int(minMaxHT.second - minMaxHT.first) / 50, minMaxHT.first, minMaxHT.second),
        HistInfo( "MET_" + flag, "p_{T}^{miss} [GeV]", 16, 0, 400),
        HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
        HistInfo( "Nr_of_leps_" + flag, "N_{l}", 6, -0.5, 5.5),        
        HistInfo( "N_mu_" + flag, "N_{#mu}", int(minMaxNMu.second -  minMaxNMu.first), minMaxNMu.first, minMaxNMu.second),
        //HistInfo( "PLBees_" + flag, "N_{b}", 7, -0.5, 6.5)
    };

    if (evClass == eventClass::trilep || evClass == eventClass::fourlep || evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv || evClass == eventClass::crzz) {
        histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 11, 10, 120) );
        histInfoVec->push_back( HistInfo( "InvMassSpectrumOSEvents_" + flag, "M_{ll} [GeV]", 30, 60, 120));
        if (evClass == eventClass::cr_conv) histInfoVec->push_back( HistInfo( "InvMassSpectrumtrilep_" + flag, "M_{lll} [GeV]", 30, 60, 120));
        if (evClass == eventClass::crz4L || evClass == eventClass::crzz || evClass == eventClass::fourlep) {
            histInfoVec->push_back( HistInfo( "leptonPtFour_" + flag, "p_{T}(l4) [GeV]", 18, 10, 100) );
        }
    }

    if (evClass < eventClass::ssdl) {
        histInfoVec->push_back(HistInfo("TriClass_Fit_" + flag, "Sum of charges", 2, -0.5, 1.5));
    }

    if (evClass == eventClass::dy || evClass == eventClass::ttbar) {
        histInfoVec->push_back(HistInfo( "InvMassSpectrumLowMassEvents_" + flag, "M_{ll} [GeV]", 20, 50, 150));
        histInfoVec->push_back(HistInfo( "N_MediumB_jets_" + flag, "N_{b}", 7, 0.5, 7.5));
        histInfoVec->push_back(HistInfo( "deltaR_HighPT_MedB_" + flag, "#Delta R(b1,b2)", 24, 0, 4.8));
        histInfoVec->push_back(HistInfo( "min_deltaR_MedB_" + flag, "#Delta R(b1,b2)", 24, 0, 4.8));
        histInfoVec->push_back(HistInfo( "deltaR_HighPT_Jets_" + flag, "#Delta R(j1,j2)", 24, 0, 4.8));
        histInfoVec->push_back(HistInfo( "min_deltaR_Jets_" + flag, "#Delta R(j1,j2)", 24, 0, 4.8));
        histInfoVec->push_back(HistInfo( "NjetNbjet_" + flag, "", 28, -0.5, 27.5));
    }

    return histInfoVec;
}

std::vector<HistInfo>* HistogramConfig::getAllBDTVarsHists(const eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
        {fail, "fail"},
        {dy, "DY"},
        {ttbar, "TTBar"},
        {crwz, "CRWZ"},
        {crzz, "CRZZ"},
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

    // only bdt variables here

    if (unsigned(evClass) >= eventClass::ssdl || unsigned(evClass) >= eventClass::crwz) {
        std::pair<double, double> minMaxNjets = {-0.5, 6.5};
        std::pair<double, double> minMaxNBjets = {-0.5, 4.5};
        std::pair<double, double> minMaxHT = {0, 800};
        std::pair<double, double> minMaxNMu = {-0.5, 5.5};
        if (evClass == eventClass::ssdl) {
            minMaxNjets = {3.5,10.5};
            minMaxNBjets = {1.5,6.5};
            minMaxHT = {250.,1300.};
            //minMaxNMu = {-0.5, 2.5};
        } else if (evClass == eventClass::trilep) {
            minMaxNjets = {2.5, 9.5};
            minMaxNBjets = {1.5,6.5};
            minMaxHT = {200.,1250.};

        } else if (evClass == eventClass::fourlep) {
            minMaxNjets = {1.5, 6.5};
            minMaxNBjets = {0.5,6.5};
            minMaxHT = {0.,1050.};
        }

        *histInfoVec = {
            HistInfo("Yield_"+flag, "Yield", 1, 0., 1.),
            HistInfo("NJets_"+flag, "N_{jets}", int(minMaxNjets.second -  minMaxNjets.first), minMaxNjets.first, minMaxNjets.second),
            HistInfo("NB_Loose_"+flag, "N_{b}^{loose}", int(minMaxNBjets.second -  minMaxNBjets.first), minMaxNBjets.first, minMaxNBjets.second),
            HistInfo("NB_Medium_"+flag, "N_{b}^{medium}", 7, -0.5, 6.5),
            HistInfo("NB_Tight_"+flag, "N_{b}^{tight}", 7, -0.5, 6.5),

            HistInfo("minDR_bb_"+flag, "min #Delta R(b,b)", 12, 0, 4.8),
            HistInfo("DR_l1l2_"+flag, "#Delta R(l1,l2)", 48, 0, 4.8),
            HistInfo("DPhi_l1l2_"+flag, "#Delta#phi (l1,l2)", 48, 0., M_PI),
            HistInfo("DEta_l1l2_"+flag, "#Delta#eta (l1,l2)", 48, 0., 4.8),
            HistInfo("HT_"+flag, "H_{T} [GeV]", 21, minMaxHT.first, minMaxHT.second),
            HistInfo("PT_miss_"+flag, "p_{T}^{miss} [GeV]", 30, 0., 300.),
            HistInfo("max_M_over_PT_jet_"+flag, "#frac{M}{p_{T}}", 30, 0.1, 0.4),
            HistInfo("minDR_lb_"+flag, "min_{1} #Delta R(l,b)", 12, 0, 4.8),
            HistInfo("sec_minDR_lb_"+flag, "min_{2} #Delta R(l,b)", 12, 0, 4.8),

            HistInfo("pt_j1_"+flag, "p_{T}(j1) [GeV]", 19, 25, 500),
            HistInfo("pt_j2_"+flag, "p_{T}(j2) [GeV]", 11, 25, 300),
            HistInfo("pt_j3_"+flag, "p_{T}(j3) [GeV]", 20, 0, 200),
            HistInfo("pt_j4_"+flag, "p_{T}(j4) [GeV]", 15, 0, 150),
            HistInfo("pt_j5_"+flag, "p_{T}(j5) [GeV]", 15, 0, 150),
            HistInfo("pt_j6_"+flag, "p_{T}(j6) [GeV]", 15, 0, 150),
            HistInfo("pt_l1_"+flag, "p_{T}(l1) [GeV]", 28, 20, 300),
            HistInfo("pt_l2_"+flag, "p_{T}(l2) [GeV]", 13, 20, 150),

            HistInfo("max_DF_1_"+flag, "max_{1} DF", 25, 0., 1.),
            HistInfo("max_DF_2_"+flag, "max_{2} DF", 25, 0., 1.),
            HistInfo("max_DF_3_"+flag, "max_{3} DF", 25, 0., 1.),
            HistInfo("max_DF_4_"+flag, "max_{4} DF", 25, 0., 1.),
            HistInfo("DF_J1_"+flag, "DF(j_{1})", 25, 0., 1.),
            HistInfo("DF_J2_"+flag, "DF(j_{2})", 25, 0., 1.),
            HistInfo("DF_J3_"+flag, "DF(j_{3})", 25, 0., 1.),
            HistInfo("DF_J4_"+flag, "DF(j_{4})", 25, 0., 1.),

            HistInfo("mtop_1_"+flag, "m(t_{1}) [GeV]", 10, 120, 220),
            HistInfo("mW_1_"+flag, "m(W_{1}) [GeV]", 8, 60, 100),
            HistInfo("mtop_2_"+flag, "m(t_{2}) [GeV]", 16, 140, 220),
            HistInfo("mW_2_"+flag, "m(W_{2}) [GeV]", 8, 60, 100),

            HistInfo("MT_l1_"+flag, "m_{T}(l_{1}) [GeV]", 13, 40, 300),
            HistInfo("MT_l2_"+flag, "m_{T}(l_{2}) [GeV]", 13, 40, 300),
            HistInfo("MT2_ll_"+flag, "m_{T2}(l) [GeV]", 15, 0, 150),
            HistInfo("MT2_bb_"+flag, "m_{T2}(b) [GeV]", 12, 80, 200),
            HistInfo("MT2_lblb_"+flag, "m_{T2}(l+b) [GeV]", 15, 0, 150),

            HistInfo( "InvMassSpectruml1l2_" + flag, "M_{ll} [GeV]", 50, 0., 150.),

            HistInfo("LooseLeps_"+flag, "nL", 7, -0.5, 6.5)
        };
        if (evClass == eventClass::trilep || evClass == eventClass::fourlep || evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv || evClass == eventClass::crzz) {
            histInfoVec->push_back( HistInfo("pt_l3_"+flag, "p_{T}(l3) [GeV]", 11, 10, 120) );
            histInfoVec->push_back( HistInfo( "InvMassSpectruml2l3_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120));
            histInfoVec->push_back( HistInfo( "InvMassSpectruml1l3_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120));
            histInfoVec->push_back( HistInfo( "InvMassSpectrumZ_" + flag, "M_{ll}(OS) [GeV]", 30, 60, 120));
            histInfoVec->push_back( HistInfo( "InvMass3L_" + flag, "M_{3l} [GeV]", 18, 76, 150));

            histInfoVec->push_back( HistInfo("min_DR_ll_"+flag, "Min. #Delta R(l,l)", 48, 0, 4.8) );
            histInfoVec->push_back( HistInfo("min_DPhi_l1l2_"+flag, "Min. #Delta#phi (l,l)", 48, 0., M_PI) );
            histInfoVec->push_back( HistInfo("min_DEta_l1l2_"+flag, "Min. #Delta#eta (l,l)", 48, 0., 4.8) );

        }
    } else {
        *histInfoVec = {
            HistInfo("Yield_"+flag, "Yield", 1, 0., 1.)
        };
    }

    return histInfoVec;
}


std::vector<double> HistogramConfig::fillMinimalHists(const eventClass evClass, EventFourT* event) {
    std::vector<double> fillVal = {
        0.5,
        event->getLepton(0)->pt(),
        event->getLepton(1)->pt(),
        double(event->numberOfJets()),
        double(event->numberOfLooseBJets()),
        double(event->numberOfMediumBJets()),
        double(event->numberOfTightBJets()),
        event->getHT(),
        event->getMET(),
        event->getMediumLepCol()->scalarPtSum(),
        double(event->numberOfLeps()),
        double(event->getMediumLepCol()->numberOfMuons()),
        //double(event->getEvent()->GetPLInfoPtr()->GetParticleLevelBees()),
    };

    if (evClass == eventClass::trilep || evClass == eventClass::fourlep || evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv || evClass == eventClass::crzz) {
        fillVal.push_back(event->getLepton(2)->pt());
        if (event->getEvent()->hasOSSFLightLeptonPair()) {
            fillVal.push_back(event->getEvent()->bestZBosonCandidateMass());
        } else {
            fillVal.push_back(0.);
        }
        if (evClass == eventClass::cr_conv) {
            double triMass = 0.;

            Lepton* l1 = event->getLepton(0);
            Lepton* l2 = event->getLepton(1);
            Lepton* l3 = event->getLepton(2);

            triMass = (*l1 + *l2 + *l3).mass();
            fillVal.push_back(triMass);
        }

        if (evClass == eventClass::crz4L || evClass == eventClass::crzz || evClass == eventClass::fourlep) {
            fillVal.push_back(event->getLepton(3)->pt());
        }
    }

    if (evClass < eventClass::ssdl) {
        if (event->getMediumLepCol()->sumCharges() >=0) fillVal.push_back(0.);
        else fillVal.push_back(1.);
    } 


    if (evClass == eventClass::dy || evClass == eventClass::ttbar) {
        Lepton* l1 = event->getLepton(0);
        Lepton* l2 = event->getLepton(1);
        double osLepMass = (*l1 + *l2).mass();

        fillVal.push_back(osLepMass);
        fillVal.push_back(event->numberOfMediumBJets());

        if (event->numberOfMediumBJets() >= 2) {
            JetCollection bJets = event->getJetCol()->mediumBTagCollection();
            std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(bJets);
            fillVal.push_back(deltaR(bJets[0], bJets[1]));
            fillVal.push_back(mindR_Bjets[0]);
        } else {
            fillVal.push_back(5.);
            fillVal.push_back(5.);
        }

        if (event->numberOfJets() >= 2) {
            JetCollection jets = *event->getJetCol();
            std::vector<double> mindR_jets = calculators::mindRInJetCollection(jets);
            fillVal.push_back(deltaR(jets[0], jets[1]));
            fillVal.push_back(mindR_jets[0]);
        } else {
            fillVal.push_back(5.);
            fillVal.push_back(5.);
        }

        int nbs = (event->numberOfMediumBJets() > 5 ? 5 : event->numberOfMediumBJets());
        int njs = (event->numberOfJets() > 10 ? 10 : event->numberOfJets());
        if (nbs < 2) nbs = 2;
        if (njs < 3) njs = 3;

        int lastPlotFillVal = (nbs - 2) * 8 + (njs - 3);
        fillVal.push_back(lastPlotFillVal);
    }

    return fillVal;
}

std::vector<double> HistogramConfig::fillNominalHists(const eventClass evClass, EventFourT* event) {
    if (evClass >= eventClass::ssdl) {
        MVAHandler_4T* mva;
        if (event->numberOfLeps() > 2) {
            mva = event->GetMLMVA();
        } else {
            mva = event->GetDLMVA();
        }

        JetCollection* jets = event->getJetCol();
        JetCollection* bJets = event->getBtagJetCol();
        LightLeptonCollection* lightLeps;

        lightLeps = (LightLeptonCollection*) event->getMediumLepCol();

        jets->sortByPt();

        std::vector<double> fillVal = {
            0.5,
            double(event->getMediumLepCol()->numberOfMuons()),
            double(event->getMediumLepCol()->numberOfElectrons()),
            (*lightLeps)[0].pt(),
            (*lightLeps)[1].pt(),
            (*lightLeps)[0].eta(),
            (*lightLeps)[1].eta(),
            (*lightLeps)[0].phi(),
            (*lightLeps)[1].phi(),
            (*lightLeps)[0].energy(),
            (*lightLeps)[1].energy(),
            (*lightLeps)[0].leptonMVATOP(),
            (*lightLeps)[1].leptonMVATOP(),

            (jets->size() > 0 ? (*jets)[0].pt() : 0.),
            (jets->size() > 1 ? (*jets)[1].pt() : 0.),
            (jets->size() > 2 ? (*jets)[2].pt() : 0.),
            (jets->size() > 3 ? (*jets)[3].pt() : 0.),
            double(bJets->size()),
            double(event->numberOfMediumBJets()),
            double(event->numberOfLooseBJets()),
            double(event->numberOfTightBJets()),
            double(jets->size()),

            jets->scalarPtSum(),
            jets->scalarPtSum() / double(jets->size()),
            event->getEvent()->metPt(),

            // Calculate DR? What is best way...
            mva->deltaRBjets, //(nb >= 2 ? mindR_Bjets[0] : 5.),

            mva->min_dr_lep_b, // (nb > 0 ? mindR_Bjet_lep[0] : 5.),
            mva->sec_min_dr_lep_b, // (nb > 0 ? mindR_Bjet_lep[1] : 5.),

            lightLeps->scalarPtSum(), // LT
            double(lightLeps->size()),

            (jets->size() > 0 ? jets->at(0)->deepFlavor() : -1.),
            (jets->size() > 1 ? jets->at(1)->deepFlavor() : -1.),
            (jets->size() > 2 ? jets->at(2)->deepFlavor() : -1.),
            (jets->size() > 3 ? jets->at(3)->deepFlavor() : -1.),

            mva->mtLeadLepMET, // mt(*lightLeps->at(0), event->getEvent()->met()),
            mva->mtSubLeadLepMET, // mt(*lightLeps->at(1), event->getEvent()->met()),

            //mt2::mt2(*lightLeps->at(0), *lightLeps->at(1), event->getEvent()->met()),
            mva->m2ll, //mt2::mt2Alt(*lightLeps->at(0), *lightLeps->at(1), event->getEvent()->met()),
            mva->m2bb, //(nb >= 2 ? mt2::mt2bb((*bJets)[0], (*bJets)[1], (*lightLeps)[0], (*lightLeps)[1], event->getEvent()->met()) : -1),
            mva->m2lblb, //(nb >= 2 ? mt2::mt2lblb((*bJets)[0], (*bJets)[1], (*lightLeps)[0], (*lightLeps)[1], event->getEvent()->met()) : -1),

            mva->dRleps,
            mva->aziAngle,
            mva->ptJetFive,
            mva->ptJetSix,
            mva->massToPt
        };

        jets->sortByAttribute([](const std::shared_ptr< Jet >& lhs, const std::shared_ptr< Jet >& rhs){ return lhs->deepFlavor() > rhs->deepFlavor(); } );

        fillVal.push_back(jets->size() > 0 ? jets->at(0)->deepFlavor() : -1.);
        fillVal.push_back(jets->size() > 1 ? jets->at(1)->deepFlavor() : -1.);
        fillVal.push_back(jets->size() > 2 ? jets->at(2)->deepFlavor() : -1.);
        fillVal.push_back(jets->size() > 3 ? jets->at(3)->deepFlavor() : -1.);

        
        if (event->getEvent()->hasOSSFLeptonPair()) {
            double mass = event->getEvent()->bestZBosonCandidateMass();
            fillVal.push_back(mass);
        } else {
            fillVal.push_back(0.);

        }

        unsigned currentSize = fillVal.size();
        for( const auto& leptonPtrPair : lightLeps->pairCollection() ){
            //veto SF pairs of low mass
            Lepton& lepton1 = *( leptonPtrPair.first );
            Lepton& lepton2 = *( leptonPtrPair.second );
            if(! sameFlavor( lepton1, lepton2 )){
                continue;
            }
            if(( lepton1 + lepton2 ).mass() < 15.){
                fillVal.push_back((lepton1 + lepton2).mass());
                break;
            }
        }

        if (currentSize == fillVal.size()) {
            fillVal.push_back(16.);
        }
        

        //TopReconstructionNew* topReco = event->getTopReco();
    //
        //topReco->RecoBestTwoTops();
        fillVal.push_back(mva->massBestTop);   //topReco->getBestRecoTop().first);
        fillVal.push_back(mva->massBestTopW);   //topReco->getBestRecoTop().second);
        fillVal.push_back(mva->massSecTop);   //topReco->getSecondBestRecoTop().first);
        fillVal.push_back(mva->massSecTopW);   //topReco->getSecondBestRecoTop().second);


        if (event->getCurrentClass() == eventClass::trilep || event->getCurrentClass() == eventClass::fourlep) {
            fillVal.push_back((*lightLeps)[2].pt());
            fillVal.push_back((*lightLeps)[2].eta());
            fillVal.push_back((*lightLeps)[2].phi());
            fillVal.push_back((*lightLeps)[2].energy());
            fillVal.push_back((*lightLeps)[2].leptonMVATOP());

            double triMass = 0.;

            Lepton* l1 = event->getLepton(0);
            Lepton* l2 = event->getLepton(1);
            Lepton* l3 = event->getLepton(2);

            triMass = (*l1 + *l2 + *l3).mass();
            fillVal.push_back(triMass);

            if (event->getCurrentClass() == eventClass::fourlep) {
                fillVal.push_back((*lightLeps)[3].pt());
                fillVal.push_back((*lightLeps)[3].eta());
                fillVal.push_back((*lightLeps)[3].phi());
                fillVal.push_back((*lightLeps)[3].energy());
                fillVal.push_back((*lightLeps)[3].leptonMVATOP());
            }
        }

        return fillVal;
    } else {
        JetCollection* jets = event->getJetCol();
        JetCollection* bJets = event->getBtagJetCol();
        LeptonCollection* lightLeps;
        lightLeps = event->getMediumLepCol();
        
        std::vector<double> mindR_Bjets = calculators::mindRInJetCollection(*bJets);
        std::vector<double> mindR_Bjet_lep = calculators::mindRLepAndJet(*bJets, *lightLeps);

        //int nb = bJets->size();
        int nlep = lightLeps->size();

        std::vector<double> fillVal = {
            0.5,
            double(event->getMediumLepCol()->numberOfMuons()),
            double(event->getMediumLepCol()->numberOfElectrons()),
            (*lightLeps)[0].pt(),
            (*lightLeps)[1].pt(),
            (*lightLeps)[0].eta(),
            (*lightLeps)[1].eta(),
            (*lightLeps)[0].energy(),
            (*lightLeps)[1].energy(),

            double(bJets->size()),
            double(event->numberOfMediumBJets()),
            double(event->numberOfTightBJets()),
            double(jets->size()),

            jets->scalarPtSum(),
            event->getEvent()->metPt(),

            lightLeps->scalarPtSum(), // LT
            double(lightLeps->size()),

                    // Calculate DR? What is best way...
            (bJets->size() >= 2. ? mindR_Bjets[0] : 5.),

            mindR_Bjet_lep.size() > 0 ? mindR_Bjet_lep[0] : 5.,
            mindR_Bjet_lep.size() > 0 ? mindR_Bjet_lep[1] : 5.
        };
        
        if (event->getEvent()->hasOSSFLeptonPair()) {
            double mass = event->getEvent()->bestZBosonCandidateMass();
            fillVal.push_back(mass);
        } else {
            fillVal.push_back(0.);
        }

        Lepton* l1 = event->getLepton(0);
        Lepton* l2 = event->getLepton(1);

        double diMass = (*l1 + *l2).mass();
        fillVal.push_back(diMass);

        if (evClass == eventClass::cro) {
            int count = 0;
            if (event->numberOfJets() < 4) {
                count += 1;
            }
            if (event->numberOfLooseBJets() < 2) {
                count += 2;
            }
            if (event->getHT() < 280) {
                count += 4;
            }
            fillVal.push_back(count);
        } else if (evClass == eventClass::cro3L) {
            int count = 0;
            if (event->numberOfJets() < 3) {
                count += 1;
            }
            if (event->numberOfLooseBJets() < 2) {
                count += 2;
            }
            fillVal.push_back(count);
        }

        if (evClass != eventClass::crwz && evClass != eventClass::cr_conv) {
            MVAHandler_4T* mva;
            if (event->numberOfLeps() > 2) {
                mva = event->GetMLMVA();
            } else {
                mva = event->GetDLMVA();
            }
            fillVal.push_back(mva->m2ll);
            fillVal.push_back(mva->m2bb);
            fillVal.push_back(mva->m2lblb);
        }

        if (evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv || evClass == eventClass::crzz) {
            fillVal.push_back(nlep >= 3? (*lightLeps)[2].pt() : 0.);
            fillVal.push_back(nlep >= 3? (*lightLeps)[2].eta() : 0.);
            fillVal.push_back(nlep >= 3? (*lightLeps)[2].energy() : 0.);

            double triMass = 0.;

            Lepton* l1 = event->getLepton(0);
            Lepton* l2 = event->getLepton(1);
            Lepton* l3 = event->getLepton(2);

            triMass = (*l1 + *l2 + *l3).mass();
            fillVal.push_back(triMass);

            if (evClass == eventClass::crz4L || evClass == eventClass::crzz) {
                std::pair<std::size_t, std::size_t> indices = event->getMediumLepCol()->bestZBosonCandidateIndices();
                std::vector<size_t> relIndices;
                for (size_t i=0; i<4; i++) {
                    if (i == indices.first || i == indices.second) continue;
                    relIndices.push_back(i);
                }
                if (relIndices.size() > 2) fillVal.push_back(0.);
                else {
                    Lepton* l1New = event->getLepton(relIndices[0]);
                    Lepton* l2New = event->getLepton(relIndices[1]);
                    double twoMass = (*l1New + *l2New).mass();
                    fillVal.push_back(twoMass);
                }
            }

            if (evClass == eventClass::crwz) {
                std::pair<std::size_t, std::size_t> indices = event->getMediumLepCol()->bestZBosonCandidateIndices();
                std::size_t otherIndex = 0;
                while ((otherIndex == indices.first || otherIndex == indices.second) && otherIndex < 3) {
                    otherIndex++;
                }
                Lepton* otherLepton = event->getLepton(otherIndex);
                double mtValue = mt(*otherLepton, event->GetFullMET());
                fillVal.push_back(mtValue);
            }
        }
        
        return fillVal;
    }
}

std::vector<double> HistogramConfig::fillAllBDTVarsHists(const eventClass evClass, EventFourT* event) {
    std::vector<double> fillVal;
    if (unsigned(evClass) >= eventClass::ssdl || unsigned(evClass) >= eventClass::crwz) {
        MVAHandler_4T* mva;
        mva = event->GetDLMVA();
        if (evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass > eventClass::ssdl) mva = event->GetMLMVA();;

        fillVal = {
            0.5,
            double(event->numberOfJets()),
            double(event->numberOfLooseBJets()),
            double(event->numberOfMediumBJets()),
            double(event->numberOfTightBJets()),

            mva->deltaRBjets,
            mva->dRleps,
            mva->aziAngle,
            deltaEta(*event->getLepton(0), *event->getLepton(1)),
            event->getHT(),
            event->getMET(),
            mva->massToPt,
            mva->min_dr_lep_b,
            mva->sec_min_dr_lep_b,

            (event->numberOfJets() >= 1 ? event->getJet(0)->pt() : 0.),
            (event->numberOfJets() >= 2 ? event->getJet(1)->pt() : 0.),
            (event->numberOfJets() >= 3 ? event->getJet(2)->pt() : 0.),
            (event->numberOfJets() >= 4 ? event->getJet(3)->pt() : 0.),
            (event->numberOfJets() >= 5 ? event->getJet(4)->pt() : 0.),
            (event->numberOfJets() >= 6 ? event->getJet(5)->pt() : 0.),

            event->getLepton(0)->pt(),
            event->getLepton(1)->pt(),

            mva->bTagLead,
            mva->bTagSub,
            mva->bTagThird,
            mva->bTagFourth,
            mva->bTagPtLead,
            mva->bTagPtSub,
            mva->bTagPtThird,
            mva->bTagPtFourth,

            mva->massBestTop,
            mva->massBestTopW,
            mva->massSecTop,
            mva->massSecTopW,

            mva->mtLeadLepMET,
            mva->mtSubLeadLepMET,
            mva->m2ll,
            mva->m2bb,
            mva->m2lblb,

            (*event->getLepton(0)+*event->getLepton(1)).mass(),

            double(event->getLooseLepCol()->size() - event->getMediumLepCol()->size())
        };
        if (evClass == eventClass::trilep || evClass == eventClass::fourlep || evClass == eventClass::crz3L || evClass == eventClass::crz4L || evClass == eventClass::cro3L || evClass == eventClass::crwz || evClass == eventClass::cr_conv || evClass == eventClass::crzz) {
            fillVal.push_back(event->getLepton(2)->pt());
            fillVal.push_back((*event->getLepton(1)+*event->getLepton(2)).mass());
            fillVal.push_back((*event->getLepton(0)+*event->getLepton(2)).mass());
            if (event->getEvent()->hasOSSFLightLeptonPair()) fillVal.push_back(event->getMediumLepCol()->bestZBosonCandidateMass());
            fillVal.push_back((*event->getLepton(0)+*event->getLepton(1)+*event->getLepton(2)).mass());

            std::vector<double> minDRLeps = calculators::mindRLeptons(*event->getMediumLepCol());
            std::vector<double> minDEtaLeps = calculators::mindEtaLeptons(*event->getMediumLepCol());
            std::vector<double> minDPhiLeps = calculators::mindPhiLeptons(*event->getMediumLepCol());
            fillVal.push_back(minDRLeps[0]);
            fillVal.push_back(minDEtaLeps[0]);
            fillVal.push_back(minDPhiLeps[0]);
        }   

    } else {
        fillVal = {
            0.5,
        };
    }
    

    return fillVal;
}

std::vector<HistInfo> HistogramConfig::createNpNmHistograms(const eventClass evClass) {
    std::vector< HistInfo > histInfoVec;

    if (evClass != eventClass::ssdl && evClass != eventClass::crw && evClass != eventClass::cro) return histInfoVec;
    std::map<eventClass, std::string> flagMapping = {
            {fail, "fail"},
            {dy, "DY"},
            {ttbar, "TTBar"},
            {crwz, "CRWZ"},
            {crzz, "CRZZ"},
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

    histInfoVec = {
        HistInfo( "NpNm_Yield_"+flag, "Yield", 1, 0., 1.),
        HistInfo( "NpNm_N_mediumB_jets_" + flag, "N_{b}^{medium}", 6, -0.5, 5.5),
        HistInfo( "NpNm_N_looseB_jets_" + flag, "N_{b}^{loose}", 6, 0.5, 6.5),
        HistInfo( "NpNm_N_tightB_jets_" + flag, "N_{b}^{tight}", 4, -0.5, 3.5),
        HistInfo( "NpNm_N_jets_" + flag, "N_{jets}", 7, 1.5, 8.5),

    };

    return histInfoVec;
}

std::vector<double> HistogramConfig::fillNpNmHistograms(const eventClass evClass, EventFourT* event) {
    std::vector<double> fillVal;

    if (evClass != eventClass::ssdl && evClass != eventClass::crw && evClass != eventClass::cro) return fillVal;

    fillVal = {
        0.5,
        double(event->numberOfMediumBJets()),
        double(event->numberOfLooseBJets()),
        double(event->numberOfTightBJets()),
        double(event->numberOfJets()),
    };

    return fillVal;
}
