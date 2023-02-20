#include "../interface/Met.h"


Met::Met( const TreeReader& treeReader,
	    const bool readAllJECVariations, const bool readGroupedJECVariations ):
    PhysicsObject( treeReader._met, 0., treeReader._metPhi, treeReader._met,
		    treeReader.is2016(), treeReader.is2016PreVFP(), treeReader.is2016PostVFP(),
		    treeReader.is2017(), treeReader.is2018() ),
    _pt_JECDown( treeReader._met_JECDown ), 
    _phi_JECDown( treeReader._metPhi_JECDown ),
    _pt_JECUp( treeReader._met_JECUp ),
    _phi_JECUp( treeReader._metPhi_JECUp ),
    _JECSources(treeReader._sourcesJEC_Ids ),
    _JECGrouped(treeReader._groupedJEC_Ids ),
    _pt_UnclDown( treeReader._met_UnclDown ),
    _phi_UnclDown( treeReader._metPhi_UnclDown ),
    _pt_UnclUp( treeReader._met_UnclUp ),
    _phi_UnclUp( treeReader._metPhi_UnclUp ) {
    
    if (readAllJECVariations) {
        _pxy_JECSourcesUp = std::vector<std::pair<double,double> >(_JECSources->size());
        _pxy_JECSourcesDown = std::vector<std::pair<double,double> >(_JECSources->size());
        for (const auto& elMap : *_JECSources) {
            _pxy_JECSourcesUp[elMap.second] = std::make_pair(
                                               treeReader._corrMETx_JECSourcesUp[elMap.second],
                                               treeReader._corrMETy_JECSourcesUp[elMap.second]);

            _pxy_JECSourcesDown[elMap.second] = std::make_pair(
                                    treeReader._corrMETx_JECSourcesDown[elMap.second],
                                    treeReader._corrMETy_JECSourcesDown[elMap.second]);
        }
    }
    if (readGroupedJECVariations) {
        _pxy_JECGroupedUp = std::vector<std::pair<double,double> >(_JECGrouped->size());
        _pxy_JECGroupedDown = std::vector<std::pair<double,double> >(_JECGrouped->size());

        for (const auto& elMap : *_JECGrouped) {
            _pxy_JECGroupedUp[elMap.second] = std::make_pair(
                                               treeReader._corrMETx_JECGroupedUp[elMap.second],
                                               treeReader._corrMETy_JECGroupedUp[elMap.second]);
            _pxy_JECGroupedDown[elMap.second] = std::make_pair(
                                                 treeReader._corrMETx_JECGroupedDown[elMap.second],
                                                 treeReader._corrMETy_JECGroupedDown[elMap.second]);
        }
    }
}


Met Met::variedMet( const double pt, const double phi ) const{
    Met variedMet( *this );
    variedMet.setLorentzVector( pt, eta(), phi, pt );
    return variedMet;
}

Met Met::variedMetPxPy( const double px, const double py) const{
    LorentzVector varvector = lorentzVectorPxPyPzEnergy( px, py, 0, sqrt(px*px + py*py));
    return variedMet( varvector.pt(), varvector.phi() );
}

Met Met::MetJECDown() const{
    return variedMet( _pt_JECDown, _phi_JECDown );
}


Met Met::MetJECUp() const{
    return variedMet( _pt_JECUp, _phi_JECUp );
}


Met Met::MetUnclusteredDown() const{
    return variedMet( _pt_UnclDown, _phi_UnclDown );
}


Met Met::MetUnclusteredUp() const{
    return variedMet( _pt_UnclUp, _phi_UnclUp );
}

Met Met::MetJECDown( const std::string source ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    for(auto test: *_JECSources ){
        if(source==test.first) newpxy = this->_pxy_JECSourcesDown[test.second];
    }
    for( auto test: *_JECGrouped ){
        if(source==test.first) newpxy = this->_pxy_JECGroupedDown[test.second];
    }
    return variedMetPxPy( newpxy.first, newpxy.second );
}

Met Met::MetJECUp( const std::string source ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    for(auto test: *_JECSources ){
        if(source==test.first) newpxy = this->_pxy_JECSourcesUp[test.second];
    }
    for( auto test: *_JECGrouped ){
        if(source==test.first) newpxy = this->_pxy_JECGroupedUp[test.second];
    }
    return variedMetPxPy( newpxy.first, newpxy.second );
}


Met Met::MetJECGroupedDown( const unsigned source) const {
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    newpxy = _pxy_JECGroupedDown[source];
    return variedMetPxPy( newpxy.first, newpxy.second );
}
Met Met::MetJECGroupedUp( const unsigned source) const {
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    newpxy = _pxy_JECGroupedUp[source];
    return variedMetPxPy( newpxy.first, newpxy.second );
}
Met Met::MetJECSourcesDown( const unsigned source) const {
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    newpxy = _pxy_JECSourcesDown[source];
    return variedMetPxPy( newpxy.first, newpxy.second );
}
Met Met::MetJECSourcesUp( const unsigned source) const {
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    newpxy = _pxy_JECSourcesUp[source];
    return variedMetPxPy( newpxy.first, newpxy.second );
}

