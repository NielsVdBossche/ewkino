#include "../interface/NanoReader.h"

NanoReader::LeptonReader::LeptonReader(NanoReader& nano, TTree* _currentTreePtr, std::string leptonType) :
    nanoReader(nano)
{
    // LeptonType: Electron, Muon or Tau
    _currentTreePtr->SetBranchAddress(("n"+leptonType).c_str(),                  &_nLepton,                 &b__nLepton);
    _currentTreePtr->SetBranchAddress((leptonType+"_pt").c_str(),                 _Lepton_pt,               &b__Lepton_pt);
    _currentTreePtr->SetBranchAddress((leptonType+"_eta").c_str(),                _Lepton_eta,              &b__Lepton_eta);
    _currentTreePtr->SetBranchAddress((leptonType+"_phi").c_str(),                _Lepton_phi,              &b__Lepton_phi);
    // _currentTreePtr->SetBranchAddress((leptonType+"_mass").c_str(),               _Lepton_mass,             &b__Lepton_mass);
    _currentTreePtr->SetBranchAddress((leptonType+"_charge").c_str(),             _Lepton_charge,           &b__Lepton_charge);
    _currentTreePtr->SetBranchAddress((leptonType+"_dxy").c_str(),                _Lepton_dxy,              &b__Lepton_dxy);
    _currentTreePtr->SetBranchAddress((leptonType+"_dz").c_str(),                 _Lepton_dz,               &b__Lepton_dz);
    _currentTreePtr->SetBranchAddress((leptonType+"_jetIdx").c_str(),             _Lepton_jetIdx,           &b__Lepton_jetIdx);

    if (nanoReader.containsGenParticles()) {
        _currentTreePtr->SetBranchAddress((leptonType+"_genPartFlav").c_str(),        _Lepton_genPartFlav,      &b__Lepton_genPartFlav);
        _currentTreePtr->SetBranchAddress((leptonType+"_genPartIdx").c_str(),         _Lepton_genPartIdx,       &b__Lepton_genPartIdx);
    }
}

void NanoReader::LeptonReader::setOutputTree(TTree* tree, std::string leptonType) {
    tree->Branch(("n"+leptonType).c_str(),                  &_nLepton,                 ("n"+leptonType+"/I").c_str());
    tree->Branch((leptonType+"_pt").c_str(),                 _Lepton_pt,               (leptonType+"_pt[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_eta").c_str(),                _Lepton_eta,              (leptonType+"_eta[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_phi").c_str(),                _Lepton_phi,              (leptonType+"_phi[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_charge").c_str(),             _Lepton_charge,           (leptonType+"_charge[n"+leptonType+"]/I").c_str());
    tree->Branch((leptonType+"_dxy").c_str(),                _Lepton_dxy,              (leptonType+"_dxy[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_dz").c_str(),                 _Lepton_dz,               (leptonType+"_dz[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_jetIdx").c_str(),             _Lepton_jetIdx,           (leptonType+"_jetIdx[n"+leptonType+"]/I").c_str());

    if (nanoReader.containsGenParticles()) {
        tree->Branch((leptonType+"_genPartFlav").c_str(),        _Lepton_genPartFlav,      (leptonType+"_genPartFlav[n"+leptonType+"]/I").c_str());
        tree->Branch((leptonType+"_genPartIdx").c_str(),         _Lepton_genPartIdx,       (leptonType+"_genPartIdx[n"+leptonType+"]/I").c_str());
    }
}

