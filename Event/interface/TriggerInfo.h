#ifndef TriggerInfo_H
#define TriggerInfo_H

// include c++ library classes
#include <iostream>
#include <map>
#include <string>

//include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"


class TriggerInfo{

    public:

        // WARNING : turning on 'readIndividualTriggers' and/or 'readIndividualMETFilters' is relatively slow;
	// better to disable if not really needed.
        TriggerInfo( const TreeReader&, 
	    const bool readIndividualTriggers = false,
	    const bool readIndividualMetFilters = false,
	    const bool makeCompositeTriggers = true,
	    const bool makeCompositeMetFilters = true );

        bool passTriggers_e() const{ return _passTriggers_e; }
        bool passTriggers_m() const{ return _passTriggers_m; }
        bool passTriggers_ee() const{ return _passTriggers_ee; }
        bool passTriggers_em() const{ return _passTriggers_em; }
        bool passTriggers_et() const{ return _passTriggers_et; }
        bool passTriggers_mm() const{ return _passTriggers_mm; }
        bool passTriggers_mt() const{ return _passTriggers_mt; }
        bool passTriggers_eee() const{ return _passTriggers_eee; }
        bool passTriggers_eem() const{ return _passTriggers_eem; }
        bool passTriggers_emm() const{ return _passTriggers_emm; }
        bool passTriggers_mmm() const{ return _passTriggers_mmm; }
        bool passTriggers_FR() const{ return _passTriggers_FR; }
        bool passTriggers_FR_iso() const{ return _passTriggers_FR_iso; }
	bool passTriggers_ref() const{ return _passTriggers_ref; }
        bool passMetFilters() const{ return _passMetFilters; }
        bool passTrigger( const std::string& ) const;
        bool passMetFilter( const std::string& ) const;

	bool containsIndividualTriggers() const{ return _containsIndividualTriggers; }
	bool containsIndividualMetFilters() const{ return _containsIndividualMetFilters; }
        void printAvailableIndividualTriggers() const;
        void printAvailableMetFilters() const;
	bool containsCompositeTriggers() const{ return _containsCompositeTriggers; }
	bool containsCompositeMetFilters() const{ return _containsCompositeMetFilters; }

    private:
        bool _passTriggers_e = false;
        bool _passTriggers_m = false;
        bool _passTriggers_ee = false;
        bool _passTriggers_em = false;
        bool _passTriggers_et = false;
        bool _passTriggers_mm = false;
        bool _passTriggers_mt = false;
        bool _passTriggers_eee = false;
        bool _passTriggers_eem = false;
        bool _passTriggers_emm = false;
        bool _passTriggers_mmm = false;
        bool _passTriggers_FR = false;
        bool _passTriggers_FR_iso =false ;
	bool _passTriggers_ref = false;
        bool _passMetFilters = false;
	bool _containsIndividualTriggers = false;
	bool _containsIndividualMetFilters = false;
	bool _containsCompositeTriggers = false;
	bool _containsCompositeMetFilters = false;
        std::map< std::string, bool > individualTriggerMap; 
        std::map< std::string, bool > individualMetFilterMap; 
};

#endif 
