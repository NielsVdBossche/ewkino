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
    qcdScale,
    pdfShapeVar,
    isrShape,
    isrNorm,
    fsrShape,
    fsrNorm,
    JER_1p93,
    JER_2p5,
    JEC,
    end  /// always at the end for easier loops
};

// potential things to add: btag, met, leptons, scale, pileup, prefire, 
//std::map<shapeUncId, std::string> translateUnc;// = {{pileup, "pileup"}};

class Uncertainty {
    private:
        shapeUncId id;
        std::string name;

        // pair of weight up and down
        // naming of the histograms is here perhaps the hardest thing
        HistogramManager* upHists;
        HistogramManager* downHists;

    public:
        Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramManager* histograms);
        virtual ~Uncertainty() {};

        virtual void newSample(std::string& uniqueName);
        //virtual void newProcess(std::string& newProcess, TFile* outfile);

        void fillHistograms(std::vector<double>& fillVec, double weightUp, double weightDown, bool nonPrompt);
        void fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt);
        void fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, bool nonPrompt);

        void fillUpOrDownHistograms(std::vector<double>& fillVec, double weight, bool up, bool nonPrompt);
        void fillUpOrDownSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weight, bool up, bool nonPrompt);
        void fillUpOrDown2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weight, bool up, bool nonPrompt);

        virtual void writeCurrentHistograms();
        void writeNonpromptHistograms();

        shapeUncId getID() const {return id;}
        std::string getName() const {return name;}
        HistogramManager* getUpHists() {return upHists;}
        HistogramManager* getDownHists() {return downHists;}
};

#endif
