#include "../interface/TopReconstructionNew.h"

void TopReconstructionNew::RecoBestTwoTops() {
    // Reconstruct best two top quarks as efficiently as possible without overlapping jets
    // extract invariant mass of pair
    // One jet must be b-tagged
    // of the other jets, the invariant mass is kept as well to compare agains mW
    
    // find best and second best, overlap

    std::vector<unsigned> excludedJets;

    if (selection->numberOfJets() < 3) {
        bestTop = {-1., -1.};
        secondBestTop = {-1., -1.};
        return;
    } 

    bestTop = getBestTop(excludedJets);

    if (selection->numberOfMediumBJets() < 2  || selection->numberOfJets() < 6) {
        secondBestTop = {-1., -1.};
        return;
    }

    secondBestTop = getBestTop(excludedJets);

}

std::pair<double, double> TopReconstructionNew::getBestTop(std::vector<unsigned>& exclusions) {
    JetCollection* jets = selection->getJetCol();

    std::vector<int> newExcl;
    double bestCurrentMass = 0.;
    double bestCurrentW_mass = 0.;

    for (unsigned k = 0; k < jets->size(); k++) {
        Jet* jetOne = jets->at(k);

        if (! jetOne->isBTaggedMedium()) continue;

        if (std::find(exclusions.begin(), exclusions.end(), k) != exclusions.end()) continue;

        for (unsigned i = 0; i < jets->size(); i++) {
            Jet* jetTwo = jets->at(i);

            if (std::find(exclusions.begin(), exclusions.end(), i) != exclusions.end()) continue;
            if (k == i) continue;

            for (unsigned j = 0; j < jets->size(); j++) {
                Jet* jetThree = jets->at(j);

                if (std::find(exclusions.begin(), exclusions.end(), j) != exclusions.end()) continue;
                if (k == j || i == j) continue;

                std::vector<int> curr_indices;
                curr_indices.push_back(k);
                curr_indices.push_back(i);
                curr_indices.push_back(j);

                PhysicsObject combination = *jetOne + *jetTwo + *jetThree;
                double mass = combination.mass();

                // Score top now
                if (fabs(mass - particle::mTop) < fabs(bestCurrentMass - particle::mTop)) {
                    bestCurrentMass = mass;
                    bestCurrentW_mass = (*jetTwo + *jetThree).mass();
                    newExcl = curr_indices; // jetcollection in this top
                }
                
            }
        }
    }

    exclusions.insert(exclusions.end(), newExcl.begin(), newExcl.end());

    return {bestCurrentMass, bestCurrentW_mass};
}
