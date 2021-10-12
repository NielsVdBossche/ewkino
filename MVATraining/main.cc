#include "interface/dataManager.h"

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>

#include <iostream>

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        std::cerr << "Mvatrainer requires at least one argument: <samplelist.txt>" << std::endl;
        exit(1);
    }
    // manage input
    // Pass files to a class using a samplelist to order these in categories
    std::string sampleList = argv[1];
    std::string tree = argv[2]; // tree should be dynamical, but don't know if it will be used as input. Maybe in some other way... 
    TMVA::DataLoader* data = mvaDataManager::buildDataLoader(sampleList, tree, mvaConfiguration::ML);

    TFile* outfile = new TFile("FourTopClassification.root" ,"RECREATE");
    TMVA::Factory* factory = new TMVA::Factory("FourTopClassification", outfile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D:AnalysisType=multiclass" ); // get type of classificationjob from argv, pass this to function deciding some extra arguments.

    // class manages a dataloader and a factory, as well as settings for the mva's

    // Main part of calling training etc
    factory->BookMethod(data,  TMVA::Types::kBDT, "BDTG_ML", 
                    "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=3");

    factory->TrainAllMethods();
	
	factory->TestAllMethods();
	
	factory->EvaluateAllMethods();

    TCanvas* rocPlot = factory->GetROCCurve(data);
    rocPlot->Print("rocCurve.png");

	outfile->Write();
	outfile->Close();

    return 0;
}
