#ifndef TreeReader_H
#define TreeReader_H

#include "BaseReader.h"

//include ROOT classes
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TLorentzVector.h"

//include other parts of code
#include "../../Tools/interface/Sample.h"


class Event;

std::pair<std::map<std::string, bool>, std::map<std::string, TBranch*> > buildBranchMap(
    TTree* treePtr, const std::vector<std::string> nameIdentifiers, const std::string& antiIdentifier = "");

template <typename T>
void setMapBranchAddresses(TTree* treePtr, std::map<std::string, T>& variableMap, std::map<std::string, TBranch*> branchMap);

class TreeReader : public BaseReader {
    public :

        //Constructor
        TreeReader();
        TreeReader( const std::string&, const std::string& ); 

        //Declare leaf types
	// constants
	// note: it should be checked that the values match the ones in the ntuples!
	//       if not, segmentation errors (or nonsensical values?) could arise.
	//       check the relevant part of the ntuplizer and its history,
	//       e.g. for leptons: https://github.com/GhentAnalysis/heavyNeutrino/blob/75854fc4b91855821b019fcae63a258c382384be/multilep/interface/LeptonAnalyzer.h#L45
        static const unsigned nL_max = 20;
        static const unsigned nJets_max = 100;
        static const unsigned gen_nL_max = 20;
        static const unsigned lhe_n_max = 20;
        static const unsigned gen_n_max = 1000;
	// global event variables and weights
        ULong_t         _runNb;
        ULong_t         _lumiBlock;
        ULong_t         _eventNb;
        UInt_t          _nVertex;
        Float_t         _prefireWeight;
        Float_t         _prefireWeightDown;
        Float_t         _prefireWeightUp;
        Double_t        _weight;
        UInt_t          _nLheWeights;
        Double_t        _lheWeight[148];
        UInt_t          _nPsWeights;
        Double_t        _psWeight[46];
        Float_t         _nTrueInt;
        Double_t        _lheHTIncoming;

        // EFT reinterpretation
        static const unsigned dynscale_n_max = 38;
        static const unsigned eft_n_max = 37;
        UInt_t          _nDynScaleWeights;
        Double_t        _dynScaleWeight[ dynscale_n_max ];
        UInt_t          _nEFTWeights;
        Double_t        _eftWeight[ eft_n_max ];

	// generator variables
        Double_t        _gen_met;
        Double_t        _gen_metPhi;
        UInt_t          _gen_nL;
        Double_t        _gen_lPt[gen_nL_max];   
        Double_t        _gen_lEta[gen_nL_max];   
        Double_t        _gen_lPhi[gen_nL_max];   
        Double_t        _gen_lE[gen_nL_max];   
        UInt_t          _gen_lFlavor[gen_nL_max];   
        Int_t           _gen_lCharge[gen_nL_max];   
        Int_t           _gen_lMomPdg[gen_nL_max];   
        Bool_t          _gen_lIsPrompt[gen_nL_max];   
        UInt_t          _ttgEventType;
        UInt_t          _zgEventType;

        // all generator variables
        UInt_t     _gen_n;
        Double_t   _gen_pt[gen_n_max];
        Double_t   _gen_eta[gen_n_max];
        Double_t   _gen_phi[gen_n_max];
        Double_t   _gen_E[gen_n_max];
        Int_t      _gen_pdgId[gen_n_max];
        Int_t      _gen_charge[gen_n_max];
        Int_t      _gen_status[gen_n_max];
        Bool_t     _gen_isPromptFinalState[gen_n_max];
        Bool_t     _gen_isDirectPromptTauDecayProductFinalState[gen_n_max];
        Bool_t     _gen_isLastCopy[gen_n_max];
        Int_t      _gen_index[gen_n_max];
        Int_t      _gen_motherIndex[gen_n_max];
        Int_t      _gen_daughter_n[gen_n_max];
        Int_t      _gen_daughterIndex[gen_n_max][10];

