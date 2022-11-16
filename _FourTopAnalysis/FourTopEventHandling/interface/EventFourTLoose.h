#ifndef EVENTFOURTLOOSE
#define EVENTFOURTLOOSE

#include "EventFourT.h"

class EventFourTLoose : public EventFourT {
    private:
        
    public:
        EventFourTLoose() : EventFourT() {};
        ~EventFourTLoose() {};

        virtual void classifyEvent();
};

#endif