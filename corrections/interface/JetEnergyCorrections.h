#ifndef JETENERGYCORRECTIONS_H
#define JETENERGYCORRECTIONS_H

#include <correction.h>
#include "../../objects/interface/Jet.h"

class JetEnergyCorrections {
    public:
        JetEnergyCorrections(std::string& corrFile, std::string& year);
        ~JetEnergyCorrections();

        double TestCorrection(std::string set, double eta, double pt);
    private:
        std::unique_ptr<correction::CorrectionSet> jercSet;
        std::string base_corr;
        std::shared_ptr<const correction::Correction> jerCorrection;
        // idk what to do with all jec variations
        // std::shared_ptr<const correction::Correction> jerCorrection;

        // get set of uncertainties to load, assign to number, just use that to load the shit?
        // but need some subclass to filter?

};

// this class is fine for general point, but might now move towards class using Jet Selectors to select the right jet and apply corrections to both Jets and mets given a jet collection and the met variable
// CorrectJets method to first create new jetcollection step by step while stuff is passed? should be possible in some form? Or just create vector then jetcollection out of vector might be easier
// can link that shit to this class using a collection, such that overarching is possible. Should it immediately create up and down vars or just use bool?

#endif