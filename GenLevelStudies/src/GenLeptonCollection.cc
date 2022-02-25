#include "../interface/GenLeptonCollection.h"

GenLeptonCollection::GenLeptonCollection(const TreeReader& treeReader) {
    for (unsigned lep = 0; lep < treeReader._gen_nL; lep++) {
        push_back(GenLepton(treeReader, lep));
    }
}

void GenLeptonCollection::selectLightLeptonsMinimal() {
    selectObjects(& GenLepton::isLightLepton);
}

void GenLeptonCollection::selectLightLeptons() {
    selectObjects(& GenLepton::isLightLepton);
    selectObjects(& GenLepton::isPrompt);
    selectObjects(& GenLepton::isUseful);
}

double GenLeptonCollection::getLT() {
    return scalarPtSum();
}

bool GenLeptonCollection::passSelection() {
    if (size() < 2) return false;
    if (size() == 2) {
        if (at(0)->charge() != at(1)->charge()) return false;
    }

    return true;
}
