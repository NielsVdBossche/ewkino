#ifndef NANOREADER_H
#define NANOREADER_H

#include "TreeReader.h"

class NanoReader : public TreeReader {
    public:
        NanoReader() = default;
        NanoReader(const std::string&, const std::string&);

        void initTree(const bool resetTriggersAndFilters = true) override;

        static const unsigned nEl_max = 20;
        static const unsigned nMu_Max = 20;
        
        Float_t        _El_dEscaleDown[nEl_max];
        Float_t        _El_dEscaleUp[nEl_max];
        Float_t        _El_dEsigmaDown[nEl_max];
        Float_t        _El_dEsigmaUp[nEl_max];
        Float_t        _El_deltaEtaSC[nEl_max];
        Float_t        _El_dr03EcalRecHitSumEt[nEl_max];
        Float_t        _El_dr03HcalDepth1TowerSumEt[nEl_max];
        Float_t        _El_dr03TkSumPt[nEl_max];
        Float_t        _El_dr03TkSumPtHEEP[nEl_max];
        Float_t        _El_dxy[nEl_max];
        Float_t        _El_dxyErr[nEl_max];
        Float_t        _El_dz[nEl_max];
        Float_t        _El_dzErr[nEl_max];
        Float_t        _El_eCorr[nEl_max];
        Float_t        _El_eInvMinusPInv[nEl_max];
        Float_t        _El_energyErr[nEl_max];
        Float_t        _El_eta[nEl_max];
        Float_t        _El_hoe[nEl_max];
        Float_t        _El_ip3d[nEl_max];
        Float_t        _El_jetPtRelv2[nEl_max];
        Float_t        _El_jetRelIso[nEl_max];
        Float_t        _El_mass[nEl_max];
        Float_t        _El_miniPFRelIso_all[nEl_max];
        Float_t        _El_miniPFRelIso_chg[nEl_max];
        Float_t        _El_mvaFall17V2Iso[nEl_max];
        Float_t        _El_mvaFall17V2noIso[nEl_max];
        Float_t        _El_pfRelIso03_all[nEl_max];
        Float_t        _El_pfRelIso03_chg[nEl_max];
        Float_t        _El_phi[nEl_max];
        Float_t        _El_pt[nEl_max];
        Float_t        _El_r9[nEl_max];
        Float_t        _El_scEtOverPt[nEl_max];
        Float_t        _El_sieie[nEl_max];
        Float_t        _El_sip3d[nEl_max];
        Float_t        _El_mvaTTH[nEl_max];
        Int_t          _El_charge[nEl_max];
        Int_t          _El_cutBased[nEl_max];
        Int_t          _El_jetIdx[nEl_max];
        Int_t          _El_pdgId[nEl_max];
        Int_t          _El_photonIdx[nEl_max];
        Int_t          _El_tightCharge[nEl_max];
        Int_t          _El_vidNestedWPBitmap[nEl_max];
        Int_t          _El_vidNestedWPBitmapHEEP[nEl_max];
        Bool_t         _El_convVeto[nEl_max];
        Bool_t         _El_cutBased_HEEP[nEl_max];
        Bool_t         _El_isPFcand[nEl_max];
        UChar_t        _El_jetNDauCharged[nEl_max];
        UChar_t        _El_lostHits[nEl_max];
        Bool_t         _El_mvaFall17V2Iso_WP80[nEl_max];
        Bool_t         _El_mvaFall17V2Iso_WP90[nEl_max];
        Bool_t         _El_mvaFall17V2Iso_WPL[nEl_max];
        Bool_t         _El_mvaFall17V2noIso_WP80[nEl_max];
        Bool_t         _El_mvaFall17V2noIso_WP90[nEl_max];
        Bool_t         _El_mvaFall17V2noIso_WPL[nEl_max];
        UChar_t        _El_seedGain[nEl_max];

