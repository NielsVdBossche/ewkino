#include "../interface/OutputTreeWeightVar.h"

OutputTreeWeightVar::OutputTreeWeightVar(TFile* outputfile, std::string& treeName, std::string& outputLevel) : 
    OutputTreeVariables(outputfile, treeName, outputLevel)
{
    // add "weightinfo tree" or somthing, basically store vector with variations or make tree and get the vector elsewhere
    // also then add stuff to main tree

    // std::shared_ptr<TTree> tree = GetTree();
    GetTree()->Branch("weightVariations",   &weightVariations);
    GetTree()->Branch("eftVariations",   &eftVariations);
    // GetTree()->Branch("eftVariations",   &eftVariations);

}

OutputTreeWeightVar::~OutputTreeWeightVar()
{
}

void OutputTreeWeightVar::FillTree(EventFourT* ftEvent, double weight) {
    FillBaseTree(weight, ftEvent);

    weightVariations = {1., 2.};

    eftVariations.clear();
    GeneratorInfo* genInfo = ftEvent->getEvent()->getGeneratorInfoPtr();
    if (genInfo->getNEFTWeights() != 0) {
        double* eftWeights = genInfo->getEFTWeights();
        for (unsigned i=0; i < genInfo->getNEFTWeights(); i++) {
            eftVariations.push_back(eftWeights[i]);
        }
    } else {
        eftVariations.push_back(weight);
        // see if we can do a "has eft"
    }
    // blabla filling
}
