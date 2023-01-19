#include "../interface/ConcreteReweighterFactory.h"

//include ROOT classes
#include "TFile.h"
#include "TH2.h"
#include "TCanvas.h"

//include other parts of framework
#include "../../Tools/interface/analysisTools.h"
#include "../../Tools/interface/stringTools.h"
#include "../../Tools/interface/systemTools.h"

#include "../interface/ConcreteLeptonReweighter.h"
#include "../interface/ConcreteReweighterLeptons.h"
#include "../interface/ConcreteSelection.h"
#include "../interface/ReweighterPileup.h"
#include "../interface/ConcreteReweighterBTag.h"
#include "../interface/ReweighterPrefire.h"
#include "../interface/ReweighterEmpty.h"
#include "../interface/ReweighterBTagShape.h"
#include "../interface/ReweighterTriggerSF.h"
#include "../interface/ConcreteEventSelection.h"

// --------------------------------------
// empty reweighter for testing purposes 
// --------------------------------------

CombinedReweighter EmptyReweighterFactory::buildReweighter(
        const std::string& weightDirectory,
        const std::string& year,
        const std::vector< Sample >& samples ) const{
 
    // reweighter to return
    CombinedReweighter combinedReweighter;
    // dummy condition on args to avoid compilation warnings
    if(weightDirectory=="" && year=="" && samples.size()==0) return combinedReweighter;

    // add all reweighters needed formally, but each reweighter returns unity for each event
    combinedReweighter.addReweighter( "muonID", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "muonIDSyst", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "muonIDStat", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "electronID", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "electronIDSyst", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "electronIDStat", std::make_shared< ReweighterEmpty >() );
    if( year == "2016" || year == "2017" ){
	combinedReweighter.addReweighter( "electronReco_pTBelow20",
        std::make_shared< ReweighterEmpty >() );
        combinedReweighter.addReweighter( "electronReco_pTAbove20",
        std::make_shared< ReweighterEmpty >() );
    } else if( year == "2018" ){
	combinedReweighter.addReweighter( "electronReco",
	std::make_shared< ReweighterEmpty >() );
    }
    combinedReweighter.addReweighter( "pileup", std::make_shared<ReweighterEmpty>() );
    combinedReweighter.addReweighter( "bTag_heavy", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "bTag_light", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "bTag_shape", std::make_shared< ReweighterEmpty >() );
    combinedReweighter.addReweighter( "prefire", std::make_shared< ReweighterEmpty >() );

    return combinedReweighter;
}

// -------------------------------
// reweighter for ewkino analysis 
// -------------------------------

