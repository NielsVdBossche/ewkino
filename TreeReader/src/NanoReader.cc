#include "../interface/NanoReader.h"
#include "../../Tools/interface/stringTools.h"

#include "../../Event/interface/Event.h"
#include "../../constants/luminosities.h"

NanoReader::NanoReader(const std::string& sampleListFile, const std::string& sampleDirectory) :
    BaseReader(sampleListFile, sampleDirectory)
{   
    // _sourcesJEC_Ids = new std::map<std::string, size_t>();
    // _groupedJEC_Ids = new std::map<std::string, size_t>();
}

double NanoReader::getSumSimulatedEventWeights() {
    TTree* runsTree = (TTree*) _currentFilePtr->Get("Runs");
    if( runsTree == nullptr ){
        throw std::invalid_argument( "Runs tree is not present in file '" + _currentSamplePtr->fileName() + "', can not properly normalize a MC sample without this." );
        exit(1);
    }
    Double_t tmp_nominalSumOfWeights;
    double nominalSumOfWeights = 0.0;
    // runsTree->SetBranchStatus("*", 0);
    runsTree->SetBranchAddress("genEventSumw",  &tmp_nominalSumOfWeights);
    for (int i = 0; i < runsTree->GetEntries(); i++) {
        runsTree->GetEntry(i);
        nominalSumOfWeights += tmp_nominalSumOfWeights;
    }
    return nominalSumOfWeights;
}

TTree* NanoReader::getTreePtr() {
    return (TTree*) _currentFilePtr->Get("Events");
}

Event NanoReader::buildEvent( const Sample& samp, long unsigned entry, 
    const bool readIndividualTriggers, const bool readIndividualMetFilters,
    const bool readAllJECVariations, const bool readGroupedJECVariations)
{
    GetEntry( samp, entry );
    return Event( *this, readIndividualTriggers, readIndividualMetFilters,
                readAllJECVariations, readGroupedJECVariations );
}

Event* NanoReader::buildEventPtr( const Sample& samp, long unsigned entry, 
    const bool readIndividualTriggers, const bool readIndividualMetFilters,
    const bool readAllJECVariations, const bool readGroupedJECVariations)
{
    GetEntry( samp, entry );
    return new Event( *this, readIndividualTriggers, readIndividualMetFilters,
                    readAllJECVariations, readGroupedJECVariations );
}

Event NanoReader::buildEvent( long unsigned entry, 
    const bool readIndividualTriggers, const bool readIndividualMetFilters,
    const bool readAllJECVariations, const bool readGroupedJECVariations)
{
    GetEntry( entry );
    return Event( *this, readIndividualTriggers, readIndividualMetFilters,
                readAllJECVariations, readGroupedJECVariations );
}

Event* NanoReader::buildEventPtr( long unsigned entry, 
    const bool readIndividualTriggers, const bool readIndividualMetFilters,
    const bool readAllJECVariations, const bool readGroupedJECVariations)
{
    GetEntry( entry );
    return new Event( *this, readIndividualTriggers, readIndividualMetFilters,
                    readAllJECVariations, readGroupedJECVariations );
}

bool NanoReader::containsGeneratorInfo() const {
    return treeHasBranchWithName( _currentTreePtr, "GenPart" );
}

bool NanoReader::containsLheInfo() const {
    return containsGeneratorInfo();
}

bool NanoReader::hasPL() const {
    return containsGeneratorInfo();
}

bool NanoReader::hasGenLvl() const {
    return containsGeneratorInfo();
}

bool NanoReader::containsEFTInfo() const {
    return treeHasBranchWithName( _currentTreePtr, "LHEReweightingWeight" );
}

