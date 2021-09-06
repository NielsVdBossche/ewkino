#ifndef eventselection4T_h
#define eventselection4T_h

#include "../../../Event/interface/Event.h"
// Class eventSelection inheriting from Event? Does this make it handy to perform event selection?
// Evt zonder inheritance maar grote call dan... Beter met? buildEvent op treeReader zou dan rechtstreeks hierop feeden...
// Anders 2 klassen voor event sel en event -> evt vervangen door event selection in FourTop klasse rechtstreeks te steken.

class EventSelection4T : public Event {


};

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
