#ifndef TreeReader_H
#define TreeReader_H

// include c++ library classes 
#include <fstream>
#include <iostream>
#include <typeinfo>

// include ROOT classes
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TLorentzVector.h"

// include other parts of the framework
#include "../../Tools/interface/Sample.h"
#include "../../Tools/interface/analysisTools.h"
#include "../../Tools/interface/stringTools.h"
#include "../../Tools/interface/systemTools.h"
#include "../../Tools/interface/analysisTools.h"
#include "../../constants/luminosities.h"

class Event;

class TreeReader {

    public :

        // constructor
        TreeReader() = default;
        TreeReader( const std::string& sampleListFile,
                    const std::string& sampleDirectory ); 

        // declare leaf types
	// constants
	// note: unsure what values to pick for nanoAOD files
        static const unsigned nElectron_max = 10;
	static const unsigned nMuon_max = 10;
	static const unsigned nTau_max = 10;
        static const unsigned nJet_max = 30;
	static const unsigned nGenPart_max = 150;
        static const unsigned nGenJet_max = 30;
	static const unsigned nLHEPdfWeight_max = 103;
	static const unsigned nLHEScaleWeight_max = 9;
	static const unsigned nPSWeight_max = 4;
	// note: variable types can be found here:
	// https://cms-nanoaod-integration.web.cern.ch/autoDoc/
	// global event variables and weights
        UInt_t           _run;
        UInt_t           _luminosityBlock;
        UInt_t           _event;
	// generator info
        Float_t         _genWeight;
        UInt_t          _nLHEPdfWeight;
        Float_t         _LHEPdfWeight[nLHEPdfWeight_max];
	UInt_t		_nLHEScaleWeight;
	Float_t		_LHEScaleWeight[nLHEScaleWeight_max];
        UInt_t          _nPSWeight;
        Float_t         _PSWeight[nPSWeight_max];
	// gen particles
	UInt_t		_nGenPart;
	Float_t		_GenPart_pt[nGenPart_max];
	Float_t		_GenPart_eta[nGenPart_max];
	Float_t		_GenPart_phi[nGenPart_max];
        Float_t		_GenPart_mass[nGenPart_max];
	UInt_t		_GenPart_genPartIdxMother[nGenPart_max];
        Int_t		_GenPart_pdgId[nGenPart_max];
	UInt_t		_GenPart_status[nGenPart_max];
        UInt_t		_GenPart_statusFlags[nGenPart_max];
	// gen MET
	Float_t		_GenMET_pt;
	Float_t		_GenMET_phi;
	// variables related to electrons
	UInt_t		_nElectron;
	Float_t		_Electron_pt[nElectron_max];
	Float_t		_Electron_eta[nElectron_max];
	Float_t		_Electron_phi[nElectron_max];
	Int_t		_Electron_charge[nElectron_max];
	Float_t		_Electron_dxy[nElectron_max];
	Float_t		_Electron_dz[nElectron_max];
	Float_t		_Electron_sip3d[nElectron_max];
	Float_t		_Electron_pfRelIso03_all[nElectron_max];
	Float_t		_Electron_miniPFRelIso_all[nElectron_max];
	Float_t		_Electron_miniPFRelIso_chg[nElectron_max];
	Int_t		_Electron_jetIdx[nElectron_max];
	Float_t		_Electron_jetPtRelv2[nElectron_max];
	Float_t		_Electron_jetRelIso[nElectron_max];
	Float_t		_Electron_mvaTTH[nElectron_max];
	Int_t		_Electron_tightCharge[nElectron_max];
	Bool_t		_Electron_convVeto[nElectron_max];
	UChar_t		_Electron_lostHits[nElectron_max];
	Float_t		_Electron_mvaFall17V2Iso[nElectron_max];
	Float_t		_Electron_mvaFall17V2noIso[nElectron_max];
	Bool_t		_Electron_mvaFall17V2noIso_WPL[nElectron_max];
	Bool_t          _Electron_mvaFall17V2noIso_WP80[nElectron_max];
	Bool_t          _Electron_mvaFall17V2noIso_WP90[nElectron_max];
	Float_t		_Electron_deltaEtaSC[nElectron_max];
	Float_t		_Electron_eInvMinusPInv[nElectron_max];
	Float_t		_Electron_hoe[nElectron_max];
	Float_t		_Electron_sieie[nElectron_max];
	Int_t		_Electron_cutBased[nElectron_max];
	Float_t		_Electron_dEscaleDown[nElectron_max];
	Float_t		_Electron_dEscaleUp[nElectron_max];
	Float_t		_Electron_dEsigmaDown[nElectron_max];
	Float_t		_Electron_dEsigmaUp[nElectron_max];
	UChar_t		_Electron_genPartFlav[nElectron_max];
	Int_t		_Electron_genPartIdx[nElectron_max];
	// variables related to muons
	UInt_t          _nMuon;
        Float_t		_Muon_pt[nMuon_max];
        Float_t		_Muon_eta[nMuon_max];
        Float_t		_Muon_phi[nMuon_max];
	Int_t		_Muon_charge[nMuon_max];
        Float_t         _Muon_dxy[nMuon_max];
        Float_t         _Muon_dz[nMuon_max];
        Float_t         _Muon_sip3d[nMuon_max];
	Float_t         _Muon_pfRelIso03_all[nMuon_max];
        Float_t         _Muon_miniPFRelIso_all[nMuon_max];
        Float_t         _Muon_miniPFRelIso_chg[nMuon_max];
        Int_t           _Muon_jetIdx[nMuon_max];
        Float_t         _Muon_jetPtRelv2[nMuon_max];
        Float_t         _Muon_jetRelIso[nMuon_max];
	Float_t		_Muon_mvaTTH[nMuon_max];
	Float_t		_Muon_segmentComp[nMuon_max];
	Float_t		_Muon_ptErr[nMuon_max];
	Float_t		_Muon_pfRelIso04_all[nMuon_max];
	Bool_t		_Muon_looseId[nMuon_max];
	Bool_t		_Muon_mediumId[nMuon_max];
	Bool_t		_Muon_tightId[nMuon_max];
	UChar_t		_Muon_genPartFlav[nMuon_max];
	Int_t		_Muon_genPartIdx[nMuon_max];
	Bool_t		_Muon_isPFCand[nMuon_max];
	Bool_t		_Muon_isGlobal[nMuon_max];
	Bool_t		_Muon_isTracker[nMuon_max];
	Bool_t		_Muon_isStandalone[nMuon_max];
	// variables related to taus
	UInt_t          _nTau;
        Float_t		_Tau_pt[nTau_max];
        Float_t		_Tau_eta[nTau_max];
        Float_t		_Tau_phi[nTau_max];
	Int_t		_Tau_charge[nTau_max];
        Float_t         _Tau_dxy[nTau_max];
        Float_t         _Tau_dz[nTau_max];
	UChar_t		_Tau_genPartFlav[nTau_max];
	Int_t		_Tau_genPartIdx[nTau_max];
	// variables related to jets
        UInt_t          _nJet;
        Float_t		_Jet_pt[nJet_max];
	Float_t		_Jet_eta[nJet_max];
	Float_t		_Jet_phi[nJet_max];
	Float_t		_Jet_bTagDeepB[nJet_max];
	Float_t		_Jet_bTagDeepFlavB[nJet_max];
	UChar_t		_Jet_nConstituents[nJet_max];
	Int_t		_Jet_hadronFlavor[nJet_max];
	Int_t		_Jet_jetId[nJet_max];
        // variables related to missing transverse energy
	Float_t        _MET_pt;
        Float_t        _MET_phi;
	// maps for passing triggers and metfilters
        std::map< std::string, bool > _triggerMap;
        std::map< std::string, bool > _METFilterMap;

