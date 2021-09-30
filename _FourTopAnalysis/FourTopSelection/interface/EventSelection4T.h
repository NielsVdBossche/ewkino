#ifndef eventselection4T_h
#define eventselection4T_h

#include "../../globalSettings.h"

#include "../../../Event/interface/Event.h"

#include "../../../Event/interface/ElectronCollection.h"
#include "../../../Event/interface/MuonCollection.h"
#include "../../../Event/interface/JetCollection.h"


// Class eventSelection inheriting from Event? Does this make it handy to perform event selection?
// Evt zonder inheritance maar grote call dan... Beter met? buildEvent op treeReader zou dan rechtstreeks hierop feeden...
// Anders 2 klassen voor event sel en event -> evt vervangen door event selection in FourTop klasse rechtstreeks te steken. <- this
// This structure will not work as it requires plugging this in TreeReader (instead of event as return). This makes it quite difficult
// Event provides plug for user specified object selections

class EventSelection4T {
    private:
        Event* event = nullptr;

        LeptonCollection* looseLeps;
        LeptonCollection* mediumLeps;

        JetCollection* jets;
        JetCollection* bTagJets;

        // Other doubles like number of leps etc
    public:

        EventSelection4T();
        ~EventSelection4T() {cleanSelection();};

        void cleanSelection();
        void addNewEvent(Event* newEvent);

        void objectSelection();
        bool passBaselineEventSelection();
        bool passFullEventSelection();
        bool passZBosonVeto();
        bool passLowMassVeto();
};

#endif
