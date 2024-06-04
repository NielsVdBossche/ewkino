#include "../interface/OutputTreeSysVar.h"

OutputTreeSysVar::OutputTreeSysVar(TFile* outputfile, std::string& treeName, std::string& outputLevel) : 
    OutputTreeVariables(outputfile, treeName, outputLevel)
{
}

OutputTreeSysVar::~OutputTreeSysVar()
{
}

void OutputTreeSysVar::FillTree(EventFourT* ftEvent, double weight) {
    FillBaseTree(weight, ftEvent);
}
