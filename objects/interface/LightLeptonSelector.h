#ifndef LightLeptonSelector_H
#define LightLeptonSelector_H

#include "LeptonSelector.h"
#include "LightLepton.h"

class LightLepton;

class LightLeptonSelector : public LeptonSelector {

    public: 
        LightLepton( const LightLepton& ) : lightLeptonPtr( &LightLepton ) {}
        
        virtual bool isLoose() const override;
        virtual bool isLooseV2() const override;
        virtual bool isGood() const override;
        virtual bool isTight() const override;

    private:
        LightLepton* lightLeptonPtr;

};


#endif 
