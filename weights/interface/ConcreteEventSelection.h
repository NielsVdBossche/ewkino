#ifndef CONCRETEEVENTSELECTION_H
#define CONCRETEEVENTSELECTION_H

#include "EventSelectionHelper.h"

class TightEESelector : public EventSelectionHelper {
    public:
        virtual bool passSelection( const Event& event ) const override{
            if (event.numberOfTightLeptons() != 2 || event.numberOfFOLeptons() != event.numberOfTightLeptons()) return false;
            return (event.lepton(0).isElectron() && event.lepton(1).isElectron());
        }
};

class TightMuMuSelector : public EventSelectionHelper {
    public:
        virtual bool passSelection( const Event& event ) const override{
            if (event.numberOfTightLeptons() != 2 || event.numberOfFOLeptons() != event.numberOfTightLeptons()) return false;
            return (event.lepton(0).isMuon() && event.lepton(1).isMuon());
        }
};

class TightEMuSelector : public EventSelectionHelper {
    public:
        virtual bool passSelection( const Event& event ) const override{
            if (event.numberOfTightLeptons() != 2 || event.numberOfFOLeptons() != event.numberOfTightLeptons()) return false;
            return (event.lepton(0).isElectron() && event.lepton(1).isMuon());
        }
};

class TightMuESelector : public EventSelectionHelper {
    public:
        virtual bool passSelection( const Event& event ) const override{
            if (event.numberOfTightLeptons() != 2 || event.numberOfFOLeptons() != event.numberOfTightLeptons()) return false;
            return (event.lepton(0).isMuon() && event.lepton(1).isElectron());
        }
};

#endif