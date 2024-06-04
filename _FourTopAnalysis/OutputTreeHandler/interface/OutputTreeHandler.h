#ifndef FOURTOP_OUTPUTTREEHANDLER_H
#define FOURTOP_OUTPUTTREEHANDLER_H

#include "OutputTree.h"

#include "../../globalSettings.h"
// similar functionality as channelmanager: make sure naming is correct, trees are generated all the same
// 

class Sample;
class EventFourT;

class OutputTreeHandler
{
    private:
        TFile* currentFile; // changes for each sample.

        std::string outputfolder;
        std::vector<std::shared_ptr<OutputTree>> mapping;
        std::vector<std::string> processNames;

    public:
        OutputTreeHandler(std::vector<std::string>& processes, std::string& outputSubfolder);
        ~OutputTreeHandler();

        void ChangeProcess(unsigned processNumber, std::string& newProcess);
        void ChangeAllProcesses(std::vector<std::string>& processes);

        // general IO
        TFile* InitializeNewSample(const Sample& sample, std::string& outputFileTags, std::string runtype, std::string name="base");
        void FlushTrees();

        void WriteExpWeightNaming(std::vector<std::string>& weightNames);
        // Filling trees and stuff
        void FillAt(unsigned pNb, EventFourT* ftEvent, double wgt);
        std::shared_ptr<OutputTree> GetTree(unsigned pNb) {return mapping[pNb];}
};

#endif
