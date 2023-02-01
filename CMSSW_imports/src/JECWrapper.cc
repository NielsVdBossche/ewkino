#include "../interface/JECWrapper.h"

JECWrapper::JECWrapper(std::string& uncertaintyFile, std::vector<std::string>& jecSources) {
    unsigned counter = 0;
    for( const auto& source : jecSources ){
        uncertaintyMapping[source] = counter;
        JetCorrectorParameters* jetCorrParameters = new JetCorrectorParameters(uncertaintyFile, source );

        jetCorrUncertainty[ counter ] = std::make_shared< JetCorrectionUncertainty >(uncertaintyFile, source );

        counter++;
    }
}


// main function called to fill uncertainties
// need a jetcorrectorParameters object clearly:
//   jetSourcesCorParameters[ source ] = std::make_shared< JetCorrectorParameters >( (iConfig.getParameter<edm::FileInPath>("jecUncertaintySourcesFile") ).fullPath(), source );
// per source:
//     std::vector< std::string > jecSources = {"AbsoluteStat", "AbsoluteScale", "AbsoluteMPFBias", "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "FlavorQCD", "FlavorZJet", "FlavorPhotonJet", "FlavorPureGluon", "FlavorPureQuark", "FlavorPureCharm", "FlavorPureBottom", "TimePtEta", "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeBal", "RelativeSample", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC", "RelativeStatHF", "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", "PileUpPtEC2", "PileUpPtHF", "PileUpMuZero", "PileUpEnvelope", "SubTotalPileUp", "SubTotalRelative", "SubTotalPt", "SubTotalScale", "SubTotalAbsolute", "SubTotalMC", "TotalNoFlavor", "TotalNoTime" ,"TotalNoFlavorNoTime", "Total" };

JetCollection JECWrapper::VariedJetCollection(unsigned variation, bool up, JetCollection& nominalJetCollection) {
    
}



template< typename T > void fillJetUncertaintySources( const std::map< std::string, std::shared_ptr< JetCorrectorParameters> >& jetCorrParameters,  T& sourcesMapDown, T& sourcesMapUp, const pat::Jet& jet, const unsigned jetIndex ){
    for( const auto& source : jetCorrParameters ){
        JetCorrectionUncertainty* jetCorUnc = new JetCorrectionUncertainty( *( source.second ) );
        jetCorUnc->setJetPt( jet.pt() );
        jetCorUnc->setJetEta( jet.eta() );

        double uncJec = jetCorUnc->getUncertainty( true );

        sourcesMapDown[ source.first ][ jetIndex ] = jet.pt()*( 1 - uncJec );
        sourcesMapUp[ source.first ][ jetIndex ] = jet.pt()*( 1 + uncJec );

        delete jetCorUnc;
    }
}
