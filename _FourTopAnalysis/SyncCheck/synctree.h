#include <string>
#include <vector>
#include <TTree.h>

class SyncTree {
    public:
        SyncTree(std::string& pathToFile);

        ULong64_t _eventNumber;
        std::vector<bool>* electrons_is_loose;
        std::vector<bool>* electrons_is_fakeable;
        std::vector<bool>* electrons_is_tight;
        std::vector<bool>* muons_is_loose;
        std::vector<bool>* muons_is_fakeable;
        std::vector<bool>* muons_is_tight;

        bool hasNonPromptLepton;
        bool hasGenPromptLepton_ChargeFlip;
        bool hasGenMatchedPromptPhotonProduct;

        float GenMET_pt;

        double PFMET_phi_final;
        double PFMET_pt_final;

        std::vector<bool>* ak4jets_is_btagged;
        std::vector<bool>* electrons_mvaFall17V2noIso_WPL;
        std::vector<bool>* electrons_is_genmatched_prompt_chargeFlip;
        std::vector<bool>* ak4jets_is_clean;
        std::vector<bool>* electrons_is_genmatched_prompt_sameCharge;
        std::vector<bool>* electrons_is_genmatched_photonProduct;
        std::vector<bool>* electrons_is_genmatched_prompt;
        std::vector<bool>* electrons_is_genmatched_prompt_photonProduct;
        std::vector<bool>* muons_is_genmatched_prompt;
        std::vector<bool>* electrons_convVeto;
        std::vector<bool>* muons_is_genmatched_prompt_sameCharge;
        std::vector<bool>* electrons_isPFcand;
        std::vector<bool>* muons_is_genmatched_photonProduct;
        std::vector<bool>* ak4jets_is_tight;
        std::vector<bool>* muons_isTracker;
        std::vector<bool>* muons_is_genmatched_prompt_photonProduct;
        std::vector<bool>* muons_inTimeMuon;
        std::vector<bool>* electrons_mvaNoIso_WPL;
        std::vector<bool>* muons_is_genmatched_prompt_chargeFlip;
        std::vector<bool>* muons_looseId;
        std::vector<bool>* muons_highPurity;
        std::vector<bool>* muons_triggerIdLoose;
        std::vector<bool>* muons_mediumId;
        std::vector<bool>* muons_isGlobal;
        std::vector<bool>* muons_tightId;
        std::vector<bool>* muons_isPFcand;

        std::vector<int>* ak4jets_electronIdx2;
        std::vector<int>* ak4jets_electronIdx1;
        std::vector<int>* ak4jets_jetId;
        std::vector<int>* muons_nStations;
        std::vector<int>* ak4jets_muonIdx2;
        std::vector<int>* muons_genmatch_pdgId;
        std::vector<int>* ak4jets_partonFlavour;
        std::vector<int>* ak4jets_hadronFlavour;
        std::vector<int>* electrons_genmatch_pdgId;
        std::vector<int>* electrons_tightCharge;
        std::vector<int>* ak4jets_muonIdx1;
        std::vector<int>* electrons_cutBased;

