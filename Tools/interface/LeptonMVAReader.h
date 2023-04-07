// helper class for facilitating the computation of the lepton MVA score.
// note: originally based on similar class in miniAOD ntuplizer 
// (https://github.com/GhentAnalysis/heavyNeutrino/blob/UL_master/multilep/interface/LeptonMvaHelper.h)
// but with modifications to fit better in this framework and simplified ignoring pre-UL trainings.

#ifndef LeptonMVAReader_H
#define LeptonMVAReader_H

// include c++ libraries
#include <xgboost/c_api.h>
#include <memory>
#include <cmath>
#include <string>

// include other parts of the framework
#include "systemTools.h"
#include "stringTools.h"

// forward declarations
class Muon;
class Electron;

// define macro
// see https://xgboost.readthedocs.io/en/stable/tutorials/c_api_tutorial.html
#define safe_xgboost(call) { \
  int err = (call); \
  if (err != 0) { \
    throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) \
                        + ": error in " + #call + ":" + XGBGetLastError()); \
  } \
}


class LeptonMVAReader{

    public:
	LeptonMVAReader() = default;
        LeptonMVAReader(const std::string, const std::string);
        double leptonMVAMuon(const Muon*);
        double leptonMVAElectron(const Electron*);
    private:
	std::string _mvaId;
	std::string _year;
        BoosterHandle _booster[2];
        float _boosterVars[2][1][15];
};
#endif
