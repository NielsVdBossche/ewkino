#ifndef LeptonGeneratorInfo_H
#define LeptonGeneratorInfo_H

/*
Class that collects the generator level information for a given reconstructed lepton
*/
#include <bitset>

//include other parts of code 
#include "../../TreeReader/interface/TreeReader.h"
#include "../../TreeReader/interface/NanoReader.h"

class LeptonGeneratorInfo{
    
    public:
        LeptonGeneratorInfo( const TreeReader&, const unsigned ); 
        LeptonGeneratorInfo( const NanoReader::LeptonReader&, const unsigned );
        LeptonGeneratorInfo( const NanoReader::LeptonReader&, const unsigned, bool, bool, bool ); 

        bool hasGenMatch() const{ return _hasGenMatch; }
        bool isPrompt() const{ return _isPrompt; }
        int matchPdgId() const{ return _matchPdgId; }
        int matchCharge() const{ return _matchCharge; }
        int momPdgId() const{ return _momPdgId; }
        unsigned provenance() const{ return _provenance; }
        unsigned provenanceCompressed() const{ return _provenanceCompressed; }
        unsigned provenanceConversion() const{ return _provenanceConversion; }

    private:
        bool _isPrompt = false;
        bool _hasGenMatch = false;
        int _matchPdgId = 0;
        int _matchCharge = 0;
        int _momPdgId = 0;
        unsigned _provenance = 0;
        unsigned _provenanceCompressed = 0;
        unsigned _provenanceConversion = 99;
}; 
#endif 