        // weight including cross section and lumi scaling 
        double          _scaledWeight;

        // set up tree for reading and writing
        void initTree( const bool resetTriggersAndFilters = true);
        void setOutputTree( TTree*, 
			    bool includeGeneratorInfo = true,
			    bool includeGenParticles = true );

        // initialize the next sample
        void initSample( const bool doInitTree = true );
        void initSample( unsigned int sampleIndex,
                         const bool doInitTree = true );
        void initSample( const Sample&, 
			 const bool doInitTree = true );  

        // read sample list from text file
        void readSamples(const std::string& list, const std::string& directory);

        // initialize the current sample directly from a root file
        void initSampleFromFile( const std::string& pathToFile,
				 const bool is2016, 
				 const bool is2016PreVFP, 
				 const bool is2016PostVFP,
				 const bool is2017,
                                 const bool is2018, 
				 const bool resetTriggersAndFilters = true );
        void initSampleFromFile( const std::string& pathToFile, 
				 const bool resetTriggersAndFilters = true );

        // get entry from Tree, should not be used except for test purposes
        void GetEntry(const Sample&, long unsigned );
        void GetEntry(long unsigned );

        // build event (this will implicitly use GetEntry )
        Event buildEvent( const Sample&, long unsigned, 
			    const bool readIndividualTriggers = false, 
			    const bool readIndividualMetFilters = false );
        Event buildEvent( long unsigned, 
			    const bool readIndividualTriggers = false, 
			    const bool readIndividualMetFilters = false );

