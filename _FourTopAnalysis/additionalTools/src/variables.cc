#include "../interface/variables.h"

std::vector<double> variables::CalculatePtAllDiobjectSystems(LeptonCollection& leptons, JetCollection& jets) {
    // First iterate all pairs of leptons
    // Then all pairs of jets
    // Then all pairs of lepton-jet systems
    std::vector<double> ret;
    for (auto itl1 = leptons.begin(); itl1 != leptons.end()-1; itl1++) {
        for (auto itl2 = itl1 + 1; itl2 != leptons.end(); itl2++) {
            if (itl1 == itl2) continue;
            PhysicsObject sum = (**itl1) + (**itl2);
            ret.push_back(sum.pt());
        }
    }
    for (auto itj1 = jets.begin(); itj1 != jets.end()-1; itj1++) {
        for (auto itj2 = itj1 + 1; itj2 != jets.end(); itj2++) {
            if (itj1 == itj2) continue;
            PhysicsObject sum = (**itj1) + (**itj2);
            ret.push_back(sum.pt());
        }
    }
    for (auto itl1 = leptons.begin(); itl1 != leptons.end(); itl1++) {
        for (auto itj2 = jets.begin(); itj2 != jets.end(); itj2++) {
            PhysicsObject sum = (**itl1) + (**itj2);
            ret.push_back(sum.pt());
        }
    }
    return ret;
}

double variables::MaxPtAllDiobjectSystems(LeptonCollection& leptons, JetCollection& jets) {
    std::vector<double> pts = CalculatePtAllDiobjectSystems(leptons, jets);
    return *std::max_element(pts.begin(), pts.end());
}
