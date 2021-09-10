#ifndef objsel4T_h
#define objsel4T_h

#include "../../globalSettings.h"

#include "../../../objects/interface/LorentzVector.h"
#include "../../../objects/interface/Electron.h"
#include "../../../objects/interface/Muon.h"
#include "../../../objects/interface/Jet.h"

namespace ObjectSelection4T {
    bool elID(const Electron& electron);
    bool muonID(const Muon& muon);
    bool isJetIsolatedFromEl(LorentzVector* currentJet);
    bool isJetIsolatedFromMu(LorentzVector* currentJet);
    bool jetID(const Jet& jet);
    bool bJetID(const Jet& jet);
}

#endif
