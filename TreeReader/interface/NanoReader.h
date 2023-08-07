#ifndef NANOREADER_H
#define NANOREADER_H

#include "TreeReader.h"

class NanoReader : public TreeReader {
    public:
        NanoReader() = default;
        NanoReader(const std::string&, const std::string&);

        void initTree(const bool resetTriggersAndFilters = true) override;
        
        class LeptonReader {
            public:
                LeptonReader(NanoReader&, TTree*, std::string);
                static const unsigned nLepton_max = 20;

                // universal for LorentzVector
                UInt_t          _nLepton;
                Float_t         _Lepton_pt[nLepton_max];
                Float_t         _Lepton_eta[nLepton_max];
                Float_t         _Lepton_phi[nLepton_max];
                Int_t           _Lepton_charge[nLepton_max];
                // vtx information, general
                Float_t         _Lepton_dxy[nLepton_max];
                Float_t         _Lepton_dz[nLepton_max];
                // gen particles
                UChar_t         _Lepton_genPartFlav[nLepton_max];
                Int_t           _Lepton_genPartIdx[nLepton_max];
                // jet-lepton variables
                Int_t           _Lepton_jetIdx[nLepton_max];

                //virtual void initTree(TTree* tree, std::string leptonType);
                virtual void setOutputTree(TTree* tree, std::string leptonType);

                const NanoReader& GetNanoReader() const {return nanoReader;}
            private:
                NanoReader& nanoReader;

                TBranch *b__nLepton, *b__Lepton_pt, *b__Lepton_eta, *b__Lepton_phi, *b__Lepton_charge, 
                        *b__Lepton_dxy, *b__Lepton_dz, *b__Lepton_jetIdx, *b__Lepton_genPartFlav, *b__Lepton_genPartIdx;
        };

        class LightLeptonReader : public LeptonReader {
            public:
                LightLeptonReader(NanoReader&, TTree*, std::string);

                Float_t         _Lepton_sip3d[nLepton_max];
                // Isolation variables
                Float_t         _Lepton_pfRelIso03_all[nLepton_max];
                Float_t         _Lepton_miniPFRelIso_all[nLepton_max];
                Float_t         _Lepton_miniPFRelIso_chg[nLepton_max];
                // jet-lepton variables
                Float_t         _Lepton_jetPtRelv2[nLepton_max];
                Float_t         _Lepton_jetRelIso[nLepton_max];

                // other variables
                Bool_t          _Lepton_isPFCand[nLepton_max];
                UChar_t         _Lepton_jetNDauCharged[nLepton_max];
                
                virtual void initTree(TTree* tree, std::string leptonType);
                virtual void setOutputTree(TTree* tree, std::string leptonType);

            private:
                TBranch *b__Lepton_sip3d, *b__Lepton_pfRelIso03_all, *b__Lepton_miniPFRelIso_all, *b__Lepton_miniPFRelIso_chg, 
                        *b__Lepton_jetPtRelv2, *b__Lepton_jetRelIso, *b__Lepton_isPFCand, *b__Lepton_jetNDauCharged;
        };

