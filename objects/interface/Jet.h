#ifndef Jet_H
#define Jet_H

//include other parts of code 
#include "PhysicsObject.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Tools/interface/stringTools.h"
#include "../../CMSSW_imports/interface/JECWrapper.h"
//#include "JetSelector.h"


template< typename objectType > class PhysicsObjectCollection;
class JetSelector;

class Jet : public PhysicsObject{
    
    friend class PhysicsObjectCollection< Jet >;

    public:
        Jet( const TreeReader&, const unsigned,
		const bool readAllJECVariations, const bool readGroupedJECVariations);

        Jet( const Jet& );
        Jet( Jet&& ) noexcept;

        ~Jet();

        Jet& operator=( const Jet& );
        Jet& operator=( Jet&& ) noexcept;

        double deepCSV() const{ return _deepCSV; }
        double deepFlavor() const{ return _deepFlavor; }
        unsigned hadronFlavor() const{ return _hadronFlavor; }
        bool isLoose() const{ return _isLoose; }
        bool isTight() const{ return _isTight; }
        bool isTightLeptonVeto() const{ return _isTightLeptonVeto; }

        //analysis-specific jet selection
        virtual bool isGood() const override;
        bool inBTagAcceptance() const;
        bool isBTaggedLoose() const;
        bool isBTaggedMedium() const;
        bool isBTaggedTight() const;
        bool isBTaggedLooseAnyVariation() const;
        bool isBTaggedMediumAnyVariation() const;
        bool isBTaggedTightAnyVariation() const;

        //create new Jet with JEC varied within uncertainties
        Jet JetJECDown() const;
        Jet JetJECUp() const;
        Jet JetJERDown() const;
        Jet JetJERUp() const;

        Jet JetJER_1p93_Down() const;
        Jet JetJER_1p93_Up() const;
        Jet JetJER_1p93_To_2p5_Down() const;
        Jet JetJER_1p93_To_2p5_Up() const;

        // create new Jet with JEC varied within uncertainties, split per source
        Jet JetJECUp( const std::string source ) const;
        Jet JetJECDown( const std::string source ) const;
        Jet JetJECGroupedDown( const unsigned source_id ) const;
        Jet JetJECGroupedUp( const unsigned source_id ) const;
        Jet JetJECSourcesDown( const unsigned source_id ) const;
        Jet JetJECSourcesUp( const unsigned source_id ) const;
        Jet JetJECUp
        Jet HEMIssue() const;

        //check if any of the jet variations passes the selection
        bool isGoodAnyVariation() const;

        //print jet information
        virtual std::ostream& print( std::ostream& ) const override;

        unsigned jetHadronFlavor() {return _hadronFlavor;}

    private:
        double _deepCSV = 0;
        double _deepFlavor = 0;
        bool _isLoose = false;
        bool _isTight = false;
        bool _isTightLeptonVeto = false;
        unsigned _hadronFlavor = 0;
        
        //JEC uncertainties 
        double _pt_JECDown = 0;
        double _pt_JECUp = 0;
        double _pt_JERDown = 0;
        double _pt_JERUp = 0;

        std::vector< double > _pt_JECSourcesUp;
        std::vector< double > _pt_JECSourcesDown;
        std::vector< double > _pt_JECGroupedUp;
        std::vector< double > _pt_JECGroupedDown;

        std::map<std::string, size_t >* _JECSources_Ids = nullptr;
        std::map<std::string, size_t >* _JECGrouped_Ids = nullptr;

        //jet selector 
        JetSelector* selector;

        Jet variedJet(const double) const;

        Jet* clone() const & { return new Jet(*this); }
        Jet* clone() && { return new Jet( std::move( *this ) ); }

        void copyNonPointerAttributes( const Jet& );
        bool isBTaggedAnyVariation( bool ( Jet::*isBTaggedWP )() const ) const;
};

#endif