        std::vector<float>* ak4jets_btagDeepFlavB;
        std::vector<float>* ak4jets_rawFactor;
        std::vector<float>* ak4jets_muEF;
        std::vector<float>* ak4jets_muonSubtrFactor;
        std::vector<float>* ak4jets_area;
        std::vector<float>* ak4jets_mass;
        std::vector<float>* ak4jets_eta;
        std::vector<float>* ak4jets_pt;
        std::vector<float>* electrons_scEtOverPt;
        std::vector<float>* electrons_r9;
        std::vector<float>* electrons_ip3d;
        std::vector<float>* electrons_dr03HcalDepth1TowerSumEt;
        std::vector<float>* electrons_dr03EcalRecHitSumEt;
        std::vector<float>* electrons_mvaFall17V2noIso;
        std::vector<float>* electrons_jetRelIso;
        std::vector<float>* electrons_jetPtRelv2;
        std::vector<float>* electrons_pfRelIso03_all;
        std::vector<float>* electrons_miniPFRelIso_chg;
        std::vector<float>* electrons_miniPFRelIso_all;
        std::vector<float>* muons_dxy;
        std::vector<float>* muons_pfRelIso03_all;
        std::vector<float>* muons_miniPFRelIso_all;
        std::vector<float>* muons_segmentComp;
        std::vector<float>* muons_phi;
        std::vector<float>* muons_miniPFRelIso_chg;
        std::vector<float>* electrons_etaSC;
        std::vector<float>* electrons_dr03TkSumPt;
        std::vector<float>* muons_bscore;
        std::vector<float>* muons_extMVAscore;
        std::vector<float>* muons_eta;
        std::vector<float>* muons_dz;
        std::vector<float>* ak4jets_phi;
        std::vector<float>* electrons_mvaIso;
        std::vector<float>* electrons_deltaEtaSC;
        std::vector<float>* electrons_dxy;
        std::vector<float>* muons_pt;
        std::vector<float>* muons_sip3d;
        std::vector<float>* muons_ptrel_final;
        std::vector<float>* muons_pfRelIso03_chg;
        std::vector<float>* muons_mass;
        std::vector<float>* electrons_pfRelIso03_chg;
        std::vector<float>* electrons_ptrel_final;
        std::vector<float>* muons_ptratio_final;
        std::vector<float>* electrons_phi;
        std::vector<float>* electrons_mvaFall17V2Iso;
        std::vector<float>* electrons_eta;
        std::vector<float>* electrons_bscore;
        std::vector<float>* muons_jetRelIso;
        std::vector<float>* electrons_mvaFall17V2noIso_raw;
        std::vector<float>* electrons_mvaNoIso;
        std::vector<float>* muons_tkRelIso;
        std::vector<float>* muons_ptErr;
        std::vector<float>* electrons_pt;
        std::vector<float>* muons_jetPtRelv2;
        std::vector<float>* electrons_extMVAscore;
        std::vector<float>* electrons_hoe;
        std::vector<float>* electrons_eInvMinusPInv;
        std::vector<float>* electrons_sieie;
        std::vector<float>* electrons_dz;
        std::vector<float>* electrons_mass;
        std::vector<float>* electrons_ptratio_final;
        std::vector<float>* electrons_sip3d;


        TTree* GetTree() {return tree;}
        
    private:
        TTree* tree;

        TBranch* b__eventNumber;
        TBranch* b__muons_is_loose;
        TBranch* b__muons_is_fakeable;
        TBranch* b__muons_is_tight;
        TBranch* b__electrons_is_loose;
        TBranch* b__electrons_is_fakeable;
        TBranch* b__electrons_is_tight;

