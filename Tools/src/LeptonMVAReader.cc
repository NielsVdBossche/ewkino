// implementation of LeptonMVAReader class

// include header
#include "../interface/LeptonMVAReader.h"

// include classes that were declared in advance
#include "../../objects/interface/Electron.h"
#include "../../objects/interface/Muon.h"

// constructor
LeptonMVAReader::LeptonMVAReader(
    const std::string mvaId, 
    const std::string year ):
    _mvaId( mvaId ), _year( year )
{
    // check if mvaId is valid
    if( mvaId!="TOP-UL" && mvaId!="TOPv2-UL" ){
	std::string msg = "ERROR in LeptonMVAReader:";
	msg.append(" the MVA ID tag " + mvaId + " is not recognized.");
	throw std::invalid_argument(msg);
    }

    // check if year is valid
    if( year!="2016PreVFP" && year!="2016PostVFP" && year!="2017" && year!="2018" ){
	std::string msg = "ERROR in LeptonMVAReader:";
        msg.append(" the year tag " + year + " is not recognized.");
        throw std::invalid_argument(msg);
    }

    // set paths to weights files
    std::string yearTag;
    if( year=="2016PreVFP" ) yearTag = "16APV";
    else if( year=="2016PostVFP" ) yearTag = "16";
    else if( year=="2017" ) yearTag = "17";
    else if( year=="2018" ) yearTag = "18";
    std::string mvaIdTag;
    if( mvaId=="TOP-UL" ) mvaIdTag = "TOPUL";
    else if( mvaId=="TOPv2-UL" ) mvaIdTag = "TOPv2UL";
    std::string weightFilesDir = "/user/llambrec/ewkinoNanoAOD/ewkino/Tools/data/";
    // todo: find a way to refer to absolute directory relatively
    std::string weightsFileMuons = weightFilesDir + "mu_" + mvaIdTag + yearTag + "_XGB.weights.bin";
    if( !systemTools::fileExists(weightsFileMuons) ){
	std::string msg = "ERROR in LeptonMVAReader:";
	msg.append(" file " + weightsFileMuons + " does not exist.");
	throw std::runtime_error(msg);
    }
    std::string weightsFileElectrons = weightFilesDir + "el_" + mvaIdTag + yearTag + "_XGB.weights.bin";
    if( !systemTools::fileExists(weightsFileElectrons) ){
        std::string msg = "ERROR in LeptonMVAReader:";
        msg.append(" file " + weightsFileElectrons + " does not exist.");
        throw std::runtime_error(msg);
    }

    // set up XGBoost reader
    safe_xgboost(XGBoosterCreate(NULL, 0, &_booster[0]));
    safe_xgboost(XGBoosterCreate(NULL, 0, &_booster[1]));
    safe_xgboost(XGBoosterLoadModel(_booster[0], weightsFileMuons.c_str()));
    safe_xgboost(XGBoosterLoadModel(_booster[1], weightsFileElectrons.c_str()));
}

double LeptonMVAReader::leptonMVAMuon(const Muon* muon){
    _boosterVars[0][0][0] = (float) muon->pt();
    _boosterVars[0][0][1] = (float) muon->eta();
    _boosterVars[0][0][2] = (float) muon->closestJetNumberOfChargedDaughters();
    _boosterVars[0][0][3] = (float) muon->miniIsoCharged();
    _boosterVars[0][0][4] = (float) muon->miniIso()-muon->miniIsoCharged();
    _boosterVars[0][0][5] = (float) muon->ptRel();
    _boosterVars[0][0][6] = (float) muon->ptRatio();
    _boosterVars[0][0][7] = (float) muon->relIso0p3();
    _boosterVars[0][0][8] = (float) muon->closestJetDeepFlavor();
    _boosterVars[0][0][9] = (float) muon->sip3d();
    _boosterVars[0][0][10] = (float) std::log(std::abs(muon->dxy()));
    _boosterVars[0][0][11] = (float) std::log(std::abs(muon->dz()));
    _boosterVars[0][0][12] = (float) muon->segmentCompatibility();
    DMatrixHandle dtest;
    int nfeat = 13;
    safe_xgboost(XGDMatrixCreateFromMat(reinterpret_cast<float*>(_boosterVars[0]), 1, nfeat, NAN, &dtest));
    bst_ulong out_len;
    const float *f;
    safe_xgboost(XGBoosterPredict(_booster[0], dtest, 0, 0, &out_len, &f));
    safe_xgboost(XGDMatrixFree(dtest));
    // printouts for testing
    /*std::cout << "temp print in LeptonMVAREader::leptonMVAMuon" << std::endl;
    for( unsigned i=0; i<13; i++ ){
	std::cout << "  " << _boosterVars[0][0][i] << std::endl;
    }
    std::cout << "  -> " << f[0] << std::endl;*/
    return f[0];
}

double LeptonMVAReader::leptonMVAElectron(const Electron* ele){
    _boosterVars[1][0][0] = (float) ele->pt();
    _boosterVars[1][0][1] = (float) ele->eta();
    _boosterVars[1][0][2] = (float) ele->closestJetNumberOfChargedDaughters();
    _boosterVars[1][0][3] = (float) ele->miniIsoCharged();
    _boosterVars[1][0][4] = (float) ele->miniIso()-ele->miniIsoCharged();
    _boosterVars[1][0][5] = (float) ele->ptRel();
    _boosterVars[1][0][6] = (float) ele->ptRatio();
    _boosterVars[1][0][7] = (float) ele->relIso0p3();
    _boosterVars[1][0][8] = (float) ele->closestJetDeepFlavor();
    _boosterVars[1][0][9] = (float) ele->sip3d();
    _boosterVars[1][0][10] = (float) std::log(std::abs(ele->dxy()));
    _boosterVars[1][0][11] = (float) std::log(std::abs(ele->dz()));
    _boosterVars[1][0][12] = (float) ele->electronMVAFall17NoIso();
    _boosterVars[1][0][13] = (float) ele->numberOfMissingHits();
    DMatrixHandle dtest;
    int nfeat = 13;
    if( _mvaId=="TOPv2-UL" ) nfeat = 14;
    safe_xgboost(XGDMatrixCreateFromMat(reinterpret_cast<float*>(_boosterVars[1]), 1, nfeat, NAN, &dtest));
    bst_ulong out_len;
    const float *f;
    safe_xgboost(XGBoosterPredict(_booster[1], dtest, 0, 0, &out_len, &f));
    safe_xgboost(XGDMatrixFree(dtest));
    return f[0];
}
