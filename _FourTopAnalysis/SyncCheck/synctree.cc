#include "synctree.h"

#include <TFile.h>

SyncTree::SyncTree(std::string& pathToFile) {
    TFile* syncfile = new TFile(pathToFile.c_str());

    tree = (TTree*) syncfile->Get("SyncObjects");

    tree->SetBranchAddress("EventNumber", &_eventNumber, &b__eventNumber);
    tree->SetBranchAddress("muons_is_loose",  &muons_is_loose, &b__muons_is_loose);
    tree->SetBranchAddress("muons_is_fakeable", &muons_is_fakeable, &b__muons_is_fakeable);
    tree->SetBranchAddress("muons_is_tight", &muons_is_tight, &b__muons_is_tight);
    tree->SetBranchAddress("electrons_is_loose", &electrons_is_loose, &b__electrons_is_loose);
    tree->SetBranchAddress("electrons_is_fakeable", &electrons_is_fakeable, &b__electrons_is_fakeable);
    tree->SetBranchAddress("electrons_is_tight", &electrons_is_tight, &b__electrons_is_tight);
}
