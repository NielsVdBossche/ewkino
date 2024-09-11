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
    _pt_UnclDown( treeReader._met_UnclDown ),
    _phi_UnclDown( treeReader._metPhi_UnclDown ),
    _pt_UnclUp( treeReader._met_UnclUp ),
    _phi_UnclUp( treeReader._metPhi_UnclUp ),
    _metVariationsInitialized(true) 
{
    isNano = false;
    if (readAllJECVariations) {
        for( const auto& mapEl: treeReader._corrMETx_JECSourcesUp ){
                std::string key = mapEl.first;
                key = stringTools::removeOccurencesOf(key,"_corrMETx_");
                key = stringTools::removeOccurencesOf(key,"_JECSourcesUp");
                _JECSources.push_back( key ); // assume they are the same for Up/Down and x/y!
            }
        for( const std::string& key: _JECSources ){
            _pxy_JECSourcesUp.insert( {key, std::make_pair( 
            treeReader._corrMETx_JECSourcesUp.at("_corrMETx_"+key+"_JECSourcesUp"), 
            treeReader._corrMETy_JECSourcesUp.at("_corrMETy_"+key+"_JECSourcesUp"))} );
            _pxy_JECSourcesDown.insert( {key, std::make_pair( 
                    treeReader._corrMETx_JECSourcesDown.at("_corrMETx_"+key+"_JECSourcesDown"),   
                    treeReader._corrMETy_JECSourcesDown.at("_corrMETy_"+key+"_JECSourcesDown"))} );
        }
    }
    if (readGroupedJECVariations) {
        for( const auto& mapEl: treeReader._corrMETx_JECGroupedUp ){
            std::string key = mapEl.first;
            key = stringTools::removeOccurencesOf(key,"_corrMETx_");
            key = stringTools::removeOccurencesOf(key,"_JECGroupedUp");
            _JECGrouped.push_back( key ); // assume they are the same for Up/Down and x/y!
        }
        for( const std::string& key: _JECGrouped ){
            _pxy_JECGroupedUp.insert( {key, std::make_pair( 
                treeReader._corrMETx_JECGroupedUp.at("_corrMETx_"+key+"_JECGroupedUp"),  
                treeReader._corrMETy_JECGroupedUp.at("_corrMETy_"+key+"_JECGroupedUp"))} );
            _pxy_JECGroupedDown.insert( {key, std::make_pair(  
                treeReader._corrMETx_JECGroupedDown.at("_corrMETx_"+key+"_JECGroupedDown"),   
                treeReader._corrMETy_JECGroupedDown.at("_corrMETy_"+key+"_JECGroupedDown"))} );
        }
    }
}