void NanoReader::initializeJESVariations( TTree* treePtr ) {
    // initialize jetPt branches
    b__jet_pt_jesSourcesUp = buildBranchMap(treePtr, {"Jet_pt_", "jes", "Up"}).second;
    for (auto mapEl : b__jet_pt_jesSourcesUp) { // cleaning should happen here?
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"Jet_pt_", "jes", "Up"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _Jet_pt_jesSourcesUpNames[mapEl.first] = branchName;        
        _Jet_pt_jesSourcesUp[branchName];
    }
    b__jet_pt_jesSourcesDown = buildBranchMap(treePtr, {"Jet_pt_", "jes", "Down"}).second;
    for (auto mapEl : b__jet_pt_jesSourcesDown) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"Jet_pt_", "jes", "Down"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _Jet_pt_jesSourcesDownNames[mapEl.first] = branchName;
        _Jet_pt_jesSourcesDown[branchName];
    }
    b__jet_mass_jesSourcesUp = buildBranchMap(treePtr, {"Jet_mass_", "jes", "Up"}).second;
    for (auto mapEl : b__jet_mass_jesSourcesUp) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"Jet_mass_", "jes", "Up"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _Jet_mass_jesSourcesUpNames[mapEl.first] = branchName;
        _Jet_mass_jesSourcesUp[branchName];
    }
    b__jet_mass_jesSourcesDown = buildBranchMap(treePtr, {"Jet_mass_", "jes", "Down"}).second;
    for (auto mapEl : b__jet_mass_jesSourcesDown) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"Jet_mass_", "jes", "Down"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _Jet_mass_jesSourcesDownNames[mapEl.first] = branchName;
        _Jet_mass_jesSourcesDown[branchName];
    }

    // initialize met branches
    b__met_pt_jesSourcesUp = buildBranchMap(treePtr, {"MET_T1Smear_pt_", "jes", "Up"}).second;
    for (auto mapEl : b__met_pt_jesSourcesUp) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"MET_T1Smear_pt_", "jes", "Up"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _MET_T1Smear_pt_jesSourcesUpNames[mapEl.first] = branchName;
        _MET_T1Smear_pt_jesSourcesUp[branchName];
    }
    b__met_pt_jesSourcesDown = buildBranchMap(treePtr, {"MET_T1Smear_pt_", "jes", "Down"}).second;
    for (auto mapEl : b__met_pt_jesSourcesDown) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"MET_T1Smear_pt_", "jes", "Down"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _MET_T1Smear_pt_jesSourcesDownNames[mapEl.first] = branchName;
        _MET_T1Smear_pt_jesSourcesDown[branchName];
    }
    b__met_phi_jesSourcesUp = buildBranchMap(treePtr, {"MET_T1Smear_phi_", "jes", "Up"}).second;
    for (auto mapEl : b__met_phi_jesSourcesUp) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"MET_T1Smear_phi_", "jes", "Up"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _MET_T1Smear_phi_jesSourcesUpNames[mapEl.first] = branchName;
        _MET_T1Smear_phi_jesSourcesUp[branchName];
    }
    b__met_phi_jesSourcesDown = buildBranchMap(treePtr, {"MET_T1Smear_phi_", "jes", "Down"}).second;
    for (auto mapEl : b__met_phi_jesSourcesDown) {
        std::string branchName = mapEl.first;
        for (std::string nameIdentifier : {"MET_T1Smear_phi_", "jes", "Down"}) {
            branchName = stringTools::removeOccurencesOf(branchName, nameIdentifier);
        }
        _MET_T1Smear_phi_jesSourcesDownNames[mapEl.first] = branchName;
        _MET_T1Smear_phi_jesSourcesDown[branchName];
    }
}