        static const unsigned nElectron_max = 20;
        static const unsigned nMuon_max = 20;
        static const unsigned nTau_max = 20;
        static const unsigned nJet_max = 60;
        static const unsigned nGenPart_max = 250;
        static const unsigned nGenJet_max = 30;
        static const unsigned nLHEPdfWeight_max = 103;
        static const unsigned nLHEScaleWeight_max = 9;
        static const unsigned nPSWeight_max = 4;
        // note: variable types can be found here:
        // https://cms-nanoaod-integration.web.cern.ch/autoDoc/
        // global event variables and weights
        UInt_t _run;
        UInt_t _luminosityBlock;
        UInt_t _event;
        // generator info
        UInt_t _nLHEPdfWeight;
        UInt_t _nLHEScaleWeight;
        UInt_t _nPSWeight;
        Float_t _genWeight;
        Float_t _LHEPdfWeight[nLHEPdfWeight_max];
        Float_t _LHEScaleWeight[nLHEScaleWeight_max];
        Float_t _PSWeight[nPSWeight_max];
        // gen particles
        UInt_t _nGenPart;
        Float_t _GenPart_pt[nGenPart_max];
        Float_t _GenPart_eta[nGenPart_max];
        Float_t _GenPart_phi[nGenPart_max];
        Float_t _GenPart_mass[nGenPart_max];
        Int_t _GenPart_genPartIdxMother[nGenPart_max];
        Int_t _GenPart_pdgId[nGenPart_max];
        Int_t _GenPart_status[nGenPart_max];
        Int_t _GenPart_statusFlags[nGenPart_max];
        // gen MET
        Float_t _GenMET_pt;
        Float_t _GenMET_phi;
        // variables related to electrons
        Float_t         _Electron_deltaEtaSC[nElectron_max];
        Float_t         _Electron_dEscaleDown[nElectron_max];
        Float_t         _Electron_dEscaleUp[nElectron_max];
        Float_t         _Electron_dEsigmaDown[nElectron_max];
        Float_t         _Electron_dEsigmaUp[nElectron_max];
        Float_t         _Electron_eInvMinusPInv[nElectron_max];
        Float_t         _Electron_hoe[nElectron_max];
        Float_t         _Electron_mvaFall17V2Iso[nElectron_max];
        Float_t         _Electron_mvaFall17V2noIso[nElectron_max];
        Float_t         _Electron_pfRelIso03_all[nElectron_max];
        Float_t         _Electron_sieie[nElectron_max];
        Int_t           _Electron_cutBased[nElectron_max];
        Int_t           _Electron_tightCharge[nElectron_max];
        UChar_t         _Electron_lostHits[nElectron_max];
        Bool_t          _Electron_convVeto[nElectron_max];
        Bool_t          _Electron_mvaFall17V2noIso_WP80[nElectron_max];
        Bool_t          _Electron_mvaFall17V2noIso_WP90[nElectron_max];
        Bool_t          _Electron_mvaFall17V2noIso_WPL[nElectron_max];

        // variables related to muons
        Float_t         _Muon_ptErr[nMuon_max];
        Float_t         _Muon_pfRelIso04_all[nMuon_max];
	    Float_t         _Muon_segmentComp[nMuon_max];
        Bool_t          _Muon_looseId[nMuon_max];
        Bool_t          _Muon_mediumId[nMuon_max];
        Bool_t          _Muon_tightId[nMuon_max];
        Bool_t          _Muon_isGlobal[nMuon_max];
        Bool_t          _Muon_isTracker[nMuon_max];
        Bool_t          _Muon_isStandalone[nMuon_max];

        // variables related to jets
        Float_t         _Jet_bTagDeepB[nJet_max];
        Float_t         _Jet_bTagDeepFlavB[nJet_max];
        Float_t         _Jet_eta[nJet_max];
        Float_t         _Jet_phi[nJet_max];
        Float_t         _Jet_pt[nJet_max];
        Int_t           _Jet_hadronFlavor[nJet_max];
        Int_t           _Jet_jetId[nJet_max];
        UChar_t         _Jet_nConstituents[nJet_max];
        UInt_t          _nJet;
        // variables related to missing transverse energy
        Float_t         _MET_pt;
        Float_t         _MET_phi;
        // maps for passing triggers and metfilters
        std::map<std::string, Bool_t> _triggerMap;
        std::map<std::string, Bool_t> _METFilterMap;


        // methods
        bool containsGeneratorInfo() const;
        bool containsGenParticles() const;

        const LightLeptonReader& GetElectronReader() const {return *electronReader;}
        const LightLeptonReader& GetMuonReader() const {return *muonReader;}
        const LeptonReader& GetTauReader() const {return *tauReader;}

    private:
        // initialize triggerMap
        void initializeTriggerMap( TTree* );
        void initializeMETFilterMap( TTree* );


        LightLeptonReader*  electronReader;
        LightLeptonReader*  muonReader;
        LeptonReader*       tauReader;

