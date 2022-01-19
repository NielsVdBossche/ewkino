#ifndef UNCERTAINTY_H
#define UNCERTAINTY_H

#include "HistogramSet.h"

enum shapeUncId {
    muonIDStat,
    muonIDSys,
    EleIDStat,
    EleIDSys,
    prefire,
    pileup,
    electronReco,
    qcdScale,
    pdfShapeVar,
    bTagShape,
    isrShape,
    isrNorm,
    fsrShape,
    fsrNorm,
    JER_1p93,
    JER_2p5,
    JEC,
    end  /// always at the end for easier loops
};

class Uncertainty {
    private:
        shapeUncId id;
        std::string name;

        // pair of weight up and down
        // naming of the histograms is here perhaps the hardest thing
        HistogramSet* bareHists;
        HistogramSet* upHists;
        HistogramSet* downHists;

        std::map<std::string, HistogramSet*>* upSubMap = nullptr;
        std::map<std::string, HistogramSet*>* downSubMap = nullptr;
    public:
        Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms);
        virtual ~Uncertainty() {};

        virtual void newSample(std::string& uniqueName);
        //virtual void newProcess(std::string& newProcess, TFile* outfile);

        void fillHistograms(std::vector<double>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);

        void fillUpOrDownHistograms(std::vector<double>& fillVec, double weight, bool up, unsigned subProc);
        void fillUpOrDownSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weight, bool up, unsigned subProc);
        void fillUpOrDown2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weight, bool up, unsigned subProc);

        virtual void writeHistograms();

        shapeUncId getID() const {return id;}
        std::string getName() const {return name;}
        HistogramSet* getUpHists() {return upHists;}
        HistogramSet* getDownHists() {return downHists;}

        void addSubUncertainties(std::vector<std::string>& subUnc);

        void fillSubHistograms(std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fillSubSingleHistograms(std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fillSub2DHistograms(std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);

        void writeSubHistograms();
};

#endif
