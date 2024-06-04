#ifndef FOURTOP_OUTPUTTREE_SYSVAR_H
#define FOURTOP_OUTPUTTREE_SYSVAR_H

#include "OutputTreeVariables.h"

class OutputTreeSysVar : public OutputTreeVariables
{
    private:
        /* data */
    public:
        OutputTreeSysVar(TFile* outputfile, std::string& treeName, std::string& outputLevel);
        ~OutputTreeSysVar();

        void FillTree(EventFourT* ftEvent, double weight);
        // main difference is just some naming?
};



#endif