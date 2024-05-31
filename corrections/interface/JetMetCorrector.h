#ifndef CORRECTIONS_JETMETCORRECTOR_H
#define CORRECTIONS_JETMETCORRECTOR_H

#include <correction.h>
#include "../../Event/interface/JetCollection.h"
#include "../../objects/interface/Met.h"

#include "JetSelectionHelper.h"

class JetMetCorrector {
    public:
        JetMetCorrector(std::unique_ptr<correction::CorrectionSet> jercSet);
        ~JetMetCorrector();

        std::pair<JetCollection, Met> ApplyJECUncertainty(bool isUp, JetCollection& nominalJets, Met& nominalMET);
    private:
        std::shared_ptr<const correction::Correction> jerCorrection;
        std::shared_ptr<JetSelectionHelper> selection;
};


#endif