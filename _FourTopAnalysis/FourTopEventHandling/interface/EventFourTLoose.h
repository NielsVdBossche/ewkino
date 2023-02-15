#include "EventFourT.h"

class EventFourTLoose : public EventFourT {
    private:
        
    public:
        EventFourTLoose(std::string uncertaintyFile) : EventFourT(uncertaintyFile) {};
        ~EventFourTLoose() {};

        virtual void classifyEvent();
};
