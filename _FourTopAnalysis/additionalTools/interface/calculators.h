#ifndef CALCULATORS_H
#define CALCULATORS_H

#include "../../../Event/interface/JetCollection.h"
#include "../../../Event/interface/LeptonCollection.h"

#include <vector>
#include <memory>

namespace calculators {
    std::vector<double> mindRInJetCollection(JetCollection& collection);
    std::vector<double> mindRLepAndJet(JetCollection& jets, LeptonCollection& leptons);
    std::vector<double> mindRLeptons(LeptonCollection& leptons);
    std::vector<double> mindEtaLeptons(LeptonCollection& leptons);
    std::vector<double> mindPhiLeptons(LeptonCollection& leptons);
    std::pair<std::shared_ptr<Jet>, std::shared_ptr<Lepton>> closestLepAndJet(JetCollection& jets, LeptonCollection& leptons);
}

#endif