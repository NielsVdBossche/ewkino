#ifndef JetCollection_H
#define JetCollection_H

//include c++ library classes 
#include <vector>
#include <memory>
#include <algorithm>

//include other parts of framework
#include "../../objects/interface/Jet.h"
#include "../../TreeReader/interface/TreeReader.h"
#include "PhysicsObjectCollection.h"
#include "../../objects/interface/Lepton.h"
//#include "LeptonCollection.h"


class LeptonCollection;

class JetCollection : public PhysicsObjectCollection< Jet > {

    public:
        JetCollection() = default;
        
        JetCollection( const TreeReader&, 
			const bool readAllJECVariations = false,
			const bool readGroupedJECVariations = false );

        //make jet collection with b-tagged jets 
        JetCollection looseBTagCollection() const;
        JetCollection mediumBTagCollection() const;
        JetCollection tightBTagCollection() const;

        //make varied collections of jets
        JetCollection JECDownCollection() const;
        JetCollection JECUpCollection() const;
        JetCollection JERDownCollection() const;
        JetCollection JERUpCollection() const;

        JetCollection JER_1p93_DownCollection() const;
        JetCollection JER_1p93_UpCollection() const;
        JetCollection JER_1p93_To_2p5_DownCollection() const;
        JetCollection JER_1p93_To_2p5_UpCollection() const;

	    JetCollection JECUpCollection( std::string source ) const;
	    JetCollection JECDownCollection( std::string source ) const;
        JetCollection JECGroupedUpCollection( unsigned source ) const;
        JetCollection JECGroupedDownCollection( unsigned source ) const;
	    JetCollection getVariedJetCollection( const std::string& variation ) const;
        JetCollection getVariedJetCollection( unsigned source, bool isUp, bool isGrouped) const;
        #if JECONRUNTIME
        JetCollection JECCustomCollection(std::shared_ptr<JECWrapper> jecwrapper, unsigned id, bool isUp) const;
        JetCollection JECCustomGoodCollection(std::shared_ptr<JECWrapper> jecwrapper, unsigned id, bool isUp) const;
        #endif

        JetCollection JECGroupedFlavorQCD(unsigned sourceID, unsigned flavor, bool up) const;
        JetCollection JECUpGroupedFlavorQCD(unsigned sourceID, unsigned flavor) const;
        JetCollection JECDownGroupedFlavorQCD(unsigned sourceID, unsigned flavor) const;

        JetCollection JECGroupedFlavorQCD(unsigned flavor, bool up) const;
        JetCollection JECUpGroupedFlavorQCD(unsigned flavor) const;
        JetCollection JECDownGroupedFlavorQCD(unsigned flavor) const;

        JetCollection HEMIssue() const;


        //select jets
        void selectGoodJets();
        JetCollection goodJetCollection() const;
        void selectGoodAnyVariationJets();
        JetCollection goodAnyVariationJetCollection() const;

        //count jets passing criteria
        size_type numberOfLooseBTaggedJets() const;
        size_type numberOfMediumBTaggedJets() const;
        size_type numberOfTightBTaggedJets() const;
        size_type numberOfGoodJets() const;
        size_type numberOfGoodAnyVariationJets() const;

        size_type minNumberOfLooseBTaggedJetsAnyVariation() const;
        size_type maxNumberOfLooseBTaggedJetsAnyVariation() const;
        size_type minNumberOfMediumBTaggedJetsAnyVariation() const;
        size_type maxNumberOfMediumBTaggedJetsAnyVariation() const;
        size_type minNumberOfTightBTaggedJetsAnyVariation() const;
        size_type maxNumberOfTightBTaggedJetsAnyVariation() const;

        //clean jets 
        void cleanJetsFromLooseLeptons( const LeptonCollection&, const double coneSize = 0.4 );
        void cleanJetsFromFOLeptons( const LeptonCollection&, const double coneSize = 0.4 );
        void cleanJetsFromTightLeptons( const LeptonCollection&, const double coneSize = 0.4 );

    
        JetCollection( const std::vector< std::shared_ptr< Jet > >& jetVector ) : PhysicsObjectCollection< Jet >( jetVector ) {}
    private:
        
        //clean jets 
        void cleanJetsFromLeptons(const LeptonCollection&, bool (Lepton::*passSelection)() const, const double coneSize );

        //build JetCollection of jets satisfying a certain requirement
        JetCollection buildSubCollection( bool (Jet::*passSelection)() const ) const;
    
        //build JetCollection of varied Jets
        JetCollection buildVariedCollection( Jet (Jet::*variedJet)() const ) const;
	    JetCollection buildVariedCollection( Jet (Jet::*variedJet)(std::string) const, std::string ) const;
	    JetCollection buildVariedCollection( Jet (Jet::*variedJet)(const unsigned) const, unsigned variationArg ) const;
        JetCollection buildVariedCollection_FlavorSet( Jet (Jet::*variedJet)(const unsigned) const, unsigned, unsigned ) const;
        JetCollection buildVariedCollection_FlavorSet( Jet (Jet::*variedJet)(std::string) const, std::string, unsigned ) const;


        //number of b-taged jets with variation
        std::vector< size_type > countsAnyVariation( bool ( Jet::*passSelection )() const ) const;
        size_type minCountAnyVariation( bool ( Jet::*passSelection )() const ) const;
        size_type maxCountAnyVariation( bool ( Jet::*passSelection )() const ) const;
};

#endif 
