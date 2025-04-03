/* 
=== b-tag reweighter ===
This reweighter accounts for a systematic difference between data and simulation 
with respect to b-tagging score.

The method relies both on b-tagging efficiencies and data-to-simulation scale factors.
-  The efficiencies should be measured by the user, by running the script
   weights/computeBTagSFCode/computeBTagEfficiencies.cc.
-  The scale factors are provided centrally. The scale factors are given in .csv format 
   and are situated in weights/weightFiles/bTagSF/ and the code to read them is provided 
   in weights/bTagSFCode/BTagCalibrationStandalone.cc

For more information: see https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods
The method implemented here corresponds to method "1a".
Note that this is in principle not enough if the b-tag shape information is used,
e.g. when using b-tagging scores in an MVA. 
In those cases, the ReweighterPUIDShape class should be used instead.
*/


#include "../interface/ReweighterPUID.h"
#include "../../Tools/interface/histogramTools.h"


ReweighterPUID::ReweighterPUID( const std::string& sfFilePath,
                                const std::string& year, 
                                const std::string& effFilePath)
    // constructor
    // input arguments:
    // - weightDirectory: path to the ewkino/weights folder
    // - sfFilePath: path to the scale factor csv file relative from ewkino/weights
    // - effFilePath: path to the efficiency root file relative from ewkino/weights
{
    TFile* sfWeightFile = TFile::Open( sfFilePath.c_str() );
    std::string formattedyear = year;
    if (year == "2016PreVFP") {
        formattedyear = "2016APV";
    } else if( year == "2016PostVFP" ){
        formattedyear = "2016";
    }
    scalefactor = std::shared_ptr< TH2 >( dynamic_cast< TH2* >( sfWeightFile->Get( ("h2_eff_sfUL" + formattedyear + "_L").c_str() ) ) );
    systunc = std::shared_ptr< TH2 >( dynamic_cast< TH2* >( sfWeightFile->Get( ("h2_eff_sfUL" + formattedyear + "_L_Systuncty").c_str() ) ) );
    scalefactor->SetDirectory( gROOT );
    systunc->SetDirectory(gROOT);
    // Loop the bins and set the errors to the bin content 
    for(int i = 0; i <= scalefactor->GetNbinsX()+1; ++i){
        for(int j = 0; j <= scalefactor->GetNbinsY()+1; ++j){
            scalefactor->SetBinError(i, j, systunc->GetBinContent(i,j));
        }
    }
    sfWeightFile->Close();

    TFile* effFile = TFile::Open( effFilePath.c_str() );
    puIDEfficiency = std::shared_ptr< TH2 >( dynamic_cast< TH2* >( effFile->Get( "puIDEff_LoosWP" ) ) );
    puIDEfficiency->SetDirectory( gROOT );
    effFile->Close();
}    


double ReweighterPUID::efficiencyMC( const Jet& jet) const {
    return histogram::contentAtValues( puIDEfficiency.get(), jet.pt(), jet.absEta() );
}


double ReweighterPUID::weight( const Jet& jet, double (&retrieveValue)( TH2*, const double, const double )) const{
    // calculate the weight (either central, up or down) for a single jet
    // the weight is calculated as follows:
    // - if the jet is a light jet and this reweighter is for heavy flavour, the weight is 1.
    // - if the jet is a b- or a c-jet and this reweighter is for light flavour, the weight is 1.
    // - if the jet is not in b-tag acceptance, the weight is 1.
    // - the correct scalefactor is read from the csv file.
    // - if the jet passes the given working point, weight = scalefactor.
    // - if not, weight = (1 - efficiency*scalefactor) / (1 - efficiency)

    // Jet should be good and PT in right range:
    if( !jet.isGood() ) return 1.;    // check if jet passes chosen b-tag working point
    if( jet.pt() < 25. || jet.pt() > 50 ) return 1.;   // check if jet pt is in the right range
    if (jet.genJetIdx() == -1) return 1.;    // check if jet is matched to a gen jet to be counted as hard scatter

    // get the scale factor for the jet
    double scaleFactor = retrieveValue(scalefactor.get(), jet.pt(), jet.absEta());
    if( jet.inBTagAcceptance() ){
        return scaleFactor;
    } else {
        double efficiency = efficiencyMC( jet );
        return ( 1. - efficiency * scaleFactor ) / ( 1. - efficiency );
    }    
}    


double ReweighterPUID::weight( const Jet& jet ) const{
    return weight( jet, histogram::contentAtValues );
}    


double ReweighterPUID::weightDown( const Jet& jet ) const{
    return weight( jet, histogram::contentDownAtValues );
}    


double ReweighterPUID::weightUp( const Jet& jet ) const{
    return weight( jet, histogram::contentUpAtValues );
}    


double ReweighterPUID::weight( const Event& event, double ( ReweighterPUID::*jetWeight )( const Jet& ) const ) const{
    // calculate the weight (central, up or down) for an event,                   
    // by multiplying those for individual jets in the event. 
    double weight = 1.;
    for( const auto& jetPtr : event.jetCollection().goodJetCollection() ){
        weight *= (this->*jetWeight)( *jetPtr );
    }
    return weight;
}    


double ReweighterPUID::weight( const Event& event ) const{
    return weight( event, &ReweighterPUID::weight );
}    


double ReweighterPUID::weightDown( const Event& event ) const{
    return weight( event, &ReweighterPUID::weightDown );
}    


double ReweighterPUID::weightUp( const Event& event ) const{
    return weight( event, &ReweighterPUID::weightUp );
}    

