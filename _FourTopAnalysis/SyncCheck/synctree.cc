#include "synctree.h"

#include <TFile.h>

SyncTree::SyncTree(std::string& pathToFile) {
    TFile* syncfile = new TFile(pathToFile.c_str());

    tree = (TTree*) syncfile->Get("SyncObjects");

    electrons_is_loose = new std::vector<bool>();
    electrons_is_fakeable = new std::vector<bool>();
    electrons_is_tight = new std::vector<bool>();
    muons_is_loose = new std::vector<bool>();
    muons_is_fakeable = new std::vector<bool>();
    muons_is_tight = new std::vector<bool>();

    tree->SetBranchAddress("EventNumber", &_eventNumber, &b__eventNumber);
    tree->SetBranchAddress("muons_is_loose",  &muons_is_loose, &b__muons_is_loose);
    tree->SetBranchAddress("muons_is_fakeable", &muons_is_fakeable, &b__muons_is_fakeable);
    tree->SetBranchAddress("muons_is_tight", &muons_is_tight, &b__muons_is_tight);
    tree->SetBranchAddress("electrons_is_loose", &electrons_is_loose, &b__electrons_is_loose);
    tree->SetBranchAddress("electrons_is_fakeable", &electrons_is_fakeable, &b__electrons_is_fakeable);
    tree->SetBranchAddress("electrons_is_tight", &electrons_is_tight, &b__electrons_is_tight);

    tree->SetBranchAddress("hasNonPromptLepton", &hasNonPromptLepton, &b__hasNonPromptLepton);
    tree->SetBranchAddress("hasGenPromptLepton_ChargeFlip", &hasGenPromptLepton_ChargeFlip, &b__hasGenPromptLepton_ChargeFlip);
    tree->SetBranchAddress("hasGenMatchedPromptPhotonProduct", &hasGenMatchedPromptPhotonProduct, &b__hasGenMatchedPromptPhotonProduct);
    tree->SetBranchAddress("GenMET_pt", &GenMET_pt, &b__GenMET_pt);
    tree->SetBranchAddress("PFMET_phi_final", &PFMET_phi_final, &b__PFMET_phi_final);
    tree->SetBranchAddress("PFMET_pt_final", &PFMET_pt_final, &b__PFMET_pt_final);
    tree->SetBranchAddress("electrons_mvaFall17V2noIso_WPL", &electrons_mvaFall17V2noIso_WPL, &b__electrons_mvaFall17V2noIso_WPL);
    tree->SetBranchAddress("electrons_is_genmatched_prompt_chargeFlip", &electrons_is_genmatched_prompt_chargeFlip, &b__electrons_is_genmatched_prompt_chargeFlip);
    tree->SetBranchAddress("ak4jets_is_btagged", &ak4jets_is_btagged, &b__ak4jets_is_btagged);
    tree->SetBranchAddress("ak4jets_is_clean", &ak4jets_is_clean, &b__ak4jets_is_clean);
    tree->SetBranchAddress("electrons_is_genmatched_prompt_sameCharge", &electrons_is_genmatched_prompt_sameCharge, &b__electrons_is_genmatched_prompt_sameCharge);
    tree->SetBranchAddress("electrons_is_genmatched_photonProduct", &electrons_is_genmatched_photonProduct, &b__electrons_is_genmatched_photonProduct);
    tree->SetBranchAddress("electrons_is_genmatched_prompt", &electrons_is_genmatched_prompt, &b__electrons_is_genmatched_prompt);
    tree->SetBranchAddress("electrons_is_genmatched_prompt_photonProduct", &electrons_is_genmatched_prompt_photonProduct, &b__electrons_is_genmatched_prompt_photonProduct);
    tree->SetBranchAddress("muons_is_genmatched_prompt", &muons_is_genmatched_prompt, &b__muons_is_genmatched_prompt);
    tree->SetBranchAddress("electrons_convVeto", &electrons_convVeto, &b__electrons_convVeto);
    tree->SetBranchAddress("muons_is_genmatched_prompt_sameCharge", &muons_is_genmatched_prompt_sameCharge, &b__muons_is_genmatched_prompt_sameCharge);
    tree->SetBranchAddress("electrons_isPFcand", &electrons_isPFcand, &b__electrons_isPFcand);
    tree->SetBranchAddress("muons_is_genmatched_photonProduct", &muons_is_genmatched_photonProduct, &b__muons_is_genmatched_photonProduct);
    tree->SetBranchAddress("ak4jets_is_tight", &ak4jets_is_tight, &b__ak4jets_is_tight);
    tree->SetBranchAddress("muons_isTracker", &muons_isTracker, &b__muons_isTracker);
    tree->SetBranchAddress("muons_is_genmatched_prompt_photonProduct", &muons_is_genmatched_prompt_photonProduct, &b__muons_is_genmatched_prompt_photonProduct);
    tree->SetBranchAddress("muons_inTimeMuon", &muons_inTimeMuon, &b__muons_inTimeMuon);
    tree->SetBranchAddress("electrons_mvaNoIso_WPL", &electrons_mvaNoIso_WPL, &b__electrons_mvaNoIso_WPL);
    tree->SetBranchAddress("muons_is_genmatched_prompt_chargeFlip", &muons_is_genmatched_prompt_chargeFlip, &b__muons_is_genmatched_prompt_chargeFlip);
    tree->SetBranchAddress("muons_looseId", &muons_looseId, &b__muons_looseId);
    tree->SetBranchAddress("muons_highPurity", &muons_highPurity, &b__muons_highPurity);
    tree->SetBranchAddress("muons_triggerIdLoose", &muons_triggerIdLoose, &b__muons_triggerIdLoose);
    tree->SetBranchAddress("muons_mediumId", &muons_mediumId, &b__muons_mediumId);
    tree->SetBranchAddress("muons_isGlobal", &muons_isGlobal, &b__muons_isGlobal);
    tree->SetBranchAddress("muons_tightId", &muons_tightId, &b__muons_tightId);
    tree->SetBranchAddress("muons_isPFcand", &muons_isPFcand, &b__muons_isPFcand);
    tree->SetBranchAddress("ak4jets_electronIdx2", &ak4jets_electronIdx2, &b__ak4jets_electronIdx2);
    tree->SetBranchAddress("ak4jets_electronIdx1", &ak4jets_electronIdx1, &b__ak4jets_electronIdx1);
    tree->SetBranchAddress("ak4jets_jetId", &ak4jets_jetId, &b__ak4jets_jetId);
    tree->SetBranchAddress("muons_nStations", &muons_nStations, &b__muons_nStations);
    tree->SetBranchAddress("ak4jets_muonIdx2", &ak4jets_muonIdx2, &b__ak4jets_muonIdx2);
    tree->SetBranchAddress("muons_genmatch_pdgId", &muons_genmatch_pdgId, &b__muons_genmatch_pdgId);
    tree->SetBranchAddress("ak4jets_partonFlavour", &ak4jets_partonFlavour, &b__ak4jets_partonFlavour);
    tree->SetBranchAddress("ak4jets_hadronFlavour", &ak4jets_hadronFlavour, &b__ak4jets_hadronFlavour);
    tree->SetBranchAddress("electrons_genmatch_pdgId", &electrons_genmatch_pdgId, &b__electrons_genmatch_pdgId);
    tree->SetBranchAddress("electrons_tightCharge", &electrons_tightCharge, &b__electrons_tightCharge);
    tree->SetBranchAddress("ak4jets_muonIdx1", &ak4jets_muonIdx1, &b__ak4jets_muonIdx1);
    tree->SetBranchAddress("electrons_cutBased", &electrons_cutBased, &b__electrons_cutBased);
    tree->SetBranchAddress("ak4jets_btagDeepFlavB", &ak4jets_btagDeepFlavB, &b__ak4jets_btagDeepFlavB);
    tree->SetBranchAddress("ak4jets_rawFactor", &ak4jets_rawFactor, &b__ak4jets_rawFactor);
    tree->SetBranchAddress("ak4jets_muEF", &ak4jets_muEF, &b__ak4jets_muEF);
    tree->SetBranchAddress("ak4jets_muonSubtrFactor", &ak4jets_muonSubtrFactor, &b__ak4jets_muonSubtrFactor);
    tree->SetBranchAddress("ak4jets_area", &ak4jets_area, &b__ak4jets_area);
    tree->SetBranchAddress("ak4jets_mass", &ak4jets_mass, &b__ak4jets_mass);
    tree->SetBranchAddress("ak4jets_eta", &ak4jets_eta, &b__ak4jets_eta);
    tree->SetBranchAddress("ak4jets_pt", &ak4jets_pt, &b__ak4jets_pt);
    tree->SetBranchAddress("electrons_scEtOverPt", &electrons_scEtOverPt, &b__electrons_scEtOverPt);
    tree->SetBranchAddress("electrons_r9", &electrons_r9, &b__electrons_r9);
    tree->SetBranchAddress("electrons_ip3d", &electrons_ip3d, &b__electrons_ip3d);
    tree->SetBranchAddress("electrons_dr03HcalDepth1TowerSumEt", &electrons_dr03HcalDepth1TowerSumEt, &b__electrons_dr03HcalDepth1TowerSumEt);
    tree->SetBranchAddress("electrons_dr03EcalRecHitSumEt", &electrons_dr03EcalRecHitSumEt, &b__electrons_dr03EcalRecHitSumEt);
    tree->SetBranchAddress("electrons_mvaFall17V2noIso", &electrons_mvaFall17V2noIso, &b__electrons_mvaFall17V2noIso);
    tree->SetBranchAddress("electrons_jetRelIso", &electrons_jetRelIso, &b__electrons_jetRelIso);
    tree->SetBranchAddress("electrons_jetPtRelv2", &electrons_jetPtRelv2, &b__electrons_jetPtRelv2);
    tree->SetBranchAddress("electrons_pfRelIso03_all", &electrons_pfRelIso03_all, &b__electrons_pfRelIso03_all);
    tree->SetBranchAddress("electrons_miniPFRelIso_chg", &electrons_miniPFRelIso_chg, &b__electrons_miniPFRelIso_chg);
    tree->SetBranchAddress("electrons_miniPFRelIso_all", &electrons_miniPFRelIso_all, &b__electrons_miniPFRelIso_all);
    tree->SetBranchAddress("muons_dxy", &muons_dxy, &b__muons_dxy);
    tree->SetBranchAddress("muons_pfRelIso03_all", &muons_pfRelIso03_all, &b__muons_pfRelIso03_all);
    tree->SetBranchAddress("muons_miniPFRelIso_all", &muons_miniPFRelIso_all, &b__muons_miniPFRelIso_all);
    tree->SetBranchAddress("muons_segmentComp", &muons_segmentComp, &b__muons_segmentComp);
    tree->SetBranchAddress("muons_phi", &muons_phi, &b__muons_phi);
    tree->SetBranchAddress("muons_miniPFRelIso_chg", &muons_miniPFRelIso_chg, &b__muons_miniPFRelIso_chg);
    tree->SetBranchAddress("electrons_etaSC", &electrons_etaSC, &b__electrons_etaSC);
    tree->SetBranchAddress("electrons_dr03TkSumPt", &electrons_dr03TkSumPt, &b__electrons_dr03TkSumPt);
    tree->SetBranchAddress("muons_bscore", &muons_bscore, &b__muons_bscore);
    tree->SetBranchAddress("muons_extMVAscore", &muons_extMVAscore, &b__muons_extMVAscore);
    tree->SetBranchAddress("muons_eta", &muons_eta, &b__muons_eta);
    tree->SetBranchAddress("muons_dz", &muons_dz, &b__muons_dz);
    tree->SetBranchAddress("ak4jets_phi", &ak4jets_phi, &b__ak4jets_phi);
    tree->SetBranchAddress("electrons_mvaIso", &electrons_mvaIso, &b__electrons_mvaIso);
    tree->SetBranchAddress("electrons_deltaEtaSC", &electrons_deltaEtaSC, &b__electrons_deltaEtaSC);
    tree->SetBranchAddress("electrons_dxy", &electrons_dxy, &b__electrons_dxy);
    tree->SetBranchAddress("muons_pt", &muons_pt, &b__muons_pt);
    tree->SetBranchAddress("muons_sip3d", &muons_sip3d, &b__muons_sip3d);
    tree->SetBranchAddress("muons_ptrel_final", &muons_ptrel_final, &b__muons_ptrel_final);
    tree->SetBranchAddress("muons_pfRelIso03_chg", &muons_pfRelIso03_chg, &b__muons_pfRelIso03_chg);
    tree->SetBranchAddress("muons_mass", &muons_mass, &b__muons_mass);
    tree->SetBranchAddress("electrons_pfRelIso03_chg", &electrons_pfRelIso03_chg, &b__electrons_pfRelIso03_chg);
    tree->SetBranchAddress("electrons_ptrel_final", &electrons_ptrel_final, &b__electrons_ptrel_final);
    tree->SetBranchAddress("muons_ptratio_final", &muons_ptratio_final, &b__muons_ptratio_final);
    tree->SetBranchAddress("electrons_phi", &electrons_phi, &b__electrons_phi);
    tree->SetBranchAddress("electrons_mvaFall17V2Iso", &electrons_mvaFall17V2Iso, &b__electrons_mvaFall17V2Iso);
    tree->SetBranchAddress("electrons_eta", &electrons_eta, &b__electrons_eta);
    tree->SetBranchAddress("electrons_bscore", &electrons_bscore, &b__electrons_bscore);
    tree->SetBranchAddress("muons_jetRelIso", &muons_jetRelIso, &b__muons_jetRelIso);
    tree->SetBranchAddress("electrons_mvaFall17V2noIso_raw", &electrons_mvaFall17V2noIso_raw, &b__electrons_mvaFall17V2noIso_raw);
    tree->SetBranchAddress("electrons_mvaNoIso", &electrons_mvaNoIso, &b__electrons_mvaNoIso);
    tree->SetBranchAddress("muons_tkRelIso", &muons_tkRelIso, &b__muons_tkRelIso);
    tree->SetBranchAddress("muons_ptErr", &muons_ptErr, &b__muons_ptErr);
    tree->SetBranchAddress("electrons_pt", &electrons_pt, &b__electrons_pt);
    tree->SetBranchAddress("muons_jetPtRelv2", &muons_jetPtRelv2, &b__muons_jetPtRelv2);
    tree->SetBranchAddress("electrons_extMVAscore", &electrons_extMVAscore, &b__electrons_extMVAscore);
    tree->SetBranchAddress("electrons_hoe", &electrons_hoe, &b__electrons_hoe);
    tree->SetBranchAddress("electrons_eInvMinusPInv", &electrons_eInvMinusPInv, &b__electrons_eInvMinusPInv);
    tree->SetBranchAddress("electrons_sieie", &electrons_sieie, &b__electrons_sieie);
    tree->SetBranchAddress("electrons_dz", &electrons_dz, &b__electrons_dz);
    tree->SetBranchAddress("electrons_mass", &electrons_mass, &b__electrons_mass);
    tree->SetBranchAddress("electrons_ptratio_final", &electrons_ptratio_final, &b__electrons_ptratio_final);
    tree->SetBranchAddress("electrons_sip3d", &electrons_sip3d, &b__electrons_sip3d);
}
