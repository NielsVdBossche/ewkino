#ifndef JECWRAPPER_H
#define JECWRAPPER_H

// include jetmetobjects code
// provide basic functions to apply JECs
#include "../CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "../CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// include other parts of framework
class Event;
class JetCollection;
class Jet;
class Met;

// include c++
#include <string>
#include <map>

class JECWrapper {
    public:
        JECWrapper(std::string& uncertaintyFile, std::vector<std::string>& jecSources);

        std::map<std::string, unsigned>& GetUncertaintyMapping() {return uncertaintyMapping;}
        std::shared_ptr< JetCorrectorParameters> GetJetCorrectionUncertainty(unsigned id) {return jetCorrUncertainty[id];};

        std::pair<JetCollection, Met> VaryJetsAndMet(Event& event, unsigned id, bool isUp);
        
    private:
        std::map<std::string, unsigned> uncertaintyMapping;
        std::map<unsigned, std::shared_ptr< JetCorrectorParameters>> jetCorrUncertainty;
};

#endif