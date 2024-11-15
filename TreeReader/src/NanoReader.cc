#include "../interface/NanoReader.h"
#include "../../Tools/interface/stringTools.h"

#include "../../Event/interface/Event.h"
#include "../../constants/luminosities.h"


NanoReader::NanoReader() :
    BaseReader()
{
    // _sourcesJEC_Ids = new std::map<std::string, size_t>();
    // _groupedJEC_Ids = new std::map<std::string, size_t>();
    electronReader = new LightLeptonReader(*this, "Electron");
    muonReader = new LightLeptonReader(*this, "Muon");
    tauReader = new LeptonReader(*this, "Tau");
}


NanoReader::NanoReader(const std::string& sampleListFile, const std::string& sampleDirectory) :
    BaseReader(sampleListFile, sampleDirectory)
{   
    // _sourcesJEC_Ids = new std::map<std::string, size_t>();
    // _groupedJEC_Ids = new std::map<std::string, size_t>();
    std::cout << "in nanoreader constructor" << std::endl;
    electronReader = new LightLeptonReader(*this, "Electron");
    muonReader = new LightLeptonReader(*this, "Muon");
    tauReader = new LeptonReader(*this, "Tau");

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

    std::cout << "Setting branches" << std::endl;
    _currentTreePtr->SetMakeClass(1);

    // set branch addresses of all variables
    _currentTreePtr->SetBranchAddress("run", &_run, &b__run);
    _currentTreePtr->SetBranchAddress("luminosityBlock", &_luminosityBlock, &b__luminosityBlock);
    _currentTreePtr->SetBranchAddress("event", &_event, &b__event);

    _currentTreePtr->SetBranchAddress("PV_npvs", &_PV_npvs, &b__PV_npvs);

    if (isMC()) {
        // TODO: check if the isMC flag works
        _currentTreePtr->SetBranchAddress("Pileup_nTrueInt", &_Pileup_nTrueInt, &b__Pileup_nTrueInt);
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
        _hasPLInfo = true;
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
    std::cout << electronReader << std::endl;
    electronReader->InitTree(_currentTreePtr);
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
    std::cout << muonReader << std::endl;

    muonReader->InitTree(_currentTreePtr);
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
    std::cout << tauReader << std::endl;

    tauReader->InitTree(_currentTreePtr);

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
    _currentTreePtr->SetBranchAddress("MET_T1_pt",  &_MET_T1_pt,  &b__MET_T1_phi);
    _currentTreePtr->SetBranchAddress("MET_T1_phi", &_MET_T1_phi, &b__MET_T1_phi);

    if (containsGeneratorInfo()) {
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
    }
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
    outputTree->Branch("run", &_run, "run/i");
    outputTree->Branch("luminosityBlock", &_luminosityBlock, "luminosityBlock/i");
    outputTree->Branch("event", &_event, "event/i");

    if (isMC()) {
        outputTree->Branch("L1PreFiringWeight_Nom",  &_L1PreFiringWeight_Nom, "L1PreFiringWeight_Nom/F");
        outputTree->Branch("L1PreFiringWeight_Up",   &_L1PreFiringWeight_Up, "L1PreFiringWeight_Up/F");
        outputTree->Branch("L1PreFiringWeight_Dn",   &_L1PreFiringWeight_Dn, "L1PreFiringWeight_Dn/F");
        outputTree->Branch("Pileup_nTrueInt", &_Pileup_nTrueInt, "Pileup_nTrueInt/F");
    }

    outputTree->Branch("PV_npvs", &_PV_npvs, "PV_npvs/I");

    if (containsGeneratorInfo()) {
        outputTree->Branch("genWeight", &_genWeight, "genWeight/F");
        outputTree->Branch("nLHEPdfWeight", &_nLHEPdfWeight, "nLHEPdfWeight/i");
        outputTree->Branch("LHEPdfWeight", _LHEPdfWeight, "LHEPdfWeight[nLHEPdfWeight]/F");
        outputTree->Branch("nLHEScaleWeight", &_nLHEScaleWeight, "nLHEScaleWeight/i");
        outputTree->Branch("LHEScaleWeight", _LHEScaleWeight, "LHEScaleWeight[nLHEScaleWeight]/F");
        outputTree->Branch("nPSWeight", &_nPSWeight, "nPSWeight/i");
        outputTree->Branch("PSWeight", _PSWeight, "PSWeight[nPSWeight]/F");
    }

    if (containsGeneratorInfo()) {
        outputTree->Branch("nGenPart", &_nGenPart,                                 "nGenPart/i");
        outputTree->Branch("GenPart_pt", _GenPart_pt,                              "GenPart_pt[nGenPart]/F");
        outputTree->Branch("GenPart_eta", _GenPart_eta,                            "GenPart_eta[nGenPart]/F");
        outputTree->Branch("GenPart_phi", _GenPart_phi,                            "GenPart_phi[nGenPart]/F");
        outputTree->Branch("GenPart_mass", _GenPart_mass,                          "GenPart_mass[nGenPart]/F");
        outputTree->Branch("GenPart_genPartIdxMother", _GenPart_genPartIdxMother,  "GenPart_genPartIdxMother[nGenPart]/I");
        outputTree->Branch("GenPart_pdgId", _GenPart_pdgId,                        "GenPart_pdgId[nGenPart]/I");
        outputTree->Branch("GenPart_status", _GenPart_status,                      "GenPart_status[nGenPart]/I");
        outputTree->Branch("GenPart_statusFlags", _GenPart_statusFlags,            "GenPart_statusFlags[nGenPart]/I");
        outputTree->Branch("GenMET_pt", &_GenMET_pt,                               "GenMET_pt/F");
        outputTree->Branch("GenMET_phi", &_GenMET_phi,                             "GenMET_phi/F");

        outputTree->Branch("GenJet_eta", _GenJet_eta, "GenJet_eta[nGenJet]/F");
        outputTree->Branch("GenJet_mass", _GenJet_mass, "GenJet_mass[nGenJet]/F");
        outputTree->Branch("GenJet_phi", _GenJet_phi, "GenJet_phi[nGenJet]/F");
        outputTree->Branch("GenJet_pt", _GenJet_pt, "GenJet_pt[nGenJet]/F");
        outputTree->Branch("GenJet_partonFlavour", _GenJet_partonFlavour, "GenJet_partonFlavour[nGenJet]/I");
        outputTree->Branch("GenJet_hadronFlavour", _GenJet_hadronFlavour, "GenJet_hadronFlavour[nGenJet]/b");
        outputTree->Branch("nGenJet", &_nGenJet, "nGenJet/i");
    }
    // Set ELectron branches:
    electronReader->setOutputTree(outputTree);
    outputTree->Branch("Electron_tightCharge", _Electron_tightCharge, "Electron_tightCharge[nElectron]/I");
    outputTree->Branch("Electron_convVeto", _Electron_convVeto, "Electron_convVeto[nElectron]/O");
    outputTree->Branch("Electron_lostHits", _Electron_lostHits, "Electron_lostHits[nElectron]/b");
    outputTree->Branch("Electron_mvaFall17V2Iso", _Electron_mvaFall17V2Iso, "Electron_mvaFall17V2Iso[nElectron]/F");
    outputTree->Branch("Electron_mvaFall17V2noIso", _Electron_mvaFall17V2noIso, "Electron_mvaFall17V2noIso[nElectron]/F");
    outputTree->Branch("Electron_mvaFall17V2noIso_WPL", _Electron_mvaFall17V2noIso_WPL, "Electron_mvaFall17V2noIso_WPL[nElectron]/O");
    outputTree->Branch("Electron_mvaFall17V2noIso_WP80", _Electron_mvaFall17V2noIso_WP80, "Electron_mvaFall17V2noIso_WP80[nElectron]/O");
    outputTree->Branch("Electron_mvaFall17V2noIso_WP90", _Electron_mvaFall17V2noIso_WP90, "Electron_mvaFall17V2noIso_WP90[nElectron]/O");
    outputTree->Branch("Electron_deltaEtaSC", _Electron_deltaEtaSC, "Electron_deltaEtaSC[nElectron]/F");
    outputTree->Branch("Electron_eInvMinusPInv", _Electron_eInvMinusPInv, "Electron_eInvMinusPInv[nElectron]/F");
    outputTree->Branch("Electron_hoe", _Electron_hoe, "Electron_hoe[nElectron]/F");
    outputTree->Branch("Electron_sieie", _Electron_sieie, "Electron_sieie[nElectron]/F");
    outputTree->Branch("Electron_cutBased", _Electron_cutBased, "Electron_cutBased[nElectron]/I");
    outputTree->Branch("Electron_eCorr", _Electron_eCorr, "Electron_eCorr[nElectron]/F");
    outputTree->Branch("Electron_dEscaleDown", _Electron_dEscaleDown, "Electron_dEscaleDown[nElectron]/F");
    outputTree->Branch("Electron_dEscaleUp", _Electron_dEscaleUp, "Electron_dEscaleUp[nElectron]/F");
    outputTree->Branch("Electron_dEsigmaDown", _Electron_dEsigmaDown, "Electron_dEsigmaDown[nElectron]/F");
    outputTree->Branch("Electron_dEsigmaUp", _Electron_dEsigmaUp, "Electron_dEsigmaUp[nElectron]/F");

    // Set Muon branches:
    muonReader->setOutputTree(outputTree);
    outputTree->Branch("Muon_corrected_pt", _Muon_corrected_pt, "Muon_corrected_pt[nMuon]/F");
    outputTree->Branch("Muon_ptErr", _Muon_ptErr, "Muon_ptErr[nMuon]/F");
    outputTree->Branch("Muon_pfRelIso04_all", _Muon_pfRelIso04_all, "Muon_pfRelIso04_all[nMuon]/F");
    outputTree->Branch("Muon_segmentComp", _Muon_segmentComp, "Muon_segmentComp[nMuon]/F");
    outputTree->Branch("Muon_looseId", _Muon_looseId, "Muon_looseId[nMuon]/O");
    outputTree->Branch("Muon_mediumId", _Muon_mediumId, "Muon_mediumId[nMuon]/O");
    outputTree->Branch("Muon_tightId", _Muon_tightId, "Muon_tightId[nMuon]/O");
    outputTree->Branch("Muon_isGlobal", _Muon_isGlobal, "Muon_isGlobal[nMuon]/O");
    outputTree->Branch("Muon_isTracker", _Muon_isTracker, "Muon_isTracker[nMuon]/O");
    outputTree->Branch("Muon_isStandalone", _Muon_isStandalone, "Muon_isStandalone[nMuon]/O");

    // Set Tau branches:
    tauReader->setOutputTree(outputTree);

    // Set Jet branches:
    outputTree->Branch("nJet", &_nJet,                          "nJet/i");
    outputTree->Branch("Jet_pt", _Jet_pt,                       "Jet_pt[nJet]/F");
    outputTree->Branch("Jet_pt_nom", _Jet_pt_nom,               "Jet_pt_nom[nJet]/F");
    outputTree->Branch("Jet_mass_nom",      _Jet_mass_nom,      "Jet_mass_nom[nJet]/F");

    outputTree->Branch("Jet_eta", _Jet_eta,                     "Jet_eta[nJet]/F");
    outputTree->Branch("Jet_phi", _Jet_phi,                     "Jet_phi[nJet]/F");
    outputTree->Branch("Jet_btagDeepB", _Jet_bTagDeepB,         "Jet_btagDeepB[nJet]/F");
    outputTree->Branch("Jet_btagDeepFlavB", _Jet_bTagDeepFlavB, "Jet_btagDeepFlavB[nJet]/F");
    outputTree->Branch("Jet_nConstituents", _Jet_nConstituents, "Jet_nConstituents[nJet]/b");
    if (containsGeneratorInfo()) {
        outputTree->Branch("Jet_hadronFlavour", _Jet_hadronFlavor, "Jet_hadronFlavour[nJet]/I");
    }
    outputTree->Branch("Jet_jetId", _Jet_jetId, "Jet_jetId[nJet]/I");
    
    // Set MET branches:
    outputTree->Branch("MET_pt",  &_MET_pt, "MET_pt/F");
    outputTree->Branch("MET_phi", &_MET_phi, "MET_phi/F");
    // T1 stuff:
    outputTree->Branch("MET_T1_pt",  &_MET_T1_pt, "MET_T1_pt/F");
    outputTree->Branch("MET_T1_phi", &_MET_T1_phi, "MET_T1_phi/F");

    // Set triggering and MET Filters output:
    outputTree->Branch("HLT_trigger_e", &_HLT_trigger_e, "HLT_trigger_e/O");
    outputTree->Branch("HLT_trigger_ee", &_HLT_trigger_ee, "HLT_trigger_ee/O");
    outputTree->Branch("HLT_trigger_eee", &_HLT_trigger_eee, "HLT_trigger_eee/O");
    outputTree->Branch("HLT_trigger_m", &_HLT_trigger_m, "HLT_trigger_m/O");
    outputTree->Branch("HLT_trigger_mm", &_HLT_trigger_mm, "HLT_trigger_mm/O");
    outputTree->Branch("HLT_trigger_mmm", &_HLT_trigger_mmm, "HLT_trigger_mmm/O");
    outputTree->Branch("HLT_trigger_em", &_HLT_trigger_em, "HLT_trigger_em/O");
    outputTree->Branch("HLT_trigger_emm", &_HLT_trigger_emm, "HLT_trigger_emm/O");
    outputTree->Branch("HLT_trigger_eem", &_HLT_trigger_eem, "HLT_trigger_eem/O");
    outputTree->Branch("HLT_trigger_ref", &_HLT_trigger_ref, "HLT_trigger_ref/O");
    outputTree->Branch("HLT_trigger_FR", &_HLT_trigger_FR, "HLT_trigger_FR/O");
    outputTree->Branch("HLT_trigger_FR_iso", &_HLT_trigger_FR_iso, "HLT_trigger_FR_iso/O");

    outputTree->Branch("Flag_METFilters", &_Flag_METFilters,                                                 "Flag_METFilters/O");
    outputTree->Branch("Flag_goodVertices", &_Flag_goodVertices,                                             "Flag_goodVertices/O");
    outputTree->Branch("Flag_globalSuperTightHalo2016Filter", &_Flag_globalSuperTightHalo2016Filter,         "Flag_globalSuperTightHalo2016Filter/O");
    outputTree->Branch("Flag_HBHENoiseFilter", &_Flag_HBHENoiseFilter,                                       "Flag_HBHENoiseFilter/O");
    outputTree->Branch("Flag_HBHENoiseIsoFilter", &_Flag_HBHENoiseIsoFilter,                                 "Flag_HBHENoiseIsoFilter/O");
    outputTree->Branch("Flag_BadPFMuonFilter", &_Flag_BadPFMuonFilter,                                       "Flag_BadPFMuonFilter/O");
    outputTree->Branch("Flag_BadPFMuonDzFilter", &_Flag_BadPFMuonDzFilter,                                   "Flag_BadPFMuonDzFilter/O");
    outputTree->Branch("Flag_eeBadScFilter", &_Flag_eeBadScFilter,                                           "Flag_eeBadScFilter/O");
    outputTree->Branch("Flag_ecalBadCalibFilter", &_Flag_ecalBadCalibFilter,                                 "Flag_ecalBadCalibFilter/O");
    outputTree->Branch("Flag_EcalDeadCellTriggerPrimitiveFilter", &_Flag_EcalDeadCellTriggerPrimitiveFilter, "Flag_EcalDeadCellTriggerPrimitiveFilter/O");
}