        // check whether specific info is present in current tree
	bool containsGeneratorInfo() const;
	bool containsGenParticles() const;

        // check whether a particular trigger is present 
        bool containsTriggerInfo( const std::string& triggerPath ) const;

        // check which year the current sample belongs to
        bool is2016() const;
	bool is2016PreVFP() const;
	bool is2016PostVFP() const;
        bool is2017() const;
        bool is2018() const;
	std::string getYearString() const;

        // check whether the current sample is data or MC, and or signal
        bool isData() const;
        bool isMC() const;

        // access number of samples and current sample
        const Sample& currentSample() const{ return *_currentSamplePtr; }
        const Sample* currentSamplePtr() const{ return _currentSamplePtr.get(); }
        std::vector< Sample >::size_type numberOfSamples() const{ return samples.size(); }
        std::vector< Sample > sampleVector() const{ return samples; }

        // access current file and tree 
        std::shared_ptr< TFile > currentFilePtr(){ return _currentFilePtr; }

        // get object from current file 
        TObject* getFromCurrentFile( const std::string& name ) const;

        // get list of histograms stored in current file
        std::vector< std::shared_ptr< TH1 > > getHistogramsFromCurrentFile() const;

        unsigned long numberOfEntries() const;

    private:

        // list of samples to loop over 
        std::vector< Sample > samples;

        // current sample
        std::shared_ptr< const Sample > _currentSamplePtr;

        // TFile associated to current sample
        std::shared_ptr< TFile > _currentFilePtr;

        // TTree associated to current sample 
        TTree* _currentTreePtr = nullptr;

        // check whether current sample is initialized, throw an error if it is not 
        void checkCurrentSample() const;

        // check whether current Tree is initialized, throw an error if it is not 
        void checkCurrentTree() const;

        // check whether current File is initialized, throw an error if it is not
        void checkCurrentFile() const;

        // current index in samples vector
        int currentSampleIndex = -1;

        // luminosity scaling
        double lumiScale = 0;

        // general function to read a list of samples
        void readSamples(const std::string&, const std::string&, std::vector<Sample>&);

        // initialize triggerMap
        void initializeTriggerMap( TTree* );
        void initializeMETFilterMap( TTree* );

