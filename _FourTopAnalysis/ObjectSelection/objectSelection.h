#include "../../objects/interface/LorentzVector.h"

namespace objFourTop {
    bool elID(int i);
    bool muonID(int i);
    bool isJetIsolatedFromEl(LorentzVector* currentJet);
    bool isJetIsolatedFromMu(LorentzVector* currentJet);
    bool jetID(int i);
    bool bJetID(int i);
}