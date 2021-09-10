#include "../interface/objectSelection4T.h"

bool ObjectSelection4T::isJetIsolatedFromEl(LorentzVector* currentJet) {
    //std::vector<int>::iterator it;
//
    //for (it = sel_electrons->begin(); it != sel_electrons->end(); it++) {
    //    LorentzVector current_el(_el_pt->at(*it), _el_eta->at(*it), _el_phi->at(*it), _el_E->at(*it));
    //    if (deltaR(*currentJet, current_el) < 0.4) return false;
    //}
    return true;
}


bool ObjectSelection4T::isJetIsolatedFromMu(LorentzVector* currentJet) {
    //std::vector<int>::iterator it;
    //for (it = sel_muons->begin(); it != sel_muons->end(); it++) {
    //    LorentzVector current_mu(_mu_pt->at(*it), _mu_eta->at(*it), _mu_phi->at(*it), _mu_E->at(*it));
    //    if (deltaR(*currentJet, current_mu) < 0.4) return false;
    //}
//
    return true;    
}

bool ObjectSelection4T::jetID(const Jet& jet) {
    if (jet.pt() < 25) return false;
    if (jet.absEta() > 2.4) return false;
    //
    //LorentzVector currentJet(_jet_pt->at(i), _jet_eta->at(i), _jet_phi->at(i), _jet_E->at(i));
    //if (! isJetIsolatedFromEl(&currentJet)) return false;
    //if (! isJetIsolatedFromMu(&currentJet)) return false;
    
    return true;
}
