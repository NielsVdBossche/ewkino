#ifndef Lepton_H
#define Lepton_H

//include c++ library classes 
#include <utility>
#include <memory>

//include other parts of code 
#include "PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"
#include "LeptonGeneratorInfo.h"
#include "LeptonSelector.h"

//class LeptonSelector;
template< typename ObjectType > class PhysicsObjectCollection;

class Lepton : public PhysicsObject {
    
    // note: the LeptonCollection class represents a collection of leptons. 
    //       its base (PhysicsObjectCollection<Lepton>) should be a friend to be able to use clone().
    friend class PhysicsObjectCollection<Lepton>;
    
    public: 
        Lepton( const TreeReader&, const unsigned, LeptonSelector* ); 
        Lepton( const NanoReader::LeptonReader&, const unsigned, LeptonSelector* ); 

        //copying of lepton will only be allowed if appropriate selector is provided by derived class
        Lepton( const Lepton& ) = delete;
        Lepton( Lepton&& ) noexcept = delete;

        virtual ~Lepton();
        
        Lepton& operator=( const Lepton& );
        Lepton& operator=( Lepton&& ) noexcept;

        //get reconstructed lepton attributes 
        double dxy() const{ return _dxy; }
        double dz() const{ return _dz; }
        int charge() const{ return _charge; }

        //access generator-level attributes
        bool hasGenMatch() const;
        bool isPrompt() const;
        int matchPdgId() const;
        int matchCharge() const;
        int momPdgId() const;
        unsigned provenance() const;
        unsigned provenanceCompressed() const;
        unsigned provenanceConversion() const;
        bool isChargeFlip() const{ return ( charge() == -1 * matchCharge() ); }
        bool isChargeMisMatch() const{ return ( charge() != matchCharge() ); }


        //lepton id decisions 
        bool isLoose() const{ return selector->isLoose(); }
        bool isLooseV2() const{ return selector->isLooseV2(); }
        bool isFO() const{ return selector->isFO(); }
        bool isTight() const{ return selector->isTight(); }
        bool isTightCharge() const {return selector->isTightCharge();}
        virtual bool isGood() const override{ return isFO(); }

        //cone-correction for fake-rate prediction
        void applyConeCorrection();
        
        //pT before cone correction
        double uncorrectedPt() const{ return _uncorrectedPt; }
        double uncorrectedE() const{ return _uncorrectedE; }

        //check what type of lepton the object is
        virtual bool isMuon() const = 0;
        virtual bool isElectron() const = 0;
        virtual bool isTau() const = 0;
        virtual bool isLightLepton() const = 0;

        //print out lepton information
        virtual std::ostream& print( std::ostream& os = std::cout ) const override;

    private:
        int _charge = 0;
    
        //lepton impact parameter variables 
        double _dxy = 0;
        double _dz = 0;

        //lepton generator information
        LeptonGeneratorInfo* generatorInfo = nullptr;

        //check whether generator-level info was initialized 
        bool hasGeneratorInfo() const{ return  generatorInfo != nullptr; }
        bool checkGeneratorInfo() const;

        //copy non-pointer attributes from other leptons, to be used in copy operations
        void copyNonPointerAttributes( const Lepton& );

        //functions to facilitate dynamically typed dynamic memory allocation 
        virtual Lepton* clone() const & = 0;
        virtual Lepton* clone() && = 0;

        //lepton selector object 
        LeptonSelector* selector;

        //check if lepton was already cone-corrected
        bool isConeCorrected = false;

    protected :

        //pT before cone correction
        double _uncorrectedPt;
        double _uncorrectedE;

        Lepton( const Lepton&, LeptonSelector* );
        Lepton( Lepton&&, LeptonSelector* ) noexcept;
};

//check whether two leptons have the same flavor
bool sameFlavor( const Lepton&, const Lepton& );
bool oppositeSignSameFlavor( const Lepton&, const Lepton& );

#endif