CombinedReweighter EwkinoReweighterFactory::buildReweighter( 
	const std::string& weightDirectory, 
	const std::string& year, 
	const std::vector< Sample >& samples ) const{

    analysisTools::checkYearString( year );

    //reweighter to return
    CombinedReweighter combinedReweighter;

    //make muon ID Reweighter
    // note: these files are not present in the repository, 
    // replace by code below in order for the reweighter to work!
    /*TFile* muonSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/leptonSF_m_" + year + "_3lTight.root" ).c_str() );
    std::shared_ptr< TH2 > muonSFHist( dynamic_cast< TH2* >( muonSFFile->Get( "SFglobal" ) ) ); */
    TFile* muonSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/looseToTight_" + year + "_m_3l.root" ).c_str() );
    std::shared_ptr< TH2 > muonSFHist( dynamic_cast< TH2* >( muonSFFile->Get( "EGamma_SF2D" ) ) );
    muonSFHist->SetDirectory( gROOT );
    muonSFFile->Close();

    MuonReweighter muonReweighter( muonSFHist, new TightSelector );
    combinedReweighter.addReweighter( "muonID", std::make_shared< ReweighterMuons >( muonReweighter ) );

    //make electron ID Reweighter
    // note: these files are not present in the repository, 
    // replace by code below in order for the reweighter to work!
    /*TFile* eleSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/leptonSF_e_" + year + "_3lTight.root" ).c_str() );
    std::shared_ptr< TH2 > electronSFHist( dynamic_cast< TH2* >( eleSFFile->Get( "SFglobal" ) ) ); */
    TFile* eleSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/looseToTight_" + year + "_e_3l.root" ).c_str() );
    std::shared_ptr< TH2 > electronSFHist( dynamic_cast< TH2* >( eleSFFile->Get( "EGamma_SF2D" ) ) );
    electronSFHist->SetDirectory( gROOT );
    eleSFFile->Close();

    ElectronIDReweighter electronIDReweighter( electronSFHist, new TightSelector );
    combinedReweighter.addReweighter( "electronID", std::make_shared< ReweighterElectronsID >( electronIDReweighter ) );

    //make electron Reconstruction Reweighter
    if( year == "2016" || year == "2017" ){

        //pT below 20 GeV
        TFile* eleRecoSFFile_pTBelow20 = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/egamma_recoEff_" + year + "_pTBelow20.root" ).c_str() );
        std::shared_ptr< TH2 > electronRecoSFHist_pTBelow20( dynamic_cast< TH2* >( eleRecoSFFile_pTBelow20->Get( "EGamma_SF2D" ) ) );
        electronRecoSFHist_pTBelow20->SetDirectory( gROOT );
        eleRecoSFFile_pTBelow20->Close();

        ElectronIDReweighter electronRecoReweighter_pTBelow20( electronRecoSFHist_pTBelow20, new LooseMaxPtSelector< 20 > );
        combinedReweighter.addReweighter( "electronReco_pTBelow20", std::make_shared< ReweighterElectronsID >( electronRecoReweighter_pTBelow20 ) );

        //pT above 20 GeV
        TFile* eleRecoSFFile_pTAbove20 = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/egamma_recoEff_" + year + "_pTAbove20.root" ).c_str() );
        std::shared_ptr< TH2 > electronRecoSFHist_pTAbove20( dynamic_cast< TH2* >( eleRecoSFFile_pTAbove20->Get( "EGamma_SF2D" ) ) );
        electronRecoSFHist_pTAbove20->SetDirectory( gROOT );
        eleRecoSFFile_pTAbove20->Close();

        ElectronIDReweighter electronRecoReweighter_pTAbove20( electronRecoSFHist_pTAbove20, new LooseMinPtSelector< 20 > );
        combinedReweighter.addReweighter( "electronReco_pTAbove20", std::make_shared< ReweighterElectronsID >( electronRecoReweighter_pTAbove20 ) );

    } else if( year == "2018" ){

        //inclusive pT 
        TFile* eleRecoSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/egamma_recoEff_" + year + ".root" ).c_str() );
        std::shared_ptr< TH2 > electronRecoSFHist ( dynamic_cast< TH2* >( eleRecoSFFile->Get( "EGamma_SF2D" ) ) );
        electronRecoSFHist->SetDirectory( gROOT );
        eleRecoSFFile->Close();

        ElectronIDReweighter electronRecoReweighter( electronRecoSFHist, new LooseSelector );
        combinedReweighter.addReweighter( "electronReco", std::make_shared< ReweighterElectronsID >( electronRecoReweighter ) );

    }
    
    //make pileup Reweighter
    combinedReweighter.addReweighter( "pileup", std::make_shared< ReweighterPileup >( samples, weightDirectory ) );
    
    //make b-tagging Reweighter 
    const std::string& bTagWP = "tight";

    //read MC b-tagging efficiency histograms
    const std::string& leptonCleaning = "looseLeptonCleaned";
    TFile* bTagEffMCFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/bTagEff/bTagEff_" + leptonCleaning + "_" + year + ".root" ).c_str() );
    std::shared_ptr< TH2 > bTagEffMCHist_udsg( dynamic_cast< TH2* >( bTagEffMCFile->Get( ( "bTagEff_" + bTagWP + "_udsg" ).c_str() ) ) );
    bTagEffMCHist_udsg->SetDirectory( gROOT );
    std::shared_ptr< TH2 > bTagEffMCHist_c( dynamic_cast< TH2* >( bTagEffMCFile->Get( ( "bTagEff_" + bTagWP + "_charm" ).c_str() ) ) );
    bTagEffMCHist_c->SetDirectory( gROOT );
    std::shared_ptr< TH2 > bTagEffMCHist_b( dynamic_cast< TH2* >( bTagEffMCFile->Get( ( "bTagEff_" + bTagWP + "_beauty" ).c_str() ) ) );
    bTagEffMCHist_b->SetDirectory( gROOT );
    bTagEffMCFile->Close();

    //path of b-tagging SF 
    std::string bTagSFFileName;
    if( year == "2016" ){
        bTagSFFileName= "DeepCSV_2016LegacySF_WP_V1.csv";
    } else if( year == "2017" ){
        bTagSFFileName = "DeepCSV_94XSF_WP_V4_B_F.csv";
    } else {
        bTagSFFileName = "DeepCSV_102XSF_WP_V1.csv";
    }
    std::string bTagSFPath = "weightFiles/bTagSF/" + bTagSFFileName;

    combinedReweighter.addReweighter( "bTag_heavy", std::make_shared< ReweighterBTagHeavyFlavorDeepCSV >( weightDirectory, bTagSFPath, bTagWP, bTagEffMCHist_c, bTagEffMCHist_b ) );
    combinedReweighter.addReweighter( "bTag_light", std::make_shared< ReweighterBTagLightFlavorDeepCSV >( weightDirectory, bTagSFPath, bTagWP, bTagEffMCHist_udsg ) );

    //make prefire Reweighter
    combinedReweighter.addReweighter( "prefire", std::make_shared< ReweighterPrefire >() );

    return combinedReweighter;
}


