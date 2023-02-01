#ifndef JECWRAPPER_H
#define JECWRAPPER_H

// include jetmetobjects code
// provide basic functions to apply JECs
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// include other parts of framework
#include "../../Event/interface/Event.h"

#include "../../objects/interface/Jet.h"
#include "../../objects/interface/Met.h"

// include c++
#include <string>
#include <map>

class JECWrapper {
    public:
        JECWrapper(std::string& uncertaintyFile, std::vector<std::string>& jecSources);

        std::map<std::string, unsigned>& GetUncertaintyMapping() {return uncertaintyMapping;}

        JetCollection VariedJetCollection(unsigned variation, bool up, JetCollection& nominalJetCollection);
        Met VaryMET(unsigned variation, Met& nominalMet, JetCollection& nominalJetCollection, JetCollection& variedJetCollection);
        
    private:
        std::map<std::string, unsigned> uncertaintyMapping;
        std::map<unsigned, std::shared_ptr< JetCorrectionUncertainty>> jetCorrUncertainty;
};

#endif