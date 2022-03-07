#include "../interface/FourTop.h"

void FourTop::initDdChargeMisID(double* corr) {
    std::string year = yearString;
    std::string objName;
    if (yearString == "2016PreVFP" || yearString == "2016PostVFP") {
        year = "2016";
        //objName = "chamidRate2016";
        *corr = 0.966;
    } else if (yearString == "2017") *corr = 1.509;
    else if (yearString == "2018") *corr = 1.515;

    std::string fullPath = "DatadrivenInput/charge/chamid_" + year + "_DYpile_onZ-nLep2-lepsPrompt_ptEtaNEWbinning.root";

    TFile* weightFilePtr = TFile::Open( fullPath.c_str() );

    MisIDRates = dynamic_cast< TH2D* >( weightFilePtr->Get( ("chamidRate" + year).c_str() ) );
    MisIDRates->SetDirectory( gROOT );

    weightFilePtr->Close();
}

double FourTop::ChmisIDWeight() {
    ElectronCollection* electrons = selection->getMediumLepCol()->electronCollectionPtr();
    double returnWeight = 0.;
    for (auto el : *electrons) {
        returnWeight += histogram::contentAtValues(MisIDRates, el->pt(), el->eta());
    }

    return returnWeight;
}

void FourTop::initFakerate() {
    std::string filename;

    if (yearString == "2016PreVFP") {
        filename = "";
    } else if( yearString == "2016PostVFP" ){
        filename= "";
    } else if( yearString == "2017" ){
        filename = "";
    } else {
        filename = "";
    }

    std::string fullPath = "DatadrivenInput/nonprompt/" + filename;

    TFile* weightFilePtr = nullptr; // TFile::Open( fullPath.c_str() );

    //FakeRates = dynamic_cast< TH2D* >( weightFilePtr->Get( "nominal" ) );
    //FakeRates->SetDirectory( gROOT );
//
    //weightFilePtr->Close();
}
