#include "../interface/OutputTree.h"

//#include <stdio.h>
#include <iostream>
#include <TFile.h>

OutputTree::OutputTree(TFile* outputfile, std::string& treeName) : outputfile(outputfile) {
    outputfile->cd();
    tree = std::make_shared<TTree>(treeName.c_str(), treeName.c_str());
    //tree->SetDirectory(outputfile);
}

OutputTree::~OutputTree()
{
}

void OutputTree::AddEntry() {
    tree->Fill();
}

void OutputTree::SaveToFile(TFile* file) {
    //std::cout << tree->GetEntries() << std::endl;
    file->Write(tree->GetName(), TObject::kOverwrite);
}
