#ifndef LightLeptonSelector_H
#define LightLeptonSelector_H

#include "LeptonSelector.h"
#include "LightLepton.h"

class LightLepton;

class LightLeptonSelector : public LeptonSelector {

    public: 
        LightLepton( const LightLepton* const llPtr ) : lightLeptonPtr( llPtr ) {};
        
        virtual bool isLoose() const override;
        virtual bool isLooseV2() const override;
        virtual bool isGood() const override;
        virtual bool isTight() const override;

    private:
        const LightLepton* const lightLeptonPtr;

};


#endif 
