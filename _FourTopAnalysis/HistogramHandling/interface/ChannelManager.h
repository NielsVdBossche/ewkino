#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include "../../globalSettings.h"

#include "Channel.h"
#include "HistogramConfigurations.h"

class ChannelManager {
    private:
        TFile* outfile;
        
        std::map<eventClass, std::string> namingScheme = {
            {fail, "fail"},
            {crwz, "CRWZ"},
            {cr_conv, "CR-Conversion"},
            {crz3L, "CR-3L-Z"},
            {crz4L, "CR-4L-Z"},
            {cro, "CR-2L-23J1B"},
            {cro3L, "CR-3L-2J1B"},
            {crw, "CR-2L-45J2B"},
            {ssdl, "SR-2L"},
            {trilep, "SR-3L"},
            {fourlep, "SR-4L"}};
        std::map<eventClass, Channel*> mapping;
        std::vector<std::string> processHistName;

        bool useUncertainties = true;
    public:
        ChannelManager(TFile* outputFile);
        ChannelManager(TFile* outputFile, std::map<eventClass, std::string> naming);
        ChannelManager(TFile* outputFile, std::vector<HistInfo>* (&histInfoGenerator)(const eventClass));

        ~ChannelManager();

        Channel* at(eventClass ev) {return mapping[ev];}
        Channel* operator[](eventClass ev) {return mapping[ev];}

        void newSample(std::string& sampleName);

        void addSubUncertainties(shapeUncId uncID, std::vector<std::string>& subUncNames);

        void initHistogramStacks(std::vector<std::string>& initialProcessNames, bool uncertainties);
        void changePrimaryProcess(std::string& newPrimProc);
        void changeProcess(unsigned procNumber, std::string& newProc);

        void writeNominalHistograms(std::string& uniqueSampleName);
        void writeUncertaintyHistograms(std::string& uniqueSampleName);
        void writeUncertaintyEnvelopeHistograms(unsigned subProc);

        void SetPrintAllUncertaintyVariations(bool setting);

        // series of functions passing through
        std::map<shapeUncId, std::string> getTranslateUnc() {return mapping[eventClass::ssdl]->getTranslateUnc();}

        void fillUpHistograms(eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight); 
        void fillDownHistograms(eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight);

        void fillAllUpHistograms(std::vector<std::string>& subs, eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight); 
        void fillAllDownHistograms(std::vector<std::string>& subs, eventClass evClass, shapeUncId id, unsigned procNumber, std::vector<double>& fillVec, std::vector<std::pair<int, double>>& singleHist, std::vector<std::pair<double, double>>& twoDimFillVec, double weight);
};

#endif