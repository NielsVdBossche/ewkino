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

bool passLight_SCdilepton_or_trilep(Event& event) {
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


bool passSkim( Event& event, const std::string& skimCondition ){
    static std::map< std::string, std::function< bool(Event&) > > skimFunctionMap = {
        { "noskim", [](Event&){ return true; } },
        { "singlelepton", passSingleLeptonSkim },
        { "dilepton", passDileptonSkim },
        { "trilepton", passTrileptonSkim },
        { "fourlepton", passFourLeptonSkim },
        { "fakerate", passFakeRateSkim },
        { "light_SCdilepton_or_trilep", passLightDLSameChargeSkim }
    };
    auto it = skimFunctionMap.find( skimCondition );
    if( it == skimFunctionMap.cend() ){
        throw std::invalid_argument( "unknown skim condition " + skimCondition );
    } else {
        return (it->second)(event);
    }
}
