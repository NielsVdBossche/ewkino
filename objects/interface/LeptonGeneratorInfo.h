#ifndef LeptonGeneratorInfo_H
#define LeptonGeneratorInfo_H

/*
Class that collects the generator level information for a given reconstructed lepton
*/

// include C++ libraries
#include <bitset>

//include other parts of code 
#include "../../TreeReader/interface/TreeReader.h"

class LeptonGeneratorInfo{
    
    public:
        LeptonGeneratorInfo( const TreeReader&, const std::string&, const unsigned ); 

        bool isPrompt() const{ return _isPrompt; }
        int matchPdgId() const{ return _matchPdgId; }
        int matchCharge() const{ return _matchCharge; }
        int momPdgId() const{ return _momPdgId; }
        unsigned provenance() const{ return _provenance; }
        unsigned provenanceCompressed() const{ return _provenanceCompressed; }
        unsigned provenanceConversion() const{ return _provenanceConversion; }

    private:
        bool _isPrompt;
        int _matchPdgId, _matchCharge, _momPdgId;
        unsigned _provenance, _provenanceCompressed, _provenanceConversion;

}; 
#endif 