        // list of branches
        TBranch* b__run;
        TBranch* b__luminosityBlock;
        TBranch* b__event;
        TBranch* b__genWeight;
        TBranch* b__nLHEPdfWeight;
        TBranch* b__LHEPdfWeight;
        TBranch* b__nLHEScaleWeight;
        TBranch* b__LHEScaleWeight;
        TBranch* b__nPSWeight;
        TBranch* b__PSWeight;
        TBranch* b__nGenPart;
        TBranch* b__GenPart_pt;
        TBranch* b__GenPart_eta;
        TBranch* b__GenPart_phi;
        TBranch* b__GenPart_mass;
        TBranch* b__GenPart_genPartIdxMother;
        TBranch* b__GenPart_pdgId;
        TBranch* b__GenPart_status;
        TBranch* b__GenPart_statusFlags;
        TBranch* b__GenMET_pt;
        TBranch* b__GenMET_phi;
        TBranch* b__nElectron;
        TBranch* b__Electron_pt;
        TBranch* b__Electron_eta;
        TBranch* b__Electron_phi;
        TBranch* b__Electron_charge;
        TBranch* b__Electron_dxy;
        TBranch* b__Electron_dz;
        TBranch* b__Electron_sip3d;
        TBranch* b__Electron_pfRelIso03_all;
        TBranch* b__Electron_miniPFRelIso_all;
        TBranch* b__Electron_miniPFRelIso_chg;
        TBranch* b__Electron_jetIdx;
        TBranch* b__Electron_jetPtRelv2;
        TBranch* b__Electron_jetRelIso;
        TBranch* b__Electron_mvaTTH;
        TBranch* b__Electron_tightCharge;
        TBranch* b__Electron_convVeto;
        TBranch* b__Electron_lostHits;
        TBranch* b__Electron_mvaFall17V2Iso;
        TBranch* b__Electron_mvaFall17V2noIso;
        TBranch* b__Electron_mvaFall17V2noIso_WPL;
        TBranch* b__Electron_mvaFall17V2noIso_WP80;
        TBranch* b__Electron_mvaFall17V2noIso_WP90;
        TBranch* b__Electron_deltaEtaSC;
        TBranch* b__Electron_eInvMinusPInv;
        TBranch* b__Electron_hoe;
        TBranch* b__Electron_sieie;
        TBranch* b__Electron_cutBased;
        TBranch* b__Electron_dEscaleDown;
        TBranch* b__Electron_dEscaleUp;
        TBranch* b__Electron_dEsigmaDown;
        TBranch* b__Electron_dEsigmaUp;
        TBranch* b__Electron_genPartFlav;
        TBranch* b__Electron_genPartIdx;
        TBranch* b__Electron_isPFCand;
        TBranch* b__Electron_jetNDauCharged;
        TBranch* b__nMuon;
        TBranch* b__Muon_pt;
        TBranch* b__Muon_eta;
        TBranch* b__Muon_phi;
        TBranch* b__Muon_charge;
        TBranch* b__Muon_dxy;
        TBranch* b__Muon_dz;
        TBranch* b__Muon_sip3d;
        TBranch* b__Muon_pfRelIso03_all;
        TBranch* b__Muon_miniPFRelIso_all;
        TBranch* b__Muon_miniPFRelIso_chg;
        TBranch* b__Muon_jetIdx;
        TBranch* b__Muon_jetPtRelv2;
        TBranch* b__Muon_jetRelIso;
        TBranch* b__Muon_mvaTTH;
        TBranch* b__Muon_segmentComp;
        TBranch* b__Muon_ptErr;
        TBranch* b__Muon_pfRelIso04_all;
        TBranch* b__Muon_looseId;
        TBranch* b__Muon_mediumId;
        TBranch* b__Muon_tightId;
        TBranch* b__Muon_genPartFlav;
        TBranch* b__Muon_genPartIdx;
        TBranch* b__Muon_isPFCand;
        TBranch* b__Muon_isGlobal;
        TBranch* b__Muon_isTracker;
        TBranch* b__Muon_isStandalone;
        TBranch* b__Muon_jetNDauCharged;
        TBranch* b__nTau;
        TBranch* b__Tau_pt;
        TBranch* b__Tau_eta;
        TBranch* b__Tau_phi;
        TBranch* b__Tau_charge;
        TBranch* b__Tau_dxy;
        TBranch* b__Tau_dz;
        TBranch* b__Tau_genPartFlav;
        TBranch* b__Tau_genPartIdx;
        TBranch* b__nJet;
        TBranch* b__Jet_pt;
        TBranch* b__Jet_eta;
        TBranch* b__Jet_phi;
        TBranch* b__Jet_bTagDeepB;
        TBranch* b__Jet_bTagDeepFlavB;
        TBranch* b__Jet_nConstituents;
        TBranch* b__Jet_hadronFlavor;
        TBranch* b__Jet_jetId;
        TBranch* b__MET_pt;
        TBranch* b__MET_phi;
        std::map<std::string, TBranch*> b__triggerMap;
        std::map<std::string, TBranch*> b__METFilterMap;
};

#endif