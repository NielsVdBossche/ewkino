#include "../interface/FourTop.h"

void FourTop::initDdChargeMisID(double* corr) {
    std::string year = yearString;
    std::string objName;
    if (yearString == "2016PreVFP" || yearString == "2016PostVFP") {
        year = "2016";
        //objName = "chamidRate2016";
        // preUL: *corr = 0.966;
        *corr = 0.921;
    } else if (yearString == "2017") *corr = 1.46; // preUL 1.509
    else if (yearString == "2018") *corr = 1.451; // preUL 1.515

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
        returnWeight += histogram::contentAtValues(MisIDRates, el->pt(), el->absEta());
    }

    return returnWeight;
}

void FourTop::initFakerate() {
    std::string filename = "fakeRateMap_data_";
    std::string filenameYear = yearString;
    if (yearString == "2016PreVFP") {
        filenameYear = "2016Merged";
    } else if( yearString == "2016PostVFP" ){
        filenameYear = "2016Merged";
    } else if( yearString == "2017" ){
        //filename = "";
    } else {
        //filename = "";
    }

    std::string fullPathElectron = "DatadrivenInput/nonprompt/" + filename + "electron_" + filenameYear + "_mT.root";
    std::string fullPathMuon = "DatadrivenInput/nonprompt/" + filename + "muon_" + filenameYear + "_mT.root";


    TFile* elWeightFile = TFile::Open( fullPathElectron.c_str() );
    FakeRatesElectron = dynamic_cast< TH2D* >( elWeightFile->Get( ("fakeRate_electron_" + filenameYear).c_str()) );
    FakeRatesElectron->SetDirectory( gROOT );
    elWeightFile->Close();

    TFile* muWeightFile = TFile::Open( fullPathMuon.c_str() );
    FakeRatesMuon = dynamic_cast< TH2D* >( muWeightFile->Get( ("fakeRate_muon_" + filenameYear).c_str()) );
    FakeRatesMuon->SetDirectory( gROOT );
    muWeightFile->Close();
}

double FourTop::FakeRateWeight() {
    double weight = -1.;

    for (const auto &lep : currentEvent->lightLeptonCollection()) {
        if (!(lep->isFO() && !lep->isTight())) continue;
        double fr;
        double leppt = lep->pt();
        if (leppt >= 45.) leppt = 44.9;
        if (lep->isMuon()) {
            fr = histogram::contentAtValues(FakeRatesMuon, leppt, lep->absEta());
        } else {
            fr = histogram::contentAtValues(FakeRatesElectron, leppt, lep->absEta());
        }
        // calculate weight
        weight *= (-fr / (1. - fr));
    }
    
    return weight;
}