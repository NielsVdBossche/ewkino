#ifndef FOURTOP_OUTPUTTREE_WEIGHTVAR
#define FOURTOP_OUTPUTTREE_WEIGHTVAR

#include "OutputTreeVariables.h"
class OutputTreeWeightVar : public OutputTreeVariables {
    private:
        /* data */
    public:
        // weight variations. Store: up, down? 
        // weight should be directly usable to fill e.g. a histogram
        std::vector<double> elSFs;
        std::vector<double> elRecoSFs;
        std::vector<double> muSFs;
        std::vector<double> btagSF_var1; // put in all btag variations in sep vectors?
        // qcd, pdf, etc etc, those first two are vectors!

        // alternative: 2 vectors, on with names and one with variations, naming vector can even be stored separately
        // metadata vector basically
        std::vector<std::string> weightVariationsNaming;
        std::vector<double> weightVariations;
        std::vector<double> eftVariations;
        std::vector<double> scaleVariations;
        
        // good for simple weights, but 100 pdfs? Might have separate vectors for such things
        

        void FillTree(EventFourT* ftEvent, double weight);
        void SetSaleVariations(std::vector<double>& scaleVar);

        OutputTreeWeightVar(TFile* outputfile, std::string& treeName, std::string& outputLevel);
        ~OutputTreeWeightVar();
};


#endif