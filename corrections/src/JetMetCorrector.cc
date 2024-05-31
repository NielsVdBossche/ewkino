#include "../interface/JetMetCorrector.h"

JetMetCorrector::JetMetCorrector(std::unique_ptr<correction::CorrectionSet> jercSet) {

}

std::pair<JetCollection, Met> JetMetCorrector::ApplyJECUncertainty(bool isUp, JetCollection& nominalJets, Met& nominalMET) {

    // loop jets
    // for jet in jets:
    //      JetPtUp = jet.pt()*( 1 + uncJec )
    // fix met

    // For met: 
    // set vector to jet uncorrected pt, eta, phi, E
    // clea from muons:
    //     subtract muons in daughter candidates ---> maybe Jet_muonSubtrFactor?
    // lastly, correction on MET for given jet (double corrPt, double rawPt, double rawEta, double rawMuonSubtractedPt, double phi, double emf, double rho, double area, const std::string& source, unsigned jetIndex, double jecShift):
    //    std::vector< float > corrections = getSubCorrections(rawPt, rawEta, rho, area);
    //      jecShift == for given variation the jetPt
    //      rho = rho? rhotoken? not immediately used except in subcorrections 
    //      area = jetArea, stored in nano
    //      rawMuonSubtractedPt = jetV.Pt() with jetV being the jetvector with uncorrected pT - muon component
    //      double l1corrptNoMuon = rawMuonSubtractedPt*corrections.front();
    //      double fullcorrpt     = rawMuonSubtractedPt*corrections.back();
    //      double PT_muon        = jet.correctedP4("Uncorrected").Pt() - rawMuonSubtractedPt;
    //      double PT_L1L2L3      = fullcorrpt + PT_muon;
    //      double PT_L1          = l1corrptNoMuon + PT_muon;
    //      
    //      float f = PT_L1L2L3*(jecShift/jet.pt());
    //      
    //      if( jet.neutralEmEnergyFraction() + jet.chargedEmEnergyFraction() > 0.90 or fullcorrpt < 15. || ( std::abs(jet.correctedP4("Uncorrected").Eta()) > 9.9 ) ) return {0., 0.};
    //      
    //      float ptdiff = (PT_L1 - f);
    //      
    //      std::pair<double, double> corr = {px(ptdiff, uncorr_phi-muon), py(ptdiff, uncorr_phi-muon)};
    //      
    //      return corr;
}