CombinedReweighter FourTopReweighterFactory::buildReweighter( const std::string& weightDirectory, const std::string& year, const std::vector< Sample >& samples ) const {

    analysisTools::checkYearString( year );

    //reweighter to return
    CombinedReweighter combinedReweighter;

    // electron reco reweighter
    //pT below 20 GeV
    TFile* eleRecoSFFile_pTBelow20 = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/egammaEffi_ptBelow20.txt_EGM2D_UL" + year + ".root" ).c_str() );
    std::shared_ptr< TH2 > electronRecoSFHist_pTBelow20( dynamic_cast< TH2* >( eleRecoSFFile_pTBelow20->Get( "EGamma_SF2D" ) ) );
    electronRecoSFHist_pTBelow20->SetDirectory( gROOT );
    eleRecoSFFile_pTBelow20->Close();

    ElectronIDReweighter electronRecoReweighter_pTBelow20( electronRecoSFHist_pTBelow20, new LooseMaxPtSelector< 20 > );
    combinedReweighter.addReweighter( "electronReco_pTBelow20", std::make_shared< ReweighterElectronsID >( electronRecoReweighter_pTBelow20 ) );

    //pT above 20 GeV
    TFile* eleRecoSFFile_pTAbove20 = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/egammaEffi_ptAbove20.txt_EGM2D_UL" + year + ".root" ).c_str() );
    std::shared_ptr< TH2 > electronRecoSFHist_pTAbove20( dynamic_cast< TH2* >( eleRecoSFFile_pTAbove20->Get( "EGamma_SF2D" ) ) );
    electronRecoSFHist_pTAbove20->SetDirectory( gROOT );
    eleRecoSFFile_pTAbove20->Close();

    ElectronIDReweighter electronRecoReweighter_pTAbove20( electronRecoSFHist_pTAbove20, new LooseMinPtSelector< 20 > );
    combinedReweighter.addReweighter( "electronReco_pTAbove20", std::make_shared< ReweighterElectronsID >( electronRecoReweighter_pTAbove20 ) );
    
    //make pileup Reweighter
    combinedReweighter.addReweighter( "pileup", std::make_shared< ReweighterPileup >( year, weightDirectory ) );

    //make muon ID Reweighter
    // note: these files are not present in the repository, replace by code below in order for the reweighter to work!
    /*TFile* muonSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/leptonSF_m_" + year + "_3lTight.root" ).c_str() );
    std::shared_ptr< TH2 > muonSFHist( dynamic_cast< TH2* >( muonSFFile->Get( "SFglobal" ) ) ); */
    TFile* muonSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/PRE_UL_muonTOPLeptonMVAMedium040" + year + ".root" ).c_str() );

    std::shared_ptr< TH2 > muonSFHist_nom( dynamic_cast< TH2* >( 
	muonSFFile->Get( "SF" ) ) );
    muonSFHist_nom->SetDirectory( gROOT );
    std::shared_ptr< TH2 > muonSFHist_syst( dynamic_cast< TH2* >( 
        muonSFFile->Get( "SFTotSys" ) ) );
    muonSFHist_syst->SetDirectory( gROOT );
    std::shared_ptr< TH2 > muonSFHist_stat( dynamic_cast< TH2* >( 
        muonSFFile->Get( "SFTotStat" ) ) );
    muonSFHist_stat->SetDirectory( gROOT );
    muonSFFile->Close();
    for(int i = 0; i <= muonSFHist_nom->GetNbinsX()+1; ++i){
        for(int j = 0; j <= muonSFHist_nom->GetNbinsY()+1; ++j){
            // process values
            muonSFHist_nom->SetBinError(i,j,0.);
            muonSFHist_syst->SetBinError(i,j,muonSFHist_syst->GetBinContent(i,j));
            muonSFHist_syst->SetBinContent(i,j,1.);
            muonSFHist_stat->SetBinError(i,j,muonSFHist_stat->GetBinContent(i,j));
            muonSFHist_stat->SetBinContent(i,j,1.);
        }
    }

    MuonReweighter muonReweighter_nom( muonSFHist_nom, new TightSelector );
    combinedReweighter.addReweighter("muonID",std::make_shared<ReweighterMuons>(muonReweighter_nom));
    MuonReweighter muonReweighter_syst( muonSFHist_syst, new TightSelector );
    combinedReweighter.addReweighter("muonIDSyst",std::make_shared<ReweighterMuons>(muonReweighter_syst));
    MuonReweighter muonReweighter_stat( muonSFHist_stat, new TightSelector );
    combinedReweighter.addReweighter("muonIDStat",std::make_shared<ReweighterMuons>(muonReweighter_stat));
    muonSFFile->Close();

    //make electron ID Reweighter
    // note: these files are not present in the repository, replace by code below in order for the reweighter to work!
    /*TFile* eleSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/leptonSF_e_" + year + "_3lTight.root" ).c_str() );
    std::shared_ptr< TH2 > electronSFHist( dynamic_cast< TH2* >( eleSFFile->Get( "SFglobal" ) ) ); */
    TFile* eleSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/PRE_UL_SF_EG_" + year + ".root" ).c_str() );
    // load the scalefactor histogram and set the errors to zero,
    // load the systematic errors and set the bin contents to one,
    // (note: the histogram syst contains the relative uncertainties as bin contents (?))
    // load the statistical errors and set the bin contents to one
    // (note: the histogram stat contains the relative uncertainties as bin contents (?))
    std::shared_ptr< TH2 > electronSFHist_nom( dynamic_cast< TH2* >
	( eleSFFile->Get( "EGamma_SF2D" ) ) );
    electronSFHist_nom->SetDirectory( gROOT );
    std::shared_ptr< TH2 > electronSFHist_syst( dynamic_cast< TH2* >
        ( eleSFFile->Get( "sys" ) ) );
    electronSFHist_syst->SetDirectory( gROOT );
    std::shared_ptr< TH2 > electronSFHist_stat( dynamic_cast< TH2* >
        ( eleSFFile->Get( "stat" ) ) );
    electronSFHist_stat->SetDirectory( gROOT );
    eleSFFile->Close();
    for(int i = 0; i <= electronSFHist_nom->GetNbinsX()+1; ++i){
        for(int j = 0; j <= electronSFHist_nom->GetNbinsY()+1; ++j){
            electronSFHist_nom->SetBinError(i,j,0.);
            electronSFHist_syst->SetBinError(i,j,electronSFHist_syst->GetBinContent(i,j));
            electronSFHist_syst->SetBinContent(i,j,1.);
            electronSFHist_stat->SetBinError(i,j,electronSFHist_stat->GetBinContent(i,j));
            electronSFHist_stat->SetBinContent(i,j,1.);
        }
    }

    ElectronIDReweighter electronIDReweighter_nom( electronSFHist_nom, new TightSelector );
    combinedReweighter.addReweighter( "electronID", std::make_shared<ReweighterElectronsID>(electronIDReweighter_nom) );
    ElectronIDReweighter electronIDReweighter_syst( electronSFHist_syst, new TightSelector );
    combinedReweighter.addReweighter( "electronIDSyst", std::make_shared<ReweighterElectronsID>(electronIDReweighter_syst) );
    ElectronIDReweighter electronIDReweighter_stat( electronSFHist_stat, new TightSelector );
    combinedReweighter.addReweighter( "electronIDStat", std::make_shared<ReweighterElectronsID>(electronIDReweighter_stat) );
    
    // make the b-tag shape reweighter
    // step 1: set correct csv file
    std::string bTagSFFileName;

    if (year == "2016PreVFP") {
        bTagSFFileName = "reshaping_deepJet_106XUL16preVFP_NEW.csv";
    } else if( year == "2016PostVFP" ){
        bTagSFFileName= "reshaping_deepJet_106XUL16postVFP_NEW.csv";
    } else if( year == "2017" ){
        bTagSFFileName = "reshaping_deepJet_106XUL17_NEW.csv";
    } else {
        bTagSFFileName = "reshaping_deepJet_106XUL18_NEW.csv";
    }

    //std::string weightDirectory = stringTools::formatDirectoryName( weightDirectory );
    std::string sfFilePath = "weightFiles/bTagSF/"+bTagSFFileName;
    // step 2: set other parameters
    std::string flavor = "all";
    std::string bTagAlgo = "deepFlavor";
    std::vector<std::string> variations = {"jes","hf","lf","hfstats1","hfstats2",
                                        "lfstats1","lfstats2","cferr1","cferr2" };
    // step 3: make the reweighter
    std::shared_ptr<ReweighterBTagShape> reweighterBTagShape = std::make_shared<ReweighterBTagShape>(stringTools::formatDirectoryName( weightDirectory ), sfFilePath, flavor, bTagAlgo, variations, samples );
    reweighterBTagShape->initialize(samples, 0);

    combinedReweighter.addReweighter("bTag_shape", reweighterBTagShape);

    combinedReweighter.addReweighter( "prefire", std::make_shared< ReweighterPrefire >() );

    return combinedReweighter;

}

