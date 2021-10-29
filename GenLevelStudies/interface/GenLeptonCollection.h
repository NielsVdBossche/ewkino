#ifndef GENLEPTONCOLLECTION_H
#define GENLEPTONCOLLECTION_H

#include "GenLepton.h"
#include "../../Event/interface/PhysicsObjectCollection.h"
#include "../../TreeReader/interface/TreeReader.h"


class GenLeptonCollection : public PhysicsObjectCollection<GenLepton> {
    public:
        GenLeptonCollection(const TreeReader&);
        GenLeptonCollection() = default;

        void selectLightLeptons(); // only keep prompt and light leptons

        double getLT();
        GenLepton* getLepton(int index);

        bool passSelection();
            
    private:


};


#endif