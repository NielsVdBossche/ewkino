#include "../interface/NanoReader.h"
#include "../../Tools/interface/stringTools.h"

NanoReader::NanoReader(const std::string& sampleListFile, const std::string& sampleDirectory) {
    readSamples(sampleListFile, sampleDirectory);

    _sourcesJEC_Ids = new std::map<std::string, size_t>();
    _groupedJEC_Ids = new std::map<std::string, size_t>();
}

bool NanoReader::containsGeneratorInfo() const {
    // note: only checking genWeight branch,
    // this does in principle not guarantee that e.g. LHEPdfWeight are present.
    return treeHasBranchWithName(_currentTreePtr, "genWeight");
}

bool NanoReader::containsGenParticles() const {
    return treeHasBranchWithName(_currentTreePtr, "nGenPart");
}

void NanoReader::initTree(const bool resetTriggersAndFilters) {
    checkCurrentTree();
    if (electronReader) delete electronReader;
    if (muonReader) delete muonReader;
    if (tauReader) delete tauReader;
    if (triggerReader) delete triggerReader;

    _currentTreePtr->SetMakeClass(1);

    // set branch addresses of all variables
    _currentTreePtr->SetBranchAddress("run", &_run, &b__run);
    _currentTreePtr->SetBranchAddress("luminosityBlock", &_luminosityBlock, &b__luminosityBlock);
    _currentTreePtr->SetBranchAddress("event", &_event, &b__event);

    _currentTreePtr->SetBranchAddress("L1PreFiringWeight_Nom",  &_L1PreFiringWeight_Nom,  &b__L1PreFiringWeight_Nom);
    _currentTreePtr->SetBranchAddress("L1PreFiringWeight_Up",   &_L1PreFiringWeight_Up,   &b__L1PreFiringWeight_Up);
    _currentTreePtr->SetBranchAddress("L1PreFiringWeight_Dn",   &_L1PreFiringWeight_Dn,   &b__L1PreFiringWeight_Dn);

    _currentTreePtr->SetBranchAddress("_Pileup_nTrueInt", &_Pileup_nTrueInt, &b__Pileup_nTrueInt);
    _currentTreePtr->SetBranchAddress("PV_npvs", &_PV_npvs, &b__PV_npvs);
    // generator info
    if (containsGeneratorInfo()) {
        _currentTreePtr->SetBranchAddress("genWeight",          &_genWeight,       &b__genWeight);
        _currentTreePtr->SetBranchAddress("nLHEPdfWeight",      &_nLHEPdfWeight,   &b__nLHEPdfWeight);
        _currentTreePtr->SetBranchAddress("LHEPdfWeight",        _LHEPdfWeight,    &b__LHEPdfWeight);
        _currentTreePtr->SetBranchAddress("nLHEScaleWeight",    &_nLHEScaleWeight, &b__nLHEScaleWeight);
        _currentTreePtr->SetBranchAddress("LHEScaleWeight",      _LHEScaleWeight,  &b__LHEScaleWeight);
        _currentTreePtr->SetBranchAddress("nPSWeight",          &_nPSWeight,       &b__nPSWeight);
        _currentTreePtr->SetBranchAddress("PSWeight",            _PSWeight,        &b__PSWeight);
    }
    // gen particles
    if (containsGenParticles()) {
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
    _currentTreePtr->SetBranchAddress("MET_pt",  &_MET_pt,  &b__MET_pt);
    _currentTreePtr->SetBranchAddress("MET_phi", &_MET_phi, &b__MET_phi);

    triggerReader = new TriggerReader(*this, _currentTreePtr, resetTriggersAndFilters);
    _currentTreePtr->SetBranchAddress("Flag_METFilters", &_Flag_METFilters, &b__Flag_METFilters);
}
