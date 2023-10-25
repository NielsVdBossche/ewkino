#ifndef CORRECTION_JETSELECTIONHELPER_H
#define CORRECTION_JETSELECTIONHELPER_H

#include "../../objects/interface/Jet.h"

class JetSelectionHelper {
    
    public:
        virtual bool passSelection( const Jet& ) const = 0;
};


#endif
