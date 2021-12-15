#ifndef UNCERTAINTY_4T_H
#define UNCERTAINTY_4T_H

#include "../../additionalTools/interface/HistogramManager.h"
#include "../../histogramSetups/histogramSetup.h"
#include "EventFourT.h"

// Shape uncertainties
enum shapeUncId {
    isrShape,
    isrNorm,
    fsrShape,
    fsrNorm,
    electronReco,
    pileup,
    end  /// always at the end for easier loops
};

// potential things to add: btag, met, leptons, scale, pileup, prefire, 
//std::map<shapeUncId, std::string> translateUnc;// = {{pileup, "pileup"}};

class Uncertainty {
    private:
        shapeUncId id;

        // pair of weight up and down
        // naming of the histograms is here perhaps the hardest thing
        HistogramManager* upHists;
        HistogramManager* downHists;
    public:
        Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms);
        ~Uncertainty();

        void newSample(std::string& uniqueName);
        void fillHistograms(std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt);
        void fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt);
        void fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt);

        void writeCurrentHistograms();
        void writeNonpromptHistograms();
};

#endif
