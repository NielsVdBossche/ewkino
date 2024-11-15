#ifndef NANOREADER_H
#define NANOREADER_H

#include "BaseReader.h"

class NanoReader : public BaseReader {
    public:
        NanoReader();
        NanoReader(const std::string&, const std::string&);

        // Tree manipulation
        void initTree(const bool resetTriggersAndFilters = true) override;
        // void initSample(const Sample& samp);
        void setOutputTree(TTree* ) override;

        
        class LeptonReader {
            public:
                LeptonReader(NanoReader&, std::string);
                virtual ~LeptonReader() = default;
                virtual bool isLightLeptonReader() const {return false;}

                static const unsigned nLepton_max = 40;

                // universal for LorentzVector
                UInt_t          _nLepton;
                Float_t         _Lepton_pt[nLepton_max];
                Float_t         _Lepton_eta[nLepton_max];
                Float_t         _Lepton_phi[nLepton_max];
                // Float_t         _Lepton_mass[nLepton_max];
                Int_t           _Lepton_charge[nLepton_max];
                // vtx information, general
                Float_t         _Lepton_dxy[nLepton_max];
                Float_t         _Lepton_dz[nLepton_max];
                // gen particles
                UChar_t         _Lepton_genPartFlav[nLepton_max];
                Int_t           _Lepton_genPartIdx[nLepton_max];
                // jet-lepton variables
                Int_t           _Lepton_jetIdx[nLepton_max];

                virtual void InitTree(TTree* _currentTreePtr);
                virtual void setOutputTree(TTree* tree);

                const NanoReader& GetNanoReader() const {return nanoReader;}
            protected:
                std::string* leptonType = nullptr;
            private:
                NanoReader& nanoReader;
                TBranch *b__nLepton, *b__Lepton_pt, *b__Lepton_eta, *b__Lepton_phi, *b__Lepton_mass, *b__Lepton_charge, 
                        *b__Lepton_dxy, *b__Lepton_dz, *b__Lepton_jetIdx, *b__Lepton_genPartFlav, *b__Lepton_genPartIdx;
        };

        class LightLeptonReader : public LeptonReader {
            public:
                LightLeptonReader(NanoReader&, std::string);
                virtual ~LightLeptonReader() = default;
                virtual bool isLightLeptonReader() const {return true;}

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

                // lepton MVA and related variables from nanoSkimmer:
                Float_t         _Lepton_TOPLeptonMVAUL[nLepton_max];
                Float_t         _Lepton_jetPtRatio[nLepton_max];
                Float_t         _Lepton_jetBTagDJ[nLepton_max];

                // Gen variables only defined for light leptons:
                Bool_t          _Lepton_isPrompt[nLepton_max];
                Bool_t          _Lepton_isChargeFlip[nLepton_max];
                Int_t           _Lepton_matchPdgId[nLepton_max];
                Int_t           _Lepton_motherPdgId[nLepton_max];
                UInt_t          _Lepton_provenanceConversion[nLepton_max];
                
                // virtual void initTree(TTree* tree, std::string leptonType);
                virtual void InitTree(TTree* tree);
                virtual void setOutputTree(TTree* tree);

            private:
                TBranch *b__Lepton_sip3d, *b__Lepton_pfRelIso03_all, *b__Lepton_miniPFRelIso_all, *b__Lepton_miniPFRelIso_chg, 
                        *b__Lepton_jetPtRelv2, *b__Lepton_jetRelIso, *b__Lepton_isPFCand, *b__Lepton_jetNDauCharged,
                        *b__Lepton_TOPLeptonMVAUL, *b__Lepton_jetPtRatio, *b__Lepton_jetBTagDJ,
                        *b__Lepton_isPrompt, *b__Lepton_isChargeFlip, *b__Lepton_matchPdgId, *b__Lepton_momPdgId, *b__Lepton_provenanceConversion;
        };

