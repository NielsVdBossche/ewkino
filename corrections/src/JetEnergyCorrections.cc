#include "../interface/JetEnergyCorrections.h"

#include <iostream>

JetEnergyCorrections::JetEnergyCorrections(std::string& corrFile, std::string& year) {
    jercSet = correction::CorrectionSet::from_file(corrFile);
    // load, the double return value is:
    //double val = jercSet->at("Summer19UL18_V5_MC_AbsoluteScale_AK4PFchs")->evaluate({1.0, 20.});
    //std::cout << val << std::endl;

    // Get JER variations
    // JERs rel stuff:
    // - Summer19UL18_JRV2_MC_PtResolution_AK4PFchs
    // - Summer19UL18_JRV2_MC_ScaleFactor_AK4PFchs
    // usage: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures
    // need to do gen matching basically with few requirements. Might be included in Luka's processing

    // Get a variable in constructor to decide which set of JEC to load, but should be straight forward
    // JECs itself already applied, but need to apply variations manually. I guess each correction is just an absolute pt difference to add and subtract

    // get year, fixed tag, add the end add AK4PFchs

    if (year == "2018") {
        base_corr = "Summer19UL18_V5_MC";
    } else if (year == "2017") {
        base_corr = "Summer19UL17_V5_MC";
    } else if (year == "2016PostVFP") {
        base_corr = "Summer19UL16_V7_MC";
    } else if (year == "2016PreVFP") {
        base_corr = "Summer19UL16APV_V7_MC";
    }
}

double JetEnergyCorrections::TestCorrection(std::string set, double eta, double pt) {
    return jercSet->at(set)->evaluate({eta, pt});
}
