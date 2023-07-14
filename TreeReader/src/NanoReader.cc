#include "../interface/NanoReader.h"


NanoReader::NanoReader( const std::string& sampleListFile, const std::string& sampleDirectory ){
    readSamples( sampleListFile, sampleDirectory );

    _sourcesJEC_Ids = new std::map<std::string, size_t>();
    _groupedJEC_Ids = new std::map<std::string, size_t>();
}

void NanoReader::initTree(const bool resetTriggersAndFilters = true) {
    

    _currentTreePtr->SetBranchAddress("run",                &_runNb,                 &b__runNb);
    _currentTreePtr->SetBranchAddress("luminosityBlock",    &_lumiBlock,             &b__lumiBlock);
    _currentTreePtr->SetBranchAddress("event",              &_eventNb,               &b__eventNb);

    /*
    nboostedTau
    boostedTau_chargedIso
    boostedTau_eta
    boostedTau_leadTkDeltaEta
    boostedTau_leadTkDeltaPhi
    boostedTau_leadTkPtOverTauPt
    boostedTau_mass
    boostedTau_neutralIso
    boostedTau_phi
    boostedTau_photonsOutsideSignalCone
    boostedTau_pt
    boostedTau_puCorr
    boostedTau_rawAntiEle2018
    boostedTau_rawIso
    boostedTau_rawIsodR03
    boostedTau_rawMVAnewDM2017v2
    boostedTau_rawMVAoldDM2017v2
    boostedTau_rawMVAoldDMdR032017v2
    boostedTau_charge
    boostedTau_decayMode
    boostedTau_jetIdx
    boostedTau_rawAntiEleCat2018
    boostedTau_idAntiEle2018
    boostedTau_idAntiMu
    boostedTau_idMVAnewDM2017v2
    boostedTau_idMVAoldDM2017v2
    boostedTau_idMVAoldDMdR032017v2
    btagWeight_CSVV2
    btagWeight_DeepCSVB
    CaloMET_phi
    CaloMET_pt
    CaloMET_sumEt
    ChsMET_phi
    ChsMET_pt
    ChsMET_sumEt
    nCorrT1METJet
    CorrT1METJet_area
    CorrT1METJet_eta
    CorrT1METJet_muonSubtrFactor
    CorrT1METJet_phi
    CorrT1METJet_rawPt
    DeepMETResolutionTune_phi
    DeepMETResolutionTune_pt
    DeepMETResponseTune_phi
    DeepMETResponseTune_pt
    */
    // electron variables
    _currentTreePtr->SetBranchAddress("nElectron",                          &_nEle,                         &b__nEle);
    _currentTreePtr->SetBranchAddress("Electron_dEscaleDown",               &_El_dEscaleDown,               &b__Electron_dEscaleDown);
    _currentTreePtr->SetBranchAddress("Electron_dEscaleUp",                 &_El_dEscaleUp,                 &b__Electron_dEscaleUp);
    _currentTreePtr->SetBranchAddress("Electron_dEsigmaDown",               &_El_dEsigmaDown,               &b__Electron_dEsigmaDown);
    _currentTreePtr->SetBranchAddress("Electron_dEsigmaUp",                 &_El_dEsigmaUp,                 &b__Electron_dEsigmaUp);
    _currentTreePtr->SetBranchAddress("Electron_deltaEtaSC",                &_El_deltaEtaSC,                &b__Electron_deltaEtaSC);
    _currentTreePtr->SetBranchAddress("Electron_dr03EcalRecHitSumEt",       &_El_dr03EcalRecHitSumEt,       &b__Electron_dr03EcalRecHitSumEt);
    _currentTreePtr->SetBranchAddress("Electron_dr03HcalDepth1TowerSumEt",  &_El_dr03HcalDepth1TowerSumEt,  &b__Electron_dr03HcalDepth1TowerSumEt);
    _currentTreePtr->SetBranchAddress("Electron_dr03TkSumPt",               &_El_dr03TkSumPt,               &b__Electron_dr03TkSumPt);
    _currentTreePtr->SetBranchAddress("Electron_dr03TkSumPtHEEP",           &_El_dr03TkSumPtHEEP,           &b__Electron_dr03TkSumPtHEEP);
    _currentTreePtr->SetBranchAddress("Electron_dxy",                       &_El_dxy,                       &b__Electron_dxy);
    _currentTreePtr->SetBranchAddress("Electron_dxyErr",                    &_El_dxyErr,                    &b__Electron_dxyErr);
    _currentTreePtr->SetBranchAddress("Electron_dz",                        &_El_dz,                        &b__Electron_dz);
    _currentTreePtr->SetBranchAddress("Electron_dzErr",                     &_El_dzErr,                     &b__Electron_dzErr);
    _currentTreePtr->SetBranchAddress("Electron_eCorr",                     &_El_eCorr,                     &b__Electron_eCorr);
    _currentTreePtr->SetBranchAddress("Electron_eInvMinusPInv",             &_El_eInvMinusPInv,             &b__Electron_eInvMinusPInv);
    _currentTreePtr->SetBranchAddress("Electron_energyErr",                 &_El_energyErr,                 &b__Electron_energyErr);
    _currentTreePtr->SetBranchAddress("Electron_eta",                       &_El_eta,                       &b__Electron_eta);
    _currentTreePtr->SetBranchAddress("Electron_hoe",                       &_El_hoe,                       &b__Electron_hoe);
    _currentTreePtr->SetBranchAddress("Electron_ip3d",                      &_El_ip3d,                      &b__Electron_ip3d);
    _currentTreePtr->SetBranchAddress("Electron_jetPtRelv2",                &_El_jetPtRelv2,                &b__Electron_jetPtRelv2);
    _currentTreePtr->SetBranchAddress("Electron_jetRelIso",                 &_El_jetRelIso,                 &b__Electron_jetRelIso);
    _currentTreePtr->SetBranchAddress("Electron_mass",                      &_El_mass,                      &b__Electron_mass);
    _currentTreePtr->SetBranchAddress("Electron_miniPFRelIso_all",          &_El_miniPFRelIso_all,          &b__Electron_miniPFRelIso_all);
    _currentTreePtr->SetBranchAddress("Electron_miniPFRelIso_chg",          &_El_miniPFRelIso_chg,          &b__Electron_miniPFRelIso_chg);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2Iso",            &_El_mvaFall17V2Iso,            &b__Electron_mvaFall17V2Iso);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso",          &_El_mvaFall17V2noIso,          &b__Electron_mvaFall17V2noIso);
    _currentTreePtr->SetBranchAddress("Electron_pfRelIso03_all",            &_El_pfRelIso03_all,            &b__Electron_pfRelIso03_all);
    _currentTreePtr->SetBranchAddress("Electron_pfRelIso03_chg",            &_El_pfRelIso03_chg,            &b__Electron_pfRelIso03_chg);
    _currentTreePtr->SetBranchAddress("Electron_phi",                       &_El_phi,                       &b__Electron_phi);
    _currentTreePtr->SetBranchAddress("Electron_pt",                        &_El_pt,                        &b__Electron_pt);
    _currentTreePtr->SetBranchAddress("Electron_r9",                        &_El_r9,                        &b__Electron_r9);
    _currentTreePtr->SetBranchAddress("Electron_scEtOverPt",                &_El_scEtOverPt,                &b__Electron_scEtOverPt);
    _currentTreePtr->SetBranchAddress("Electron_sieie",                     &_El_sieie,                     &b__Electron_sieie);
    _currentTreePtr->SetBranchAddress("Electron_sip3d",                     &_El_sip3d,                     &b__Electron_sip3d);
    _currentTreePtr->SetBranchAddress("Electron_mvaTTH",                    &_El_mvaTTH,                    &b__Electron_mvaTTH);
    _currentTreePtr->SetBranchAddress("Electron_charge",                    &_El_charge,                    &b__Electron_charge);
    _currentTreePtr->SetBranchAddress("Electron_cutBased",                  &_El_cutBased,                  &b__Electron_cutBased);
    _currentTreePtr->SetBranchAddress("Electron_jetIdx",                    &_El_jetIdx,                    &b__Electron_jetIdx);
    _currentTreePtr->SetBranchAddress("Electron_pdgId",                     &_El_pdgId,                     &b__Electron_pdgId);
    _currentTreePtr->SetBranchAddress("Electron_photonIdx",                 &_El_photonIdx,                 &b__Electron_photonIdx);
    _currentTreePtr->SetBranchAddress("Electron_tightCharge",               &_El_tightCharge,               &b__Electron_tightCharge);
    _currentTreePtr->SetBranchAddress("Electron_vidNestedWPBitmap",         &_El_vidNestedWPBitmap,         &b__Electron_vidNestedWPBitmap);
    _currentTreePtr->SetBranchAddress("Electron_vidNestedWPBitmapHEEP",     &_El_vidNestedWPBitmapHEEP,     &b__Electron_vidNestedWPBitmapHEEP);
    _currentTreePtr->SetBranchAddress("Electron_convVeto",                  &_El_convVeto,                  &b__Electron_convVeto);
    _currentTreePtr->SetBranchAddress("Electron_cutBased_HEEP",             &_El_cutBased_HEEP,             &b__Electron_cutBased_HEEP);
    _currentTreePtr->SetBranchAddress("Electron_isPFcand",                  &_El_isPFcand,                  &b__Electron_isPFcand);
    _currentTreePtr->SetBranchAddress("Electron_jetNDauCharged",            &_El_jetNDauCharged,            &b__Electron_jetNDauCharged);
    _currentTreePtr->SetBranchAddress("Electron_lostHits",                  &_El_lostHits,                  &b__Electron_lostHits);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2Iso_WP80",       &_El_mvaFall17V2Iso_WP80,       &b__Electron_mvaFall17V2Iso_WP80);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2Iso_WP90",       &_El_mvaFall17V2Iso_WP90,       &b__Electron_mvaFall17V2Iso_WP90);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2Iso_WPL",        &_El_mvaFall17V2Iso_WPL,        &b__Electron_mvaFall17V2Iso_WPL);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WP80",     &_El_mvaFall17V2noIso_WP80,     &b__Electron_mvaFall17V2noIso_WP80);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WP90",     &_El_mvaFall17V2noIso_WP90,     &b__Electron_mvaFall17V2noIso_WP90);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WPL",      &_El_mvaFall17V2noIso_WPL,      &b__Electron_mvaFall17V2noIso_WPL);
    _currentTreePtr->SetBranchAddress("Electron_seedGain",                  &_El_seedGain,                  &b__Electron_seedGain);