void NanoReader::initTree(const bool resetTriggersAndFilters) {
    checkCurrentTree();
    _hasPLInfo = false;
    _hasGenLevelInfo = false;
    _hasEFTInfo = false;

    std::cout << "deleting existing readers" << std::endl;
    if (electronReader) delete electronReader;
    if (muonReader) delete muonReader;
    if (tauReader) delete tauReader;

    std::cout << "Setting branches" << std::endl;
    _currentTreePtr->SetMakeClass(1);

    // set branch addresses of all variables
    _currentTreePtr->SetBranchAddress("run", &_run, &b__run);
    _currentTreePtr->SetBranchAddress("luminosityBlock", &_luminosityBlock, &b__luminosityBlock);
    _currentTreePtr->SetBranchAddress("event", &_event, &b__event);

    _currentTreePtr->SetBranchAddress("Pileup_nTrueInt", &_Pileup_nTrueInt, &b__Pileup_nTrueInt);
    _currentTreePtr->SetBranchAddress("PV_npvs", &_PV_npvs, &b__PV_npvs);

    if (isMC()) {
        // TODO: check if the isMC flag works
        _currentTreePtr->SetBranchAddress("L1PreFiringWeight_Nom",  &_L1PreFiringWeight_Nom,  &b__L1PreFiringWeight_Nom);
        _currentTreePtr->SetBranchAddress("L1PreFiringWeight_Up",   &_L1PreFiringWeight_Up,   &b__L1PreFiringWeight_Up);
        _currentTreePtr->SetBranchAddress("L1PreFiringWeight_Dn",   &_L1PreFiringWeight_Dn,   &b__L1PreFiringWeight_Dn);
    }

    if (containsEFTInfo()) {
        _hasEFTInfo = true;
        _currentTreePtr->SetBranchAddress("nLHEReweightingWeight", &_nLHEReweightingWeight, &b__nLHEReweightingWeight);
        _currentTreePtr->SetBranchAddress("LHEReweightingWeight",   _LHEReweightingWeight,  &b__LHEReweightingWeight);
    }

    // generator info
    if (containsGeneratorInfo()) {
        _hasGenLevelInfo = true;
        _currentTreePtr->SetBranchAddress("genWeight",          &_genWeight,       &b__genWeight);
        _currentTreePtr->SetBranchAddress("nLHEPdfWeight",      &_nLHEPdfWeight,   &b__nLHEPdfWeight);
        _currentTreePtr->SetBranchAddress("LHEPdfWeight",        _LHEPdfWeight,    &b__LHEPdfWeight);
        _currentTreePtr->SetBranchAddress("nLHEScaleWeight",    &_nLHEScaleWeight, &b__nLHEScaleWeight);
        _currentTreePtr->SetBranchAddress("LHEScaleWeight",      _LHEScaleWeight,  &b__LHEScaleWeight);
        _currentTreePtr->SetBranchAddress("nPSWeight",          &_nPSWeight,       &b__nPSWeight);
        _currentTreePtr->SetBranchAddress("PSWeight",            _PSWeight,        &b__PSWeight);
    }
    // gen particles
    if (containsGeneratorInfo()) {
        _currentTreePtr->SetBranchAddress("nGenPart",                  &_nGenPart,                  &b__nGenPart);
        _currentTreePtr->SetBranchAddress("GenPart_pt",                _GenPart_pt,                 &b__GenPart_pt);
        _currentTreePtr->SetBranchAddress("GenPart_eta",               _GenPart_eta,                &b__GenPart_eta);
        _currentTreePtr->SetBranchAddress("GenPart_phi",               _GenPart_phi,                &b__GenPart_phi);
        _currentTreePtr->SetBranchAddress("GenPart_mass",              _GenPart_mass,               &b__GenPart_mass);
        _currentTreePtr->SetBranchAddress("GenPart_genPartIdxMother",  _GenPart_genPartIdxMother,   &b__GenPart_genPartIdxMother);
        _currentTreePtr->SetBranchAddress("GenPart_pdgId",             _GenPart_pdgId,              &b__GenPart_pdgId);
        _currentTreePtr->SetBranchAddress("GenPart_status",            _GenPart_status,             &b__GenPart_status);
        _currentTreePtr->SetBranchAddress("GenPart_statusFlags",       _GenPart_statusFlags,        &b__GenPart_statusFlags);
        _currentTreePtr->SetBranchAddress("GenMET_pt",                 &_GenMET_pt,                 &b__GenMET_pt);
        _currentTreePtr->SetBranchAddress("GenMET_phi",                &_GenMET_phi,                &b__GenMET_phi);

        _currentTreePtr->SetBranchAddress("GenJet_eta",                _GenJet_eta,            &b__GenJet_eta);
        _currentTreePtr->SetBranchAddress("GenJet_mass",               _GenJet_mass,           &b__GenJet_mass);
        _currentTreePtr->SetBranchAddress("GenJet_phi",                _GenJet_phi,            &b__GenJet_phi);
        _currentTreePtr->SetBranchAddress("GenJet_pt",                 _GenJet_pt,             &b__GenJet_pt);
        _currentTreePtr->SetBranchAddress("GenJet_partonFlavour",      _GenJet_partonFlavour,  &b__GenJet_partonFlavour);
        _currentTreePtr->SetBranchAddress("GenJet_hadronFlavour",      _GenJet_hadronFlavour,  &b__GenJet_hadronFlavour);
        _currentTreePtr->SetBranchAddress("nGenJet",                  &_nGenJet,               &b__nGenJet);
        
        _currentTreePtr->SetBranchAddress("nLHEPart",                 &_nLHEPart,              &b__nLHEPart);
        _currentTreePtr->SetBranchAddress("LHEPart_pt",                _LHEPart_pt,            &b__LHEPart_pt);
        _currentTreePtr->SetBranchAddress("LHEPart_eta",               _LHEPart_eta,           &b__LHEPart_eta);
        _currentTreePtr->SetBranchAddress("LHEPart_phi",               _LHEPart_phi,           &b__LHEPart_phi);
        _currentTreePtr->SetBranchAddress("LHEPart_mass",              _LHEPart_mass,          &b__LHEPart_mass);
        _currentTreePtr->SetBranchAddress("LHEPart_status",            _LHEPart_status,        &b__LHEPart_status);
        _currentTreePtr->SetBranchAddress("LHEPart_pdgId",             _LHEPart_pdgId,         &b__LHEPart_pdgId);
    }
    // variables only related to electrons
    electronReader = new LightLeptonReader(*this, _currentTreePtr, "Electron");
    _currentTreePtr->SetBranchAddress("Electron_tightCharge",           _Electron_tightCharge,            &b__Electron_tightCharge);
    _currentTreePtr->SetBranchAddress("Electron_convVeto",              _Electron_convVeto,               &b__Electron_convVeto);
    _currentTreePtr->SetBranchAddress("Electron_lostHits",              _Electron_lostHits,               &b__Electron_lostHits);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2Iso",        _Electron_mvaFall17V2Iso,         &b__Electron_mvaFall17V2Iso);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso",      _Electron_mvaFall17V2noIso,       &b__Electron_mvaFall17V2noIso);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WPL",  _Electron_mvaFall17V2noIso_WPL,   &b__Electron_mvaFall17V2noIso_WPL);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WP80", _Electron_mvaFall17V2noIso_WP80,  &b__Electron_mvaFall17V2noIso_WP80);
    _currentTreePtr->SetBranchAddress("Electron_mvaFall17V2noIso_WP90", _Electron_mvaFall17V2noIso_WP90,  &b__Electron_mvaFall17V2noIso_WP90);
    _currentTreePtr->SetBranchAddress("Electron_deltaEtaSC",            _Electron_deltaEtaSC,             &b__Electron_deltaEtaSC);
    _currentTreePtr->SetBranchAddress("Electron_eInvMinusPInv",         _Electron_eInvMinusPInv,          &b__Electron_eInvMinusPInv);
    _currentTreePtr->SetBranchAddress("Electron_hoe",                   _Electron_hoe,                    &b__Electron_hoe);
    _currentTreePtr->SetBranchAddress("Electron_sieie",                 _Electron_sieie,                  &b__Electron_sieie);
    _currentTreePtr->SetBranchAddress("Electron_cutBased",              _Electron_cutBased,               &b__Electron_cutBased);
    _currentTreePtr->SetBranchAddress("Electron_eCorr",                 _Electron_eCorr,                  &b__Electron_eCorr);
    _currentTreePtr->SetBranchAddress("Electron_dEscaleDown",           _Electron_dEscaleDown,            &b__Electron_dEscaleDown);
    _currentTreePtr->SetBranchAddress("Electron_dEscaleUp",             _Electron_dEscaleUp,              &b__Electron_dEscaleUp);
    _currentTreePtr->SetBranchAddress("Electron_dEsigmaDown",           _Electron_dEsigmaDown,            &b__Electron_dEsigmaDown);
    _currentTreePtr->SetBranchAddress("Electron_dEsigmaUp",             _Electron_dEsigmaUp,              &b__Electron_dEsigmaUp);

    // variables only related to muons
    muonReader = new LightLeptonReader(*this, _currentTreePtr, "Muon");
    // does this branch work in data?
    _currentTreePtr->SetBranchAddress("Muon_corrected_pt",       _Muon_corrected_pt,     &b__Muon_corrected_pt);
    _currentTreePtr->SetBranchAddress("Muon_ptErr",              _Muon_ptErr,            &b__Muon_ptErr);
    _currentTreePtr->SetBranchAddress("Muon_pfRelIso04_all",     _Muon_pfRelIso04_all,   &b__Muon_pfRelIso04_all);
    _currentTreePtr->SetBranchAddress("Muon_segmentComp",        _Muon_segmentComp,      &b__Muon_segmentComp);
    _currentTreePtr->SetBranchAddress("Muon_looseId",            _Muon_looseId,          &b__Muon_looseId);
    _currentTreePtr->SetBranchAddress("Muon_mediumId",           _Muon_mediumId,         &b__Muon_mediumId);
    _currentTreePtr->SetBranchAddress("Muon_tightId",            _Muon_tightId,          &b__Muon_tightId);
    _currentTreePtr->SetBranchAddress("Muon_isGlobal",        _Muon_isGlobal,        &b__Muon_isGlobal);
    _currentTreePtr->SetBranchAddress("Muon_isTracker",       _Muon_isTracker,       &b__Muon_isTracker);
    _currentTreePtr->SetBranchAddress("Muon_isStandalone",    _Muon_isStandalone,    &b__Muon_isStandalone);

    // variables related to taus
    tauReader = new LeptonReader(*this, _currentTreePtr, "Tau");

    // variables related to jets
    _currentTreePtr->SetBranchAddress("nJet",               &_nJet,               &b__nJet);
    _currentTreePtr->SetBranchAddress("Jet_pt",              _Jet_pt,             &b__Jet_pt);
    _currentTreePtr->SetBranchAddress("Jet_pt_nom",          _Jet_pt_nom,         &b__Jet_pt_nom);
    _currentTreePtr->SetBranchAddress("Jet_mass_nom",        _Jet_mass_nom,       &b__Jet_mass_nom);
    _currentTreePtr->SetBranchAddress("Jet_eta",             _Jet_eta,            &b__Jet_eta);
    _currentTreePtr->SetBranchAddress("Jet_phi",             _Jet_phi,            &b__Jet_phi);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepB",       _Jet_bTagDeepB,      &b__Jet_bTagDeepB);
    _currentTreePtr->SetBranchAddress("Jet_btagDeepFlavB",   _Jet_bTagDeepFlavB,  &b__Jet_bTagDeepFlavB);
    _currentTreePtr->SetBranchAddress("Jet_nConstituents",   _Jet_nConstituents,  &b__Jet_nConstituents);
    if (containsGeneratorInfo()) {
        _currentTreePtr->SetBranchAddress("Jet_hadronFlavour", _Jet_hadronFlavor, &b__Jet_hadronFlavor);
        // this branch is not present in data!
    }
    _currentTreePtr->SetBranchAddress("Jet_jetId", _Jet_jetId, &b__Jet_jetId);

    // variables related to missing transverse energy
    // Default MET
    _currentTreePtr->SetBranchAddress("MET_pt",  &_MET_pt,  &b__MET_pt);
    _currentTreePtr->SetBranchAddress("MET_phi", &_MET_phi, &b__MET_phi);
    // JECs reapplied, smeared MET: 
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt",   &_MET_T1Smear_pt,   &b__MET_T1Smear_pt);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi",  &_MET_T1Smear_phi,  &b__MET_T1Smear_phi);
    // Unclustered MET
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_unclustEnUp",   &_MET_T1Smear_pt_unclustEnUp,   &b__MET_T1Smear_pt_unclustEnUp);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_unclustEnUp",  &_MET_T1Smear_phi_unclustEnUp,  &b__MET_T1Smear_phi_unclustEnUp);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_unclustEnDown", &_MET_T1Smear_pt_unclustEnDown, &b__MET_T1Smear_pt_unclustEnDown);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_unclustEnDown", &_MET_T1Smear_phi_unclustEnDown, &b__MET_T1Smear_phi_unclustEnDown);

    // Total JEC:
    _currentTreePtr->SetBranchAddress("Jet_pt_jesTotalUp",             _Jet_pt_jesTotalUp,            &b__Jet_pt_jesTotalUp);
    _currentTreePtr->SetBranchAddress("Jet_pt_jesTotalDown",           _Jet_pt_jesTotalDown,          &b__Jet_pt_jesTotalDown);
    _currentTreePtr->SetBranchAddress("Jet_mass_jesTotalUp",           _Jet_mass_jesTotalUp,          &b__Jet_mass_jesTotalUp);
    _currentTreePtr->SetBranchAddress("Jet_mass_jesTotalDown",         _Jet_mass_jesTotalDown,        &b__Jet_mass_jesTotalDown);
    _currentTreePtr->SetBranchAddress("MET_T1_pt_jesTotalUp",         &_MET_T1_pt_jesTotalUp,         &b__MET_T1_pt_jesTotalUp);
    _currentTreePtr->SetBranchAddress("MET_T1_phi_jesTotalUp",        &_MET_T1_phi_jesTotalUp,        &b__MET_T1_phi_jesTotalUp);
    _currentTreePtr->SetBranchAddress("MET_T1_pt_jesTotalDown",       &_MET_T1_pt_jesTotalDown,       &b__MET_T1_pt_jesTotalDown);
    _currentTreePtr->SetBranchAddress("MET_T1_phi_jesTotalDown",      &_MET_T1_phi_jesTotalDown,      &b__MET_T1_phi_jesTotalDown);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_jesTotalUp",    & _MET_T1Smear_pt_jesTotalUp,   &b__MET_T1Smear_pt_jesTotalUp);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_jesTotalUp",   & _MET_T1Smear_phi_jesTotalUp,  &b__MET_T1Smear_phi_jesTotalUp);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_jesTotalDown",  & _MET_T1Smear_pt_jesTotalDown, &b__MET_T1Smear_pt_jesTotalDown);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_jesTotalDown", &_MET_T1Smear_phi_jesTotalDown, &b__MET_T1Smear_phi_jesTotalDown);

    // Jet JER smearing:
    _currentTreePtr->SetBranchAddress("Jet_pt_jer0Up",             _Jet_pt_jer0Up,            &b__Jet_pt_jer0Up);
    _currentTreePtr->SetBranchAddress("Jet_pt_jer0Down",           _Jet_pt_jer0Down,          &b__Jet_pt_jer0Down);
    _currentTreePtr->SetBranchAddress("Jet_pt_jer1Up",             _Jet_pt_jer1Up,            &b__Jet_pt_jer1Up);
    _currentTreePtr->SetBranchAddress("Jet_pt_jer1Down",           _Jet_pt_jer1Down,          &b__Jet_pt_jer1Down);
    _currentTreePtr->SetBranchAddress("Jet_mass_jer0Up",           _Jet_mass_jer0Up,          &b__Jet_mass_jer0Up);
    _currentTreePtr->SetBranchAddress("Jet_mass_jer0Down",         _Jet_mass_jer0Down,        &b__Jet_mass_jer0Down);
    _currentTreePtr->SetBranchAddress("Jet_mass_jer1Up",           _Jet_mass_jer1Up,          &b__Jet_mass_jer1Up);
    _currentTreePtr->SetBranchAddress("Jet_mass_jer1Down",         _Jet_mass_jer1Down,        &b__Jet_mass_jer1Down);
    // MET JER smearing:
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_jer0Down",  &_MET_T1Smear_pt_jer0Down,  &b__MET_T1Smear_pt_jer0Down);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_jer0Down", &_MET_T1Smear_phi_jer0Down, &b__MET_T1Smear_phi_jer0Down);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_jer0Up",    &_MET_T1Smear_pt_jer0Up,    &b__MET_T1Smear_pt_jer0Up);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_jer0Up",   &_MET_T1Smear_phi_jer0Up,   &b__MET_T1Smear_phi_jer0Up);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_jer1Down",  &_MET_T1Smear_pt_jer1Down,  &b__MET_T1Smear_pt_jer1Down);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_jer1Down", &_MET_T1Smear_phi_jer1Down, &b__MET_T1Smear_phi_jer1Down);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_pt_jer1Up",    &_MET_T1Smear_pt_jer1Up,    &b__MET_T1Smear_pt_jer1Up);
    _currentTreePtr->SetBranchAddress("MET_T1Smear_phi_jer1Up",   &_MET_T1Smear_phi_jer1Up,   &b__MET_T1Smear_phi_jer1Up);

    // variables related to triggers and MET filters
    _currentTreePtr->SetBranchAddress("HLT_trigger_e",      &_HLT_trigger_e,      &b__HLT_trigger_e);
    _currentTreePtr->SetBranchAddress("HLT_trigger_ee",     &_HLT_trigger_ee,     &b__HLT_trigger_ee);
    _currentTreePtr->SetBranchAddress("HLT_trigger_eee",    &_HLT_trigger_eee,    &b__HLT_trigger_ee);
    _currentTreePtr->SetBranchAddress("HLT_trigger_m",      &_HLT_trigger_m,      &b__HLT_trigger_m);
    _currentTreePtr->SetBranchAddress("HLT_trigger_mm",     &_HLT_trigger_mm,     &b__HLT_trigger_mm);
    _currentTreePtr->SetBranchAddress("HLT_trigger_mmm",    &_HLT_trigger_mmm,    &b__HLT_trigger_mmm);
    _currentTreePtr->SetBranchAddress("HLT_trigger_em",     &_HLT_trigger_em,     &b__HLT_trigger_em);
    _currentTreePtr->SetBranchAddress("HLT_trigger_emm",    &_HLT_trigger_emm,    &b__HLT_trigger_emm);
    _currentTreePtr->SetBranchAddress("HLT_trigger_eem",    &_HLT_trigger_eem,    &b__HLT_trigger_eem);
    _currentTreePtr->SetBranchAddress("HLT_trigger_ref",    &_HLT_trigger_ref,    &b__HLT_trigger_ref);
    _currentTreePtr->SetBranchAddress("HLT_trigger_FR",     &_HLT_trigger_FR,     &b__HLT_trigger_FR);
    _currentTreePtr->SetBranchAddress("HLT_trigger_FR_iso", &_HLT_trigger_FR_iso, &b__HLT_trigger_FR_iso);
    // The following should never be used!
    _currentTreePtr->SetBranchAddress("Flag_METFilters",    &_Flag_METFilters,    &b__Flag_METFilters);
    // Individual flags instead:
    _currentTreePtr->SetBranchAddress("Flag_goodVertices",                         &_Flag_goodVertices,                        &b__Flag_goodVertices);
    _currentTreePtr->SetBranchAddress("Flag_globalSuperTightHalo2016Filter",       &_Flag_globalSuperTightHalo2016Filter,      &b__Flag_globalSuperTightHalo2016Filter);
    _currentTreePtr->SetBranchAddress("Flag_HBHENoiseFilter",                      &_Flag_HBHENoiseFilter,                     &b__Flag_HBHENoiseFilter);
    _currentTreePtr->SetBranchAddress("Flag_HBHENoiseIsoFilter",                   &_Flag_HBHENoiseIsoFilter,                  &b__Flag_HBHENoiseIsoFilter);
    _currentTreePtr->SetBranchAddress("Flag_BadPFMuonFilter",                      &_Flag_BadPFMuonFilter,                     &b__Flag_BadPFMuonFilter);
    _currentTreePtr->SetBranchAddress("Flag_BadPFMuonDzFilter",                    &_Flag_BadPFMuonDzFilter,                   &b__Flag_BadPFMuonDzFilter);
    _currentTreePtr->SetBranchAddress("Flag_eeBadScFilter",                        &_Flag_eeBadScFilter,                       &b__Flag_eeBadScFilter);
    // 2017 + 2018 only:
    _currentTreePtr->SetBranchAddress("Flag_ecalBadCalibFilter",                   &_Flag_ecalBadCalibFilter,                  &b__Flag_ecalBadCalibFilter);
    // 2016 only:
    _currentTreePtr->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter",   &_Flag_EcalDeadCellTriggerPrimitiveFilter,  &b__Flag_EcalDeadCellTriggerPrimitiveFilter);

    if (getReadGroupedJECVariations()) {
        initializeJESVariations(_currentTreePtr);
        setMapBranchAddressesWithNameMap( _currentTreePtr, _Jet_pt_jesSourcesUp,             b__jet_pt_jesSourcesUp, _Jet_pt_jesSourcesUpNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _Jet_pt_jesSourcesDown,           b__jet_pt_jesSourcesDown, _Jet_pt_jesSourcesDownNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _Jet_mass_jesSourcesUp,           b__jet_mass_jesSourcesUp, _Jet_mass_jesSourcesUpNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _Jet_mass_jesSourcesDown,         b__jet_mass_jesSourcesDown, _Jet_mass_jesSourcesDownNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _MET_T1Smear_pt_jesSourcesUp,     b__met_pt_jesSourcesUp, _MET_T1Smear_pt_jesSourcesUpNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _MET_T1Smear_pt_jesSourcesDown,   b__met_pt_jesSourcesDown, _MET_T1Smear_pt_jesSourcesDownNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _MET_T1Smear_phi_jesSourcesUp,    b__met_phi_jesSourcesUp, _MET_T1Smear_phi_jesSourcesUpNames );
        setMapBranchAddressesWithNameMap( _currentTreePtr, _MET_T1Smear_phi_jesSourcesDown,  b__met_phi_jesSourcesDown, _MET_T1Smear_phi_jesSourcesDownNames );
    }

    std::cout << "Done setting branches" << std::endl;
}

