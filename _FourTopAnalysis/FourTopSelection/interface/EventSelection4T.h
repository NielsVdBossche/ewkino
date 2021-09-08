#ifndef eventselection4T_h
#define eventselection4T_h

#include "../../../Event/interface/Event.h"

#include "../../../Event/interface/ElectronCollection.h"
#include "../../../Event/interface/MuonCollection.h"
#include "../../../Event/interface/JetCollection.h"

#include "objectSelection4T.h"


// Class eventSelection inheriting from Event? Does this make it handy to perform event selection?
// Evt zonder inheritance maar grote call dan... Beter met? buildEvent op treeReader zou dan rechtstreeks hierop feeden...
// Anders 2 klassen voor event sel en event -> evt vervangen door event selection in FourTop klasse rechtstreeks te steken. <- this
// This structure will not work as it requires plugging this in TreeReader (instead of event as return). This makes it quite difficult
// Event provides plug for user specified object selections

namespace EventSelection4T {
    void applyBaselineObjectSelection(Event* event);
    void applyFullObjectSelection(Event* event);
    bool passBaselineEventSelection(Event* event);
    bool passFullEventSelection(Event* event);
    bool passZBosonVeto(Event* event);
    bool passLowMassVeto(Event* event);
}

/*
namespace FourTopSelection
{
    bool baselineSelection(); // Prob event object passen
    bool passZBosonVeto();
    bool passLowMassVeto(); // mss deze twee samen brengen in 1 veto? Idk
    // Overlap??
    
} // namespace FourTop
*/
#endif