        TBranch* b__hasNonPromptLepton;
        TBranch* b__hasGenPromptLepton_ChargeFlip;
        TBranch* b__hasGenMatchedPromptPhotonProduct;
        TBranch* b__GenMET_pt;
        TBranch* b__PFMET_phi_final;
        TBranch* b__PFMET_pt_final;
        TBranch* b__electrons_mvaFall17V2noIso_WPL;
        TBranch* b__electrons_is_genmatched_prompt_chargeFlip;
        TBranch* b__ak4jets_is_btagged;
        TBranch* b__ak4jets_is_clean;
        TBranch* b__electrons_is_genmatched_prompt_sameCharge;
        TBranch* b__electrons_is_genmatched_photonProduct;
        TBranch* b__electrons_is_genmatched_prompt;
        TBranch* b__electrons_is_genmatched_prompt_photonProduct;
        TBranch* b__muons_is_genmatched_prompt;
        TBranch* b__electrons_convVeto;
        TBranch* b__muons_is_genmatched_prompt_sameCharge;
        TBranch* b__electrons_isPFcand;
        TBranch* b__muons_is_genmatched_photonProduct;
        TBranch* b__ak4jets_is_tight;
        TBranch* b__muons_isTracker;
        TBranch* b__muons_is_genmatched_prompt_photonProduct;
        TBranch* b__muons_inTimeMuon;
        TBranch* b__electrons_mvaNoIso_WPL;
        TBranch* b__muons_is_genmatched_prompt_chargeFlip;
        TBranch* b__muons_looseId;
        TBranch* b__muons_highPurity;
        TBranch* b__muons_triggerIdLoose;
        TBranch* b__muons_mediumId;
        TBranch* b__muons_isGlobal;
        TBranch* b__muons_tightId;
        TBranch* b__muons_isPFcand;
        TBranch* b__ak4jets_electronIdx2;
        TBranch* b__ak4jets_electronIdx1;
        TBranch* b__ak4jets_jetId;
        TBranch* b__muons_nStations;
        TBranch* b__ak4jets_muonIdx2;
        TBranch* b__muons_genmatch_pdgId;
        TBranch* b__ak4jets_partonFlavour;
        TBranch* b__ak4jets_hadronFlavour;
        TBranch* b__electrons_genmatch_pdgId;
        TBranch* b__electrons_tightCharge;
        TBranch* b__ak4jets_muonIdx1;
        TBranch* b__electrons_cutBased;
        TBranch* b__ak4jets_btagDeepFlavB;
        TBranch* b__ak4jets_rawFactor;
        TBranch* b__ak4jets_muEF;
        TBranch* b__ak4jets_muonSubtrFactor;
        TBranch* b__ak4jets_area;
        TBranch* b__ak4jets_mass;
        TBranch* b__ak4jets_eta;
        TBranch* b__ak4jets_pt;
        TBranch* b__electrons_scEtOverPt;
        TBranch* b__electrons_r9;
        TBranch* b__electrons_ip3d;
        TBranch* b__electrons_dr03HcalDepth1TowerSumEt;
        TBranch* b__electrons_dr03EcalRecHitSumEt;
        TBranch* b__electrons_mvaFall17V2noIso;
        TBranch* b__electrons_jetRelIso;
        TBranch* b__electrons_jetPtRelv2;
        TBranch* b__electrons_pfRelIso03_all;
        TBranch* b__electrons_miniPFRelIso_chg;
        TBranch* b__electrons_miniPFRelIso_all;
        TBranch* b__muons_dxy;
        TBranch* b__muons_pfRelIso03_all;
        TBranch* b__muons_miniPFRelIso_all;
        TBranch* b__muons_segmentComp;
        TBranch* b__muons_phi;
        TBranch* b__muons_miniPFRelIso_chg;
        TBranch* b__electrons_etaSC;
        TBranch* b__electrons_dr03TkSumPt;
        TBranch* b__muons_bscore;
        TBranch* b__muons_extMVAscore;
        TBranch* b__muons_eta;
        TBranch* b__muons_dz;
        TBranch* b__ak4jets_phi;
        TBranch* b__electrons_mvaIso;
        TBranch* b__electrons_deltaEtaSC;
        TBranch* b__electrons_dxy;
        TBranch* b__muons_pt;
        TBranch* b__muons_sip3d;
        TBranch* b__muons_ptrel_final;
        TBranch* b__muons_pfRelIso03_chg;
        TBranch* b__muons_mass;
        TBranch* b__electrons_pfRelIso03_chg;
        TBranch* b__electrons_ptrel_final;
        TBranch* b__muons_ptratio_final;
        TBranch* b__electrons_phi;
        TBranch* b__electrons_mvaFall17V2Iso;
        TBranch* b__electrons_eta;
        TBranch* b__electrons_bscore;
        TBranch* b__muons_jetRelIso;
        TBranch* b__electrons_mvaFall17V2noIso_raw;
        TBranch* b__electrons_mvaNoIso;
        TBranch* b__muons_tkRelIso;
        TBranch* b__muons_ptErr;
        TBranch* b__electrons_pt;
        TBranch* b__muons_jetPtRelv2;
        TBranch* b__electrons_extMVAscore;
        TBranch* b__electrons_hoe;
        TBranch* b__electrons_eInvMinusPInv;
        TBranch* b__electrons_sieie;
        TBranch* b__electrons_dz;
        TBranch* b__electrons_mass;
        TBranch* b__electrons_ptratio_final;
        TBranch* b__electrons_sip3d;
};