        // lhe variables
        UInt_t          _nLheTau;
        UInt_t          _nLheParticles;
        Int_t           _lheStatus[lhe_n_max];
        Int_t           _lhePdgId[lhe_n_max];
        Int_t           _lheMother1[lhe_n_max];
        Int_t           _lheMother2[lhe_n_max];
        Double_t        _lhePt[lhe_n_max];
        Double_t        _lheEta[lhe_n_max];
        Double_t        _lhePhi[lhe_n_max];
        Double_t        _lheE[lhe_n_max];
        Double_t        _lheMass[lhe_n_max];
	// triggers and filters
        Bool_t          _passTrigger_e;
        Bool_t          _passTrigger_ee;
        Bool_t          _passTrigger_eee;
        Bool_t          _passTrigger_em;
        Bool_t          _passTrigger_m;
        Bool_t          _passTrigger_eem;
        Bool_t          _passTrigger_mm;
        Bool_t          _passTrigger_emm;
        Bool_t          _passTrigger_mmm;
        Bool_t          _passTrigger_et;
        Bool_t          _passTrigger_mt;
        Bool_t          _passTrigger_FR;
        Bool_t          _passTrigger_FR_iso;
        Bool_t          _passMETFilters;
	// variables related to leptons
        UInt_t          _nL;
        UInt_t          _nMu;
        UInt_t          _nEle;
        UInt_t          _nLight;
        UInt_t          _nTau;
        Double_t        _lPt[nL_max];   
        Double_t        _lPtCorr[nL_max];
	Double_t	_lPtScaleUp[nL_max];
	Double_t	_lPtScaleDown[nL_max];
	Double_t	_lPtResUp[nL_max];
	Double_t	_lPtResDown[nL_max];
        Double_t        _lEta[nL_max];   
        Double_t        _lEtaSC[nL_max];   
        Double_t        _lPhi[nL_max];   
        Double_t        _lE[nL_max];   
        Double_t        _lECorr[nL_max];
	Double_t        _lEScaleUp[nL_max];
        Double_t        _lEScaleDown[nL_max];
        Double_t        _lEResUp[nL_max];
        Double_t        _lEResDown[nL_max];
        UInt_t          _lFlavor[nL_max];   
        Int_t           _lCharge[nL_max];   
        Double_t        _dxy[nL_max];   
        Double_t        _dz[nL_max];   
        Double_t        _3dIP[nL_max];   
        Double_t        _3dIPSig[nL_max];   
        Float_t         _lElectronSummer16MvaGP[nL_max];   
        Float_t         _lElectronSummer16MvaHZZ[nL_max];
        Float_t         _lElectronMvaFall17Iso[nL_max];
        Float_t         _lElectronMvaFall17NoIso[nL_max];
        Bool_t          _lElectronPassMVAFall17NoIsoWPLoose[nL_max];
        Bool_t          _lElectronPassMVAFall17NoIsoWP90[nL_max];
        Bool_t          _lElectronPassMVAFall17NoIsoWP80[nL_max];
        Bool_t          _lElectronPassEmu[nL_max];   
        Bool_t          _lElectronPassConvVeto[nL_max];
        Bool_t          _lElectronChargeConst[nL_max];
        UInt_t          _lElectronMissingHits[nL_max];
        Double_t        _lElectronEInvMinusPInv[nL_max];
        Double_t        _lElectronHOverE[nL_max];
        Double_t        _lElectronSigmaIetaIeta[nL_max];
        Double_t        _leptonMvaTTH[nL_max];
        Double_t        _leptonMvatZq[nL_max];
	Double_t        _leptonMvaTOP[nL_max];
	Double_t        _leptonMvaTOPUL[nL_max];
	Double_t        _leptonMvaTOPULv2[nL_max];
        Bool_t          _lPOGVeto[nL_max];   
        Bool_t          _lPOGLoose[nL_max];   
        Bool_t          _lPOGMedium[nL_max];   
        Bool_t          _lPOGTight[nL_max];   

