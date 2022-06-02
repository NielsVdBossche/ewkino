#include "../interface/ReweighterSample.h"

//include other parts of framework
#include "../../../Tools/interface/stringTools.h"
#include "../../../Tools/interface/systemTools.h"
#include "../../../Tools/interface/histogramTools.h"

ReweighterSample::ReweighterSample( const std::string& filterName, const std::string& weightDirectory ) : filterName(filterName) {
    // load njet distribution
    // could put a sample requirement on here? maybe less computations... Or just check it at analysis level which sample it is and directly creating the samplereweighter there
    std::string filepath = ( stringTools::formatDirectoryName( weightDirectory ) + filterName + ".root" );
    if( !systemTools::fileExists( filepath ) ){
        throw std::runtime_error( "File " + filepath + " with njets SF for process" + filterName + ", necessary for reweighting, is not present." );
    }

    TFile* njetReweightingPtr = TFile::Open( filepath.c_str() );
    std::shared_ptr< TH1 > nJetDistribution = std::shared_ptr< TH1 >( dynamic_cast< TH1* >( njetReweightingPtr->Get( "SF" ) ) ); /// fix 
    nJetDistribution->SetDirectory( gROOT ); 
    njetReweightingPtr->Close();
}

double ReweighterSample::weight(const Event& event, const int njet, double (&retrieveValue)( TH1*, const double ) ) const {
    std::string sampleName = event.sample().fileName();
    if (stringTools::stringStartsWith(sampleName, filterName)) {
        return retrieveValue( nJetDistribution.get(), njet);
    } else {
        return 1.; 
    }
}

double ReweighterSample::weight(const Event& event, const int njet) const{
    return weight(event, njet, histogram::contentAtValue);
}


double ReweighterSample::weightDown(const Event& event, const int njet) const{
    return weight(event, njet, histogram::contentDownAtValue);
}


double ReweighterSample::weightUp(const Event& event, const int njet) const{
    return weight(event, njet, histogram::contentUpAtValue);
}