Met::Met(const NanoReader& nanoReader, const bool readAllJECVariations, const bool readGroupedJECVariations) :
    PhysicsObject( nanoReader._MET_T1Smear_pt, 0., nanoReader._MET_T1Smear_phi, nanoReader._MET_T1Smear_pt,
		    nanoReader.is2016(), nanoReader.is2016PreVFP(), nanoReader.is2016PostVFP(),
		    nanoReader.is2017(), nanoReader.is2018() ),
    _pt_JECDown(nanoReader._MET_T1Smear_pt_jesTotalDown),
    _phi_JECDown(nanoReader._MET_T1Smear_phi_jesTotalDown),
    _pt_JECUp(nanoReader._MET_T1Smear_pt_jesTotalUp),
    _phi_JECUp(nanoReader._MET_T1Smear_phi_jesTotalUp),
    _pt_UnclDown(nanoReader._MET_T1Smear_pt_unclustEnDown),
    _phi_UnclDown(nanoReader._MET_T1Smear_phi_unclustEnDown),
    _pt_UnclUp(nanoReader._MET_T1Smear_pt_unclustEnUp),
    _phi_UnclUp(nanoReader._MET_T1Smear_phi_unclustEnUp),
    _pt_JER1p93Down(nanoReader._MET_T1Smear_pt_jer0Down),
    _phi_JER1p93Down(nanoReader._MET_T1Smear_phi_jer0Down),
    _pt_JER1p93Up(nanoReader._MET_T1Smear_pt_jer0Up),
    _phi_JER1p93Up(nanoReader._MET_T1Smear_phi_jer0Up),
    _pt_JER2p5Down(nanoReader._MET_T1Smear_pt_jer1Down),
    _phi_JER2p5Down(nanoReader._MET_T1Smear_phi_jer1Down),
    _pt_JER2p5Up(nanoReader._MET_T1Smear_pt_jer1Up),
    _phi_JER2p5Up(nanoReader._MET_T1Smear_phi_jer1Up),
    _metVariationsInitialized(true),
    _metJERVariationsInitialized(true)
{
    isNano = true;
    // Smeared MET should be read. 
    // TODO: not yet supported:
    if (readAllJECVariations) {

    }
    if (readGroupedJECVariations) {
        for( const auto& mapEl: nanoReader._MET_T1Smear_pt_jesSourcesUp ){
            std::string key = mapEl.first;
            _JECGrouped.push_back( key );
            _pt_phi_JECGroupedUp.insert({key, std::make_pair(
                nanoReader._MET_T1Smear_pt_jesSourcesUp.at(key),
                nanoReader._MET_T1Smear_phi_jesSourcesUp.at(key)
            )});
            _pt_phi_JECGroupedDown.insert({key, std::make_pair(
                nanoReader._MET_T1Smear_pt_jesSourcesDown.at(key),
                nanoReader._MET_T1Smear_phi_jesSourcesDown.at(key)
            )});
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

Met Met::MetJER1p93Down() const {
    if (! IsMetJERVariationsInitialized()) {
        std::cerr << "Warning: MET JER Variations are not initialized. Returning nominal MET." << std::endl;
        return *this;
    }
    return variedMet(_pt_JER1p93Down, _phi_JER1p93Down);
}

Met Met::MetJER1p93Up() const {
    if (! IsMetJERVariationsInitialized()) {
        std::cerr << "Warning: MET JER Variations are not initialized. Returning nominal MET." << std::endl;
        return *this;
    }
    return variedMet(_pt_JER1p93Up, _phi_JER1p93Up);
}

Met Met::MetJER2p5Down() const {
    if (! IsMetJERVariationsInitialized()) {
        std::cerr << "Warning: MET JER Variations are not initialized. Returning nominal MET." << std::endl;
        return *this;
    }
    return variedMet(_pt_JER2p5Down, _phi_JER2p5Down);
}

Met Met::MetJER2p5Up() const {
    if (! IsMetJERVariationsInitialized()) {
        std::cerr << "Warning: MET JER Variations are not initialized. Returning nominal MET." << std::endl;
        return *this;
    }
    return variedMet(_pt_JER2p5Up, _phi_JER2p5Up);
}


Met Met::MetJECDown( const std::string& variation ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    if (isNano) {
        std::pair< double, double > new_pt_phi = std::make_pair( 0,0 );
        try {
            new_pt_phi = _pt_phi_JECGroupedDown.at(variation);
        } catch(const std::exception& e) {
            std::cerr << "Met::MetJECDown NANO Variation " << variation << " not found in JEC sources or grouped variations." << '\n';
            std::cerr << e.what() << '\n';
        }
        return variedMet( new_pt_phi.first, new_pt_phi.second );
    } else {
        std::pair< double, double > newpxy = std::make_pair( 0,0 );
        try {
            newpxy = _pxy_JECGroupedDown.at(variation);
        } catch(const std::exception& e) {
            try {
                newpxy = _pxy_JECSourcesDown.at(variation);
            } catch(const std::exception& e) {
                std::cerr << "Met::MetJECDown Variation " << variation << " not found in JEC sources or grouped variations." << '\n';
                std::cerr << e.what() << '\n';
            }
        }
        return variedMetPxPy( newpxy.first, newpxy.second );
    }
}

Met Met::MetJECUp( const std::string& variation ) const{
    // note: this function checks both all and grouped variations,
    // need to check if there is no overlap in names between them!
    if (isNano) {
        std::pair< double, double > new_pt_phi = std::make_pair( 0,0 );
        try {
            new_pt_phi = _pt_phi_JECGroupedUp.at(variation);
        } catch(const std::exception& e) {
            std::cerr << "Met::MetJECUp NANO Variation " << variation << " not found in JEC sources or grouped variations." << '\n';
            std::cerr << e.what() << '\n';
        }
        return variedMet( new_pt_phi.first, new_pt_phi.second );
    } else {
        std::pair< double, double > newpxy = std::make_pair( 0,0 );
        try {
            newpxy = _pxy_JECGroupedDown.at(variation);
        } catch(const std::exception& e) {
            try {
                newpxy = _pxy_JECSourcesDown.at(variation);
            } catch(const std::exception& e) {
                std::cerr << "Met::MetJECUp Variation " << variation << " not found in JEC sources or grouped variations." << '\n';
                std::cerr << e.what() << '\n';
            }
        }
        return variedMetPxPy( newpxy.first, newpxy.second );
    }
}


Met Met::MetJECGroupedDown(const std::string& variation) const {
    if (isNano) {
        std::pair< double, double > new_pt_phi = std::make_pair( 0,0 );
        new_pt_phi = _pt_phi_JECGroupedDown.at(variation);
        return variedMet( new_pt_phi.first, new_pt_phi.second );
    } else {
        std::pair< double, double > newpxy = std::make_pair( 0,0 );
        newpxy = _pxy_JECGroupedDown.at(variation);
        return variedMetPxPy( newpxy.first, newpxy.second );
    }
}

Met Met::MetJECGroupedUp(const std::string& variation) const {
    if (isNano) {
        std::pair< double, double > new_pt_phi = std::make_pair( 0,0 );
        new_pt_phi = _pt_phi_JECGroupedUp.at(variation);
        return variedMet( new_pt_phi.first, new_pt_phi.second );
    } else {
        std::pair< double, double > newpxy = std::make_pair( 0,0 );
        newpxy = _pxy_JECGroupedUp.at(variation);
        return variedMetPxPy( newpxy.first, newpxy.second );
    }
}

Met Met::MetJECSourcesDown(const std::string& variation) const {
    if (isNano) {
        std::cerr << "JEC SOURCES NOT YET IMPLEMENTED" << std::endl;
        exit(1);
    }
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    newpxy = _pxy_JECSourcesDown.at(variation);
    return variedMetPxPy( newpxy.first, newpxy.second );
}

Met Met::MetJECSourcesUp(const std::string& variation) const {
    if (isNano) {
        std::cerr << "JEC SOURCES NOT YET IMPLEMENTED" << std::endl;
        exit(1);
    }
    std::pair< double, double > newpxy = std::make_pair( 0,0 );
    newpxy = _pxy_JECSourcesUp.at(variation);
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

Met Met::getVariedMet(JetCollection& nomJets, const std::string& variation, unsigned flavor, bool up) const {
    // rough approach
    // generate lorentzvector corresponding to met
    LorentzVector ret = LorentzVector(pt(), eta(), phi(), energy());

    for (const auto& jetPtr : nomJets) {
        double ptNom = jetPtr->pt();
        if (jetPtr->hadronFlavor() != flavor) continue;
        
        std::shared_ptr<Jet> varJet;
        if (up) varJet = std::make_shared< Jet >(jetPtr->JetJECUp( variation ));
        else varJet = std::make_shared< Jet >(jetPtr->JetJECDown( variation ));

        double ptdiff = ptNom - varJet->pt();

        ret += LorentzVector(ptdiff, 0., jetPtr->phi(), ptdiff);
    }

    Met variedMet( *this );
    variedMet.setLorentzVector(ret.pt(), eta(), ret.phi(), ret.pt() );
    return variedMet;
}

Met Met::ApplyPhiModulation(int runnb, std::string year, bool isMC, int npv, bool isUL) {
    bool ispuppi = false;
    std::pair<double, double> TheXYCorr_Met_MetPhi(pt(), phi());

    if (npv > 100) npv = 100;
    int runera = -1;
    bool usemetv2 = false;
    if (isMC && year == "2016" && !isUL)
        runera = y2016MC;
    else if (isMC && year == "2017" && !isUL) {
        runera = y2017MC;
        usemetv2 = true;
    } else if (isMC && year == "2018" && !isUL)
        runera = y2018MC;
    else if (isMC && year == "2016APV" && isUL)
        runera = yUL2016MCAPV;
    else if (isMC && year == "2016nonAPV" && isUL)
        runera = yUL2016MCnonAPV;
    else if (isMC && year == "2017" && isUL)
        runera = yUL2017MC;
    else if (isMC && year == "2018" && isUL)
        runera = yUL2018MC;

    else if (!isMC && runnb >= 272007 && runnb <= 275376 && !isUL)
        runera = y2016B;
    else if (!isMC && runnb >= 275657 && runnb <= 276283 && !isUL)
        runera = y2016C;
    else if (!isMC && runnb >= 276315 && runnb <= 276811 && !isUL)
        runera = y2016D;
    else if (!isMC && runnb >= 276831 && runnb <= 277420 && !isUL)
        runera = y2016E;
    else if (!isMC && runnb >= 277772 && runnb <= 278808 && !isUL)
        runera = y2016F;
    else if (!isMC && runnb >= 278820 && runnb <= 280385 && !isUL)
        runera = y2016G;
    else if (!isMC && runnb >= 280919 && runnb <= 284044 && !isUL)
        runera = y2016H;

    else if (!isMC && runnb >= 297020 && runnb <= 299329 && !isUL) {
        runera = y2017B;
        usemetv2 = true;
    } else if (!isMC && runnb >= 299337 && runnb <= 302029 && !isUL) {
        runera = y2017C;
        usemetv2 = true;
    } else if (!isMC && runnb >= 302030 && runnb <= 303434 && !isUL) {
        runera = y2017D;
        usemetv2 = true;
    } else if (!isMC && runnb >= 303435 && runnb <= 304826 && !isUL) {
        runera = y2017E;
        usemetv2 = true;
    } else if (!isMC && runnb >= 304911 && runnb <= 306462 && !isUL) {
        runera = y2017F;
        usemetv2 = true;
    }

    else if (!isMC && runnb >= 315252 && runnb <= 316995 && !isUL)
        runera = y2018A;
    else if (!isMC && runnb >= 316998 && runnb <= 319312 && !isUL)
        runera = y2018B;
    else if (!isMC && runnb >= 319313 && runnb <= 320393 && !isUL)
        runera = y2018C;
    else if (!isMC && runnb >= 320394 && runnb <= 325273 && !isUL)
        runera = y2018D;

    else if (!isMC && runnb >= 315252 && runnb <= 316995 && isUL)
        runera = yUL2018A;
    else if (!isMC && runnb >= 316998 && runnb <= 319312 && isUL)
        runera = yUL2018B;
    else if (!isMC && runnb >= 319313 && runnb <= 320393 && isUL)
        runera = yUL2018C;
    else if (!isMC && runnb >= 320394 && runnb <= 325273 && isUL)
        runera = yUL2018D;

    else if (!isMC && runnb >= 297020 && runnb <= 299329 && isUL) {
        runera = yUL2017B;
        usemetv2 = false;
    } else if (!isMC && runnb >= 299337 && runnb <= 302029 && isUL) {
        runera = yUL2017C;
        usemetv2 = false;
    } else if (!isMC && runnb >= 302030 && runnb <= 303434 && isUL) {
        runera = yUL2017D;
        usemetv2 = false;
    } else if (!isMC && runnb >= 303435 && runnb <= 304826 && isUL) {
        runera = yUL2017E;
        usemetv2 = false;
    } else if (!isMC && runnb >= 304911 && runnb <= 306462 && isUL) {
        runera = yUL2017F;
        usemetv2 = false;
    }

    else if (!isMC && runnb >= 272007 && runnb <= 275376 && isUL)
        runera = yUL2016B;
    else if (!isMC && runnb >= 275657 && runnb <= 276283 && isUL)
        runera = yUL2016C;
    else if (!isMC && runnb >= 276315 && runnb <= 276811 && isUL)
        runera = yUL2016D;
    else if (!isMC && runnb >= 276831 && runnb <= 277420 && isUL)
        runera = yUL2016E;
    else if (!isMC && ((runnb >= 277772 && runnb <= 278768) || runnb == 278770) && isUL)
        runera = yUL2016F;
    else if (!isMC && ((runnb >= 278801 && runnb <= 278808) || runnb == 278769) && isUL)
        runera = yUL2016Flate;
    else if (!isMC && runnb >= 278820 && runnb <= 280385 && isUL)
        runera = yUL2016G;
    else if (!isMC && runnb >= 280919 && runnb <= 284044 && isUL)
        runera = yUL2016H;

    else {
        // Couldn't find data/MC era => no correction applied
        return variedMet(pt(), phi());
    }

    double METxcorr(0.), METycorr(0.);

    if (!usemetv2) {  // Current recommendation for 2016 and 2018
        if (!ispuppi) {
            if (runera == y2016B) METxcorr = -(-0.0478335 * npv - 0.108032);
            if (runera == y2016B) METycorr = -(0.125148 * npv + 0.355672);
            if (runera == y2016C) METxcorr = -(-0.0916985 * npv + 0.393247);
            if (runera == y2016C) METycorr = -(0.151445 * npv + 0.114491);
            if (runera == y2016D) METxcorr = -(-0.0581169 * npv + 0.567316);
            if (runera == y2016D) METycorr = -(0.147549 * npv + 0.403088);
            if (runera == y2016E) METxcorr = -(-0.065622 * npv + 0.536856);
            if (runera == y2016E) METycorr = -(0.188532 * npv + 0.495346);
            if (runera == y2016F) METxcorr = -(-0.0313322 * npv + 0.39866);
            if (runera == y2016F) METycorr = -(0.16081 * npv + 0.960177);
            if (runera == y2016G) METxcorr = -(0.040803 * npv - 0.290384);
            if (runera == y2016G) METycorr = -(0.0961935 * npv + 0.666096);
            if (runera == y2016H) METxcorr = -(0.0330868 * npv - 0.209534);
            if (runera == y2016H) METycorr = -(0.141513 * npv + 0.816732);
            if (runera == y2017B) METxcorr = -(-0.259456 * npv + 1.95372);
            if (runera == y2017B) METycorr = -(0.353928 * npv - 2.46685);
            if (runera == y2017C) METxcorr = -(-0.232763 * npv + 1.08318);
            if (runera == y2017C) METycorr = -(0.257719 * npv - 1.1745);
            if (runera == y2017D) METxcorr = -(-0.238067 * npv + 1.80541);
            if (runera == y2017D) METycorr = -(0.235989 * npv - 1.44354);
            if (runera == y2017E) METxcorr = -(-0.212352 * npv + 1.851);
            if (runera == y2017E) METycorr = -(0.157759 * npv - 0.478139);
            if (runera == y2017F) METxcorr = -(-0.232733 * npv + 2.24134);
            if (runera == y2017F) METycorr = -(0.213341 * npv + 0.684588);
            if (runera == y2018A) METxcorr = -(0.362865 * npv - 1.94505);
            if (runera == y2018A) METycorr = -(0.0709085 * npv - 0.307365);
            if (runera == y2018B) METxcorr = -(0.492083 * npv - 2.93552);
            if (runera == y2018B) METycorr = -(0.17874 * npv - 0.786844);
            if (runera == y2018C) METxcorr = -(0.521349 * npv - 1.44544);
            if (runera == y2018C) METycorr = -(0.118956 * npv - 1.96434);
            if (runera == y2018D) METxcorr = -(0.531151 * npv - 1.37568);
            if (runera == y2018D) METycorr = -(0.0884639 * npv - 1.57089);
            if (runera == y2016MC) METxcorr = -(-0.195191 * npv - 0.170948);
            if (runera == y2016MC) METycorr = -(-0.0311891 * npv + 0.787627);
            if (runera == y2017MC) METxcorr = -(-0.217714 * npv + 0.493361);
            if (runera == y2017MC) METycorr = -(0.177058 * npv - 0.336648);
            if (runera == y2018MC) METxcorr = -(0.296713 * npv - 0.141506);
            if (runera == y2018MC) METycorr = -(0.115685 * npv + 0.0128193);

            // UL2017
            if (runera == yUL2017B) METxcorr = -(-0.211161 * npv + 0.419333);
            if (runera == yUL2017B) METycorr = -(0.251789 * npv + -1.28089);
            if (runera == yUL2017C) METxcorr = -(-0.185184 * npv + -0.164009);
            if (runera == yUL2017C) METycorr = -(0.200941 * npv + -0.56853);
            if (runera == yUL2017D) METxcorr = -(-0.201606 * npv + 0.426502);
            if (runera == yUL2017D) METycorr = -(0.188208 * npv + -0.58313);
            if (runera == yUL2017E) METxcorr = -(-0.162472 * npv + 0.176329);
            if (runera == yUL2017E) METycorr = -(0.138076 * npv + -0.250239);
            if (runera == yUL2017F) METxcorr = -(-0.210639 * npv + 0.72934);
            if (runera == yUL2017F) METycorr = -(0.198626 * npv + 1.028);
            if (runera == yUL2017MC) METxcorr = -(-0.300155 * npv + 1.90608);
            if (runera == yUL2017MC) METycorr = -(0.300213 * npv + -2.02232);

            // UL2018
            if (runera == yUL2018A) METxcorr = -(0.263733 * npv + -1.91115);
            if (runera == yUL2018A) METycorr = -(0.0431304 * npv + -0.112043);
            if (runera == yUL2018B) METxcorr = -(0.400466 * npv + -3.05914);
            if (runera == yUL2018B) METycorr = -(0.146125 * npv + -0.533233);
            if (runera == yUL2018C) METxcorr = -(0.430911 * npv + -1.42865);
            if (runera == yUL2018C) METycorr = -(0.0620083 * npv + -1.46021);
            if (runera == yUL2018D) METxcorr = -(0.457327 * npv + -1.56856);
            if (runera == yUL2018D) METycorr = -(0.0684071 * npv + -0.928372);
            if (runera == yUL2018MC) METxcorr = -(0.183518 * npv + 0.546754);
            if (runera == yUL2018MC) METycorr = -(0.192263 * npv + -0.42121);

            // UL2016
            if (runera == yUL2016B) METxcorr = -(-0.0214894 * npv + -0.188255);
            if (runera == yUL2016B) METycorr = -(0.0876624 * npv + 0.812885);
            if (runera == yUL2016C) METxcorr = -(-0.032209 * npv + 0.067288);
            if (runera == yUL2016C) METycorr = -(0.113917 * npv + 0.743906);
            if (runera == yUL2016D) METxcorr = -(-0.0293663 * npv + 0.21106);
            if (runera == yUL2016D) METycorr = -(0.11331 * npv + 0.815787);
            if (runera == yUL2016E) METxcorr = -(-0.0132046 * npv + 0.20073);
            if (runera == yUL2016E) METycorr = -(0.134809 * npv + 0.679068);
            if (runera == yUL2016F) METxcorr = -(-0.0543566 * npv + 0.816597);
            if (runera == yUL2016F) METycorr = -(0.114225 * npv + 1.17266);
            if (runera == yUL2016Flate) METxcorr = -(0.134616 * npv + -0.89965);
            if (runera == yUL2016Flate) METycorr = -(0.0397736 * npv + 1.0385);
            if (runera == yUL2016G) METxcorr = -(0.121809 * npv + -0.584893);
            if (runera == yUL2016G) METycorr = -(0.0558974 * npv + 0.891234);
            if (runera == yUL2016H) METxcorr = -(0.0868828 * npv + -0.703489);
            if (runera == yUL2016H) METycorr = -(0.0888774 * npv + 0.902632);
            if (runera == yUL2016MCnonAPV) METxcorr = -(-0.153497 * npv + -0.231751);
            if (runera == yUL2016MCnonAPV) METycorr = -(0.00731978 * npv + 0.243323);
            if (runera == yUL2016MCAPV) METxcorr = -(-0.188743 * npv + 0.136539);
            if (runera == yUL2016MCAPV) METycorr = -(0.0127927 * npv + 0.117747);
        }

        // UL2017Puppi
        if (ispuppi) {
            if (runera == yUL2017B) METxcorr = -(-0.00382117 * npv + -0.666228);
            if (runera == yUL2017B) METycorr = -(0.0109034 * npv + 0.172188);
            if (runera == yUL2017C) METxcorr = -(-0.00110699 * npv + -0.747643);
            if (runera == yUL2017C) METycorr = -(-0.0012184 * npv + 0.303817);
            if (runera == yUL2017D) METxcorr = -(-0.00141442 * npv + -0.721382);
            if (runera == yUL2017D) METycorr = -(-0.0011873 * npv + 0.21646);
            if (runera == yUL2017E) METxcorr = -(0.00593859 * npv + -0.851999);
            if (runera == yUL2017E) METycorr = -(-0.00754254 * npv + 0.245956);
            if (runera == yUL2017F) METxcorr = -(0.00765682 * npv + -0.945001);
            if (runera == yUL2017F) METycorr = -(-0.0154974 * npv + 0.804176);
            if (runera == yUL2017MC) METxcorr = -(-0.0102265 * npv + -0.446416);
            if (runera == yUL2017MC) METycorr = -(0.0198663 * npv + 0.243182);

            // UL2018Puppi
            if (runera == yUL2018A) METxcorr = -(-0.0073377 * npv + 0.0250294);
            if (runera == yUL2018A) METycorr = -(-0.000406059 * npv + 0.0417346);
            if (runera == yUL2018B) METxcorr = -(0.00434261 * npv + 0.00892927);
            if (runera == yUL2018B) METycorr = -(0.00234695 * npv + 0.20381);
            if (runera == yUL2018C) METxcorr = -(0.00198311 * npv + 0.37026);
            if (runera == yUL2018C) METycorr = -(-0.016127 * npv + 0.402029);
            if (runera == yUL2018D) METxcorr = -(0.00220647 * npv + 0.378141);
            if (runera == yUL2018D) METycorr = -(-0.0160244 * npv + 0.471053);
            if (runera == yUL2018MC) METxcorr = -(-0.0214557 * npv + 0.969428);
            if (runera == yUL2018MC) METycorr = -(0.0167134 * npv + 0.199296);

            // UL2016Puppi
            if (runera == yUL2016B) METxcorr = -(-0.00109025 * npv + -0.338093);
            if (runera == yUL2016B) METycorr = -(-0.00356058 * npv + 0.128407);
            if (runera == yUL2016C) METxcorr = -(-0.00271913 * npv + -0.342268);
            if (runera == yUL2016C) METycorr = -(0.00187386 * npv + 0.104);
            if (runera == yUL2016D) METxcorr = -(-0.00254194 * npv + -0.305264);
            if (runera == yUL2016D) METycorr = -(-0.00177408 * npv + 0.164639);
            if (runera == yUL2016E) METxcorr = -(-0.00358835 * npv + -0.225435);
            if (runera == yUL2016E) METycorr = -(-0.000444268 * npv + 0.180479);
            if (runera == yUL2016F) METxcorr = -(0.0056759 * npv + -0.454101);
            if (runera == yUL2016F) METycorr = -(-0.00962707 * npv + 0.35731);
            if (runera == yUL2016Flate) METxcorr = -(0.0234421 * npv + -0.371298);
            if (runera == yUL2016Flate) METycorr = -(-0.00997438 * npv + 0.0809178);
            if (runera == yUL2016G) METxcorr = -(0.0182134 * npv + -0.335786);
            if (runera == yUL2016G) METycorr = -(-0.0063338 * npv + 0.093349);
            if (runera == yUL2016H) METxcorr = -(0.015702 * npv + -0.340832);
            if (runera == yUL2016H) METycorr = -(-0.00544957 * npv + 0.199093);
            if (runera == yUL2016MCnonAPV) METxcorr = -(-0.0058341 * npv + -0.395049);
            if (runera == yUL2016MCnonAPV) METycorr = -(0.00971595 * npv + -0.101288);
            if (runera == yUL2016MCAPV) METxcorr = -(-0.0060447 * npv + -0.4183);
            if (runera == yUL2016MCAPV) METycorr = -(0.008331 * npv + -0.0990046);
        }

    } else {  // these are the corrections for v2 MET recipe (currently recommended for 2017)
        if (runera == y2016B) METxcorr = -(-0.0374977 * npv + 0.00488262);
        if (runera == y2016B) METycorr = -(0.107373 * npv + -0.00732239);
        if (runera == y2016C) METxcorr = -(-0.0832562 * npv + 0.550742);
        if (runera == y2016C) METycorr = -(0.142469 * npv + -0.153718);
        if (runera == y2016D) METxcorr = -(-0.0400931 * npv + 0.753734);
        if (runera == y2016D) METycorr = -(0.127154 * npv + 0.0175228);
        if (runera == y2016E) METxcorr = -(-0.0409231 * npv + 0.755128);
        if (runera == y2016E) METycorr = -(0.168407 * npv + 0.126755);
        if (runera == y2016F) METxcorr = -(-0.0161259 * npv + 0.516919);
        if (runera == y2016F) METycorr = -(0.141176 * npv + 0.544062);
        if (runera == y2016G) METxcorr = -(0.0583851 * npv + -0.0987447);
        if (runera == y2016G) METycorr = -(0.0641427 * npv + 0.319112);
        if (runera == y2016H) METxcorr = -(0.0706267 * npv + -0.13118);
        if (runera == y2016H) METycorr = -(0.127481 * npv + 0.370786);
        if (runera == y2017B) METxcorr = -(-0.19563 * npv + 1.51859);
        if (runera == y2017B) METycorr = -(0.306987 * npv + -1.84713);
        if (runera == y2017C) METxcorr = -(-0.161661 * npv + 0.589933);
        if (runera == y2017C) METycorr = -(0.233569 * npv + -0.995546);
        if (runera == y2017D) METxcorr = -(-0.180911 * npv + 1.23553);
        if (runera == y2017D) METycorr = -(0.240155 * npv + -1.27449);
        if (runera == y2017E) METxcorr = -(-0.149494 * npv + 0.901305);
        if (runera == y2017E) METycorr = -(0.178212 * npv + -0.535537);
        if (runera == y2017F) METxcorr = -(-0.165154 * npv + 1.02018);
        if (runera == y2017F) METycorr = -(0.253794 * npv + 0.75776);
        if (runera == y2018A) METxcorr = -(0.362642 * npv + -1.55094);
        if (runera == y2018A) METycorr = -(0.0737842 * npv + -0.677209);
        if (runera == y2018B) METxcorr = -(0.485614 * npv + -2.45706);
        if (runera == y2018B) METycorr = -(0.181619 * npv + -1.00636);
        if (runera == y2018C) METxcorr = -(0.503638 * npv + -1.01281);
        if (runera == y2018C) METycorr = -(0.147811 * npv + -1.48941);
        if (runera == y2018D) METxcorr = -(0.520265 * npv + -1.20322);
        if (runera == y2018D) METycorr = -(0.143919 * npv + -0.979328);
        if (runera == y2016MC) METxcorr = -(-0.159469 * npv + -0.407022);
        if (runera == y2016MC) METycorr = -(-0.0405812 * npv + 0.570415);
        if (runera == y2017MC) METxcorr = -(-0.182569 * npv + 0.276542);
        if (runera == y2017MC) METycorr = -(0.155652 * npv + -0.417633);
        if (runera == y2018MC) METxcorr = -(0.299448 * npv + -0.13866);
        if (runera == y2018MC) METycorr = -(0.118785 * npv + 0.0889588);
    }

    double CorrectedMET_x = pt() * cos(phi()) + METxcorr;
    double CorrectedMET_y = pt() * sin(phi()) + METycorr;

    double CorrectedMET = sqrt(CorrectedMET_x * CorrectedMET_x + CorrectedMET_y * CorrectedMET_y);
    double CorrectedMETPhi;
    if (CorrectedMET_x == 0 && CorrectedMET_y > 0)
        CorrectedMETPhi = TMath::Pi();
    else if (CorrectedMET_x == 0 && CorrectedMET_y < 0)
        CorrectedMETPhi = -TMath::Pi();
    else if (CorrectedMET_x > 0)
        CorrectedMETPhi = TMath::ATan(CorrectedMET_y / CorrectedMET_x);
    else if (CorrectedMET_x < 0 && CorrectedMET_y > 0)
        CorrectedMETPhi = TMath::ATan(CorrectedMET_y / CorrectedMET_x) + TMath::Pi();
    else if (CorrectedMET_x < 0 && CorrectedMET_y < 0)
        CorrectedMETPhi = TMath::ATan(CorrectedMET_y / CorrectedMET_x) - TMath::Pi();
    else
        CorrectedMETPhi = 0;

    //TheXYCorr_Met_MetPhi.first = CorrectedMET;
    //TheXYCorr_Met_MetPhi.second = CorrectedMETPhi;
    return variedMet(CorrectedMET, CorrectedMETPhi);
}