        UInt_t          _tauDecayMode[nL_max];
        Bool_t          _decayModeFinding[nL_max];
        Bool_t          _decayModeFindingNew[nL_max];   
        Bool_t          _tauMuonVetoLoose[nL_max];
        Bool_t          _tauMuonVetoTight[nL_max];
        Bool_t          _tauEleVetoVLoose[nL_max];
        Bool_t          _tauEleVetoLoose[nL_max];
        Bool_t          _tauEleVetoMedium[nL_max];
        Bool_t          _tauEleVetoTight[nL_max];
        Bool_t          _tauEleVetoVTight[nL_max];
	Bool_t 		_tauPOGVLoose2015[nL_max];
	Bool_t		_tauPOGLoose2015[nL_max];
	Bool_t		_tauPOGMedium2015[nL_max];
	Bool_t		_tauPOGTight2015[nL_max];
	Bool_t		_tauPOGVTight2015[nL_max];
	Bool_t		_tauVLooseMvaNew2015[nL_max];
	Bool_t		_tauLooseMvaNew2015[nL_max];
	Bool_t		_tauMediumMvaNew2015[nL_max];
	Bool_t		_tauTightMvaNew2015[nL_max];
	Bool_t		_tauVTightMvaNew2015[nL_max];
	Bool_t		_tauPOGVVLoose2017v2[nL_max];
	Bool_t		_tauPOGVTight2017v2[nL_max];
	Bool_t		_tauPOGVVTight2017v2[nL_max];
	Bool_t		_tauVLooseMvaNew2017v2[nL_max];
	Bool_t		_tauLooseMvaNew2017v2[nL_max];
	Bool_t		_tauMediumMvaNew2017v2[nL_max];
	Bool_t		_tauTightMvaNew2017v2[nL_max];
	Bool_t		_tauVTightMvaNew2017v2[nL_max];

        Double_t        _relIso[nL_max];   
        Double_t        _relIso0p4[nL_max];
        Double_t        _relIso0p4MuDeltaBeta[nL_max];
        Double_t        _miniIso[nL_max];   
        Double_t        _miniIsoCharged[nL_max];   
        Double_t        _ptRel[nL_max];   
        Double_t        _ptRatio[nL_max];   
        Double_t        _closestJetCsvV2[nL_max];
        Double_t        _closestJetDeepCsv_b[nL_max];
        Double_t        _closestJetDeepCsv_bb[nL_max];
        Double_t        _closestJetDeepFlavor_b[nL_max];
        Double_t        _closestJetDeepFlavor_bb[nL_max];
        Double_t        _closestJetDeepFlavor_lepb[nL_max];
        UInt_t          _selectedTrackMult[nL_max];   
        Double_t        _lMuonSegComp[nL_max];   
        Double_t        _lMuonTrackPt[nL_max];
        Double_t        _lMuonTrackPtErr[nL_max];
        Bool_t          _lIsPrompt[nL_max];   
        Int_t           _lMatchPdgId[nL_max];   
        Int_t           _lMatchCharge[nL_max];
        Int_t           _lMomPdgId[nL_max];
        UInt_t          _lProvenance[nL_max];
        UInt_t          _lProvenanceCompressed[nL_max];
        UInt_t          _lProvenanceConversion[nL_max];
	// variables related to jets
        UInt_t          _nJets;
        Double_t        _jetPt[nJets_max];   
        Double_t        _jetPt_JECUp[nJets_max];   
        Double_t        _jetPt_JECDown[nJets_max];   
        Double_t        _jetSmearedPt[nJets_max];
        Double_t        _jetSmearedPt_JECDown[nJets_max];
        Double_t        _jetSmearedPt_JECUp[nJets_max];
        Double_t        _jetSmearedPt_JERDown[nJets_max];
        Double_t        _jetSmearedPt_JERUp[nJets_max];
        Double_t        _jetPt_Uncorrected[nJets_max];
        Double_t        _jetPt_L1[nJets_max];
        Double_t        _jetPt_L2[nJets_max];
        Double_t        _jetPt_L3[nJets_max];
        Double_t        _jetEta[nJets_max];   
        Double_t        _jetPhi[nJets_max];   
        Double_t        _jetE[nJets_max];   
        Double_t        _jetCsvV2[nJets_max];   
        Double_t        _jetDeepCsv_udsg[nJets_max];   
        Double_t        _jetDeepCsv_b[nJets_max];   
        Double_t        _jetDeepCsv_c[nJets_max];   
        Double_t        _jetDeepCsv_bb[nJets_max];
        Double_t        _jetDeepFlavor_b[nJets_max];
        Double_t        _jetDeepFlavor_bb[nJets_max];
        Double_t        _jetDeepFlavor_lepb[nJets_max];
        UInt_t          _jetHadronFlavor[nJets_max];   
        Bool_t          _jetIsTight[nJets_max];
        Bool_t          _jetIsTightLepVeto[nJets_max];
        Double_t        _jetNeutralHadronFraction[nJets_max];
        Double_t        _jetChargedHadronFraction[nJets_max];
        Double_t        _jetNeutralEmFraction[nJets_max];
        Double_t        _jetChargedEmFraction[nJets_max];
        Double_t        _jetHFHadronFraction[nJets_max];
        Double_t        _jetHFEmFraction[nJets_max];

