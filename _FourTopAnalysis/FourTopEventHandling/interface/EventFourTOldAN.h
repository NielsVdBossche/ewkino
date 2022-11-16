#ifndef EVENTFOURTOLDAN
#define EVENTFOURTOLDAN

#include "EventFourT.h"

class EventFourTOldAN : public EventFourT {
    private:
        
    public:
        EventFourTOldAN() : EventFourT() {};
        ~EventFourTOldAN() {};

        virtual void classifyEvent();
};

#endif