void NanoReader::setOutputTree(TTree* outputTree) {
    outputTree->Branch("run", &_run);
    outputTree->Branch("luminosityBlock", &_luminosityBlock);
    outputTree->Branch("event", &_event);

    if (isMC()) {
        outputTree->Branch("L1PreFiringWeight_Nom",  &_L1PreFiringWeight_Nom);
        outputTree->Branch("L1PreFiringWeight_Up",   &_L1PreFiringWeight_Up);
        outputTree->Branch("L1PreFiringWeight_Dn",   &_L1PreFiringWeight_Dn);
    }

    outputTree->Branch("Pileup_nTrueInt", &_Pileup_nTrueInt);
    outputTree->Branch("PV_npvs", &_PV_npvs);

    if (containsGeneratorInfo()) {
        outputTree->Branch("genWeight", &_genWeight);
        outputTree->Branch("nLHEPdfWeight", &_nLHEPdfWeight);
        outputTree->Branch("LHEPdfWeight", _LHEPdfWeight);
        outputTree->Branch("nLHEScaleWeight", &_nLHEScaleWeight);
        outputTree->Branch("LHEScaleWeight", _LHEScaleWeight);
        outputTree->Branch("nPSWeight", &_nPSWeight);
        outputTree->Branch("PSWeight", _PSWeight);
    }

    if (containsGeneratorInfo()) {
        outputTree->Branch("nGenPart", &_nGenPart);
        outputTree->Branch("GenPart_pt", _GenPart_pt);
        outputTree->Branch("GenPart_eta", _GenPart_eta);
        outputTree->Branch("GenPart_phi", _GenPart_phi);
        outputTree->Branch("GenPart_mass", _GenPart_mass);
        outputTree->Branch("GenPart_genPartIdxMother", _GenPart_genPartIdxMother);
        outputTree->Branch("GenPart_pdgId", _GenPart_pdgId);
        outputTree->Branch("GenPart_status", _GenPart_status);
        outputTree->Branch("GenPart_statusFlags", _GenPart_statusFlags);
        outputTree->Branch("GenMET_pt", &_GenMET_pt);
        outputTree->Branch("GenMET_phi", &_GenMET_phi);

        outputTree->Branch("GenJet_eta", _GenJet_eta);
        outputTree->Branch("GenJet_mass", _GenJet_mass);
        outputTree->Branch("GenJet_phi", _GenJet_phi);
        outputTree->Branch("GenJet_pt", _GenJet_pt);
        outputTree->Branch("GenJet_partonFlavour", _GenJet_partonFlavour);
        outputTree->Branch("GenJet_hadronFlavour", _GenJet_hadronFlavour);
        outputTree->Branch("nGenJet", &_nGenJet);
    }
    // Set ELectron branches:
    electronReader->setOutputTree(outputTree, "Electron");
    outputTree->Branch("Electron_tightCharge", _Electron_tightCharge);
    outputTree->Branch("Electron_convVeto", _Electron_convVeto);
    outputTree->Branch("Electron_lostHits", _Electron_lostHits);
    outputTree->Branch("Electron_mvaFall17V2Iso", _Electron_mvaFall17V2Iso);
    outputTree->Branch("Electron_mvaFall17V2noIso", _Electron_mvaFall17V2noIso);
    outputTree->Branch("Electron_mvaFall17V2noIso_WPL", _Electron_mvaFall17V2noIso_WPL);
    outputTree->Branch("Electron_mvaFall17V2noIso_WP80", _Electron_mvaFall17V2noIso_WP80);
    outputTree->Branch("Electron_mvaFall17V2noIso_WP90", _Electron_mvaFall17V2noIso_WP90);
    outputTree->Branch("Electron_deltaEtaSC", _Electron_deltaEtaSC);
    outputTree->Branch("Electron_eInvMinusPInv", _Electron_eInvMinusPInv);
    outputTree->Branch("Electron_hoe", _Electron_hoe);
    outputTree->Branch("Electron_sieie", _Electron_sieie);
    outputTree->Branch("Electron_cutBased", _Electron_cutBased);
    outputTree->Branch("Electron_eCorr", _Electron_eCorr);
    outputTree->Branch("Electron_dEscaleDown", _Electron_dEscaleDown);
    outputTree->Branch("Electron_dEscaleUp", _Electron_dEscaleUp);
    outputTree->Branch("Electron_dEsigmaDown", _Electron_dEsigmaDown);
    outputTree->Branch("Electron_dEsigmaUp", _Electron_dEsigmaUp);

    // Set Muon branches:
    muonReader->setOutputTree(outputTree, "Muon");
    outputTree->Branch("Muon_corrected_pt", _Muon_corrected_pt);
    outputTree->Branch("Muon_ptErr", _Muon_ptErr);
    outputTree->Branch("Muon_pfRelIso04_all", _Muon_pfRelIso04_all);
    outputTree->Branch("Muon_segmentComp", _Muon_segmentComp);
    outputTree->Branch("Muon_looseId", _Muon_looseId);
    outputTree->Branch("Muon_mediumId", _Muon_mediumId);
    outputTree->Branch("Muon_tightId", _Muon_tightId);
    outputTree->Branch("Muon_isGlobal", _Muon_isGlobal);
    outputTree->Branch("Muon_isTracker", _Muon_isTracker);
    outputTree->Branch("Muon_isStandalone", _Muon_isStandalone);

    // Set Tau branches:
    tauReader->setOutputTree(outputTree, "Tau");

    // Set Jet branches:
    outputTree->Branch("nJet", &_nJet);
    outputTree->Branch("Jet_pt", _Jet_pt);
    outputTree->Branch("Jet_pt_nom", _Jet_pt_nom);
    outputTree->Branch("Jet_eta", _Jet_eta);
    outputTree->Branch("Jet_phi", _Jet_phi);
    outputTree->Branch("Jet_btagDeepB", _Jet_bTagDeepB);
    outputTree->Branch("Jet_btagDeepFlavB", _Jet_bTagDeepFlavB);
    outputTree->Branch("Jet_nConstituents", _Jet_nConstituents);
    if (containsGeneratorInfo()) {
        outputTree->Branch("Jet_hadronFlavour", _Jet_hadronFlavor);
    }
    outputTree->Branch("Jet_jetId", _Jet_jetId);
    
    // Set MET branches:
    outputTree->Branch("MET_pt", &_MET_pt);
    outputTree->Branch("MET_phi", &_MET_phi);

    // Set triggering and MET Filters output:
    outputTree->Branch("HLT_trigger_e", &_HLT_trigger_e);
    outputTree->Branch("HLT_trigger_ee", &_HLT_trigger_ee);
    outputTree->Branch("HLT_trigger_eee", &_HLT_trigger_eee);
    outputTree->Branch("HLT_trigger_m", &_HLT_trigger_m);
    outputTree->Branch("HLT_trigger_mm", &_HLT_trigger_mm);
    outputTree->Branch("HLT_trigger_mmm", &_HLT_trigger_mmm);
    outputTree->Branch("HLT_trigger_em", &_HLT_trigger_em);
    outputTree->Branch("HLT_trigger_emm", &_HLT_trigger_emm);
    outputTree->Branch("HLT_trigger_eem", &_HLT_trigger_eem);
    outputTree->Branch("HLT_trigger_ref", &_HLT_trigger_ref);
    outputTree->Branch("HLT_trigger_FR", &_HLT_trigger_FR);
    outputTree->Branch("HLT_trigger_FR_iso", &_HLT_trigger_FR_iso);

    outputTree->Branch("Flag_METFilters", &_Flag_METFilters);
    outputTree->Branch("Flag_goodVertices", &_Flag_goodVertices);
    outputTree->Branch("Flag_globalSuperTightHalo2016Filter", &_Flag_globalSuperTightHalo2016Filter);
    outputTree->Branch("Flag_HBHENoiseFilter", &_Flag_HBHENoiseFilter);
    outputTree->Branch("Flag_HBHENoiseIsoFilter", &_Flag_HBHENoiseIsoFilter);
    outputTree->Branch("Flag_BadPFMuonFilter", &_Flag_BadPFMuonFilter);
    outputTree->Branch("Flag_BadPFMuonDzFilter", &_Flag_BadPFMuonDzFilter);
    outputTree->Branch("Flag_eeBadScFilter", &_Flag_eeBadScFilter);
    outputTree->Branch("Flag_ecalBadCalibFilter", &_Flag_ecalBadCalibFilter);
    outputTree->Branch("Flag_EcalDeadCellTriggerPrimitiveFilter", &_Flag_EcalDeadCellTriggerPrimitiveFilter);
}