        static const unsigned nElectron_max = 40;
        static const unsigned nMuon_max = 40;
        static const unsigned nTau_max = 40;
        static const unsigned nJet_max = 60;
        static const unsigned nGenPart_max = 200; // NOTE: might be too low
        static const unsigned nLHEPart_max = 10;
        static const unsigned nGenJet_max = 30;
        static const unsigned nLHEPdfWeight_max = 103;
        static const unsigned nLHEScaleWeight_max = 9;
        static const unsigned nLHEReweightingWeight_max = 100;
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
        Float_t _L1PreFiringWeight_Nom;
        Float_t _L1PreFiringWeight_Up;
        Float_t _L1PreFiringWeight_Dn;

        UInt_t  _nLHEReweightingWeight;
        Float_t _LHEReweightingWeight[nLHEReweightingWeight_max];
        // Pileup
        Float_t _Pileup_nTrueInt;
        Int_t _PV_npvs;
        // gen particles
        UInt_t  _nGenPart;
        Float_t _GenPart_pt[nGenPart_max];
        Float_t _GenPart_eta[nGenPart_max];
        Float_t _GenPart_phi[nGenPart_max];
        Float_t _GenPart_mass[nGenPart_max];
        Int_t   _GenPart_genPartIdxMother[nGenPart_max];
        Int_t   _GenPart_pdgId[nGenPart_max];
        Int_t   _GenPart_status[nGenPart_max];
        Int_t   _GenPart_statusFlags[nGenPart_max];
        // LHE Particles
        UInt_t     _nLHEPart;
        Float_t    _LHEPart_pt[nLHEPart_max];
        Float_t    _LHEPart_eta[nLHEPart_max];
        Float_t    _LHEPart_phi[nLHEPart_max];
        Float_t    _LHEPart_mass[nLHEPart_max];
        Int_t      _LHEPart_status[nLHEPart_max];
        Int_t      _LHEPart_pdgId[nLHEPart_max];
        // GenJet variables (replacing PL jets, not sure if best option but ok)
        Float_t    _GenJet_eta[nGenJet_max];
        Float_t    _GenJet_mass[nGenJet_max];
        Float_t    _GenJet_phi[nGenJet_max];
        Float_t    _GenJet_pt[nGenJet_max];
        Int_t      _GenJet_partonFlavour[nGenJet_max];
        UChar_t    _GenJet_hadronFlavour[nGenJet_max];
        UInt_t     _nGenJet;
        // gen MET
        Float_t _GenMET_pt;
        Float_t _GenMET_phi;
        // variables related to electrons
        Float_t         _Electron_eCorr[nElectron_max];
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
        Float_t         _Muon_corrected_pt[nMuon_max];
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
        Float_t         _Jet_pt_nom[nJet_max];
        Float_t         _Jet_mass_nom[nJet_max];
        Int_t           _Jet_hadronFlavor[nJet_max];
        Int_t           _Jet_jetId[nJet_max];
        UChar_t         _Jet_nConstituents[nJet_max];
        UInt_t          _nJet;


        // default MET:
        Float_t         _MET_pt;
        Float_t         _MET_phi;
        // T1, no smear:
        Float_t         _MET_T1_pt;
        Float_t         _MET_T1_phi;

        // Smeared, JEC reapplied MET:
        Float_t         _MET_T1Smear_pt;
        Float_t         _MET_T1Smear_phi;
        // MET Unclustered:
        Float_t         _MET_T1Smear_pt_unclustEnUp;
        Float_t         _MET_T1Smear_phi_unclustEnUp;
        Float_t         _MET_T1Smear_pt_unclustEnDown;
        Float_t         _MET_T1Smear_phi_unclustEnDown;

        // Jet JER smearing: (var 0 and 1)
        Float_t         _Jet_pt_jer0Down[nJet_max];
        Float_t         _Jet_pt_jer0Up[nJet_max];
        Float_t         _Jet_pt_jer1Down[nJet_max];
        Float_t         _Jet_pt_jer1Up[nJet_max];
        Float_t         _Jet_mass_jer0Down[nJet_max];
        Float_t         _Jet_mass_jer0Up[nJet_max];
        Float_t         _Jet_mass_jer1Down[nJet_max];
        Float_t         _Jet_mass_jer1Up[nJet_max];