NanoReader::LightLeptonReader::LightLeptonReader(NanoReader& nano, TTree* _currentTreePtr, std::string leptonType) :
    LeptonReader(nano, _currentTreePtr, leptonType)
{
    _currentTreePtr->SetBranchAddress((leptonType+"_sip3d").c_str(),              _Lepton_sip3d,            &b__Lepton_sip3d);
    _currentTreePtr->SetBranchAddress((leptonType+"_pfRelIso03_all").c_str(),     _Lepton_pfRelIso03_all,   &b__Lepton_pfRelIso03_all);
    _currentTreePtr->SetBranchAddress((leptonType+"_miniPFRelIso_all").c_str(),   _Lepton_miniPFRelIso_all, &b__Lepton_miniPFRelIso_all);
    _currentTreePtr->SetBranchAddress((leptonType+"_miniPFRelIso_chg").c_str(),   _Lepton_miniPFRelIso_chg, &b__Lepton_miniPFRelIso_chg);
    _currentTreePtr->SetBranchAddress((leptonType+"_jetPtRelv2").c_str(),         _Lepton_jetPtRelv2,       &b__Lepton_jetPtRelv2);
    _currentTreePtr->SetBranchAddress((leptonType+"_jetRelIso").c_str(),          _Lepton_jetRelIso,        &b__Lepton_jetRelIso);
    _currentTreePtr->SetBranchAddress((leptonType+"_isPFcand").c_str(),           _Lepton_isPFCand,         &b__Lepton_isPFCand);
    _currentTreePtr->SetBranchAddress((leptonType+"_jetNDauCharged").c_str(),     _Lepton_jetNDauCharged,   &b__Lepton_jetNDauCharged);
    _currentTreePtr->SetBranchAddress((leptonType+"_mvaTOP").c_str(),             _Lepton_TOPLeptonMVAUL,   &b__Lepton_TOPLeptonMVAUL);
    _currentTreePtr->SetBranchAddress((leptonType+"_jetPtRatio").c_str(),         _Lepton_jetPtRatio,       &b__Lepton_jetPtRatio);
    _currentTreePtr->SetBranchAddress((leptonType+"_jetBTagDeepFlavor").c_str(),  _Lepton_jetBTagDJ,        &b__Lepton_jetBTagDJ);

    if (GetNanoReader().containsGenParticles()) {
        _currentTreePtr->SetBranchAddress((leptonType+"_isPrompt").c_str(),             _Lepton_isPrompt,             &b__Lepton_isPrompt);
        _currentTreePtr->SetBranchAddress((leptonType+"_isChargeFlip").c_str(),         _Lepton_isChargeFlip,         &b__Lepton_isChargeFlip);
        _currentTreePtr->SetBranchAddress((leptonType+"_matchPdgId").c_str(),           _Lepton_matchPdgId,           &b__Lepton_matchPdgId);
        _currentTreePtr->SetBranchAddress((leptonType+"_motherPdgId").c_str(),             _Lepton_motherPdgId,          &b__Lepton_momPdgId);
        _currentTreePtr->SetBranchAddress((leptonType+"_provenanceConversion").c_str(), _Lepton_provenanceConversion, &b__Lepton_provenanceConversion);
    }
}

void NanoReader::LightLeptonReader::setOutputTree(TTree* tree, std::string leptonType) {
    LeptonReader::setOutputTree(tree, leptonType);
    tree->Branch((leptonType+"_sip3d").c_str(),              _Lepton_sip3d,            (leptonType+"_sip3d[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_pfRelIso03_all").c_str(),     _Lepton_pfRelIso03_all,   (leptonType+"_pfRelIso03_all[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_miniPFRelIso_all").c_str(),   _Lepton_miniPFRelIso_all, (leptonType+"_miniPFRelIso_all[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_miniPFRelIso_chg").c_str(),   _Lepton_miniPFRelIso_chg, (leptonType+"_miniPFRelIso_chg[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_jetPtRelv2").c_str(),         _Lepton_jetPtRelv2,       (leptonType+"_jetPtRelv2[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_jetRelIso").c_str(),          _Lepton_jetRelIso,        (leptonType+"_jetRelIso[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_isPFcand").c_str(),           _Lepton_isPFCand,         (leptonType+"_isPFcand[n"+leptonType+"]/O").c_str());
    tree->Branch((leptonType+"_jetNDauCharged").c_str(),     _Lepton_jetNDauCharged,   (leptonType+"_jetNDauCharged[n"+leptonType+"]/I").c_str());
    tree->Branch((leptonType+"_mvaTOP").c_str(),             _Lepton_TOPLeptonMVAUL,   (leptonType+"_mvaTOP[n"+leptonType+"]/F").c_str());
    tree->Branch((leptonType+"_jetPtRatio").c_str(),         _Lepton_jetPtRatio,       (leptonType+"_jetPtRatio[n"+leptonType+"]/F").c_str());
}
