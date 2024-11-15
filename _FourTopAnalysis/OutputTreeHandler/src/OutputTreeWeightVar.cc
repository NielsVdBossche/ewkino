#include "../interface/OutputTreeWeightVar.h"

OutputTreeWeightVar::OutputTreeWeightVar(TFile* outputfile, std::string& treeName, std::string& outputLevel) : 
    OutputTreeVariables(outputfile, treeName, outputLevel)
{
    // add "weightinfo tree" or somthing, basically store vector with variations or make tree and get the vector elsewhere
    // also then add stuff to main tree

    // std::shared_ptr<TTree> tree = GetTree();
    GetTree()->Branch("weightVariations",   &weightVariations);
    GetTree()->Branch("eftVariations",      &eftVariations);
    GetTree()->Branch("scaleVariations",    &scaleVariations);
    GetTree()->Branch("isrfsrVariations",   &isrfsrVariations);
    GetTree()->Branch("pdfVariations",      &pdfVariations);
    GetTree()->Branch("otherVariations",    &otherVariations);
    GetTree()->Branch("genJetHT",           &genJetHT,         "genJetHT/F");
    GetTree()->Branch("expUp",              &expUp);
    GetTree()->Branch("expDown",            &expDown);

}

OutputTreeWeightVar::~OutputTreeWeightVar()
{
}

void OutputTreeWeightVar::FillTree(EventFourT* ftEvent, double weight) {
    FillBaseTree(weight, ftEvent);

    weightVariations = {1., 2.};
    genJetHT = 0.;
    if (ftEvent->getEvent()->GetPLInfoPtr()) {
        genJetHT = ftEvent->getEvent()->GetPLInfoPtr()->GetGenJetHT();
    }

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

void OutputTreeWeightVar::SetScaleVariations(std::vector<double>& scaleVar) {
    // I wonder how dangerous this is... Maybe make a pointer out of them
    scaleVariations = scaleVar;
}

void OutputTreeWeightVar::AddPDFVariations(EventFourT* ftEvent, double weight, std::shared_ptr< SampleCrossSections > xsecs, bool hasValidPdfs) {
    pdfVariations.clear();
    int max = 100;
    if (hasValidPdfs) {
        Event* currentEvent = ftEvent->getEvent();
        unsigned numberOfPdfVariations = currentEvent->generatorInfo().numberOfPdfVariations();

        if (numberOfPdfVariations < max) {
            max = numberOfPdfVariations;
        }
        //if (isSignalSample) {
        for(int i=1; i<max+1; ++i){
            pdfVariations.push_back(weight * currentEvent->generatorInfo().relativeWeightPdfVar(i) / xsecs.get()->crossSectionRatio_pdfVar(i));
        }
        for (int i=max; i < 101; i++) {
            pdfVariations.push_back(weight);
        }
    } else {
        for (int i=1; i < max+1; i++) {
            pdfVariations.push_back(weight);
        }
    }
}

void OutputTreeWeightVar::SetISRFSRVariations(std::vector<double>& isrfsrVar) {
    isrfsrVariations = isrfsrVar;
}

void OutputTreeWeightVar::SetOtherVariations(std::vector<double>& otherVar) {
    otherVariations = otherVar;
}

void OutputTreeWeightVar::SetExperimentalWeightVariations(std::vector<double>& up, std::vector<double>& down) {
    expUp = up;
    expDown = down;
}
