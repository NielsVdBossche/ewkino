#ifndef CHANNEL_H
#define CHANNEL_H

#include "../../globalSettings.h"

#include "HistogramSet.h"
#include "Uncertainty.h"
#include "UncertaintyEnvelope.h"

class Channel {
    /*
    unsigned subDiv: 0-X: 0 is nominal, higher ones are different like nonPrompt and saved at the end
    */
    private:
        std::map<shapeUncId, std::string> translateUnc = { {muonIDSys, "muonIDSyst"}, {muonIDStat, "muonIDStat"}, {EleIDSys, "electronIDSyst"}, {EleIDStat, "electronIDStat"}, {prefire, "prefire"},
                                                           {pileup, "pileup"}, {electronReco, "electronReco"}, {qcdScale, "qcdScale"}, {pdfShapeVar, "pdfShapeVar"}, {bTagShape, "bTagShape"},
                                                           {isrShape, "isrShape"}, {fsrShape, "fsrShape"}, {isrNorm, "isrNorm"}, {fsrNorm, "fsrNorm"}, {JER_1p93, "JER_1p93"}, {JER_2p5, "JER_2p5"}, {JEC, "JEC"}, };
        std::string ChannelName;
        std::string SubChannelName = "";
        HistogramSet* nominalHistograms;
        std::map<shapeUncId, Uncertainty*> uncHistMap;

        std::vector<HistInfo>* oneDimInfo;
        std::vector<HistInfo_2D>* twoDimInfo = nullptr;

        std::map<std::string, Channel*>* subChannels = nullptr;

        std::vector<HistInfo> hardCopyInfoVector(std::vector<HistInfo>* infoVec);
        std::vector<HistInfo_2D> hardCopy2DInfoVector(std::vector<HistInfo_2D>* infoVec);


    public:
        Channel(std::string& channel, std::vector<HistInfo>* histInfo);
        Channel(std::string& channel, std::string& subChannel, std::vector<HistInfo>* histInfo);

        ~Channel();

        // initialization functions
        void addSubChannels(std::vector<std::string>& subchannels);

        void updateHistInfo(std::vector<HistInfo>* extraInfo);
        void set2DHistInfo(std::vector<HistInfo_2D>* new2DInfo);

        std::vector<HistInfo>* getHistInfo() {return oneDimInfo;}
        std::vector<HistInfo_2D>* get2DHistInfo() {return twoDimInfo;}
        Uncertainty* getUncertainty(shapeUncId id) {return uncHistMap[id];}

        void initializeHistogramStack(std::vector<std::string>& divsInitial);

        //void subDivisions(std::vector<std::string>& divs);
        void changeProcess(unsigned index, std::string& newTitle);

        // histogram management
        void newSample(std::string& uniqueSampleName);
        void writeNominalHistograms(unsigned subProc);
        void writeUncertaintyHistograms(unsigned subProc);
        void writeUncertaintyEnvelopeHistograms(unsigned subProc);

        // getters
        HistogramSet* getNominalHistograms() {return nominalHistograms;}
        std::map<std::string, Channel*>* getSubChannels() {return subChannels;}
        std::map<shapeUncId, std::string> getTranslateUnc() {return translateUnc;}

        // series of filling functions
        void fillHistograms(unsigned subProc, std::vector<double>& fillVec, double eventWeight);
        void fillSingleHistograms(unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fill2DHistograms(unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void fillSubchannelHistogram(std::vector<std::string>& subs, unsigned subProc, std::vector<double>& fillVec, double eventWeight);
        void fillSubchannelSingleHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSubchannel2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight);
        void fillSubchannelSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void fillAllHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<double>& fillVec, double eventWeight);
        void fillAllSingleHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillAll2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight);
        void fillAllSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void fillUncertainties(shapeUncId id, unsigned subProc, std::vector<double>& fillVec, double weightUp, double weightDown);
        void fillSingleUncertainties(shapeUncId id, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown);
        void fill2DUncertainties(shapeUncId id, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown);
        void fillSingle2DUncertainties(shapeUncId id, unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double weightUp, double weightDown);

        void fillAllUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<double>& fillVec, double weightUp, double weightDown);
        void fillAllSingleUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown);
        void fillAll2DUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown);
        void fillAllSingle2DUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::map<size_t, std::pair<double, double>>& fillVec, double weightUp, double weightDown);

        void addSubUncertainties(shapeUncId id, std::vector<std::string>& subUnc);

        void fillEnvelope(shapeUncId id,  unsigned subProc, std::vector<double>& fillVec, std::vector<double> weight);
        void fillEnvelopeSingles(shapeUncId id,  unsigned subProc, std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight);
        void fillEnvelope2Ds(shapeUncId id,  unsigned subProc, std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight);

        void fillSubUncertainty(shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown);
        void fillSingleSubUncertainty(shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown);
        void fill2DSubUncertainty(shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown);

        void fillAllEnvelope(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::vector<double>& fillVec, std::vector<double> weight);
        void fillAllEnvelopeSingles(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::vector<std::pair<int, double>>& fillVec, std::vector<double> weight);
        void fillAllEnvelope2Ds(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::vector<std::pair<double, double>>& fillVec, std::vector<double> weight);

        void fillAllSubUncertainty(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<double>& fillVec, double weightUp, double weightDown);
        void fillAllSingleSubUncertainty(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown);
        void fillAll2DSubUncertainty(std::vector<std::string>& subs, shapeUncId id,  unsigned subProc, std::string subUnc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown);

        void fillUpHistograms(shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight);
        void fillDownHistograms(shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight);

        void fillAllUpHistograms(std::vector<std::string>& subs, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight);
        void fillAllDownHistograms(std::vector<std::string>& subs, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight);

};

#endif