Met Met::MetVariation(JetCollection& nominalJets, Jet (Jet::*jetVariation)() const) const {
    LorentzVector ret = LorentzVector(pt(), eta(), phi(), energy());

    for (const auto& jetPtr : nominalJets) {
        double ptNom = jetPtr->pt();
        Jet varJet = ((*jetPtr).*jetVariation)();
        double ptdiff = ptNom - varJet.pt();
        ret += LorentzVector(ptdiff, 0., jetPtr->phi(), ptdiff);
    }

    Met variedMet( *this );
    variedMet.setLorentzVector(ret.pt(), eta(), ret.phi(), ret.pt() );
    return variedMet;
}


Met Met::HEMIssue(JetCollection& nominalJets) const {
    LorentzVector ret = LorentzVector(pt(), eta(), phi(), energy());
    for (const auto& jetPtr : nominalJets) {
        double ptNom = jetPtr->pt();
        Jet varJet = jetPtr->HEMIssue();
        double ptdiff = ptNom - varJet.pt();
        if (fabs(ptdiff) > 1e-1) {
            ret += LorentzVector(ptdiff, 0., jetPtr->phi(), ptdiff);
        }
    }
    Met variedMet( *this );
    variedMet.setLorentzVector(ret.pt(), eta(), ret.phi(), ret.pt() );
    return variedMet;
}


Met Met::getVariedMet( const std::string& variation ) const{
    if( variation == "nominal" ){
        return *this;
    } else if( variation == "JECDown" ){
        return this->MetJECDown();
    } else if( variation == "JECUp" ){
        return this->MetJECUp();
    } else if( variation == "JERDown" ){
        return *this;
    } else if( variation == "JERUp" ){
        return *this;
    } else if( variation == "UnclDown" ){
        return this->MetUnclusteredDown();
    } else if( variation == "UnclUp" ){
        return this->MetUnclusteredUp();
    } else if( stringTools::stringEndsWith(variation,"Up") ){
        std::string jecvar = variation.substr(0, variation.size()-2);
        return this->MetJECUp( jecvar );
    } else if( stringTools::stringEndsWith(variation,"Down") ){
        std::string jecvar = variation.substr(0, variation.size()-4);
        return this->MetJECDown( jecvar );
    } else {
        throw std::invalid_argument( std::string("### ERROR ### in Met::getVariedMet: ")
	+ "met variation " + variation + " is unknown." );
    }
}


std::vector< double > Met::metPtVariations() const{
    return {
        pt(),
        MetJECDown().pt(),
        MetJECUp().pt(),
        MetUnclusteredDown().pt(),
        MetUnclusteredUp().pt()
    };
}


double Met::maxPtAnyVariation() const{
    auto variations = metPtVariations();
    return *std::max_element( variations.cbegin(), variations.cend() );
}


double Met::minPtAnyVariation() const{
    auto variations = metPtVariations();
    return *std::min_element( variations.cbegin(), variations.cend() );
}


std::ostream& Met::print( std::ostream& os ) const{
    os << "Met : ";
    os << "(pt = " << pt() << ", phi = " << phi() << ")";
    os << " / pt_JECDown = " << _pt_JECDown << " / phi_JECDown = " << _phi_JECDown << " / pt_JECUp = " << _pt_JECUp << " / phi_JECUp = " << _phi_JECUp << " / pt_UnclDown = " << _pt_UnclDown << " / phi_UnclDown = " << _phi_UnclDown << " / pt_UnclUp = " << _pt_UnclUp << " / phi_UnclUp = " << _phi_UnclUp;
    return os;
}

Met Met::getVariedMet(JetCollection& nomJets, unsigned variationSource, unsigned flavor, bool up) const {
    // rough approach
    // generate lorentzvector corresponding to met
    LorentzVector ret = LorentzVector(pt(), eta(), phi(), energy());

    for (const auto& jetPtr : nomJets) {
        double ptNom = jetPtr->pt();
        if (jetPtr->hadronFlavor() != flavor) continue;
        
        std::shared_ptr<Jet> varJet;
        if (up) varJet = std::make_shared< Jet >(jetPtr->JetJECGroupedUp( variationSource ));
        else varJet = std::make_shared< Jet >(jetPtr->JetJECGroupedDown( variationSource ));

        double ptdiff = ptNom - varJet->pt();

        ret += LorentzVector(ptdiff, 0., jetPtr->phi(), ptdiff);
    }

    Met variedMet( *this );
    variedMet.setLorentzVector(ret.pt(), eta(), ret.phi(), ret.pt() );
    return variedMet;
}