        Float_t        _Mu_dxy[nMu_Max];
        Float_t        _Mu_dxyErr[nMu_Max];
        Float_t        _Mu_dxybs[nMu_Max];
        Float_t        _Mu_dz[nMu_Max];
        Float_t        _Mu_dzErr[nMu_Max];
        Float_t        _Mu_eta[nMu_Max];
        Float_t        _Mu_ip3d[nMu_Max];
        Float_t        _Mu_jetPtRelv2[nMu_Max];
        Float_t        _Mu_jetRelIso[nMu_Max];
        Float_t        _Mu_mass[nMu_Max];
        Float_t        _Mu_miniPFRelIso_all[nMu_Max];
        Float_t        _Mu_miniPFRelIso_chg[nMu_Max];
        Float_t        _Mu_pfRelIso03_all[nMu_Max];
        Float_t        _Mu_pfRelIso03_chg[nMu_Max];
        Float_t        _Mu_pfRelIso04_all[nMu_Max];
        Float_t        _Mu_phi[nMu_Max];
        Float_t        _Mu_pt[nMu_Max];
        Float_t        _Mu_ptErr[nMu_Max];
        Float_t        _Mu_segmentComp[nMu_Max];
        Float_t        _Mu_sip3d[nMu_Max];
        Float_t        _Mu_softMva[nMu_Max];
        Float_t        _Mu_tkRelIso[nMu_Max];
        Float_t        _Mu_tunepRelPt[nMu_Max];
        Float_t        _Mu_mvaLowPt[nMu_Max];
        Float_t        _Mu_mvaTTH[nMu_Max];
        Int_t          _Mu_charge[nMu_Max];
        Int_t          _Mu_jetIdx[nMu_Max];
        Int_t          _Mu_nStations[nMu_Max];
        Int_t          _Mu_nTrackerLayers[nMu_Max];
        Int_t          _Mu_pdgId[nMu_Max];
        Int_t          _Mu_tightCharge[nMu_Max];
        Int_t          _Mu_fsrPhotonIdx[nMu_Max];
        UChar_t        _Mu_highPtId[nMu_Max];
        Bool_t         _Mu_highPurity[nMu_Max];
        Bool_t         _Mu_inTimeMuon[nMu_Max];
        Bool_t         _Mu_isGlobal[nMu_Max];
        Bool_t         _Mu_isPFcand[nMu_Max];
        Bool_t         _Mu_isStandalone[nMu_Max];
        Bool_t         _Mu_isTracker[nMu_Max];
        UChar_t        _Mu_jetNDauCharged[nMu_Max];
        Bool_t         _Mu_looseId[nMu_Max];
        Bool_t         _Mu_mediumId[nMu_Max];
        Bool_t         _Mu_mediumPromptId[nMu_Max];
        UChar_t        _Mu_miniIsoId[nMu_Max];
        UChar_t        _Mu_multiIsoId[nMu_Max];
        UChar_t        _Mu_mvaId[nMu_Max];
        UChar_t        _Mu_mvaLowPtId[nMu_Max];
        UChar_t        _Mu_pfIsoId[nMu_Max];
        UChar_t        _Mu_puppiIsoId[nMu_Max];
        Bool_t         _Mu_softId[nMu_Max];
        Bool_t         _Mu_softMvaId[nMu_Max];
        Bool_t         _Mu_tightId[nMu_Max];
        Bool_t         _Mu_tkIsoId[nMu_Max];
        Bool_t         _Mu_triggerIdLoose[nMu_Max];


        Float_t        _Jet_area[nJets_max];
        Float_t        _Jet_btagCSVV2[nJets_max];
        Float_t        _Jet_btagDeepB[nJets_max];
        Float_t        _Jet_btagDeepCvB[nJets_max];
        Float_t        _Jet_btagDeepCvL[nJets_max];
        Float_t        _Jet_btagDeepFlavB[nJets_max];
        Float_t        _Jet_btagDeepFlavCvB[nJets_max];
        Float_t        _Jet_btagDeepFlavCvL[nJets_max];
        Float_t        _Jet_btagDeepFlavQG[nJets_max];
        Float_t        _Jet_chEmEF[nJets_max];
        Float_t        _Jet_chFPV0EF[nJets_max];
        Float_t        _Jet_chHEF[nJets_max];
        Float_t        _Jet_eta[nJets_max];
        Float_t        _Jet_hfsigmaEtaEta[nJets_max];
        Float_t        _Jet_hfsigmaPhiPhi[nJets_max];
        Float_t        _Jet_mass[nJets_max];
        Float_t        _Jet_muEF[nJets_max];
        Float_t        _Jet_muonSubtrFactor[nJets_max];
        Float_t        _Jet_neEmEF[nJets_max];
        Float_t        _Jet_neHEF[nJets_max];
        Float_t        _Jet_phi[nJets_max];
        Float_t        _Jet_pt[nJets_max];
        Float_t        _Jet_puIdDisc[nJets_max];
        Float_t        _Jet_qgl[nJets_max];
        Float_t        _Jet_rawFactor[nJets_max];
        Float_t        _Jet_bRegCorr[nJets_max];
        Float_t        _Jet_bRegRes[nJets_max];
        Float_t        _Jet_cRegCorr[nJets_max];
        Float_t        _Jet_cRegRes[nJets_max];
        Int_t          _Jet_electronIdx1[nJets_max];
        Int_t          _Jet_electronIdx2[nJets_max];
        Int_t          _Jet_hfadjacentEtaStripsSize[nJets_max];
        Int_t          _Jet_hfcentralEtaStripSize[nJets_max];
        Int_t          _Jet_jetId[nJets_max];
        Int_t          _Jet_muonIdx1[nJets_max];
        Int_t          _Jet_muonIdx2[nJets_max];
        Int_t          _Jet_nElectrons[nJets_max];
        Int_t          _Jet_nMuons[nJets_max];
        Int_t          _Jet_puId[nJets_max];
        UChar_t        _Jet_nConstituents[nJets_max];

        Int_t          _Jet_genJetIdx[nJets_max];
        Int_t          _Jet_hadronFlavour[nJets_max]; // hadron ghost clustering
        Int_t          _Jet_partonFlavour[nJets_max]; // parton matching

};

#endif