        std::vector< Double_t[nJets_max] > _jetPt_JECGroupedDown;
        std::vector< Double_t[nJets_max] > _jetPt_JECGroupedUp;
        std::vector< Double_t[nJets_max] > _jetPt_JECSourcesDown;
        std::vector< Double_t[nJets_max] > _jetPt_JECSourcesUp;
        std::vector< Double_t[nJets_max] > _jetSmearedPt_JECGroupedDown;
        std::vector< Double_t[nJets_max] > _jetSmearedPt_JECGroupedUp;
        std::vector< Double_t[nJets_max] > _jetSmearedPt_JECSourcesDown;
        std::vector< Double_t[nJets_max] > _jetSmearedPt_JECSourcesUp;

        std::map< std::string, size_t >* _groupedJEC_Ids = nullptr;
        std::map< std::string, size_t >* _sourcesJEC_Ids = nullptr;
        
        std::map< std::string, size_t > _jetPt_JECGroupedUp_Ids;
        std::map< std::string, size_t > _jetPt_JECGroupedDown_Ids;
        std::map< std::string, size_t > _jetSmearedPt_JECGroupedUp_Ids;
        std::map< std::string, size_t > _jetSmearedPt_JECGroupedDown_Ids;
        std::map< std::string, size_t > _jetPt_JECSourcesUp_Ids;
        std::map< std::string, size_t > _jetPt_JECSourcesDown_Ids;
        std::map< std::string, size_t > _jetSmearedPt_JECSourcesUp_Ids;
        std::map< std::string, size_t > _jetSmearedPt_JECSourcesDown_Ids;


        // variables related to missing transverse energy
	Double_t        _met;
        Double_t        _met_JECDown;
        Double_t        _met_JECUp;
        Double_t        _met_UnclDown;
        Double_t        _met_UnclUp;
        Double_t        _metPhi;
        Double_t        _metPhi_JECDown;
        Double_t        _metPhi_JECUp;
        Double_t        _metPhi_UnclDown;
        Double_t        _metPhi_UnclUp;       
        Double_t        _metSignificance;
	std::vector< Double_t > _corrMETx_JECGroupedDown;
        std::vector< Double_t > _corrMETx_JECGroupedUp;
        std::vector< Double_t > _corrMETx_JECSourcesDown;
        std::vector< Double_t > _corrMETx_JECSourcesUp;
	std::vector< Double_t > _corrMETy_JECGroupedDown;
        std::vector< Double_t > _corrMETy_JECGroupedUp;
        std::vector< Double_t > _corrMETy_JECSourcesDown;
        std::vector< Double_t > _corrMETy_JECSourcesUp;

        std::map< std::string, size_t > _corrMETx_JECSourcesUp_Ids;
        std::map< std::string, size_t > _corrMETx_JECSourcesDown_Ids;
        std::map< std::string, size_t > _corrMETy_JECSourcesUp_Ids;
        std::map< std::string, size_t > _corrMETy_JECSourcesDown_Ids;
        std::map< std::string, size_t > _corrMETx_JECGroupedUp_Ids;
        std::map< std::string, size_t > _corrMETx_JECGroupedDown_Ids;
        std::map< std::string, size_t > _corrMETy_JECGroupedUp_Ids;
	std::map< std::string, size_t > _corrMETy_JECGroupedDown_Ids;
	// analysis specific variables: ewkino
        Double_t        _mChi1;
        Double_t        _mChi2;

