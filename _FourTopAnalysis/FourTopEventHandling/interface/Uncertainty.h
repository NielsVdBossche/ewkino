#ifndef UNCERTAINTY_4T_H
#define UNCERTAINTY_4T_H

#include "../../additionalTools/interface/HistogramManager.h"
#include <map>

// Shape uncertainties
enum shapeUncId {
    muonIDStat,
    muonIDSys,
    EleIDStat,
    EleIDSys,
    pileup,
    electronReco,
    isrShape,
    isrNorm,
    fsrShape,
    fsrNorm,
    JER,
    JEC,
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

        void fillUpOrDownHistograms(std::vector<double>& fillVec, double weight, bool up, bool nonPrompt);
        void fillUpOrDownSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weight, bool up, bool nonPrompt);
        void fillUpOrDown2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weight, bool up, bool nonPrompt);

        void writeCurrentHistograms();
        void writeNonpromptHistograms();
};

#endif
