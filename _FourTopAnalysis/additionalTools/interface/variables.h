#ifndef FT_VARIABLES_H
#define FT_VARIABLES_H

#include <vector>

#include "../../../Event/interface/LeptonCollection.h"
#include "../../../Event/interface/JetCollection.h"

namespace variables {
    std::vector<double> CalculatePtAllDiobjectSystems(LeptonCollection& leptons, JetCollection& jets);
    double MaxPtAllDiobjectSystems(LeptonCollection& leptons, JetCollection& jets);
}
#endif