/*
nFsrPhoton
FsrPhoton_dROverEt2
FsrPhoton_eta
FsrPhoton_phi
FsrPhoton_pt
FsrPhoton_relIso03
FsrPhoton_muonIdx

GenJet_eta
GenJet_mass
GenJet_phi
GenJet_pt
nGenPart
GenPart_eta
GenPart_mass
GenPart_phi
GenPart_pt
GenPart_genPartIdxMother
GenPart_pdgId
GenPart_status
GenPart_statusFlags
*/


// Runs TTree:
/*
genEventCount: aantal events
sumw: sum of weights
sumw2: squared sum

LHEScaleSumw[i]: sum of genEventWeight * LHEScaleweight[i] / genEventSumw (and summed over all events)
similar for LHEPdfWeight

*/
Generator_binvar
Generator_scalePDF
Generator_weight  // gen weight -> nominal one
Generator_x1
Generator_x2
Generator_xpdf1
Generator_xpdf2
Generator_id1
Generator_id2
GenVtx_x
GenVtx_y
GenVtx_z
nGenVisTau
GenVisTau_eta
GenVisTau_mass
GenVisTau_phi
GenVisTau_pt
GenVisTau_charge
GenVisTau_genPartIdxMother
GenVisTau_status
genWeight  // gen weight -> nominal one
LHEWeight_originalXWGTUP // irrelevent (one of 2 parts in genweight)
nLHEPdfWeight    // weights from a pdf set -> first in pref list given in config present in sample. LHAPDF IDs stored are mentioned in title of branch
LHEPdfWeight     // weights from a pdf set -> first in pref list given in config present in sample. LHAPDF IDs stored are mentioned in title of branch
nLHEReweightingWeight
LHEReweightingWeight
nLHEScaleWeight  // variations in fact and norm scale of ME
LHEScaleWeight   // variations in fact and norm scale of ME
nPSWeight        // renorm in parton shower variations
PSWeight         // renorm in parton shower variations

/*
nIsoTrack
IsoTrack_dxy
IsoTrack_dz
IsoTrack_eta
IsoTrack_pfRelIso03_all
IsoTrack_pfRelIso03_chg
IsoTrack_phi
IsoTrack_pt
IsoTrack_miniPFRelIso_all
IsoTrack_miniPFRelIso_chg
IsoTrack_charge
IsoTrack_fromPV
IsoTrack_pdgId
IsoTrack_isHighPurityTrack
IsoTrack_isPFcand
IsoTrack_isFromLostTrack
*/

    _currentTreePtr->SetBranchAddress("nJet",                          &_nJet,                         &b__nJet);
    _currentTreePtr->SetBranchAddress("Jet_area",                      &_Jet_area,                     &b__Jet_area);
    _currentTreePtr->SetBranchAddress("Jet_btagCSVV2",                 &_Jet_btagCSVV2,                &b__Jet_btagCSVV2);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepB",                 &_Jet_btagDeepB,                &b__Jet_btagDeepB);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepCvB",               &_Jet_btagDeepCvB,              &b__Jet_btagDeepCvB);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepCvL",               &_Jet_btagDeepCvL,              &b__Jet_btagDeepCvL);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepFlavB",             &_Jet_btagDeepFlavB,            &b__Jet_btagDeepFlavB);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepFlavCvB",           &_Jet_btagDeepFlavCvB,          &b__Jet_btagDeepFlavCvB);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepFlavCvL",           &_Jet_btagDeepFlavCvL,          &b__Jet_btagDeepFlavCvL);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepFlavQG",            &_Jet_btagDeepFlavQG,           &b__Jet_btagDeepFlavQG);
    _currentTreePtr->SetBranchAddress("Jet_chEmEF",                    &_Jet_chEmEF,                   &b__Jet_chEmEF);
    _currentTreePtr->SetBranchAddress("Jet_chFPV0EF",                  &_Jet_chFPV0EF,                 &b__Jet_chFPV0EF);
    _currentTreePtr->SetBranchAddress("Jet_chHEF",                     &_Jet_chHEF,                    &b__Jet_chHEF);
    _currentTreePtr->SetBranchAddress("Jet_eta",                       &_Jet_eta,                      &b__Jet_eta);
    _currentTreePtr->SetBranchAddress("Jet_hfsigmaEtaEta",             &_Jet_hfsigmaEtaEta,            &b__Jet_hfsigmaEtaEta);
    _currentTreePtr->SetBranchAddress("Jet_hfsigmaPhiPhi",             &_Jet_hfsigmaPhiPhi,            &b__Jet_hfsigmaPhiPhi);
    _currentTreePtr->SetBranchAddress("Jet_mass",                      &_Jet_mass,                     &b__Jet_mass);
    _currentTreePtr->SetBranchAddress("Jet_muEF",                      &_Jet_muEF,                     &b__Jet_muEF);
    _currentTreePtr->SetBranchAddress("Jet_muonSubtrFactor",           &_Jet_muonSubtrFactor,          &b__Jet_muonSubtrFactor);
    _currentTreePtr->SetBranchAddress("Jet_neEmEF",                    &_Jet_neEmEF,                   &b__Jet_neEmEF);
    _currentTreePtr->SetBranchAddress("Jet_neHEF",                     &_Jet_neHEF,                    &b__Jet_neHEF);
    _currentTreePtr->SetBranchAddress("Jet_phi",                       &_Jet_phi,                      &b__Jet_phi);
    _currentTreePtr->SetBranchAddress("Jet_pt",                        &_Jet_pt,                       &b__Jet_pt);
    _currentTreePtr->SetBranchAddress("Jet_puIdDisc",                  &_Jet_puIdDisc,                 &b__Jet_puIdDisc);
    _currentTreePtr->SetBranchAddress("Jet_qgl",                       &_Jet_qgl,                      &b__Jet_qgl);
    _currentTreePtr->SetBranchAddress("Jet_rawFactor",                 &_Jet_rawFactor,                &b__Jet_rawFactor);
    _currentTreePtr->SetBranchAddress("Jet_bRegCorr",                  &_Jet_bRegCorr,                 &b__Jet_bRegCorr);
    _currentTreePtr->SetBranchAddress("Jet_bRegRes",                   &_Jet_bRegRes,                  &b__Jet_bRegRes);
    _currentTreePtr->SetBranchAddress("Jet_cRegCorr",                  &_Jet_cRegCorr,                 &b__Jet_cRegCorr);
    _currentTreePtr->SetBranchAddress("Jet_cRegRes",                   &_Jet_cRegRes,                  &b__Jet_cRegRes);
    _currentTreePtr->SetBranchAddress("Jet_electronIdx1",              &_Jet_electronIdx1,             &b__Jet_electronIdx1);
    _currentTreePtr->SetBranchAddress("Jet_electronIdx2",              &_Jet_electronIdx2,             &b__Jet_electronIdx2);
    _currentTreePtr->SetBranchAddress("Jet_hfadjacentEtaStripsSize",   &_Jet_hfadjacentEtaStripsSize,  &b__Jet_hfadjacentEtaStripsSize);
    _currentTreePtr->SetBranchAddress("Jet_hfcentralEtaStripSize",     &_Jet_hfcentralEtaStripSize,    &b__Jet_hfcentralEtaStripSize);
    _currentTreePtr->SetBranchAddress("Jet_jetId",                     &_Jet_jetId,                    &b__Jet_jetId);
    _currentTreePtr->SetBranchAddress("Jet_muonIdx1",                  &_Jet_muonIdx1,                 &b__Jet_muonIdx1);
    _currentTreePtr->SetBranchAddress("Jet_muonIdx2",                  &_Jet_muonIdx2,                 &b__Jet_muonIdx2);
    _currentTreePtr->SetBranchAddress("Jet_nElectrons",                &_Jet_nElectrons,               &b__Jet_nElectrons);
    _currentTreePtr->SetBranchAddress("Jet_nMuons",                    &_Jet_nMuons,                   &b__Jet_nMuons);
    _currentTreePtr->SetBranchAddress("Jet_puId",                      &_Jet_puId,                     &b__Jet_puId);
    _currentTreePtr->SetBranchAddress("Jet_nConstituents",             &_Jet_nConstituents,            &b__Jet_nConstituents);
    _currentTreePtr->SetBranchAddress("Jet_genJetIdx",                 &_Jet_genJetIdx,                &b__Jet_genJetIdx);
    _currentTreePtr->SetBranchAddress("Jet_hadronFlavour",             &_Jet_hadronFlavour,            &b__Jet_hadronFlavour);
    _currentTreePtr->SetBranchAddress("Jet_partonFlavour",             &_Jet_partonFlavour,            &b__Jet_partonFlavour);
        