        // MET JER smearing:
        Float_t         _MET_T1Smear_pt_jer0Down;
        Float_t         _MET_T1Smear_phi_jer0Down;
        Float_t         _MET_T1Smear_pt_jer0Up;
        Float_t         _MET_T1Smear_phi_jer0Up;
        Float_t         _MET_T1Smear_pt_jer1Down;
        Float_t         _MET_T1Smear_phi_jer1Down;
        Float_t         _MET_T1Smear_pt_jer1Up;
        Float_t         _MET_T1Smear_phi_jer1Up;

        // Total JEC:
        Float_t         _Jet_pt_jesTotalUp[nJet_max];
        Float_t         _Jet_pt_jesTotalDown[nJet_max];
        Float_t         _Jet_mass_jesTotalUp[nJet_max];
        Float_t         _Jet_mass_jesTotalDown[nJet_max];
        Float_t         _MET_T1_pt_jesTotalUp;
        Float_t         _MET_T1_phi_jesTotalUp;
        Float_t         _MET_T1_pt_jesTotalDown;
        Float_t         _MET_T1_phi_jesTotalDown;
        Float_t         _MET_T1Smear_pt_jesTotalUp;
        Float_t         _MET_T1Smear_phi_jesTotalUp;
        Float_t         _MET_T1Smear_pt_jesTotalDown;
        Float_t         _MET_T1Smear_phi_jesTotalDown;

        // Individual JES Variations:
        std::map<std::string, Float_t[nJet_max]> _Jet_pt_jesSourcesUp;
        std::map<std::string, Float_t[nJet_max]> _Jet_pt_jesSourcesDown;
        std::map<std::string, Float_t[nJet_max]> _Jet_mass_jesSourcesUp;
        std::map<std::string, Float_t[nJet_max]> _Jet_mass_jesSourcesDown;
        std::map<std::string, Float_t>           _MET_T1Smear_pt_jesSourcesUp;
        std::map<std::string, Float_t>           _MET_T1Smear_pt_jesSourcesDown;
        std::map<std::string, Float_t>           _MET_T1Smear_phi_jesSourcesUp;
        std::map<std::string, Float_t>           _MET_T1Smear_phi_jesSourcesDown;
        std::map<std::string, std::string>      _Jet_pt_jesSourcesUpNames;
        std::map<std::string, std::string>      _Jet_pt_jesSourcesDownNames;
        std::map<std::string, std::string>      _Jet_mass_jesSourcesUpNames;
        std::map<std::string, std::string>      _Jet_mass_jesSourcesDownNames;
        std::map<std::string, std::string>      _MET_T1Smear_pt_jesSourcesUpNames;
        std::map<std::string, std::string>      _MET_T1Smear_pt_jesSourcesDownNames;
        std::map<std::string, std::string>      _MET_T1Smear_phi_jesSourcesUpNames;
        std::map<std::string, std::string>      _MET_T1Smear_phi_jesSourcesDownNames;

        // Trigger variables:
        Bool_t          _HLT_trigger_e;
        Bool_t          _HLT_trigger_ee;
        Bool_t          _HLT_trigger_eee;
        Bool_t          _HLT_trigger_m;
        Bool_t          _HLT_trigger_mm;
        Bool_t          _HLT_trigger_mmm;
        Bool_t          _HLT_trigger_em;
        Bool_t          _HLT_trigger_emm;
        Bool_t          _HLT_trigger_eem;
        Bool_t          _HLT_trigger_ref;
        Bool_t          _HLT_trigger_FR;
        Bool_t          _HLT_trigger_FR_iso;