	// maps for passing triggers and metfilters
        std::map< std::string, bool > _triggerMap;
        std::map< std::string, bool > _MetFilterMap;

        // particle level branches
        static const unsigned pl_nL_max = 10;
        static const unsigned pl_nPh_max = 5;
        static const unsigned pl_nJet_max = 10;
        
        //particle level MET
        double   _pl_met;
        double   _pl_metPhi;

        //particle level photons
        unsigned _pl_nPh = 0;
        double   _pl_phPt[pl_nPh_max];
        double   _pl_phEta[pl_nPh_max];
        double   _pl_phPhi[pl_nPh_max];
        double   _pl_phE[pl_nPh_max];

        //particle level leptons
        unsigned _pl_nL = 0;
        double   _pl_lPt[pl_nL_max];
        double   _pl_lEta[pl_nL_max];
        double   _pl_lPhi[pl_nL_max];
        double   _pl_lE[pl_nL_max];
        unsigned _pl_lFlavor[pl_nL_max];
        int      _pl_lCharge[pl_nL_max];

        //particle level jets
        unsigned _pl_nJets = 0;
        double   _pl_jetPt[pl_nJet_max];
        double   _pl_jetEta[pl_nJet_max];
        double   _pl_jetPhi[pl_nJet_max];
        double   _pl_jetE[pl_nJet_max];
        unsigned _pl_jetHadronFlavor[pl_nJet_max];

        //set up tree for reading and writing
        //always reset triggers instead of rare case of combining primary datasets!
        virtual void initTree( const bool resetTriggersAndFilters = true) override;
        virtual void setOutputTree( TTree* ) override;

        //Build event (this will implicitly use GetEntry )
        //Use these functions in analysis code 
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
        

        //check whether generator info is present in current tree
        virtual bool containsGeneratorInfo() const;
        virtual bool containsLheInfo() const;
        bool containsFullGeneratorInfo() const;
        bool containsParticleLevelInfo() const;
        bool containsLheInfo() const;
        bool containsEFTInfo() const;
        virtual bool hasPL() const { return _hasPLInfo;}
        virtual bool hasGenLvl() const { return _hasGenLevelInfo;}
        
        //check whether SUSY mass info is present in the current sample
	// ( this is the case for SUSY signal scans )
        bool containsSusyMassInfo() const;

        //check whether a particular trigger is present 
        bool containsTriggerInfo( const std::string& triggerPath ) const;

    protected:
        // same for PL info
        bool _hasPLInfo = false;
        bool _hasGenLevelInfo = false;
        bool _hasEFTInfo = false;

        //current index in samples vector
        int currentSampleIndex = -1;

        //general function to read a list of samples
        // Methods from baseclass that need to be overridden:
        virtual double getSumSimulatedEventWeights() override;
        virtual TTree* getTreePtr() override;
        virtual double getWeight() override {return _weight;};

        //initialize triggerMap
        void initializeTriggerMap( TTree* );
        void initializeMetFilterMap( TTree* );

	// initialize split jec uncertainty source maps
        void initializeJecSourcesMaps( TTree* );
        void initializeJecSourcesGroupedMaps( TTree* );