L1PreFiringWeight_Dn
L1PreFiringWeight_ECAL_Dn
L1PreFiringWeight_ECAL_Nom
L1PreFiringWeight_ECAL_Up
L1PreFiringWeight_Muon_Nom
L1PreFiringWeight_Muon_StatDn
L1PreFiringWeight_Muon_StatUp
L1PreFiringWeight_Muon_SystDn
L1PreFiringWeight_Muon_SystUp
L1PreFiringWeight_Nom
L1PreFiringWeight_Up
LHE_HT
LHE_HTIncoming
LHE_Vpt
LHE_AlphaS
LHE_Njets
LHE_Nb
LHE_Nc
LHE_Nuds
LHE_Nglu
LHE_NpNLO
LHE_NpLO
nLHEPart
LHEPart_pt
LHEPart_eta
LHEPart_phi
LHEPart_mass
LHEPart_incomingpz
LHEPart_pdgId
LHEPart_status
LHEPart_spin

GenMET_phi
GenMET_pt
MET_MetUnclustEnUpDeltaX
MET_MetUnclustEnUpDeltaY
MET_covXX
MET_covXY
MET_covYY
MET_phi
MET_pt
MET_significance
MET_sumEt
MET_sumPtUnclustered

    _currentTreePtr->SetBranchAddress("nMuon",                  &_nMu,                 &b__nMu);
    _currentTreePtr->SetBranchAddress("Muon_dxy",               &_Mu_dxy,              &b__Mu_dxy);
    _currentTreePtr->SetBranchAddress("Muon_dxyErr",            &_Mu_dxyErr,           &b__Mu_dxyErr);
    _currentTreePtr->SetBranchAddress("Muon_dxybs",             &_Mu_dxybs,            &b__Mu_dxybs);
    _currentTreePtr->SetBranchAddress("Muon_dz",                &_Mu_dz,               &b__Mu_dz);
    _currentTreePtr->SetBranchAddress("Muon_dzErr",             &_Mu_dzErr,            &b__Mu_dzErr);
    _currentTreePtr->SetBranchAddress("Muon_eta",               &_Mu_eta,              &b__Mu_eta);
    _currentTreePtr->SetBranchAddress("Muon_ip3d",              &_Mu_ip3d,             &b__Mu_ip3d);
    _currentTreePtr->SetBranchAddress("Muon_jetPtRelv2",        &_Mu_jetPtRelv2,       &b__Mu_jetPtRelv2);
    _currentTreePtr->SetBranchAddress("Muon_jetRelIso",         &_Mu_jetRelIso,        &b__Mu_jetRelIso);
    _currentTreePtr->SetBranchAddress("Muon_mass",              &_Mu_mass,             &b__Mu_mass);
    _currentTreePtr->SetBranchAddress("Muon_miniPFRelIso_all",  &_Mu_miniPFRelIso_all, &b__Mu_miniPFRelIso_all);
    _currentTreePtr->SetBranchAddress("Muon_miniPFRelIso_chg",  &_Mu_miniPFRelIso_chg, &b__Mu_miniPFRelIso_chg);
    _currentTreePtr->SetBranchAddress("Muon_pfRelIso03_all",    &_Mu_pfRelIso03_all,   &b__Mu_pfRelIso03_all);
    _currentTreePtr->SetBranchAddress("Muon_pfRelIso03_chg",    &_Mu_pfRelIso03_chg,   &b__Mu_pfRelIso03_chg);
    _currentTreePtr->SetBranchAddress("Muon_pfRelIso04_all",    &_Mu_pfRelIso04_all,   &b__Mu_pfRelIso04_all);
    _currentTreePtr->SetBranchAddress("Muon_phi",               &_Mu_phi,              &b__Mu_phi);
    _currentTreePtr->SetBranchAddress("Muon_pt",                &_Mu_pt,               &b__Mu_pt);
    _currentTreePtr->SetBranchAddress("Muon_ptErr",             &_Mu_ptErr,            &b__Mu_ptErr);
    _currentTreePtr->SetBranchAddress("Muon_segmentComp",       &_Mu_segmentComp,      &b__Mu_segmentComp);
    _currentTreePtr->SetBranchAddress("Muon_sip3d",             &_Mu_sip3d,            &b__Mu_sip3d);
    _currentTreePtr->SetBranchAddress("Muon_softMva",           &_Mu_softMva,          &b__Mu_softMva);
    _currentTreePtr->SetBranchAddress("Muon_tkRelIso",          &_Mu_tkRelIso,         &b__Mu_tkRelIso);
    _currentTreePtr->SetBranchAddress("Muon_tunepRelPt",        &_Mu_tunepRelPt,       &b__Mu_tunepRelPt);
    _currentTreePtr->SetBranchAddress("Muon_mvaLowPt",          &_Mu_mvaLowPt,         &b__Mu_mvaLowPt);
    _currentTreePtr->SetBranchAddress("Muon_mvaTTH",            &_Mu_mvaTTH,           &b__Mu_mvaTTH);
    _currentTreePtr->SetBranchAddress("Muon_charge",            &_Mu_charge,           &b__Mu_charge);
    _currentTreePtr->SetBranchAddress("Muon_jetIdx",            &_Mu_jetIdx,           &b__Mu_jetIdx);
    _currentTreePtr->SetBranchAddress("Muon_nStations",         &_Mu_nStations,        &b__Mu_nStations);
    _currentTreePtr->SetBranchAddress("Muon_nTrackerLayers",    &_Mu_nTrackerLayers,   &b__Mu_nTrackerLayers);
    _currentTreePtr->SetBranchAddress("Muon_pdgId",             &_Mu_pdgId,            &b__Mu_pdgId);
    _currentTreePtr->SetBranchAddress("Muon_tightCharge",       &_Mu_tightCharge,      &b__Mu_tightCharge);
    _currentTreePtr->SetBranchAddress("Muon_fsrPhotonIdx",      &_Mu_fsrPhotonIdx,     &b__Mu_fsrPhotonIdx);
    _currentTreePtr->SetBranchAddress("Muon_highPtId",          &_Mu_highPtId,         &b__Mu_highPtId);
    _currentTreePtr->SetBranchAddress("Muon_highPurity",        &_Mu_highPurity,       &b__Mu_highPurity);
    _currentTreePtr->SetBranchAddress("Muon_inTimeMuon",        &_Mu_inTimeMuon,       &b__Mu_inTimeMuon);
    _currentTreePtr->SetBranchAddress("Muon_isGlobal",          &_Mu_isGlobal,         &b__Mu_isGlobal);
    _currentTreePtr->SetBranchAddress("Muon_isPFcand",          &_Mu_isPFcand,         &b__Mu_isPFcand);
    _currentTreePtr->SetBranchAddress("Muon_isStandalone",      &_Mu_isStandalone,     &b__Mu_isStandalone);
    _currentTreePtr->SetBranchAddress("Muon_isTracker",         &_Mu_isTracker,        &b__Mu_isTracker);
    _currentTreePtr->SetBranchAddress("Muon_jetNDauCharged",    &_Mu_jetNDauCharged,   &b__Mu_jetNDauCharged);
    _currentTreePtr->SetBranchAddress("Muon_looseId",           &_Mu_looseId,          &b__Mu_looseId);
    _currentTreePtr->SetBranchAddress("Muon_mediumId",          &_Mu_mediumId,         &b__Mu_mediumId);
    _currentTreePtr->SetBranchAddress("Muon_mediumPromptId",    &_Mu_mediumPromptId,   &b__Mu_mediumPromptId);
    _currentTreePtr->SetBranchAddress("Muon_miniIsoId",         &_Mu_miniIsoId,        &b__Mu_miniIsoId);
    _currentTreePtr->SetBranchAddress("Muon_multiIsoId",        &_Mu_multiIsoId,       &b__Mu_multiIsoId);
    _currentTreePtr->SetBranchAddress("Muon_mvaId",             &_Mu_mvaId,            &b__Mu_mvaId);
    _currentTreePtr->SetBranchAddress("Muon_mvaLowPtId",        &_Mu_mvaLowPtId,       &b__Mu_mvaLowPtId);
    _currentTreePtr->SetBranchAddress("Muon_pfIsoId",           &_Mu_pfIsoId,          &b__Mu_pfIsoId);
    _currentTreePtr->SetBranchAddress("Muon_puppiIsoId",        &_Mu_puppiIsoId,       &b__Mu_puppiIsoId);
    _currentTreePtr->SetBranchAddress("Muon_softId",            &_Mu_softId,           &b__Mu_softId);
    _currentTreePtr->SetBranchAddress("Muon_softMvaId",         &_Mu_softMvaId,        &b__Mu_softMvaId);
    _currentTreePtr->SetBranchAddress("Muon_tightId",           &_Mu_tightId,          &b__Mu_tightId);
    _currentTreePtr->SetBranchAddress("Muon_tkIsoId",           &_Mu_tkIsoId,          &b__Mu_tkIsoId);
    _currentTreePtr->SetBranchAddress("Muon_triggerIdLoose",    &_Mu_triggerIdLoose,   &b__Mu_triggerIdLoose);

