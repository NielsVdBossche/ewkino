#ifndef CORRECTION_CONCRETEJETSELECTION_H
#define CORRECTION_CONCRETEJETSELECTION_H

#include "JetSelectionHelper.h"

class BFlavorSelector : public JetSelectionHelper {
    public:
        virtual bool passSelection( const Jet& jet ) const override{ return jet.hadronFlavor() == 5; }
};

class CFlavorSelector : public JetSelectionHelper {
    public:
        virtual bool passSelection( const Jet& jet ) const override{ return jet.hadronFlavor() == 4; } // not sure if hadron flavor or the other thing. Check later
};

// add other selectors

#endif