        // MET Filters.
        // Never use the MET Filters flag:
        Bool_t          _Flag_METFilters;
        // Instead, load individual MET filters:
        // Individual flags instead:
        Bool_t          _Flag_goodVertices;
        Bool_t          _Flag_globalSuperTightHalo2016Filter;
        Bool_t          _Flag_HBHENoiseFilter;
        Bool_t          _Flag_HBHENoiseIsoFilter;
        Bool_t          _Flag_BadPFMuonFilter;
        Bool_t          _Flag_BadPFMuonDzFilter;
        Bool_t          _Flag_eeBadScFilter;
        // 2017 + 2018 only:
        Bool_t          _Flag_ecalBadCalibFilter;
        // 2016 only:
        Bool_t          _Flag_EcalDeadCellTriggerPrimitiveFilter;

        // methods
        virtual bool containsGeneratorInfo() const;
        virtual bool containsLheInfo() const;
        virtual bool hasPL() const;
        virtual bool hasGenLvl() const;
        virtual bool containsEFTInfo() const;

        const LightLeptonReader& GetElectronReader() const {return *electronReader;}
        const LightLeptonReader& GetMuonReader() const {return *muonReader;}
        const LeptonReader& GetTauReader() const {return *tauReader;}

        // Event building overrides:
        virtual Event buildEvent( const Sample&, long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false,
                const bool readGroupedJECVariations = false ) override;

        virtual Event* buildEventPtr( const Sample&, long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false,
                const bool readGroupedJECVariations = false ) override;

        virtual Event buildEvent( long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false, 
                const bool readGroupedJECVariations = false ) override;

        virtual Event* buildEventPtr( long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false, 
                const bool readGroupedJECVariations = false ) override;

    protected:
        virtual double getSumSimulatedEventWeights() override;
        virtual TTree* getTreePtr() override;
        virtual double getWeight() override {return _genWeight;};

        void initializeJESVariations( TTree* );