Pileup_nTrueInt
Pileup_pudensity
Pileup_gpudensity
Pileup_nPU
Pileup_sumEOOT
Pileup_sumLOOT
/*
RawMET_phi
RawMET_pt
RawMET_sumEt

fixedGridRhoFastjetAll
fixedGridRhoFastjetCentral
fixedGridRhoFastjetCentralCalo
fixedGridRhoFastjetCentralChargedPileUp
fixedGridRhoFastjetCentralNeutral
nGenDressedLepton
GenDressedLepton_eta
GenDressedLepton_mass
GenDressedLepton_phi
GenDressedLepton_pt
GenDressedLepton_pdgId
GenDressedLepton_hasTauAnc
nGenIsolatedPhoton
GenIsolatedPhoton_eta
GenIsolatedPhoton_mass
GenIsolatedPhoton_phi
GenIsolatedPhoton_pt
nSoftActivityJet
SoftActivityJet_eta
SoftActivityJet_phi
SoftActivityJet_pt
SoftActivityJetHT
SoftActivityJetHT10
SoftActivityJetHT2
SoftActivityJetHT5
SoftActivityJetNjets10
SoftActivityJetNjets2
SoftActivityJetNjets5

nTau
Tau_chargedIso
Tau_dxy
Tau_dz
Tau_eta
Tau_leadTkDeltaEta
Tau_leadTkDeltaPhi
Tau_leadTkPtOverTauPt
Tau_mass
Tau_neutralIso
Tau_phi
Tau_photonsOutsideSignalCone
Tau_pt
Tau_puCorr
Tau_rawDeepTau2017v2p1VSe
Tau_rawDeepTau2017v2p1VSjet
Tau_rawDeepTau2017v2p1VSmu
Tau_rawIso
Tau_rawIsodR03
Tau_charge
Tau_decayMode
Tau_jetIdx
Tau_idAntiEleDeadECal
Tau_idAntiMu
Tau_idDecayModeOldDMs
Tau_idDeepTau2017v2p1VSe
Tau_idDeepTau2017v2p1VSjet
Tau_idDeepTau2017v2p1VSmu
TkMET_phi
TkMET_pt
TkMET_sumEt
nTrigObj
TrigObj_pt
TrigObj_eta
TrigObj_phi
TrigObj_l1pt
TrigObj_l1pt_2
TrigObj_l2pt
TrigObj_id
TrigObj_l1iso
TrigObj_l1charge
TrigObj_filterBits
genTtbarId
nOtherPV
OtherPV_z
PV_ndof
PV_x
PV_y
PV_z
PV_chi2
PV_score
PV_npvs
PV_npvsGood
nSV
SV_dlen
SV_dlenSig
SV_dxy
SV_dxySig
SV_pAngle
SV_charge
boostedTau_genPartIdx
boostedTau_genPartFlav
Electron_genPartIdx
Electron_genPartFlav
FatJet_genJetAK8Idx
FatJet_hadronFlavour
FatJet_nBHadrons
FatJet_nCHadrons
GenJetAK8_partonFlavour
GenJetAK8_hadronFlavour
GenJet_partonFlavour
GenJet_hadronFlavour
GenVtx_t0ex
LowPtElectron_genPartIdx
LowPtElectron_genPartFlav
Muon_genPartIdx
Muon_genPartFlav
Photon_genPartIdx
Photon_genPartFlav
MET_fiducialGenPhi
MET_fiducialGenPt
Electron_cleanmask
Jet_cleanmask
Muon_cleanmask
Photon_cleanmask
Tau_cleanmask
SubJet_hadronFlavour
SubJet_nBHadrons
SubJet_nCHadrons
SV_chi2
SV_eta
SV_mass
SV_ndof
SV_phi
SV_pt
SV_x
SV_y
SV_z
SV_ntracks
Tau_genPartIdx
Tau_genPartFlav
*/
/*
Flag_HBHENoiseFilter
Flag_HBHENoiseIsoFilter
Flag_CSCTightHaloFilter
Flag_CSCTightHaloTrkMuUnvetoFilter
Flag_CSCTightHalo2015Filter
Flag_globalTightHalo2016Filter
Flag_globalSuperTightHalo2016Filter
Flag_HcalStripHaloFilter
Flag_hcalLaserEventFilter
Flag_EcalDeadCellTriggerPrimitiveFilter
Flag_EcalDeadCellBoundaryEnergyFilter
Flag_ecalBadCalibFilter
Flag_goodVertices
Flag_eeBadScFilter
Flag_ecalLaserCorrFilter
Flag_trkPOGFilters
Flag_chargedHadronTrackResolutionFilter
Flag_muonBadTrackFilter
Flag_BadChargedCandidateFilter
Flag_BadPFMuonFilter
Flag_BadPFMuonDzFilter
Flag_hfNoisyHitsFilter
Flag_BadChargedCandidateSummer16Filter
Flag_BadPFMuonSummer16Filter
Flag_trkPOG_manystripclus53X
Flag_trkPOG_toomanystripclus53X
Flag_trkPOG_logErrorTooManyClusters
Flag_METFilters
L1Reco_step

*/

