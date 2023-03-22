#ifndef EVENTSELECTIONHELPER_H
#define EVENTSELECTIONHELPER_H

#include "../../Event/interface/Event.h"

class EventSelectionHelper{
    
    public:
        virtual bool passSelection( const Event& ) const = 0;
};


#endif