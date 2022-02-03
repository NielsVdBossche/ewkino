#include <TH1.h>
#include <TFile.h>
#include <iostream>
#include <TKey.h>
#include <string>

void makeCopyNest(TFile* outputFile, TFile* inputFile, std::string currentLoc) {
    inputFile->cd(currentLoc.c_str());
    TList* objects = gDirectory->GetListOfKeys();
    std::cout << currentLoc << std::endl;

    for(const auto&& obj: *objects) {
        outputFile->cd(currentLoc.c_str());

        // if folder, do this
        if (std::string(((TKey*) obj)->GetClassName()) == "TH1D" || std::string(((TKey*) obj)->GetClassName()) == "TH2D") {
            TH1* finalHist;

            if (gDirectory->GetListOfKeys()->Contains(obj->GetName())) {
                gDirectory->GetObject(obj->GetName(), finalHist);
                
                TH1* interHist;
                inputFile->cd(currentLoc.c_str());
                gDirectory->GetObject(obj->GetName(), interHist);
                finalHist->Add(interHist);
            } else {
                inputFile->cd(currentLoc.c_str());
                gDirectory->GetObject(obj->GetName(), finalHist);
            }
            outputFile->cd(currentLoc.c_str());

            finalHist->Write(finalHist->GetName(), TObject::kOverwrite);
        } else {
            // check if it exists
            if (! gDirectory->GetDirectory(obj->GetName())) gDirectory->mkdir(obj->GetName());
            makeCopyNest(outputFile, inputFile, currentLoc + "/" + obj->GetName());
        }
    }
}


int main(int argc, char const *argv[]) {
    /*
    First input is outputfile
    Others are inputfiles
    */
    
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    size_t splitPos = argvStr[1].find_last_of("_");
    std::string outputFilename = argvStr[1].substr(0, splitPos + 1) + "All.root";
    std::cout << outputFilename << std::endl;
    TFile* outputFile = new TFile(outputFilename.c_str(), "RECREATE");

    for (unsigned i=1; i < argvStr.size(); i++) {
        std::string inputFileName = argvStr[i];
        TFile* inputFile = new TFile(inputFileName.c_str(), "READ");
        makeCopyNest(outputFile, inputFile, "");

        inputFile->Close();
    }
    outputFile->Close();

    return 0;
}
