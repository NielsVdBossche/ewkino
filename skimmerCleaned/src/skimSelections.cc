#include "../interface/skimSelections.h"

//include c++ library classes
#include <functional>


bool passLeptonicSkim( Event& event, LeptonCollection::size_type numberOfLeptons ){
    event.selectLooseLeptons();
    event.cleanElectronsFromLooseMuons();
    event.cleanTausFromLooseLightLeptons();
    return ( event.numberOfLeptons() >= numberOfLeptons );
}

bool passLightDileptonSkim(Event & event) {
    return (event.numberOfLightLeptons() >= 2);
}

bool passLightTrilepSkim(Event& event) {
    return (event.numberOfLightLeptons() >= 3);
}

bool passLightDLSameChargeSkim(Event& event) {
    if (! passLightDileptonSkim(event)) return false;

    return ((*(event.lightLeptonCollection().begin()))->charge() == (*(event.lightLeptonCollection().begin() + 1))->charge());
}

bool passLight_SSdilepton_or_trilep(Event& event) {
    event.selectLooseLeptons();
    //event.cleanElectronsFromFOMuons();
    //event.makeSubLeptonCollections(); 
    return (passLightDLSameChargeSkim(event) || passLightTrilepSkim(event));
}

bool passSingleLeptonSkim( Event& event ){
    return passLeptonicSkim( event, 1 );
}


bool passDileptonSkim( Event& event ){
    return passLeptonicSkim( event, 2 );
}


bool passTrileptonSkim( Event& event ){
    return passLeptonicSkim( event, 3 );
}


bool passFourLeptonSkim( Event& event ){
    return passLeptonicSkim( event, 4 );
}

bool passLightLeptonSkimNew(Event& event) {
    event.selectLooseLeptons();
    event.cleanElectronsFromLooseMuons();
    return ( event.numberOfLightLeptons() >= 2 );
}

bool passFakeRateSkim( Event& event ){
    event.selectLooseLeptons();
    event.cleanElectronsFromLooseMuons();
    event.cleanTausFromLooseLightLeptons();
    if( event.numberOfLightLeptons() != 1 ) return false;

    event.selectGoodJets();
    event.cleanJetsFromLooseLeptons();
    if( event.numberOfJets() < 1 ) return false;
    return true;
}

bool passFourTopBaseSkim(Event& event) {
    event.selectLooseLeptons();
    event.cleanElectronsFromLooseMuons();
    event.removeTaus();
    if (event.numberOfLightLeptons() < 2) return false;
    if (event.numberOfLightLeptons() == 2 && event.hasOSLeptonPair()) return false;

    return true;
}

bool passFourTopMvaSkim(Event& event) {
    event.selectLooseLeptons();
    event.cleanElectronsFromLooseMuons();
    event.removeTaus();
    if (event.numberOfLightLeptons() < 2) return false;
    if (event.numberOfLightLeptons() - event.numberOfTightLeptons() > 1) return false;
    if (event.numberOfLightLeptons() == 2 && event.hasOSLeptonPair()) return false;
    if (event.numberOfLightLeptons() == 4) {
        if (event.numberOfJets() < 2) return false;
        if (event.numberOfLooseBTaggedJets() < 1) return false;
    } else if (event.numberOfLightLeptons() == 3) {
        if (event.numberOfJets() < 3) return false;
        if (event.numberOfLooseBTaggedJets() < 2) return false;
        if (event.HT() < 200) return false; 
    } else if (event.numberOfLightLeptons() == 2) {
        if (event.numberOfJets() < 4) return false;
        if (event.numberOfLooseBTaggedJets() < 2) return false;
        if (event.numberOfLooseBTaggedJets() == 2 && event.numberOfJets() < 6) return false;
        if (event.HT() < 200) return false; 
    }
    // might make this tighter in the future
    return true;
}


skimCondition giveCondition(const std::string& condstring) {
    static std::map< std::string, skimCondition > skimFunctionMap = {
        { "noskim", noskim },
        { "singlelepton", singlelepton },
        { "dilepton", dilepton },
        { "trilepton", trilepton },
        { "fourlepton", fourlepton },
        { "fakerate", fakerate },
        { "light_SSdilepton_or_trilep", light_SSdilepton_or_trilep },
        { "lightDileptonSkim", lightDileptonSkim},
        { "fourTopBase", fourTopBase},
        { "fourTopMva", fourTopMva}
    };

    auto it = skimFunctionMap.find( condstring );

    if( it == skimFunctionMap.cend() ){
        throw std::invalid_argument( "unknown skim condition " + condstring );
    } else {
        return it->second;
    }
}

bool passSkim( Event& event, skimCondition cond){
    static std::map< skimCondition, std::function< bool(Event&) > > skimFunctionMap = {
        { noskim, [](Event&){ return true; } },
        { singlelepton, passSingleLeptonSkim },
        { dilepton, passDileptonSkim },
        { trilepton, passTrileptonSkim },
        { fourlepton, passFourLeptonSkim },
        { fakerate, passFakeRateSkim },
        { light_SSdilepton_or_trilep, passLight_SSdilepton_or_trilep },
        { lightDileptonSkim, passLightLeptonSkimNew},
        { fourTopBase, passFourTopBaseSkim},
        { fourTopMva, passFourTopMvaSkim}
    };
    //auto it = skimFunctionMap.find( cond );

    //event.makeSubLeptonCollections();
    return skimFunctionMap[cond](event);
}