        //list of branches
        TBranch        *b__runNb;   
        TBranch        *b__lumiBlock;   
        TBranch        *b__eventNb;   
        TBranch        *b__nVertex;   
        TBranch        *b__prefireWeight;
        TBranch        *b__prefireWeightDown;
        TBranch        *b__prefireWeightUp;
        TBranch        *b__weight;   
        TBranch        *b__nLheWeights;   
        TBranch        *b__nPsWeights;
        TBranch        *b__psWeight;
        TBranch        *b__lheWeight;   
        TBranch        *b__nTrueInt;   
        TBranch        *b__lheHTIncoming;
        TBranch        *b__nDynScaleWeights;
        TBranch        *b__dynScaleWeight;
        TBranch        *b__nEFTWeights;
        TBranch        *b__eftWeight;
        TBranch        *b__gen_met;   
        TBranch        *b__gen_metPhi;   
        TBranch        *b__gen_nL;   
        TBranch        *b__gen_lPt;   
        TBranch        *b__gen_lEta;   
        TBranch        *b__gen_lPhi;   
        TBranch        *b__gen_lE;   
        TBranch        *b__gen_lFlavor;   
        TBranch        *b__gen_lCharge;   
        TBranch        *b__gen_lMomPdg;   
        TBranch        *b__gen_lIsPrompt;
        TBranch        *b__nLheTau;
        TBranch        *b__nLheParticles;
        TBranch        *b__lheStatus;
        TBranch        *b__lhePdgId;
        TBranch        *b__lheMother1;
        TBranch        *b__lheMother2;
        TBranch        *b__lhePt;
        TBranch        *b__lheEta;
        TBranch        *b__lhePhi;
        TBranch        *b__lheE;
        TBranch        *b__lheMass;
        TBranch        *b__ttgEventType;
        TBranch        *b__zgEventType;
        TBranch        *b__passTrigger_e;   
        TBranch        *b__passTrigger_ee;   
        TBranch        *b__passTrigger_eee;   
        TBranch        *b__passTrigger_em;   
        TBranch        *b__passTrigger_m;   
        TBranch        *b__passTrigger_eem;   
        TBranch        *b__passTrigger_mm;   
        TBranch        *b__passTrigger_emm;   
        TBranch        *b__passTrigger_mmm;   
        TBranch        *b__passTrigger_et;
        TBranch        *b__passTrigger_mt;
        TBranch        *b__passTrigger_FR;
        TBranch        *b__passTrigger_FR_iso;
        TBranch        *b__passMETFilters;   
        TBranch        *b__nL;   
        TBranch        *b__nMu;   
        TBranch        *b__nEle;   
        TBranch        *b__nLight;   
        TBranch        *b__nTau;   
        TBranch        *b__lPt;   
        TBranch        *b__lPtCorr;
	TBranch	       *b__lPtScaleUp;
	TBranch	       *b__lPtScaleDown;
	TBranch	       *b__lPtResUp;
	TBranch	       *b__lPtResDown;
        TBranch        *b__lEta;   
        TBranch        *b__lEtaSC;   
        TBranch        *b__lPhi;   
        TBranch        *b__lE;   
        TBranch        *b__lECorr;
	TBranch        *b__lEScaleUp;
        TBranch        *b__lEScaleDown;
        TBranch        *b__lEResUp;
        TBranch        *b__lEResDown;
        TBranch        *b__lFlavor;   
        TBranch        *b__lCharge;   
        TBranch        *b__dxy;   
        TBranch        *b__dz;   
        TBranch        *b__3dIP;   
        TBranch        *b__3dIPSig;   
        TBranch        *b__lElectronSummer16MvaGP;   
        TBranch        *b__lElectronSummer16MvaHZZ;
        TBranch        *b__lElectronMvaFall17Iso;
        TBranch        *b__lElectronMvaFall17NoIso;
        TBranch        *b__lElectronPassMVAFall17NoIsoWPLoose;
        TBranch        *b__lElectronPassMVAFall17NoIsoWP90;
        TBranch        *b__lElectronPassMVAFall17NoIsoWP80;
        TBranch        *b__lElectronPassEmu;   
        TBranch        *b__lElectronPassConvVeto;
        TBranch        *b__lElectronChargeConst;
        TBranch        *b__lElectronMissingHits;
        TBranch        *b__lElectronEInvMinusPInv;
        TBranch        *b__lElectronHOverE;
        TBranch        *b__lElectronSigmaIetaIeta;
        TBranch        *b__leptonMvaTTH;
        TBranch        *b__leptonMvatZq;
	TBranch        *b__leptonMvaTOP;
	TBranch        *b__leptonMvaTOPUL;
	TBranch        *b__leptonMvaTOPv2UL;
        TBranch        *b__lPOGVeto;   
        TBranch        *b__lPOGLoose;   
        TBranch        *b__lPOGMedium;   
        TBranch        *b__lPOGTight;   
	TBranch		   *b__tauDecayMode;
        TBranch		   *b__decayModeFinding;
        TBranch		   *b__decayModeFindingNew;
        TBranch		   *b__tauMuonVetoLoose;
        TBranch		   *b__tauMuonVetoTight;
        TBranch		   *b__tauEleVetoVLoose;
        TBranch		   *b__tauEleVetoLoose;
        TBranch		   *b__tauEleVetoMedium;
        TBranch		   *b__tauEleVetoTight;
        TBranch		   *b__tauEleVetoVTight;
        TBranch		   *b__tauPOGVLoose2015;
        TBranch		   *b__tauPOGLoose2015;
        TBranch		   *b__tauPOGMedium2015;
        TBranch		   *b__tauPOGTight2015;
        TBranch		   *b__tauPOGVTight2015;
        TBranch		   *b__tauVLooseMvaNew2015;
        TBranch		   *b__tauLooseMvaNew2015;
        TBranch		   *b__tauMediumMvaNew2015;
        TBranch		   *b__tauTightMvaNew2015;
        TBranch		   *b__tauVTightMvaNew2015;
        TBranch		   *b__tauPOGVVLoose2017v2;
        TBranch		   *b__tauPOGVTight2017v2;
        TBranch		   *b__tauPOGVVTight2017v2;
        TBranch		   *b__tauVLooseMvaNew2017v2;
        TBranch		   *b__tauLooseMvaNew2017v2;
        TBranch		   *b__tauMediumMvaNew2017v2;
        TBranch		   *b__tauTightMvaNew2017v2;
        TBranch		   *b__tauVTightMvaNew2017v2;	
        TBranch        *b__relIso;   
        TBranch        *b__relIso0p4;
        TBranch        *b__relIso0p4MuDeltaBeta;
        TBranch        *b__miniIso;   
        TBranch        *b__miniIsoCharged;   
        TBranch        *b__ptRel;   
        TBranch        *b__ptRatio;   
        TBranch        *b__closestJetCsvV2;
        TBranch        *b__closestJetDeepCsv_b;
        TBranch        *b__closestJetDeepCsv_bb;
        TBranch        *b__closestJetDeepFlavor_b;
        TBranch        *b__closestJetDeepFlavor_bb;
        TBranch        *b__closestJetDeepFlavor_lepb;
        TBranch        *b__selectedTrackMult;   
        TBranch        *b__lMuonSegComp;   
        TBranch        *b__lMuonTrackPt;
        TBranch        *b__lMuonTrackPtErr;
        TBranch        *b__lIsPrompt;   
        TBranch        *b__lMatchPdgId;
        TBranch        *b__lMatchCharge;
        TBranch        *b__lMomPdgId;
        TBranch        *b__lProvenance;
        TBranch        *b__lProvenanceCompressed;
        TBranch        *b__lProvenanceConversion;
        TBranch        *b__nJets;   
        TBranch        *b__jetPt;   
        TBranch        *b__jetPt_JECUp;   
        TBranch        *b__jetPt_JECDown;   
        TBranch        *b__jetSmearedPt;
        TBranch        *b__jetSmearedPt_JECDown;
        TBranch        *b__jetSmearedPt_JECUp;
        TBranch        *b__jetSmearedPt_JERDown;
        TBranch        *b__jetSmearedPt_JERUp;
        TBranch        *b__jetPt_Uncorrected;
        TBranch        *b__jetPt_L1;
        TBranch        *b__jetPt_L2;
        TBranch        *b__jetPt_L3;
        TBranch        *b__jetEta;   
        TBranch        *b__jetPhi;   
        TBranch        *b__jetE;   
        TBranch        *b__jetCsvV2;   
        TBranch        *b__jetDeepCsv_udsg;   
        TBranch        *b__jetDeepCsv_b;   
        TBranch        *b__jetDeepCsv_c;   
        TBranch        *b__jetDeepCsv_bb;   
        TBranch        *b__jetDeepFlavor_b;
        TBranch        *b__jetDeepFlavor_bb;
        TBranch        *b__jetDeepFlavor_lepb;
        TBranch        *b__jetHadronFlavor;   
        TBranch        *b__jetId;   
        TBranch        *b__jetIsTight;
        TBranch        *b__jetIsTightLepVeto;
        TBranch        *b__jetNeutralHadronFraction;
        TBranch        *b__jetChargedHadronFraction;
        TBranch        *b__jetNeutralEmFraction;
        TBranch        *b__jetChargedEmFraction;
        TBranch        *b__jetHFHadronFraction;
        TBranch        *b__jetHFEmFraction;
	std::map< std::string, TBranch* > b__jetPt_JECGroupedDown;
        std::map< std::string, TBranch* > b__jetPt_JECGroupedUp;
        std::map< std::string, TBranch* > b__jetPt_JECSourcesDown;
        std::map< std::string, TBranch* > b__jetPt_JECSourcesUp;
        std::map< std::string, TBranch* > b__jetSmearedPt_JECGroupedDown;
        std::map< std::string, TBranch* > b__jetSmearedPt_JECGroupedUp;
        std::map< std::string, TBranch* > b__jetSmearedPt_JECSourcesDown;
        std::map< std::string, TBranch* > b__jetSmearedPt_JECSourcesUp;
        TBranch        *b__met;   
        TBranch        *b__met_JECDown;   
        TBranch        *b__met_JECUp;   
        TBranch        *b__met_UnclDown;   
        TBranch        *b__met_UnclUp;   
        TBranch        *b__metPhi;   
        TBranch        *b__metPhi_JECDown;   
        TBranch        *b__metPhi_JECUp;   
        TBranch        *b__metPhi_UnclDown;   
        TBranch        *b__metPhi_UnclUp;   
        TBranch        *b__metSignificance;
	std::map< std::string, TBranch* > b__corrMETx_JECGroupedDown;
        std::map< std::string, TBranch* > b__corrMETx_JECGroupedUp;
        std::map< std::string, TBranch* > b__corrMETx_JECSourcesDown;
        std::map< std::string, TBranch* > b__corrMETx_JECSourcesUp;
	std::map< std::string, TBranch* > b__corrMETy_JECGroupedDown;
        std::map< std::string, TBranch* > b__corrMETy_JECGroupedUp;
        std::map< std::string, TBranch* > b__corrMETy_JECSourcesDown;
        std::map< std::string, TBranch* > b__corrMETy_JECSourcesUp;
        TBranch        *b__mChi1;
        TBranch        *b__mChi2;

