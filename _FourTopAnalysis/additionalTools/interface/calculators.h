#ifndef CALCULATORS_H
#define CALCULATORS_H

#include "../../../Event/interface/JetCollection.h"
#include "../../../Event/interface/LeptonCollection.h"

#include <vector>
#include <memory>

namespace calculators {
    std::vector<double> mindRInJetCollection(JetCollection& collection);

    std::vector<double> mindRLepAndJet(JetCollection& jets, LeptonCollection& leptons);
}

#endif