HLTriggerFirstPath
HLT_AK8PFJet360_TrimMass30
HLT_AK8PFJet380_TrimMass30
HLT_AK8PFJet400_TrimMass30
HLT_AK8PFJet420_TrimMass30
HLT_AK8PFHT750_TrimMass50
HLT_AK8PFHT800_TrimMass50
HLT_AK8PFHT850_TrimMass50
HLT_AK8PFHT900_TrimMass50
HLT_CaloJet500_NoJetID
HLT_CaloJet550_NoJetID
HLT_DoubleMu5_Upsilon_DoubleEle3_CaloIdL_TrackIdL
HLT_DoubleMu3_DoubleEle7p5_CaloIdL_TrackIdL_Upsilon
HLT_Trimuon5_3p5_2_Upsilon_Muon
HLT_TrimuonOpen_5_3p5_2_Upsilon_Muon
HLT_DoubleEle25_CaloIdL_MW
HLT_DoubleEle27_CaloIdL_MW
HLT_DoubleEle33_CaloIdL_MW
HLT_DoubleEle24_eta2p1_WPTight_Gsf
HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350
HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350
HLT_Ele27_Ele37_CaloIdL_MW
HLT_Mu27_Ele37_CaloIdL_MW
HLT_Mu37_Ele27_CaloIdL_MW
HLT_Mu37_TkMu27
HLT_DoubleMu4_3_Bs
HLT_DoubleMu4_3_Jpsi
HLT_DoubleMu4_JpsiTrk_Displaced
HLT_DoubleMu4_LowMassNonResonantTrk_Displaced
HLT_DoubleMu3_Trk_Tau3mu
HLT_DoubleMu3_TkMu_DsTau3Mu
HLT_DoubleMu4_PsiPrimeTrk_Displaced
HLT_DoubleMu4_Mass3p8_DZ_PFHT350
HLT_Mu3_PFJet40
HLT_Mu7p5_L2Mu2_Jpsi
HLT_Mu7p5_L2Mu2_Upsilon
HLT_Mu7p5_Track2_Jpsi
HLT_Mu7p5_Track3p5_Jpsi
HLT_Mu7p5_Track7_Jpsi
HLT_Mu7p5_Track2_Upsilon
HLT_Mu7p5_Track3p5_Upsilon
HLT_Mu7p5_Track7_Upsilon
HLT_Mu3_L1SingleMu5orSingleMu7
HLT_DoublePhoton33_CaloIdL
HLT_DoublePhoton70
HLT_DoublePhoton85
HLT_Ele20_WPTight_Gsf
HLT_Ele15_WPLoose_Gsf
HLT_Ele17_WPLoose_Gsf
HLT_Ele20_WPLoose_Gsf
HLT_Ele20_eta2p1_WPLoose_Gsf
HLT_DiEle27_WPTightCaloOnly_L1DoubleEG
HLT_Ele27_WPTight_Gsf
HLT_Ele28_WPTight_Gsf
HLT_Ele30_WPTight_Gsf
HLT_Ele32_WPTight_Gsf
HLT_Ele35_WPTight_Gsf
HLT_Ele35_WPTight_Gsf_L1EGMT
HLT_Ele38_WPTight_Gsf
HLT_Ele40_WPTight_Gsf
HLT_Ele32_WPTight_Gsf_L1DoubleEG
HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1
HLT_Ele24_eta2p1_WPTight_Gsf_MediumChargedIsoPFTauHPS30_eta2p1_CrossL1
HLT_Ele24_eta2p1_WPTight_Gsf_TightChargedIsoPFTauHPS30_eta2p1_CrossL1
HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_TightID_CrossL1
HLT_Ele24_eta2p1_WPTight_Gsf_MediumChargedIsoPFTauHPS30_eta2p1_TightID_CrossL1
HLT_Ele24_eta2p1_WPTight_Gsf_TightChargedIsoPFTauHPS30_eta2p1_TightID_CrossL1
HLT_HT450_Beamspot
HLT_HT300_Beamspot
HLT_ZeroBias_Beamspot
HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1
HLT_IsoMu20_eta2p1_MediumChargedIsoPFTauHPS27_eta2p1_CrossL1
HLT_IsoMu20_eta2p1_TightChargedIsoPFTauHPS27_eta2p1_CrossL1
HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_TightID_CrossL1
HLT_IsoMu20_eta2p1_MediumChargedIsoPFTauHPS27_eta2p1_TightID_CrossL1
HLT_IsoMu20_eta2p1_TightChargedIsoPFTauHPS27_eta2p1_TightID_CrossL1
HLT_IsoMu24_eta2p1_TightChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_CrossL1
HLT_IsoMu24_eta2p1_MediumChargedIsoPFTauHPS35_Trk1_TightID_eta2p1_Reg_CrossL1
HLT_IsoMu24_eta2p1_TightChargedIsoPFTauHPS35_Trk1_TightID_eta2p1_Reg_CrossL1
HLT_IsoMu24_eta2p1_MediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_CrossL1
HLT_IsoMu27_LooseChargedIsoPFTauHPS20_Trk1_eta2p1_SingleL1
HLT_IsoMu27_MediumChargedIsoPFTauHPS20_Trk1_eta2p1_SingleL1
HLT_IsoMu27_TightChargedIsoPFTauHPS20_Trk1_eta2p1_SingleL1
HLT_IsoMu20
HLT_IsoMu24
HLT_IsoMu24_eta2p1
HLT_IsoMu27
HLT_IsoMu30
HLT_UncorrectedJetE30_NoBPTX
HLT_UncorrectedJetE30_NoBPTX3BX
HLT_UncorrectedJetE60_NoBPTX3BX
HLT_UncorrectedJetE70_NoBPTX3BX
HLT_L1SingleMu18
HLT_L1SingleMu25
HLT_L2Mu10
HLT_L2Mu10_NoVertex_NoBPTX3BX
HLT_L2Mu10_NoVertex_NoBPTX
HLT_L2Mu45_NoVertex_3Sta_NoBPTX3BX
HLT_L2Mu40_NoVertex_3Sta_NoBPTX3BX
HLT_L2Mu50
HLT_L2Mu23NoVtx_2Cha
HLT_L2Mu23NoVtx_2Cha_CosmicSeed
HLT_DoubleL2Mu30NoVtx_2Cha_CosmicSeed_Eta2p4
HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4
HLT_DoubleL2Mu50
HLT_DoubleL2Mu23NoVtx_2Cha_CosmicSeed
HLT_DoubleL2Mu23NoVtx_2Cha_CosmicSeed_NoL2Matched
HLT_DoubleL2Mu25NoVtx_2Cha_CosmicSeed
HLT_DoubleL2Mu25NoVtx_2Cha_CosmicSeed_NoL2Matched
HLT_DoubleL2Mu25NoVtx_2Cha_CosmicSeed_Eta2p4
HLT_DoubleL2Mu23NoVtx_2Cha
HLT_DoubleL2Mu23NoVtx_2Cha_NoL2Matched
HLT_DoubleL2Mu25NoVtx_2Cha
HLT_DoubleL2Mu25NoVtx_2Cha_NoL2Matched
HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4
HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL
HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL
HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ
HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ
HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8
HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8
HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8
HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8
HLT_Mu25_TkMu0_Onia
HLT_Mu30_TkMu0_Psi
HLT_Mu30_TkMu0_Upsilon
HLT_Mu20_TkMu0_Phi
HLT_Mu25_TkMu0_Phi
HLT_Mu12
HLT_Mu15
HLT_Mu20
HLT_Mu27
HLT_Mu50
HLT_Mu55
HLT_OldMu100
HLT_TkMu100
HLT_DiPFJetAve40
HLT_DiPFJetAve60
HLT_DiPFJetAve80
HLT_DiPFJetAve140
HLT_DiPFJetAve200
HLT_DiPFJetAve260
HLT_DiPFJetAve320
HLT_DiPFJetAve400
HLT_DiPFJetAve500
HLT_DiPFJetAve60_HFJEC
HLT_DiPFJetAve80_HFJEC
HLT_DiPFJetAve100_HFJEC
HLT_DiPFJetAve160_HFJEC
HLT_DiPFJetAve220_HFJEC
HLT_DiPFJetAve300_HFJEC
HLT_AK8PFJet15
HLT_AK8PFJet25
HLT_AK8PFJet40
HLT_AK8PFJet60
HLT_AK8PFJet80
HLT_AK8PFJet140
HLT_AK8PFJet200
HLT_AK8PFJet260
HLT_AK8PFJet320
HLT_AK8PFJet400
HLT_AK8PFJet450
HLT_AK8PFJet500
HLT_AK8PFJet550
HLT_PFJet15
HLT_PFJet25
HLT_PFJet40
HLT_PFJet60
HLT_PFJet80
HLT_PFJet140
HLT_PFJet200
HLT_PFJet260
HLT_PFJet320
HLT_PFJet400
HLT_PFJet450
HLT_PFJet500
HLT_PFJet550
HLT_PFJetFwd15
HLT_PFJetFwd25
HLT_PFJetFwd40
HLT_PFJetFwd60
HLT_PFJetFwd80
HLT_PFJetFwd140
HLT_PFJetFwd200
HLT_PFJetFwd260
HLT_PFJetFwd320
HLT_PFJetFwd400
HLT_PFJetFwd450
HLT_PFJetFwd500
HLT_AK8PFJetFwd15
HLT_AK8PFJetFwd25
HLT_AK8PFJetFwd40
HLT_AK8PFJetFwd60
HLT_AK8PFJetFwd80
HLT_AK8PFJetFwd140
HLT_AK8PFJetFwd200
HLT_AK8PFJetFwd260
HLT_AK8PFJetFwd320
HLT_AK8PFJetFwd400
HLT_AK8PFJetFwd450
HLT_AK8PFJetFwd500
HLT_PFHT180
HLT_PFHT250
HLT_PFHT370
HLT_PFHT430
HLT_PFHT510
HLT_PFHT590
HLT_PFHT680
HLT_PFHT780
HLT_PFHT890
HLT_PFHT1050
HLT_PFHT500_PFMET100_PFMHT100_IDTight
HLT_PFHT500_PFMET110_PFMHT110_IDTight
HLT_PFHT700_PFMET85_PFMHT85_IDTight
HLT_PFHT700_PFMET95_PFMHT95_IDTight
HLT_PFHT800_PFMET75_PFMHT75_IDTight
HLT_PFHT800_PFMET85_PFMHT85_IDTight
HLT_PFMET110_PFMHT110_IDTight
HLT_PFMET120_PFMHT120_IDTight
HLT_PFMET130_PFMHT130_IDTight
HLT_PFMET140_PFMHT140_IDTight
HLT_PFMET100_PFMHT100_IDTight_CaloBTagDeepCSV_3p1
HLT_PFMET110_PFMHT110_IDTight_CaloBTagDeepCSV_3p1
HLT_PFMET120_PFMHT120_IDTight_CaloBTagDeepCSV_3p1
HLT_PFMET130_PFMHT130_IDTight_CaloBTagDeepCSV_3p1
HLT_PFMET140_PFMHT140_IDTight_CaloBTagDeepCSV_3p1
HLT_PFMET120_PFMHT120_IDTight_PFHT60
HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60
HLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60
HLT_PFMETTypeOne110_PFMHT110_IDTight
HLT_PFMETTypeOne120_PFMHT120_IDTight
HLT_PFMETTypeOne130_PFMHT130_IDTight
HLT_PFMETTypeOne140_PFMHT140_IDTight
HLT_PFMETNoMu110_PFMHTNoMu110_IDTight
HLT_PFMETNoMu120_PFMHTNoMu120_IDTight
HLT_PFMETNoMu130_PFMHTNoMu130_IDTight
HLT_PFMETNoMu140_PFMHTNoMu140_IDTight
HLT_MonoCentralPFJet80_PFMETNoMu110_PFMHTNoMu110_IDTight
HLT_MonoCentralPFJet80_PFMETNoMu120_PFMHTNoMu120_IDTight
HLT_MonoCentralPFJet80_PFMETNoMu130_PFMHTNoMu130_IDTight
HLT_MonoCentralPFJet80_PFMETNoMu140_PFMHTNoMu140_IDTight
HLT_L1ETMHadSeeds
HLT_CaloMHT90
HLT_CaloMET80_NotCleaned
HLT_CaloMET90_NotCleaned
HLT_CaloMET100_NotCleaned
HLT_CaloMET110_NotCleaned
HLT_CaloMET250_NotCleaned
HLT_CaloMET70_HBHECleaned
HLT_CaloMET80_HBHECleaned
HLT_CaloMET90_HBHECleaned
HLT_CaloMET100_HBHECleaned
HLT_CaloMET250_HBHECleaned
HLT_CaloMET300_HBHECleaned
HLT_CaloMET350_HBHECleaned
HLT_PFMET200_NotCleaned
HLT_PFMET200_HBHECleaned
HLT_PFMET250_HBHECleaned
HLT_PFMET300_HBHECleaned
HLT_PFMET200_HBHE_BeamHaloCleaned
HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned
HLT_MET105_IsoTrk50
HLT_MET120_IsoTrk50
HLT_SingleJet30_Mu12_SinglePFJet40
HLT_Mu12_DoublePFJets40_CaloBTagDeepCSV_p71
HLT_Mu12_DoublePFJets100_CaloBTagDeepCSV_p71
HLT_Mu12_DoublePFJets200_CaloBTagDeepCSV_p71
HLT_Mu12_DoublePFJets350_CaloBTagDeepCSV_p71
HLT_Mu12_DoublePFJets40MaxDeta1p6_DoubleCaloBTagDeepCSV_p71
HLT_Mu12_DoublePFJets54MaxDeta1p6_DoubleCaloBTagDeepCSV_p71
HLT_Mu12_DoublePFJets62MaxDeta1p6_DoubleCaloBTagDeepCSV_p71
HLT_DoublePFJets40_CaloBTagDeepCSV_p71
HLT_DoublePFJets100_CaloBTagDeepCSV_p71
HLT_DoublePFJets200_CaloBTagDeepCSV_p71
HLT_DoublePFJets350_CaloBTagDeepCSV_p71
HLT_DoublePFJets116MaxDeta1p6_DoubleCaloBTagDeepCSV_p71
HLT_DoublePFJets128MaxDeta1p6_DoubleCaloBTagDeepCSV_p71
HLT_Photon300_NoHE
HLT_Mu8_TrkIsoVVL
HLT_Mu8_DiEle12_CaloIdL_TrackIdL_DZ
HLT_Mu8_DiEle12_CaloIdL_TrackIdL
HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ
HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350
HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ
HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30
HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_CaloDiJet30
HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30_PFBtagDeepCSV_1p5
HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_CaloDiJet30_CaloBtagDeepCSV_1p5
HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL
HLT_Mu17_TrkIsoVVL
HLT_Mu19_TrkIsoVVL
HLT_BTagMu_AK4DiJet20_Mu5
HLT_BTagMu_AK4DiJet40_Mu5
HLT_BTagMu_AK4DiJet70_Mu5
HLT_BTagMu_AK4DiJet110_Mu5
HLT_BTagMu_AK4DiJet170_Mu5
HLT_BTagMu_AK4Jet300_Mu5
HLT_BTagMu_AK8DiJet170_Mu5
HLT_BTagMu_AK8Jet170_DoubleMu5
HLT_BTagMu_AK8Jet300_Mu5
HLT_BTagMu_AK4DiJet20_Mu5_noalgo
HLT_BTagMu_AK4DiJet40_Mu5_noalgo
HLT_BTagMu_AK4DiJet70_Mu5_noalgo
HLT_BTagMu_AK4DiJet110_Mu5_noalgo
HLT_BTagMu_AK4DiJet170_Mu5_noalgo
HLT_BTagMu_AK4Jet300_Mu5_noalgo
HLT_BTagMu_AK8DiJet170_Mu5_noalgo
HLT_BTagMu_AK8Jet170_DoubleMu5_noalgo
HLT_BTagMu_AK8Jet300_Mu5_noalgo
HLT_Ele15_Ele8_CaloIdL_TrackIdL_IsoVL
HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ
HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL
HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ
HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL
HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL
HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ
HLT_Mu12_DoublePhoton20
HLT_TriplePhoton_20_20_20_CaloIdLV2
HLT_TriplePhoton_20_20_20_CaloIdLV2_R9IdVL
HLT_TriplePhoton_30_30_10_CaloIdLV2
HLT_TriplePhoton_30_30_10_CaloIdLV2_R9IdVL
HLT_TriplePhoton_35_35_5_CaloIdLV2_R9IdVL
HLT_Photon20
HLT_Photon33
HLT_Photon50
HLT_Photon75
HLT_Photon90
HLT_Photon120
HLT_Photon150
HLT_Photon175
HLT_Photon200
HLT_Photon100EB_TightID_TightIso
HLT_Photon110EB_TightID_TightIso
HLT_Photon120EB_TightID_TightIso
HLT_Photon100EBHE10
HLT_Photon100EEHE10
HLT_Photon100EE_TightID_TightIso
HLT_Photon50_R9Id90_HE10_IsoM
HLT_Photon75_R9Id90_HE10_IsoM
HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3
HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3
HLT_Photon90_R9Id90_HE10_IsoM
HLT_Photon120_R9Id90_HE10_IsoM
HLT_Photon165_R9Id90_HE10_IsoM
HLT_Photon90_CaloIdL_PFHT700
HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90
HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95
HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_PixelVeto_Mass55
HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_NoPixelVeto_Mass55
HLT_Photon35_TwoProngs35
HLT_IsoMu24_TwoProngs35
HLT_Dimuon0_Jpsi_L1_NoOS
HLT_Dimuon0_Jpsi_NoVertexing_NoOS
HLT_Dimuon0_Jpsi
HLT_Dimuon0_Jpsi_NoVertexing
HLT_Dimuon0_Jpsi_L1_4R_0er1p5R
HLT_Dimuon0_Jpsi_NoVertexing_L1_4R_0er1p5R
HLT_Dimuon0_Jpsi3p5_Muon2
HLT_Dimuon0_Upsilon_L1_4p5
HLT_Dimuon0_Upsilon_L1_5
HLT_Dimuon0_Upsilon_L1_4p5NoOS
HLT_Dimuon0_Upsilon_L1_4p5er2p0
HLT_Dimuon0_Upsilon_L1_4p5er2p0M
HLT_Dimuon0_Upsilon_NoVertexing
HLT_Dimuon0_Upsilon_L1_5M
HLT_Dimuon0_LowMass_L1_0er1p5R
HLT_Dimuon0_LowMass_L1_0er1p5
HLT_Dimuon0_LowMass
HLT_Dimuon0_LowMass_L1_4
HLT_Dimuon0_LowMass_L1_4R
HLT_Dimuon0_LowMass_L1_TM530
HLT_Dimuon0_Upsilon_Muon_L1_TM0
HLT_Dimuon0_Upsilon_Muon_NoL1Mass
HLT_TripleMu_5_3_3_Mass3p8_DZ
HLT_TripleMu_10_5_5_DZ
HLT_TripleMu_12_10_5
HLT_Tau3Mu_Mu7_Mu1_TkMu1_Tau15
HLT_Tau3Mu_Mu7_Mu1_TkMu1_Tau15_Charge1
HLT_Tau3Mu_Mu7_Mu1_TkMu1_IsoTau15
HLT_Tau3Mu_Mu7_Mu1_TkMu1_IsoTau15_Charge1
HLT_DoubleMu3_DZ_PFMET50_PFMHT60
HLT_DoubleMu3_DZ_PFMET70_PFMHT70
HLT_DoubleMu3_DZ_PFMET90_PFMHT90
HLT_DoubleMu3_Trk_Tau3mu_NoL1Mass
HLT_DoubleMu4_Jpsi_Displaced
HLT_DoubleMu4_Jpsi_NoVertexing
HLT_DoubleMu4_JpsiTrkTrk_Displaced
HLT_DoubleMu43NoFiltersNoVtx
HLT_DoubleMu48NoFiltersNoVtx
HLT_Mu43NoFiltersNoVtx_Photon43_CaloIdL
HLT_Mu48NoFiltersNoVtx_Photon48_CaloIdL
HLT_Mu38NoFiltersNoVtxDisplaced_Photon38_CaloIdL
HLT_Mu43NoFiltersNoVtxDisplaced_Photon43_CaloIdL
HLT_DoubleMu33NoFiltersNoVtxDisplaced
HLT_DoubleMu40NoFiltersNoVtxDisplaced
HLT_DoubleMu20_7_Mass0to30_L1_DM4
HLT_DoubleMu20_7_Mass0to30_L1_DM4EG
HLT_HT425
HLT_HT430_DisplacedDijet40_DisplacedTrack
HLT_HT500_DisplacedDijet40_DisplacedTrack
HLT_HT430_DisplacedDijet60_DisplacedTrack
HLT_HT400_DisplacedDijet40_DisplacedTrack
HLT_HT650_DisplacedDijet60_Inclusive
HLT_HT550_DisplacedDijet60_Inclusive
HLT_DiJet110_35_Mjj650_PFMET110
HLT_DiJet110_35_Mjj650_PFMET120
HLT_DiJet110_35_Mjj650_PFMET130
HLT_TripleJet110_35_35_Mjj650_PFMET110
HLT_TripleJet110_35_35_Mjj650_PFMET120
HLT_TripleJet110_35_35_Mjj650_PFMET130
HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned
HLT_Ele28_eta2p1_WPTight_Gsf_HT150
HLT_Ele28_HighEta_SC20_Mass55
HLT_DoubleMu20_7_Mass0to30_Photon23
HLT_Ele15_IsoVVVL_PFHT450_CaloBTagDeepCSV_4p5
HLT_Ele15_IsoVVVL_PFHT450_PFMET50
HLT_Ele15_IsoVVVL_PFHT450
HLT_Ele50_IsoVVVL_PFHT450
HLT_Ele15_IsoVVVL_PFHT600
HLT_Mu4_TrkIsoVVL_DiPFJet90_40_DEta3p5_MJJ750_HTT300_PFMETNoMu60
HLT_Mu8_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT300_PFMETNoMu60
HLT_Mu10_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT350_PFMETNoMu60
HLT_Mu15_IsoVVVL_PFHT450_CaloBTagDeepCSV_4p5
HLT_Mu15_IsoVVVL_PFHT450_PFMET50
HLT_Mu15_IsoVVVL_PFHT450
HLT_Mu50_IsoVVVL_PFHT450
HLT_Mu15_IsoVVVL_PFHT600
HLT_Mu3er1p5_PFJet100er2p5_PFMET70_PFMHT70_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMET80_PFMHT80_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMET90_PFMHT90_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMET100_PFMHT100_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu70_PFMHTNoMu70_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu80_PFMHTNoMu80_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu90_PFMHTNoMu90_IDTight
HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu100_PFMHTNoMu100_IDTight
HLT_Dimuon10_PsiPrime_Barrel_Seagulls
HLT_Dimuon20_Jpsi_Barrel_Seagulls
HLT_Dimuon12_Upsilon_y1p4
HLT_Dimuon14_Phi_Barrel_Seagulls
HLT_Dimuon18_PsiPrime
HLT_Dimuon25_Jpsi
HLT_Dimuon18_PsiPrime_noCorrL1
HLT_Dimuon24_Upsilon_noCorrL1
HLT_Dimuon24_Phi_noCorrL1
HLT_Dimuon25_Jpsi_noCorrL1
HLT_DiMu4_Ele9_CaloIdL_TrackIdL_DZ_Mass3p8
HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ
HLT_DiMu9_Ele9_CaloIdL_TrackIdL
HLT_DoubleIsoMu20_eta2p1
HLT_TrkMu12_DoubleTrkMu5NoFiltersNoVtx
HLT_TrkMu16_DoubleTrkMu6NoFiltersNoVtx
HLT_TrkMu17_DoubleTrkMu8NoFiltersNoVtx
HLT_Mu8
HLT_Mu17
HLT_Mu19
HLT_Mu17_Photon30_IsoCaloId
HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30
HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30
HLT_Ele15_CaloIdL_TrackIdL_IsoVL_PFJet30
HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30
HLT_Ele8_CaloIdM_TrackIdM_PFJet30
HLT_Ele17_CaloIdM_TrackIdM_PFJet30
HLT_Ele23_CaloIdM_TrackIdM_PFJet30
HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165
HLT_Ele115_CaloIdVT_GsfTrkIdT
HLT_Ele135_CaloIdVT_GsfTrkIdT
HLT_Ele145_CaloIdVT_GsfTrkIdT
HLT_Ele200_CaloIdVT_GsfTrkIdT
HLT_Ele250_CaloIdVT_GsfTrkIdT
HLT_Ele300_CaloIdVT_GsfTrkIdT
HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5
HLT_PFHT330PT30_QuadPFJet_75_60_45_40
HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94
HLT_PFHT400_SixPFJet32
HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59
HLT_PFHT450_SixPFJet36
HLT_PFHT350
HLT_PFHT350MinPFJet15
HLT_Photon60_R9Id90_CaloIdL_IsoL
HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL
HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15
HLT_ECALHT800
HLT_DiSC30_18_EIso_AND_HE_Mass70
HLT_Physics
HLT_Physics_part0
HLT_Physics_part1
HLT_Physics_part2
HLT_Physics_part3
HLT_Physics_part4
HLT_Physics_part5
HLT_Physics_part6
HLT_Physics_part7
HLT_Random
HLT_ZeroBias
HLT_ZeroBias_Alignment
HLT_ZeroBias_part0
HLT_ZeroBias_part1
HLT_ZeroBias_part2
HLT_ZeroBias_part3
HLT_ZeroBias_part4
HLT_ZeroBias_part5
HLT_ZeroBias_part6
HLT_ZeroBias_part7
HLT_AK4CaloJet30
HLT_AK4CaloJet40
HLT_AK4CaloJet50
HLT_AK4CaloJet80
HLT_AK4CaloJet100
HLT_AK4CaloJet120
HLT_AK4PFJet30
HLT_AK4PFJet50
HLT_AK4PFJet80
HLT_AK4PFJet100
HLT_AK4PFJet120
HLT_SinglePhoton10_Eta3p1ForPPRef
HLT_SinglePhoton20_Eta3p1ForPPRef
HLT_SinglePhoton30_Eta3p1ForPPRef
HLT_Photon20_HoverELoose
HLT_Photon30_HoverELoose
HLT_EcalCalibration
HLT_HcalCalibration
HLT_L1UnpairedBunchBptxMinus
HLT_L1UnpairedBunchBptxPlus
HLT_L1NotBptxOR
HLT_L1_CDC_SingleMu_3_er1p2_TOP120_DPHI2p618_3p142
HLT_CDC_L2cosmic_5_er1p0
HLT_CDC_L2cosmic_5p5_er1p0
HLT_HcalNZS
HLT_HcalPhiSym
HLT_HcalIsolatedbunch
HLT_IsoTrackHB
HLT_IsoTrackHE
HLT_ZeroBias_FirstCollisionAfterAbortGap
HLT_ZeroBias_IsolatedBunches
HLT_ZeroBias_FirstCollisionInTrain
HLT_ZeroBias_LastCollisionInTrain
HLT_ZeroBias_FirstBXAfterTrain
HLT_IsoMu24_eta2p1_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET90
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET100
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET110
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET120
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET130
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET140
HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr
HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_1pr
HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1
HLT_MediumChargedIsoPFTau200HighPtRelaxedIso_Trk50_eta2p1
HLT_MediumChargedIsoPFTau220HighPtRelaxedIso_Trk50_eta2p1
HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL
HLT_Rsq0p35
HLT_Rsq0p40
HLT_RsqMR300_Rsq0p09_MR200
HLT_RsqMR320_Rsq0p09_MR200
HLT_RsqMR300_Rsq0p09_MR200_4jet
HLT_RsqMR320_Rsq0p09_MR200_4jet
HLT_IsoMu27_MET90
HLT_DoubleTightChargedIsoPFTauHPS35_Trk1_eta2p1_Reg
HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_TightID_eta2p1_Reg
HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg
HLT_DoubleTightChargedIsoPFTauHPS35_Trk1_TightID_eta2p1_Reg
HLT_DoubleMediumChargedIsoPFTauHPS40_Trk1_eta2p1_Reg
HLT_DoubleTightChargedIsoPFTauHPS40_Trk1_eta2p1_Reg
HLT_DoubleMediumChargedIsoPFTauHPS40_Trk1_TightID_eta2p1_Reg
HLT_DoubleTightChargedIsoPFTauHPS40_Trk1_TightID_eta2p1_Reg
HLT_VBF_DoubleLooseChargedIsoPFTauHPS20_Trk1_eta2p1
HLT_VBF_DoubleMediumChargedIsoPFTauHPS20_Trk1_eta2p1
HLT_VBF_DoubleTightChargedIsoPFTauHPS20_Trk1_eta2p1
HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50
HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3
HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3
HLT_PFMET100_PFMHT100_IDTight_PFHT60
HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_PFHT60
HLT_PFMETTypeOne100_PFMHT100_IDTight_PFHT60
HLT_Mu18_Mu9_SameSign
HLT_Mu18_Mu9_SameSign_DZ
HLT_Mu18_Mu9
HLT_Mu18_Mu9_DZ
HLT_Mu20_Mu10_SameSign
HLT_Mu20_Mu10_SameSign_DZ
HLT_Mu20_Mu10
HLT_Mu20_Mu10_DZ
HLT_Mu23_Mu12_SameSign
HLT_Mu23_Mu12_SameSign_DZ
HLT_Mu23_Mu12
HLT_Mu23_Mu12_DZ
HLT_DoubleMu2_Jpsi_DoubleTrk1_Phi1p05
HLT_DoubleMu2_Jpsi_DoubleTkMu0_Phi
HLT_DoubleMu3_DCA_PFMET50_PFMHT60
HLT_TripleMu_5_3_3_Mass3p8_DCA
HLT_QuadPFJet98_83_71_15_DoublePFBTagDeepCSV_1p3_7p7_VBF1
HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepCSV_1p3_7p7_VBF1
HLT_QuadPFJet111_90_80_15_DoublePFBTagDeepCSV_1p3_7p7_VBF1
HLT_QuadPFJet98_83_71_15_PFBTagDeepCSV_1p3_VBF2
HLT_QuadPFJet103_88_75_15_PFBTagDeepCSV_1p3_VBF2
HLT_QuadPFJet105_88_76_15_PFBTagDeepCSV_1p3_VBF2
HLT_QuadPFJet111_90_80_15_PFBTagDeepCSV_1p3_VBF2
HLT_QuadPFJet98_83_71_15
HLT_QuadPFJet103_88_75_15
HLT_QuadPFJet105_88_76_15
HLT_QuadPFJet111_90_80_15
HLT_AK8PFJet330_TrimMass30_PFAK8BTagDeepCSV_p17
HLT_AK8PFJet330_TrimMass30_PFAK8BTagDeepCSV_p1
HLT_AK8PFJet330_TrimMass30_PFAK8BoostedDoubleB_p02
HLT_AK8PFJet330_TrimMass30_PFAK8BoostedDoubleB_np2
HLT_AK8PFJet330_TrimMass30_PFAK8BoostedDoubleB_np4
HLT_Diphoton30_18_R9IdL_AND_HE_AND_IsoCaloId_NoPixelVeto_Mass55
HLT_Diphoton30_18_R9IdL_AND_HE_AND_IsoCaloId_NoPixelVeto
HLT_Mu12_IP6_part0
HLT_Mu12_IP6_part1
HLT_Mu12_IP6_part2
HLT_Mu12_IP6_part3
HLT_Mu12_IP6_part4
HLT_Mu9_IP5_part0
HLT_Mu9_IP5_part1
HLT_Mu9_IP5_part2
HLT_Mu9_IP5_part3
HLT_Mu9_IP5_part4
HLT_Mu7_IP4_part0
HLT_Mu7_IP4_part1
HLT_Mu7_IP4_part2
HLT_Mu7_IP4_part3
HLT_Mu7_IP4_part4
HLT_Mu9_IP4_part0
HLT_Mu9_IP4_part1
HLT_Mu9_IP4_part2
HLT_Mu9_IP4_part3
HLT_Mu9_IP4_part4
HLT_Mu8_IP5_part0
HLT_Mu8_IP5_part1
HLT_Mu8_IP5_part2
HLT_Mu8_IP5_part3
HLT_Mu8_IP5_part4
HLT_Mu8_IP6_part0
HLT_Mu8_IP6_part1
HLT_Mu8_IP6_part2
HLT_Mu8_IP6_part3
HLT_Mu8_IP6_part4
HLT_Mu9_IP6_part0
HLT_Mu9_IP6_part1
HLT_Mu9_IP6_part2
HLT_Mu9_IP6_part3
HLT_Mu9_IP6_part4
HLT_Mu8_IP3_part0
HLT_Mu8_IP3_part1
HLT_Mu8_IP3_part2
HLT_Mu8_IP3_part3
HLT_Mu8_IP3_part4
HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepCSV_1p3_7p7_VBF1
HLT_TrkMu6NoFiltersNoVtx
HLT_TrkMu16NoFiltersNoVtx
HLT_DoubleTrkMu_16_6_NoFiltersNoVtx
HLTriggerFinalPath
L1simulation_step

}