        // list of branches
        TBranch*	b__run;
        TBranch*	b__luminosityBlock;
        TBranch*        b__event;
        TBranch*        b__genWeight;
        TBranch*        b__nLHEPdfWeight;
        TBranch*        b__LHEPdfWeight;
        TBranch*        b__nLHEScaleWeight;
        TBranch*        b__LHEScaleWeight;
        TBranch*        b__nPSWeight;
        TBranch*        b__PSWeight;
        TBranch*        b__nGenPart;
        TBranch*        b__GenPart_pt;
        TBranch*        b__GenPart_eta;
        TBranch*        b__GenPart_phi;
        TBranch*        b__GenPart_mass;
        TBranch*        b__GenPart_genPartIdxMother;
        TBranch*        b__GenPart_pdgId;
        TBranch*        b__GenPart_status;
        TBranch*        b__GenPart_statusFlags;
	TBranch*	b__GenMET_pt;
	TBranch*	b__GenMET_phi;
        TBranch*        b__nElectron;
        TBranch*        b__Electron_pt;
        TBranch*        b__Electron_eta;
        TBranch*        b__Electron_phi;
	TBranch*        b__Electron_charge;
        TBranch*        b__Electron_dxy;
        TBranch*        b__Electron_dz;
        TBranch*        b__Electron_sip3d;
	TBranch*        b__Electron_pfRelIso03_all;
        TBranch*        b__Electron_miniPFRelIso_all;
        TBranch*        b__Electron_miniPFRelIso_chg;
        TBranch*        b__Electron_jetIdx;
        TBranch*        b__Electron_jetPtRelv2;
        TBranch*        b__Electron_jetRelIso;
	TBranch*	b__Electron_mvaTTH;
	TBranch*	b__Electron_tightCharge;
        TBranch*        b__Electron_convVeto;
        TBranch*        b__Electron_lostHits;
        TBranch*        b__Electron_mvaFall17V2Iso;
        TBranch*        b__Electron_mvaFall17V2noIso;
        TBranch*        b__Electron_mvaFall17V2noIso_WPL;
        TBranch*        b__Electron_mvaFall17V2noIso_WP80;
        TBranch*        b__Electron_mvaFall17V2noIso_WP90;
        TBranch*        b__Electron_deltaEtaSC;
        TBranch*        b__Electron_eInvMinusPInv;
        TBranch*        b__Electron_hoe;
        TBranch*        b__Electron_sieie;
	TBranch*	b__Electron_cutBased;
	TBranch*	b__Electron_dEscaleDown;
	TBranch*        b__Electron_dEscaleUp;
	TBranch*        b__Electron_dEsigmaDown;
        TBranch*        b__Electron_dEsigmaUp;
	TBranch*	b__Electron_genPartFlav;
	TBranch*	b__Electron_genPartIdx;
        TBranch*        b__nMuon;
        TBranch*        b__Muon_pt;
        TBranch*        b__Muon_eta;
        TBranch*        b__Muon_phi;
	TBranch*        b__Muon_charge;
        TBranch*        b__Muon_dxy;
        TBranch*        b__Muon_dz;
        TBranch*        b__Muon_sip3d;
	TBranch*        b__Muon_pfRelIso03_all;
        TBranch*        b__Muon_miniPFRelIso_all;
        TBranch*        b__Muon_miniPFRelIso_chg;
        TBranch*        b__Muon_jetIdx;
        TBranch*        b__Muon_jetPtRelv2;
        TBranch*        b__Muon_jetRelIso;
	TBranch*	b__Muon_mvaTTH;
	TBranch*        b__Muon_segmentComp;
        TBranch*        b__Muon_ptErr;
        TBranch*        b__Muon_pfRelIso04_all;
        TBranch*        b__Muon_looseId;
        TBranch*        b__Muon_mediumId;
        TBranch*        b__Muon_tightId;
	TBranch*        b__Muon_genPartFlav;
        TBranch*        b__Muon_genPartIdx;
	TBranch*	b__Muon_isPFCand;
	TBranch*	b__Muon_isGlobal;
	TBranch*	b__Muon_isTracker;
	TBranch*	b__Muon_isStandalone;
	TBranch*	b__nTau;
	TBranch*	b__Tau_pt;
	TBranch*        b__Tau_eta;
	TBranch*        b__Tau_phi;
	TBranch*        b__Tau_charge;
        TBranch*        b__Tau_dxy;
        TBranch*        b__Tau_dz;
	TBranch*        b__Tau_genPartFlav;
        TBranch*        b__Tau_genPartIdx;
        TBranch*        b__nJet;
        TBranch*        b__Jet_pt;
        TBranch*        b__Jet_eta;
        TBranch*        b__Jet_phi;
	TBranch*	b__Jet_bTagDeepB;
	TBranch*	b__Jet_bTagDeepFlavB;
	TBranch*	b__Jet_nConstituents;
	TBranch*	b__Jet_hadronFlavor;
        TBranch*        b__Jet_jetId;
        TBranch*        b__MET_pt;
        TBranch*        b__MET_phi;
        std::map< std::string, TBranch* > b__triggerMap;
        std::map< std::string, TBranch* > b__METFilterMap; 
};

#endif
