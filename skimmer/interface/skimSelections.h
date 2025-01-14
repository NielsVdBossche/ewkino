/*
Functions to apply a certain skimming condition. If you need new skimming conditions, add them here, and implement them in src/skimSelection.cc
Make sure to always add new skim conditions to the std::map in the passSkim function for them to be able to be used!
*/


#ifndef skimSelections_H
#define skimSelections_H

//include other parts of framework
#include "../../Event/interface/Event.h"

enum skimCondition {
    noskim,
    singlelepton,
    dilepton,
    trilepton,
    fourlepton,
    fakerate,
    light_SSdilepton_or_trilep,
    lightDileptonSkim
};

skimCondition giveCondition(const std::string& condstring);

bool passSingleLeptonSkim( Event& );
bool passDileptonSkim( Event& );
bool passTrileptonSkim( Event& );
bool passFourLeptonSkim( Event& );
bool passFakeRateSkim( Event& );
bool passSkim( Event&, skimCondition );

bool passLightDileptonSkim(Event &);
bool passLightTrilepSkim(Event&);
bool passLightDLSameChargeSkim(Event&);
bool passLight_SSdilepton_or_trilep(Event&);

bool passLightLeptonSkimNew(Event&);

#endif