        std::map< std::string, TBranch* > b__triggerMap;
        std::map< std::string, TBranch* > b__MetFilterMap; 

        // all generator variables
        TBranch        *b__gen_n;
        TBranch        *b__gen_pt;
        TBranch        *b__gen_eta;
        TBranch        *b__gen_phi;
        TBranch        *b__gen_E;
        TBranch        *b__gen_pdgId;
        TBranch        *b__gen_charge;
        TBranch        *b__gen_status;
        TBranch        *b__gen_isPromptFinalState;
        TBranch        *b__gen_isDirectPromptTauDecayProductFinalState;
        TBranch        *b__gen_isLastCopy;
        TBranch        *b__gen_index;
        TBranch        *b__gen_motherIndex;
        TBranch        *b__gen_daughter_n;
        TBranch        *b__gen_daughterIndex;

        TBranch        *b__pl_met;
        TBranch        *b__pl_metPhi;
        TBranch        *b__pl_nPh;
        TBranch        *b__pl_phPt;
        TBranch        *b__pl_phEta;
        TBranch        *b__pl_phPhi;
        TBranch        *b__pl_phE;
        TBranch        *b__pl_nL;
        TBranch        *b__pl_lPt;
        TBranch        *b__pl_lEta;
        TBranch        *b__pl_lPhi;
        TBranch        *b__pl_lE;
        TBranch        *b__pl_lFlavor;
        TBranch        *b__pl_lCharge;
        TBranch        *b__pl_nJets;
        TBranch        *b__pl_jetPt;
        TBranch        *b__pl_jetEta;
        TBranch        *b__pl_jetPhi;
        TBranch        *b__pl_jetE;
        TBranch        *b__pl_jetHadronFlavor;
};

#endif