CombinedReweighter FourTopReweighterFactory::buildReweighter( const std::string& weightDirectory, const std::string& year, const std::vector< Sample >& samples, ReweighterBTagShape** btagReweighter, bool testRun ) const {

    analysisTools::checkYearString( year );

    //reweighter to return
    CombinedReweighter combinedReweighter;

    // electron reco reweighter
    //pT below 20 GeV
    TFile* eleRecoSFFile_pTBelow20 = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/egammaEffi_ptBelow20.txt_EGM2D_UL" + year + ".root" ).c_str() );
    std::shared_ptr< TH2 > electronRecoSFHist_pTBelow20( dynamic_cast< TH2* >( eleRecoSFFile_pTBelow20->Get( "EGamma_SF2D" ) ) );
    electronRecoSFHist_pTBelow20->SetDirectory( gROOT );
    eleRecoSFFile_pTBelow20->Close();

    ElectronIDReweighter electronRecoReweighter_pTBelow20( electronRecoSFHist_pTBelow20, new LooseMaxPtSelector< 20 > );
    combinedReweighter.addReweighter( "electronReco_pTBelow20", std::make_shared< ReweighterElectronsID >( electronRecoReweighter_pTBelow20 ) );

    //pT above 20 GeV
    TFile* eleRecoSFFile_pTAbove20 = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/egammaEffi_ptAbove20.txt_EGM2D_UL" + year + ".root" ).c_str() );
    std::shared_ptr< TH2 > electronRecoSFHist_pTAbove20( dynamic_cast< TH2* >( eleRecoSFFile_pTAbove20->Get( "EGamma_SF2D" ) ) );
    electronRecoSFHist_pTAbove20->SetDirectory( gROOT );
    eleRecoSFFile_pTAbove20->Close();

    ElectronIDReweighter electronRecoReweighter_pTAbove20( electronRecoSFHist_pTAbove20, new LooseMinPtSelector< 20 > );
    combinedReweighter.addReweighter( "electronReco_pTAbove20", std::make_shared< ReweighterElectronsID >( electronRecoReweighter_pTAbove20 ) );
    
    //make pileup Reweighter
    combinedReweighter.addReweighter( "pileup", std::make_shared< ReweighterPileup >( year, weightDirectory ) );

    //make muon ID Reweighter
    // note: these files are not present in the repository, replace by code below in order for the reweighter to work!
    /*TFile* muonSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/leptonSF_m_" + year + "_3lTight.root" ).c_str() );
    std::shared_ptr< TH2 > muonSFHist( dynamic_cast< TH2* >( muonSFFile->Get( "SFglobal" ) ) ); */
    TFile* muonSFFile = TFile::Open(( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_ALL_UL" + year + ".root" ).c_str());
    
    std::shared_ptr< TH2 > muonSFHist_nom( dynamic_cast< TH2* >( muonSFFile->Get( "NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt" ) ) );
    //std::shared_ptr< TH2 > muonSFHist_nom( dynamic_cast< TH2* >( muonSFHist_nomCanv->GetPrimitive( "NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt" ) ) );
    muonSFHist_nom->SetDirectory( gROOT );
    
    //TFile* muonSFFileSyst = TFile::Open(( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_combined_syst_UL" + year + ".root" ).c_str());

    std::shared_ptr< TH2 > muonSFHist_syst( dynamic_cast< TH2* >( muonSFFile->Get( "NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_combined_syst" ) ) );
    //std::shared_ptr< TH2 > muonSFHist_syst( dynamic_cast< TH2* >( muonSFHist_systCanv->GetPrimitive( "NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_combined_syst" ) ) );
    muonSFHist_syst->SetDirectory( gROOT );

    //muonSFFileSyst->Close();

    //TFile* muonSFFileStat = TFile::Open(( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_stat_UL" + year + ".root" ).c_str());

    std::shared_ptr< TH2 > muonSFHist_stat( dynamic_cast< TH2* >( muonSFFile->Get( "NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_stat" ) ) );
    //std::shared_ptr< TH2 > muonSFHist_stat( dynamic_cast< TH2* >( muonSFHist_statCanv->GetPrimitive( "NUM_LeptonMvaMedium_DEN_TrackerMuons_abseta_pt_stat" ) ) );
    muonSFHist_stat->SetDirectory( gROOT );
    //muonSFFileStat->Close();
    
    muonSFFile->Close();
    
    for(int i = 0; i <= muonSFHist_nom->GetNbinsX()+1; ++i){
        for(int j = 0; j <= muonSFHist_nom->GetNbinsY()+1; ++j){
            // process values
            muonSFHist_nom->SetBinError(i,j,0.);
            muonSFHist_syst->SetBinError(i,j,muonSFHist_syst->GetBinContent(i,j));
            muonSFHist_syst->SetBinContent(i,j,1.);
            // muonSFHist_stat->SetBinError(i,j,muonSFHist_stat->GetBinContent(i,j));
            muonSFHist_stat->SetBinContent(i,j,1.);
        }
    }
    MuonReweighter muonReweighter_nom( muonSFHist_nom, new TightSelector );
    combinedReweighter.addReweighter("muonID",std::make_shared<ReweighterMuons>(muonReweighter_nom));
    MuonReweighter muonReweighter_syst( muonSFHist_syst, new TightSelector );
    combinedReweighter.addReweighter("muonIDSyst",std::make_shared<ReweighterMuons>(muonReweighter_syst));
    MuonReweighter muonReweighter_stat( muonSFHist_stat, new TightSelector );
    combinedReweighter.addReweighter("muonIDStat",std::make_shared<ReweighterMuons>(muonReweighter_stat));
    //muonSFFile->Close();



    //make electron ID Reweighter
    // note: these files are not present in the repository, replace by code below in order for the reweighter to work!
    /*TFile* eleSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF/leptonSF_e_" + year + "_3lTight.root" ).c_str() );
    std::shared_ptr< TH2 > electronSFHist( dynamic_cast< TH2* >( eleSFFile->Get( "SFglobal" ) ) ); */
    TFile* eleSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/leptonSF_UL/egammaEffi.txt_EGM2D_TOPULMVA_UL" + year + ".root" ).c_str() );
    // load the scalefactor histogram and set the errors to zero,
    // load the systematic errors and set the bin contents to one,
    // (note: the histogram syst contains the relative uncertainties as bin contents (?))
    // load the statistical errors and set the bin contents to one
    // (note: the histogram stat contains the relative uncertainties as bin contents (?))
    std::shared_ptr< TH2 > electronSFHist_nom( dynamic_cast< TH2* >
	( eleSFFile->Get( "EGamma_SF2D" ) ) );
    electronSFHist_nom->SetDirectory( gROOT );
    std::shared_ptr< TH2 > electronSFHist_syst( dynamic_cast< TH2* >
        ( eleSFFile->Get( "sys" ) ) );
    electronSFHist_syst->SetDirectory( gROOT );
    std::shared_ptr< TH2 > electronSFHist_stat( dynamic_cast< TH2* >
        ( eleSFFile->Get( "stat" ) ) );
    electronSFHist_stat->SetDirectory( gROOT );
    eleSFFile->Close();
    for(int i = 0; i <= electronSFHist_nom->GetNbinsX()+1; ++i){
        for(int j = 0; j <= electronSFHist_nom->GetNbinsY()+1; ++j){
            electronSFHist_nom->SetBinError(i,j,0.);
            electronSFHist_syst->SetBinError(i,j,electronSFHist_syst->GetBinContent(i,j));
            electronSFHist_syst->SetBinContent(i,j,1.);
            electronSFHist_stat->SetBinError(i,j,electronSFHist_stat->GetBinContent(i,j));
            electronSFHist_stat->SetBinContent(i,j,1.);
        }
    }

    ElectronIDReweighter electronIDReweighter_nom( electronSFHist_nom, new TightSelector );
    combinedReweighter.addReweighter( "electronID", std::make_shared<ReweighterElectronsID>(electronIDReweighter_nom) );
    ElectronIDReweighter electronIDReweighter_syst( electronSFHist_syst, new TightSelector );
    combinedReweighter.addReweighter( "electronIDSyst", std::make_shared<ReweighterElectronsID>(electronIDReweighter_syst) );
    ElectronIDReweighter electronIDReweighter_stat( electronSFHist_stat, new TightSelector );
    combinedReweighter.addReweighter( "electronIDStat", std::make_shared<ReweighterElectronsID>(electronIDReweighter_stat) );
    
    // make the b-tag shape reweighter
    // step 1: set correct csv file
    std::string bTagSFFileName;

    std::vector<std::string> variations = {"jes","hf","lf","hfstats1","hfstats2",
                                        "lfstats1","lfstats2","cferr1","cferr2",
                                        "jesAbsolute", "jesBBEC1", 
                                        "jesEC2", "jesFlavorQCD", "jesHF",
                                        "jesRelativeBal"};
    std::vector<std::string> variations_stat = {"jesAbsolute_", "jesBBEC1_", "jesEC2_",
                                        "jesHF_", "jesRelativeSample_"};

    if (year == "2016PreVFP") {
        bTagSFFileName = "reshaping_deepJet_106XUL16preVFP_NEW.csv";
        for (auto var : variations_stat) {
            var += "2016";
            variations.push_back(var);
        }
    } else if( year == "2016PostVFP" ){
        bTagSFFileName= "reshaping_deepJet_106XUL16postVFP_NEW.csv";
        for (auto var : variations_stat) {
            var += "2016";
            variations.push_back(var);
        }
    } else if( year == "2017" ){
        bTagSFFileName = "reshaping_deepJet_106XUL17_NEW.csv";
        for (auto var : variations_stat) {
            var += "2017";
            variations.push_back(var);
        }
    } else {
        bTagSFFileName = "reshaping_deepJet_106XUL18_NEW.csv";
        for (auto var : variations_stat) {
            var += "2018";
            variations.push_back(var);
        }
    }

    std::cout << "bTag SF file: " << bTagSFFileName << std::endl;
    //std::string weightDirectory = stringTools::formatDirectoryName( weightDirectory );
    std::string sfFilePath = "weightFiles/bTagSF/"+bTagSFFileName;
    // step 2: set other parameters
    std::string flavor = "all";
    std::string bTagAlgo = "deepFlavor";

    // step 3: make the reweighter
    if (! testRun) {
        std::shared_ptr<ReweighterBTagShape> reweighterBTagShape = std::make_shared<ReweighterBTagShape>(stringTools::formatDirectoryName( weightDirectory ), sfFilePath, flavor, bTagAlgo, variations, samples );
        //reweighterBTagShape->initialize(samples, 0);
        *btagReweighter = reweighterBTagShape.get();
        
        combinedReweighter.addReweighter("bTag_shape", reweighterBTagShape);
    }

    combinedReweighter.addReweighter( "prefire", std::make_shared< ReweighterPrefire >() );

    TFile* trigSFFile = TFile::Open( ( stringTools::formatDirectoryName( weightDirectory ) + "weightFiles/triggerSF/scalefactors_" + year + ".root" ).c_str() );

    std::shared_ptr< TH2 > sf_mm( dynamic_cast< TH2* >( eleSFFile->Get( ("scalefactors_"+year+"_mm").c_str() ) ) );
    sf_mm->SetDirectory( gROOT );
    std::shared_ptr< TH2 > sf_me( dynamic_cast< TH2* >( eleSFFile->Get( ("scalefactors_"+year+"_me").c_str() ) ) );
    sf_me->SetDirectory( gROOT );
    std::shared_ptr< TH2 > sf_em( dynamic_cast< TH2* >( eleSFFile->Get( ("scalefactors_"+year+"_em").c_str() ) ) );
    sf_em->SetDirectory( gROOT );
    std::shared_ptr< TH2 > sf_ee( dynamic_cast< TH2* >( eleSFFile->Get( ("scalefactors_"+year+"_ee").c_str() ) ) );
    sf_ee->SetDirectory( gROOT );


    trigSFFile->Close();

    ReweighterTrigger rewTrig_mm( sf_mm, new TightMuMuSelector );
    combinedReweighter.addReweighter( "TriggerSF_mm", std::make_shared<ReweighterTrigger>(rewTrig_mm) );
    ReweighterTrigger rewTrig_me( sf_me, new TightMuESelector );
    combinedReweighter.addReweighter( "TriggerSF_me", std::make_shared<ReweighterTrigger>(rewTrig_me) );
    ReweighterTrigger rewTrig_em( sf_em, new TightEMuSelector );
    combinedReweighter.addReweighter( "TriggerSF_em", std::make_shared<ReweighterTrigger>(rewTrig_em) );
    ReweighterTrigger rewTrig_ee( sf_ee, new TightEESelector );
    combinedReweighter.addReweighter( "TriggerSF_ee", std::make_shared<ReweighterTrigger>(rewTrig_ee) );


    return combinedReweighter;

}
