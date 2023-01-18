#ifndef UNCERTAINTY_H
#define UNCERTAINTY_H

#include "../../globalSettings.h"

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
    WZSF,
    JER_1p93,
    JER_2p5,
    JEC,
    JECFlavorQCD,
    MET,
    HEMIssue,
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

        bool printAllVariations = false;
    public:
        Uncertainty(std::map<shapeUncId, std::string>& translateUnc, shapeUncId id, HistogramSet* histograms);
        Uncertainty(std::string& uncName, shapeUncId uncID, HistogramSet* histograms);
        virtual ~Uncertainty();

        virtual void newSample(std::string& uniqueName);
        virtual void changeProcess(unsigned index, std::string& newTitle);
        virtual void addProcess(std::string& newProc);
        //virtual void newProcess(std::string& newProcess, TFile* outfile);

        void fillHistograms(std::vector<double>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fillSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fill2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);

        void fillUpOrDownHistograms(std::vector<double>& fillVec, double weight, bool up, unsigned subProc);
        void fillUpOrDownSingleHistograms(std::vector<std::pair<int, double>>& fillVec, double weight, bool up, unsigned subProc);
        void fillUpOrDown2DHistograms(std::vector<std::pair<double, double>>& fillVec, double weight, bool up, unsigned subProc);

        void fillSubUpOrDownHistograms(std::string& subUnc, std::vector<double>& fillVec, double weight, bool up, unsigned subProc);
        void fillSubUpOrDownSingleHistograms(std::string& subUnc, std::vector<std::pair<int, double>>& fillVec, double weight, bool up, unsigned subProc);
        void fillSubUpOrDown2DHistograms(std::string& subUnc, std::vector<std::pair<double, double>>& fillVec, double weight, bool up, unsigned subProc);

        bool GetPrintAllVariations() {return printAllVariations;}
        void SetPrintAllVariations(bool printVars) {printAllVariations = printVars;}

        shapeUncId getID() const {return id;}
        std::string getName() const {return name;}
        HistogramSet* getUpHists() {return upHists;}
        HistogramSet* getDownHists() {return downHists;}

        void addSubUncertainties(std::vector<std::string>& subUnc);

        void fillSubHistograms(std::string& subUnc, std::vector<double>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fillSubSingleHistograms(std::string& subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);
        void fillSub2DHistograms(std::string& subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown, unsigned subProc);

        virtual void writeHistograms(unsigned subProc);
        void writeSubHistograms(unsigned subProc);
};

#endif
