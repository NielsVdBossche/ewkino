#include "../interface/JECWrapper.h"

#include "../../Event/interface/Event.h"
JECWrapper::JECWrapper(std::string& uncertaintyFile, std::vector<std::string>& jecSources) {
    unsigned counter = 0;
    for( const auto& source : jecSources ){
        uncertaintyMapping[source] = counter;
        jetCorrUncertainty[ counter ] = std::make_shared<JetCorrectorParameters>(uncertaintyFile, source );
        counter++;
    }
}


std::pair<JetCollection, Met> JECWrapper::VaryJetsAndMet(Event& event, unsigned id, bool isUp) {
    JetCollection nominalJets = event.jetCollection();

        // similar to above but with argument passed to Jet::*variedJet
    std::vector< std::shared_ptr< Jet > > jetVector;
    std::shared_ptr< JetCorrectorParameters> jetCorrUncParams = GetJetCorrectionUncertainty(id);

    Met met = event.met();
    LorentzVector retMet = LorentzVector(met.pt(), met.eta(), met.phi(), met.energy());

    for( const auto& jetPtr : nominalJets ){
        JetCorrectionUncertainty* jetCorrUnc = new JetCorrectionUncertainty( *( jetCorrUncParams ) );
        double ptNom = jetPtr->pt();
        
        std::shared_ptr<Jet> variedJet;
        if (isUp) variedJet = std::make_shared< Jet >( jetPtr->JetJECUp(jetCorrUnc) );
        else variedJet = std::make_shared< Jet >( jetPtr->JetJECDown(jetCorrUnc) );

        jetVector.push_back(variedJet);
        double ptdiff = ptNom - variedJet->pt();

        retMet += LorentzVector(ptdiff, 0., jetPtr->phi(), ptdiff);
        delete jetCorrUnc;
    }

    Met variedMet( met );
    variedMet.setLorentzVector(retMet.pt(), met.eta(), retMet.phi(), retMet.pt() );


    return {JetCollection( jetVector ), variedMet};
}