    private:
        LightLeptonReader*  electronReader = nullptr;
        LightLeptonReader*  muonReader = nullptr;
        LeptonReader*       tauReader = nullptr;

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
        TBranch* b__nLHEReweightingWeight;
        TBranch* b__LHEReweightingWeight;
        TBranch* b__L1PreFiringWeight_Nom;
        TBranch* b__L1PreFiringWeight_Up;
        TBranch* b__L1PreFiringWeight_Dn;
        TBranch* b__Pileup_nTrueInt;
        TBranch* b__PV_npvs;
        TBranch* b__nGenPart;
        TBranch* b__GenPart_pt;
        TBranch* b__GenPart_eta;
        TBranch* b__GenPart_phi;
        TBranch* b__GenPart_mass;
        TBranch* b__GenPart_genPartIdxMother;
        TBranch* b__GenPart_pdgId;
        TBranch* b__GenPart_status;
        TBranch* b__GenPart_statusFlags;
        TBranch* b__nLHEPart;
        TBranch* b__LHEPart_pt;
        TBranch* b__LHEPart_eta;
        TBranch* b__LHEPart_phi;
        TBranch* b__LHEPart_mass;
        TBranch* b__LHEPart_status;
        TBranch* b__LHEPart_pdgId;
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
        TBranch* b__Electron_eCorr;
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
        TBranch* b__Muon_corrected_pt;
        TBranch* b__Muon_ptErr;
        TBranch* b__Muon_pfRelIso04_all;
        TBranch* b__Muon_segmentComp;
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
        TBranch* b__Jet_pt_nom;
        TBranch* b__Jet_mass_nom;
        TBranch* b__Jet_eta;
        TBranch* b__Jet_phi;
        TBranch* b__Jet_bTagDeepB;
        TBranch* b__Jet_bTagDeepFlavB;
        TBranch* b__Jet_nConstituents;
        TBranch* b__Jet_hadronFlavor;
        TBranch* b__Jet_jetId;
        TBranch* b__GenJet_eta;
        TBranch* b__GenJet_mass;
        TBranch* b__GenJet_phi;
        TBranch* b__GenJet_pt;
        TBranch* b__GenJet_partonFlavour;
        TBranch* b__GenJet_hadronFlavour;
        TBranch* b__nGenJet;
        TBranch* b__MET_pt;
        TBranch* b__MET_phi;
        TBranch* b__MET_T1Smear_pt;
        TBranch* b__MET_T1Smear_phi;
        TBranch* b__MET_T1Smear_pt_unclustEnUp;
        TBranch* b__MET_T1Smear_phi_unclustEnUp;
        TBranch* b__MET_T1Smear_pt_unclustEnDown;
        TBranch* b__MET_T1Smear_phi_unclustEnDown;
        TBranch* b__Jet_pt_jesTotalUp;
        TBranch* b__Jet_pt_jesTotalDown;
        TBranch* b__Jet_mass_jesTotalUp;
        TBranch* b__Jet_mass_jesTotalDown;
        TBranch* b__MET_T1_pt_jesTotalUp;
        TBranch* b__MET_T1_phi_jesTotalUp;
        TBranch* b__MET_T1_pt_jesTotalDown;
        TBranch* b__MET_T1_phi_jesTotalDown;
        TBranch* b__MET_T1Smear_pt_jesTotalUp;
        TBranch* b__MET_T1Smear_phi_jesTotalUp;
        TBranch* b__MET_T1Smear_pt_jesTotalDown;
        TBranch* b__MET_T1Smear_phi_jesTotalDown;
        TBranch* b__MET_T1Smear_pt_jer0Down;
        TBranch* b__MET_T1Smear_phi_jer0Down;
        TBranch* b__MET_T1Smear_pt_jer0Up;
        TBranch* b__MET_T1Smear_phi_jer0Up;
        TBranch* b__MET_T1Smear_pt_jer1Down;
        TBranch* b__MET_T1Smear_phi_jer1Down;
        TBranch* b__MET_T1Smear_pt_jer1Up;
        TBranch* b__MET_T1Smear_phi_jer1Up;
        TBranch* b__Jet_pt_jer0Down;
        TBranch* b__Jet_pt_jer0Up;
        TBranch* b__Jet_pt_jer1Down;
        TBranch* b__Jet_pt_jer1Up;
        TBranch* b__Jet_mass_jer0Down;
        TBranch* b__Jet_mass_jer0Up;
        TBranch* b__Jet_mass_jer1Down;
        TBranch* b__Jet_mass_jer1Up;
        TBranch* b__HLT_trigger_e;
        TBranch* b__HLT_trigger_ee;
        TBranch* b__HLT_trigger_eee;
        TBranch* b__HLT_trigger_m;
        TBranch* b__HLT_trigger_mm;
        TBranch* b__HLT_trigger_mmm;
        TBranch* b__HLT_trigger_em;
        TBranch* b__HLT_trigger_emm;
        TBranch* b__HLT_trigger_eem;
        TBranch* b__HLT_trigger_ref;
        TBranch* b__HLT_trigger_FR;
        TBranch* b__HLT_trigger_FR_iso;
        TBranch* b__Flag_METFilters;
        TBranch* b__Flag_goodVertices;
        TBranch* b__Flag_globalSuperTightHalo2016Filter;
        TBranch* b__Flag_HBHENoiseFilter;
        TBranch* b__Flag_HBHENoiseIsoFilter;
        TBranch* b__Flag_BadPFMuonFilter;
        TBranch* b__Flag_BadPFMuonDzFilter;
        TBranch* b__Flag_eeBadScFilter;
        TBranch* b__Flag_ecalBadCalibFilter;
        TBranch* b__Flag_EcalDeadCellTriggerPrimitiveFilter;
        TBranch* b__MET_T1_pt;
        TBranch* b__MET_T1_phi;

        std::map< std::string, TBranch* > b__jet_pt_jesSourcesDown;
        std::map< std::string, TBranch* > b__jet_pt_jesSourcesUp;
        std::map< std::string, TBranch* > b__jet_mass_jesSourcesDown;
        std::map< std::string, TBranch* > b__jet_mass_jesSourcesUp;
        std::map< std::string, TBranch* > b__met_pt_jesSourcesDown;
        std::map< std::string, TBranch* > b__met_pt_jesSourcesUp;
        std::map< std::string, TBranch* > b__met_phi_jesSourcesDown;
        std::map< std::string, TBranch* > b__met_phi_jesSourcesUp;
};

#endif