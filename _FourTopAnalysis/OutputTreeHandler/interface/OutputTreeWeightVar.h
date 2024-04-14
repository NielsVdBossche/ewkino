#ifndef FOURTOP_OUTPUTTREE_WEIGHTVAR
#define FOURTOP_OUTPUTTREE_WEIGHTVAR

#include "OutputTreeVariables.h"
#include "../../../Tools/interface/SampleCrossSections.h"

class OutputTreeWeightVar : public OutputTreeVariables {
    private:
        /* data */
    public:
        // weight variations. Store: up, down? 
        // weight should be directly usable to fill e.g. a histogram
        // std::vector<double> elSFs;
        // std::vector<double> elRecoSFs;
        // std::vector<double> muSFs;
        // std::vector<double> btagSF_var1; // put in all btag variations in sep vectors?
        // // qcd, pdf, etc etc, those first two are vectors!

        // alternative: 2 vectors, on with names and one with variations, naming vector can even be stored separately
        // metadata vector basically
        std::vector<std::string> weightVariationsNaming;
        std::vector<double> weightVariations;
        std::vector<double> eftVariations;
        std::vector<double> scaleVariations;
        std::vector<double> isrfsrVariations;
        std::vector<double> pdfVariations;

        std::vector<double> expUp;
        std::vector<double> expDown;

        // good for simple weights, but 100 pdfs? Might have separate vectors for such things
        void FillTree(EventFourT* ftEvent, double weight);
        void SetScaleVariations(std::vector<double>& scaleVar);
        void SetISRFSRVariations(std::vector<double>& isrfsrVar);

        void AddPDFVariations(EventFourT* ftEvent, double weight, std::shared_ptr< SampleCrossSections > xsecs);
        void SetExperimentalWeightVariations(std::vector<double>& up, std::vector<double>& down);

        OutputTreeWeightVar(TFile* outputfile, std::string& treeName, std::string& outputLevel);
        ~OutputTreeWeightVar();
};


#endif