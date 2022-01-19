#ifndef CHANNEL_H
#define CHANNEL_H

#include "HistogramSet.h"
#include "Uncertainty.h"
#include "UncertaintyEnvelope.h"

class Channel {
    /*
    unsigned subDiv: 0-X: 0 is nominal, higher ones are different like nonPrompt and saved at the end
    */
    private:
        std::string channel;
        HistogramSet* nominalHistograms;
        std::map<shapeUncId, Uncertainty*> uncHistMap;

        std::vector<HistInfo>* oneDimInfo;
        std::vector<HistInfo_2D>* twoDimInfo = nullptr;

        std::map<std::string, Channel*>* subChannels = nullptr;

        std::vector<HistInfo> hardCopyInfoVector(std::vector<HistInfo>* infoVec);

        std::map<shapeUncId, std::string> translateUnc = { {muonIDSys, "muonIDSyst"}, {muonIDStat, "muonIDStat"}, {EleIDSys, "electronIDSyst"}, {EleIDStat, "electronIDStat"}, {prefire, "prefire"},
                                                           {pileup, "pileup"}, {electronReco, "electronReco"}, {qcdScale, "qcdScale"}, {pdfShapeVar, "pdfShapeVar"}, {bTagShape, "bTagShape"},
                                                           {isrShape, "isrShape"}, {fsrShape, "fsrShape"}, {isrNorm, "isrNorm"}, {fsrNorm, "fsrNorm"}, {JER_1p93, "JER_1p93"}, {JER_2p5, "JER_2p5"}, {JEC, "JEC"} };

    public:
        Channel(std::string channel, std::vector<HistInfo>* histInfo);
        ~Channel();

        // initialization functions
        void addSubChannels(std::vector<std::string>& subchannels);

        void updateHistInfo(std::vector<HistInfo>* extraInfo);
        void set2DHistInfo(std::vector<HistInfo_2D>* new2DInfo);

        void initializeHistogramStack();

        void subDivisions(std::vector<std::string>& divs);
        void changeProcessTitle(unsigned index, std::string& newTitle);

        // histogram management
        void newSample(std::string& uniqueSampleName);
        void writeNominalHistograms(unsigned subProc);
        void writeUncertaintyHistograms(unsigned subProc);
        void writeUncertaintyEnvelopeHistograms(unsigned subProc);

        // series of filling functions
        void fillHistograms(unsigned subProc, std::vector<double>& fillVec, double eventWeight);
        void fillSingleHistograms(unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fill2DHistograms(unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight);
        void fillSingle2DHistograms(unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void fillSubchannelHistogram(std::vector<std::string>& subs, unsigned subProc, std::vector<double>& fillVec, double eventWeight);
        void fillSubchannelSingleHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillSubchannel2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight);
        void fillSubchannelSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void fillAllHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<double>& fillVec, double eventWeight);
        void fillAllSingleHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double eventWeight);
        void fillAll2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double eventWeight);
        void fillAllSingle2DHistograms(std::vector<std::string>& subs, unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double eventWeight);

        void fillUncertainties(shapeUncId id, unsigned subProc, std::vector<double>& fillVec, double weightUp, double weightDown);
        void fillSingleUncertainties(shapeUncId id, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown);
        void fill2DUncertainties(shapeUncId id, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown);
        void fillSingle2DUncertainties(shapeUncId id, unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double weightUp, double weightDown);

        void fillAllUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<double>& fillVec, double weightUp, double weightDown);
        void fillAllSingleUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<std::pair<int, double>>& fillVec, double weightUp, double weightDown);
        void fillAll2DUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<std::pair<double, double>>& fillVec, double weightUp, double weightDown);
        void fillAllSingle2DUncertainties(std::vector<std::string>& subs, shapeUncId id, unsigned subProc, std::vector<size_t, std::pair<double, double>>& fillVec, double weightUp, double weightDown);

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

};

#endif
