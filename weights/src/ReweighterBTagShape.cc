/*
=== b-tag shape reweighter ===
This is an alternative b-tagging reweighter appropriate when reweighting the shape.
It should be used instead of ReweighterBTag.cc (fixed working point reweighting)
whenever the shape of the b-tagging discriminant matters,
e.g. if the b-tagging scores of jets are used as MVA inputs.

For more information, see following links:
-  general overview: https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods
-  this specific method: https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration
This method corresponds to "1d", whereas ReweighterBTag corresponds to "1a".
*/

#include "../interface/ReweighterBTagShape.h"
#include "../interface/ReweighterBTag.h"

/// constructor ///
ReweighterBTagShape::ReweighterBTagShape(const std::string &weightDirectory,
                                         const std::string &sfFilePath,
                                         const std::string &flavor,
                                         const std::string &bTagAlgo,
                                         const std::vector<std::string> &variations,
                                         const std::vector<Sample> &samples)
// input arguments:
// - weightDirectory: path to the ewkino/weights folder
// - sfFilePath: path to the scale factor csv file relative from ewkino/weights
// - flavor: either "heavy", "light" or "all"
// - bTagAlgo: either "deepFlavor" or "deepCSV"
// - variations: vector of systematic variations to consider
//		     see list of allowed values below.
//		     note: they must be provided at initialization so their validity can be checked,
//			   and to make sure only the needed variations are read from the csv file.
// - samples: vector of Sample objects that will be reweighted
//		  note: they must be provided at initialization so their normalization
//			can be initialized.
{

    std::cout << "creating a ReweighterBTagShape with following parameters:" << std::endl;
    std::cout << "... (useful printing to implement)" << std::endl;

    // set the flavor to the provided value if it is valid
    if (!(flavor == "heavy" || flavor == "light" || flavor == "all"))
    {
        throw std::invalid_argument(std::string("ERROR in ReweighterBTagShape: ") + "argument 'flavor' is '" + flavor + "' while it should be " + "'heavy', 'light', or 'all'.");
    }
    _flavor = flavor;

    // set the b-tagging algorithm to the provided value if it is valid
    if (!(bTagAlgo == "deepCSV" || bTagAlgo == "deepFlavor"))
    {
        throw std::invalid_argument(std::string("ERROR in ReweighterBTagShape: ") + "argument 'bTagAlgo' is '" + bTagAlgo + "' while it should be " + "'deepCSV' or 'deepFlavor'.");
    }
    _bTagAlgo = bTagAlgo;

    // define lists of valid "variations" and "systematics"
    // note: "variations" are all varied scale factors present in the csv file;
    //       they consist of "systematics" on the scale factors
    //       (needed to determine an uncertainty on the b-tag reweighting)
    //       and jet energy variations
    //	     (needed only to propagate jet energy variations to central b-tag weights!)
    // note: maybe find a way to read these from the csv file
    //       instead of copying them from the twiki mentioned above?

    std::vector<std::string> allowedvar = {"jes", "hf", "lf", "hfstats1", "hfstats2",
                                           "lfstats1", "lfstats2", "cferr1", "cferr2",
                                           "jesAbsoluteMPFBias", "jesAbsoluteScale", "jesAbsoluteStat",
                                           "jesRelativeBal", "jesRelativeFSR", "jesRelativeJEREC1",
                                           "jesRelativeJEREC2", "jesRelativeJERHF",
                                           "jesRelativePtBB", "jesRelativePtEC1", "jesRelativePtEC2",
                                           "jesRelativePtHF",
                                           "jesRelativeStatEC", "jesRelativeStatFSR", "jesRelativeStatHF",
                                           "jesPileUpDataMC", "jesPileUpPtBB", "jesPileUpPtEC1",
                                           "jesPileUpPtEC2", "jesPileUpPtHF", "jesPileUpPtRef",
                                           "jesFlavorQCD", "jesFragmentation", "jesSinglePionECAL",
                                           "jesSinglePionHCAL", "jesTimePtEta",
                                           "jesAbsolute", "jesAbsolute_2018", "jesBBEC1", "jesBBEC1_2018",
                                           "jesEC2", "jesEC2_2018", "jesFlavorQCD", "jesHF",
                                           "jesHF_2018", "jesRelativeBal", "jesRelativeSample_2018",
                                           "jesAbsolute_2017", "jesBBEC1_2017", "jesEC2_2017",
                                           "jesHF_2017", "jesRelativeSample_2017",
                                           "jesAbsolute_2016", "jesBBEC1_2016", "jesEC2_2016",
                                           "jesHF_2016", "jesRelativeSample_2016"};

    std::vector<std::string> allowedsys = {"hf", "lf", "hfstats1", "hfstats2",
                                           "lfstats1", "lfstats2", "cferr1", "cferr2"};
    // (note: allowedsys must be a subcollection of allowedvar, excluding jec variations)
    _variations = std::vector<std::string>();
    _systematics = std::vector<std::string>();
    for (std::string variation : variations)
    {
        // check if provided variation is valid
        if (std::find(allowedvar.begin(), allowedvar.end(), variation) == allowedvar.end())
        {
            throw std::invalid_argument(std::string("ERROR in ReweighterBTagShape: ") + "argument 'variations' contains '" + variation + "' " + "which is not recognized.");
        }
        _variations.push_back(variation);
        // check if it is also a systematic
        if (std::find(allowedsys.begin(), allowedsys.end(), variation) != allowedsys.end())
        {
            _systematics.push_back(variation);
        }
    }

    // initialize normalization factors
    for (Sample sample : samples)
    {      
        std::string sampleName = sample.fileName();
        for (int nLep = 2; nLep<5; nLep++) {
            _normFactors[sampleName]["central"][nLep][0] = 1.;
            for (auto sys : allowedvar) {
                std::string variation = "up_" + sys;
                _normFactors[sampleName][variation][nLep][0] = 1.;
                variation = "down_" + sys;
                _normFactors[sampleName][variation][nLep][0] = 1.;

                if (sys == "jesFlavorQCD") {
                    std::vector<std::string> flavors = {"_0", "_4", "_5"};
                    for (auto flavVar : flavors) {
                        variation = "up_" + sys + flavVar;
                        _normFactors[sampleName][variation][nLep][0] = 1.;
                        variation = "down_" + sys + flavVar;
                        _normFactors[sampleName][variation][nLep][0] = 1.;
                    }
                }
            }
        }
        // (initialize one element at 0 jets for each sample;
        // events with higher jet multiplicities will fall back to this default value)
    }

    // set the the working point to "reshaping"
    BTagEntry::OperatingPoint wp = BTagEntry::OP_RESHAPING;

    // make the scale factor reader
    std::vector<std::string> var_ext;
    for (std::string var : _variations)
    {
        var_ext.push_back("up_" + var);
        var_ext.push_back("down_" + var);
    }
    std::cout << "creating BTagCalibrationReader instance..." << std::endl;
    bTagSFReader.reset(new BTagCalibrationReader(wp, "central", var_ext));

    // set the type of scale factors to be extracted
    std::string fitMethod = "iterativefit";

    // calibrate the reader
    // note: this part can be commented out for quicker testing!
    //	     in that case, make sure to also comment out the bTagSFReader->eval_auto_bounds call below
    //	     and return a default value instead!
    std::cout << "reading requested scale factors from csv file..." << std::endl;
    bTagSFCalibration = std::shared_ptr<BTagCalibration>(
        new BTagCalibration("", stringTools::formatDirectoryName(weightDirectory) + sfFilePath));
    if (_flavor == "heavy" || _flavor == "all")
    {
        bTagSFReader->load(*bTagSFCalibration, BTagEntry::FLAV_B, fitMethod);
        bTagSFReader->load(*bTagSFCalibration, BTagEntry::FLAV_C, fitMethod);
    }
    if (_flavor == "light" || _flavor == "all")
    {
        bTagSFReader->load(*bTagSFCalibration, BTagEntry::FLAV_UDSG, fitMethod);
    }

    std::cout << "done creating the ReweighterBTagShape instance." << std::endl;
}

/// initializer ///
void ReweighterBTagShape::initialize(const std::vector<Sample> &samples,
                                     long unsigned numberOfEntries)
{
    // initialize the reweighter for a collection of samples, i.e. set the normalization factors
    // input arguments:
    // - samples: vector of Sample objects
    // - numberOfEntries: maximum number of entries to take into account
    //                    (default value of 0 = all events) should be used
    // note: it is not very clear at what point the normalization factors should be determined...
    //       in principle, after applying all selections except for b-tag selections,
    //       but that is very hard to implement at this level since it depends on the
    //       event selection for the specific use case.
    //       it is also not very clear how to weight the events correctly for this normalization
    //       (all reweighting factors except for b-tag factors?);
    //       one can either use this function (which does basically no event selection
    //	     and uses weight 1 for each entry) and assume this is "good enough",
    //       OR, one has to manually calculate the sum of weights after appropriate selections,
    //       and then set the norm factors with setNormFactors (see below).
    std::cout << "initializing ReweighterBTagShape" << std::endl;
    // loop over samples
    for (Sample sample : samples)
    {
        std::string pathToFile = sample.filePath();
        std::string sampleName = sample.fileName();
        // calculate the sum of weights for this sample (per jet multiplicity)
        // and update the normalization factor
        std::map<int, double> averageOfWeights = this->calcAverageOfWeights(sample,
                                                                            numberOfEntries);
        this->setNormFactors(sample, averageOfWeights, 2);
        this->setNormFactors(sample, averageOfWeights, 3);
        this->setNormFactors(sample, averageOfWeights, 4);
    }
    std::cout << "done initializing ReweighterBTagShape" << std::endl;
}

/// help functions for checking a variation or systematic ///

bool ReweighterBTagShape::hasVariation(const std::string &variation) const
{
    // determine whether this instance has a given variation
    // note: the variation could be either a systematic uncertainty or a JEC variation
    if (std::find(_variations.begin(), _variations.end(), variation) == _variations.end())
        return false;
    return true;
}

bool ReweighterBTagShape::hasSystematic(const std::string systematic) const
{
    // determine whether this instance has a given systematic uncertainty
    if (std::find(_systematics.begin(), _systematics.end(), systematic) == _systematics.end())
        return false;
    return true;
}

bool ReweighterBTagShape::considerVariation(const Jet &jet,
                                            const std::string &variation) const
{
    // check if a given variation needs to be considered for a given jet
    // see the recommendations: some systematics should only be applied to b-jets and light jets,
    //                          and others only to c-jets;
    //                          the jec variations should not be applied to c-jets.
    std::vector<std::string> forbidden_variations;
    if (jet.hadronFlavor() == 5 || jet.hadronFlavor() == 0)
    {
        forbidden_variations = {"cferr1", "cferr2"};
    }
    else if (jet.hadronFlavor() == 4)
    {
        forbidden_variations = {"hf", "lf", "hfstats1", "hfstats2", "lfstats1", "lfstats2",
                                "jes", "jesAbsoluteMPFBias", "jesAbsoluteScale", "jesAbsoluteStat",
                                "jesRelativeBal", "jesRelativeFSR", "jesRelativeJEREC1",
                                "jesRelativeJEREC2", "jesRelativeJERHF",
                                "jesRelativePtBB", "jesRelativePtEC1", "jesRelativePtEC2",
                                "jesRelativePtHF",
                                "jesRelativeStatEC", "jesRelativeStatFSR", "jesRelativeStatHF",
                                "jesPileUpDataMC", "jesPileUpPtBB", "jesPileUpPtEC1",
                                "jesPileUpPtEC2", "jesPileUpPtHF", "jesPileUpPtRef",
                                "jesFlavorQCD", "jesFragmentation", "jesSinglePionECAL",
                                "jesSinglePionHCAL", "jesTimePtEta",
                                "jesAbsolute", "jesAbsolute_2018", "jesBBEC1", "jesBBEC1_2018",
                                "jesEC2", "jesEC2_2018", "jesFlavorQCD", "jesHF",
                                "jesHF_2018", "jesRelativeBal", "jesRelativeSample_2018",
                                "jesAbsolute_2017", "jesBBEC1_2017", "jesEC2_2017",
                                "jesHF_2017", "jesRelativeSample_2017",
                                "jesAbsolute_2016", "jesBBEC1_2016", "jesEC2_2016",
                                "jesHF_2016", "jesRelativeSample_2016",
                                "jesFlavorQCD_0", "jesFlavorQCD_4", "jesFlavorQCD_5"};
    }
    for (std::string var : forbidden_variations)
    {
        if (variation == var || variation == "up_" + var || variation == "down_" + var)
            return false;
    }
    return true;
}

/// help functions for getting and setting normalization factors ///

void ReweighterBTagShape::setNormFactors(const Sample &sample,
                                         const std::map<int, double> normFactors, int nLeptons)
{
    // set the normalization factors
    // input arguments:
    // - sample: a Sample object for which to set the normalization
    // - normFactors: a map of jet multiplicity to averages-of-weights
    //                note: it is initialized to {0: 1.} in the constructor,
    //		      which implies the normalization factor will be 1 for each event.

    std::cout << "setting norm factors" << std::endl;

    std::string sampleName = sample.fileName();
    if (_normFactors.find(sampleName) == _normFactors.end())
    {
        throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape was not initialized for this sample!");
    }

    _normFactors[sampleName]["central"][nLeptons] = normFactors;
    std::cout << "done setting norm factors" << std::endl;
}

void ReweighterBTagShape::setNormFactors(const Sample &sample, const std::map<int, double> normFactors, std::string& variation, int nLeptons )
{
    // set the normalization factors
    // input arguments:
    // - sample: a Sample object for which to set the normalization
    // - normFactors: a map of jet multiplicity to averages-of-weights
    //                note: it is initialized to {0: 1.} in the constructor,
    //		      which implies the normalization factor will be 1 for each event.

    std::cout << "setting norm factors" << std::endl;

    std::string entry = sample.fileName();
    if (_normFactors.find(entry) == _normFactors.end())
    {
        throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape was not initialized for this sample!");
    }

    _normFactors[entry][variation][nLeptons] = normFactors;
    std::cout << "done setting norm factors" << std::endl;
}

double ReweighterBTagShape::getNormFactor(const Event &event, const std::string &jecVariation, const std::string& systematic) const
{
    // get the normalization factor for an event
    // note: the normalization factor depends on the sample to which the event belongs
    //       and on the jet multiplicity of the event.
    // note: jecVariation has a default value: 'nominal', i.e. no variation of JEC
    std::string sampleName = event.sample().fileName();
    // check validity of sample to which event belongs
    if (_normFactors.find(sampleName) == _normFactors.end())
    {
        throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape was not initialized for this sample!");
    }
    // determine number of jets
    int njets = event.getJetCollection(jecVariation).size();
    int nLeptons = event.numberOfFOLeptons();
    if (nLeptons > 4) nLeptons=4;
    // retrieve the normalization factor
    // note: if no normalization factor was initialized for this jet multiplicity,
    //	     the value for lower jet multiplicities is retrieved instead.
    //std::cout << nLeptons << "+" << njets << " nleps+njets & syst " << systematic << " for jec var "<< jecVariation << std::endl;
    for (int n = njets; n >= 0; n--)
    {   
        //std::cout << "get norm factor" << std::endl;
        if (_normFactors.at(sampleName).at(systematic).at(nLeptons).find(n) != _normFactors.at(sampleName).at(systematic).at(nLeptons).end())
        {
            //std::cout << "found norm factor" << std::endl;
            return _normFactors.at(sampleName).at(systematic).at(nLeptons).at(n);
        }
    }

    throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape got event for which no norm factor could be retrieved.");
}

double ReweighterBTagShape::getNormFactor_FlavorFilter(const Event &event, unsigned flavor, const std::string &jecVariation, const std::string& systematic) const
{
    // get the normalization factor for an event
    // note: the normalization factor depends on the sample to which the event belongs
    //       and on the jet multiplicity of the event.
    // note: jecVariation has a default value: 'nominal', i.e. no variation of JEC
    std::string sampleName = event.sample().fileName();
    // check validity of sample to which event belongs
    if (_normFactors.find(sampleName) == _normFactors.end())
    {
        throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape was not initialized for this sample! " + sampleName);
    }
    if (_normFactors.at(sampleName).find(systematic) == _normFactors.at(sampleName).end())
    {
        throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape was not initialized for this systematic! " + systematic);
    }
    //std::cout << "normfactor" << std::endl;
    // determine number of jets
    int njets = 0;
    int nLeptons = event.numberOfFOLeptons();
    //std::cout << "nelps" << nLeptons << std::endl;

    if (nLeptons > 4) nLeptons=4;
    if (stringTools::stringContains(jecVariation, "Up")) {
        njets = event.jetCollection().JECUpGroupedFlavorQCD(flavor).size();
    } else {
        njets = event.jetCollection().JECDownGroupedFlavorQCD(flavor).size();
    }
    // retrieve the normalization factor
    // note: if no normalization factor was initialized for this jet multiplicity,
    //	     the value for lower jet multiplicities is retrieved instead.
    //std::cout << njets << " njets & syst " << systematic << " for jec var "<< jecVariation << std::endl;
    //std::cout << "get out" << nLeptons << std::endl;
    
    for (int n = njets; n >= 0; n--)
    {   
        //std::cout << "get norm factor" << std::endl;
        if (_normFactors.at(sampleName).at(systematic).at(nLeptons).find(n) != _normFactors.at(sampleName).at(systematic).at(nLeptons).end())
        {
            //std::cout << "found norm factor" << std::endl;
            return _normFactors.at(sampleName).at(systematic).at(nLeptons).at(n);
        }
    }

    throw std::invalid_argument(std::string("ERROR: ") + "ReweighterBTagShape got event for which no norm factor could be retrieved.");
}

std::map<std::string, std::map<std::string, std::map<int,std::map<int,double>>>> ReweighterBTagShape::getNormFactors() const
{
    return _normFactors;
}

void ReweighterBTagShape::printNormFactors() const
{
    //for (auto el : _normFactors)
    //{   
    //    std::cout << el.first << ": " << std::endl;
    //    for (auto el2 : el.second){
    //        std::cout << el2.first << ": " << std::endl;
    //        for (auto el3 : el2.second)
    //        {
    //            //std::cout << " - " << el3.first << " -> " << el3.second << std::endl;
    //        }
    //    }   
    //}
}

double ReweighterBTagShape::weightVariation( const Event& event, const std::string& variation ) const {
    return this->weight(event, variation);
}


/// member functions for weights ///

double ReweighterBTagShape::weight(const Jet &jet, const std::string &variation) const
{
    // get the weight for a single jet
    // the weight is determined as follows:
    // - if this instance if for heavy flavor and the jet is light, weight = 1
    // - if this instance is for light flavor and the jet is heavy, weight = 1
    // - if the jet is outside b-tag acceptance, weight = 1
    // - else correct weight is read depending on flavor, eta, pt, b-tag score and systematic.

    std::string sys = variation;
    // check if variation is valid for this jet
    if (!this->considerVariation(jet, variation))
        sys = "central";
    // check if jet is of correct flavor for this reweighter
    if (jet.hadronFlavor() == 5 || jet.hadronFlavor() == 4)
    {
        if (!(_flavor == "heavy" || _flavor == "all"))
            return 1;
    }
    else
    {
        if (!(_flavor == "light" || _flavor == "all"))
            return 1;
    }

    // make sure jet is in b-tag acceptance
    if (!jet.inBTagAcceptance())
    {
        return 1.;
    }

    // determine bTagScore (note: maybe modify later to the model of bTagReweighter,
    // which is probably faster as it does not involve evaluating this string for every jet)
    double bTagScore = (_bTagAlgo == "deepFlavor") ? jet.deepFlavor() : (_bTagAlgo == "deepCSV") ? jet.deepCSV()
                                                                                                 : -99;
    // (in principle no checking for other values is needed, as already done in constructor)

    // note: https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration#Using_b_tag_scale_factors_in_you
    // this page recommends to use absolute value of eta, but BTagCalibrationStandalone.cc
    // seems to handle negative values of eta more correctly (only taking abs when needed)
    double scaleFactor = bTagSFReader->eval_auto_bounds(sys, jetFlavorEntry(jet),
                                                        jet.absEta(), jet.pt(), bTagScore);
    // printouts for testing
    /*if( scaleFactor==0 ){
    std::cout << "found scale factor 0 ..." << std::endl;
    std::cout << "sys: " << sys << std::endl;
    std::cout << "jet flavor: " << jetFlavorEntry(jet) << std::endl;
    std::cout << "eta: " << jet.eta() << std::endl;
    std::cout << "pt: " << jet.pt() << std::endl;
    std::cout << "b tag score: " << bTagScore << std::endl;
    }*/
    return scaleFactor;
}

double ReweighterBTagShape::weight(const Jet &jet) const
{
    return weight(jet, "central");
}

double ReweighterBTagShape::weightUp(const Jet &jet, const std::string &systematic) const
{
    return weight(jet, "up_" + systematic);
}

double ReweighterBTagShape::weightDown(const Jet &jet, const std::string &systematic) const
{
    return weight(jet, "down_" + systematic);
}

double ReweighterBTagShape::weight(const Event &event, const std::string &variation) const
{
    // get the weight for an event by multiplying individual jet weights
    // note: the nominal jet collection in the event is used;
    //       for the JEC variations: see below
    double weight = 1.;
    for (const auto &jetPtr : event.jetCollection().goodJetCollection())
    {
        weight *= this->weight(*jetPtr, variation);
    }
    // take into account normalization
    double normweight = weight / getNormFactor(event, "nominal", variation);
    // prints for testing
    // std::cout << "raw weight: " << weight << std::endl;
    // std::cout << "normalized weight: " << normweight << std::endl;
    return normweight;
}

double ReweighterBTagShape::weight(const Event &event) const
{
    // get nominal weight for event
    return this->weight(event, "central");
}

double ReweighterBTagShape::weightUp(const Event &event,
                                     const std::string &systematic) const
{
    // get up weight for event and given systematic
    return this->weight(event, "up_" + systematic);
}

double ReweighterBTagShape::weightDown(const Event &event,
                                       const std::string &systematic) const
{
    // get down weight for event and given systematic
    return this->weight(event, "down_" + systematic);
}

double ReweighterBTagShape::weightNoNorm(const Event &event) const
{
    // same as weight but no normalization factor (mainly for testing)
    double weight = 1.;
    for (const auto &jetPtr : event.jetCollection().goodJetCollection())
    {
        weight *= this->weight(*jetPtr);
    }
    return weight;
}

double ReweighterBTagShape::weightJecVar(const Event &event,
                                         const std::string &jecVariation, bool grouped, const unsigned var ) const
{
    // same as weight but with propagation of jec variations
    // jecvar is expected to be of the form e.g. AbsoluteScaleUp or AbsoluteScaleDown
    // special case JECUp and JECDown (for single variations) are also allowed
    std::string jecVar = stringTools::removeOccurencesOf(jecVariation, "JEC");
    std::string varName;
    std::string jesVarName;
    bool isup = true;
    //std::cout << "preprocess" << std::endl;
    if (stringTools::stringEndsWith(jecVar, "Up"))
    {
        varName = "jes" + jecVar.substr(0, jecVar.size() - 2);
        jesVarName = "up_" + varName;
    }
    else if (stringTools::stringEndsWith(jecVar, "Down"))
    {
        varName = "jes" + jecVar.substr(0, jecVar.size() - 4);
        jesVarName = "down_" + varName;
        isup = false;
    }
    if (!hasVariation(varName))
    {
        std::string msg = "### ERROR ### in ReweighterBTagShape::weightJecVar:";
        msg += " jec variation '" + jecVariation + "' (corresponding to '" + varName + "') not valid";
        std::cerr << msg << std::endl;
        std::cerr << "returning nominal weight" << std::endl;
        return this->weight(event, "central");
        //throw std::invalid_argument(msg);
    }
    //std::cout << "safety checked" << std::endl;

    double weight = 1.;
    //std::cout << "getting collection" << std::endl;

    for (const auto &jetPtr : event.getJetCollectionPtr()->getVariedJetCollection(var, isup, grouped))
    {   
        //std::cout << "loop";

        if (isup)
            weight *= this->weightUp(*jetPtr, varName);
        else
            weight *= this->weightDown(*jetPtr, varName);
    }
    //std::cout << " LOOPED " << std::endl;

    return weight  / getNormFactor(event, jecVariation, jesVarName);
}

double ReweighterBTagShape::weightJecVar_FlavorFilter(const Event &event,
                                         const std::string &jecVariation, unsigned flavor) const
{
    // same as weight but with propagation of jec variations
    // jecvar is expected to be of the form e.g. AbsoluteScaleUp or AbsoluteScaleDown
    // special case JECUp and JECDown (for single variations) are also allowed
    std::string jecVar = stringTools::removeOccurencesOf(jecVariation, "JEC");
    std::string varName;
    std::string jesVarName;
    bool isup = true;
    if (stringTools::stringEndsWith(jecVar, "Up"))
    {
        varName = "jes" + jecVar.substr(0, jecVar.size() - 2);
        jesVarName = "up_" + varName + "_" + std::to_string(flavor);
    }
    else if (stringTools::stringEndsWith(jecVar, "Down"))
    {
        varName = "jes" + jecVar.substr(0, jecVar.size() - 4);
        jesVarName = "down_" + varName + "_" + std::to_string(flavor);
        isup = false;
    }
    //std::cout << jecVariation << " " << flavor << std::endl;
    if (!hasVariation(varName))
    {
        std::string msg = "### ERROR ### in ReweighterBTagShape::weightJecVar_FlavorFilter:";
        msg += " jec variation '" + jecVariation + "' (corresponding to '" + varName + "') not valid";
        std::cerr << msg << std::endl;
        std::cerr << "returning nominal weight" << std::endl;
        return this->weight(event, "central");
        //throw std::invalid_argument(msg);
    }
    double weight = 1.;
    for (const auto &jetPtr : event.jetCollection().JECGroupedFlavorQCD(flavor, isup))
    {   
        if (jetPtr->hadronFlavor() == flavor) {
            if (isup)
                weight *= this->weightUp(*jetPtr, varName);
            else
                weight *= this->weightDown(*jetPtr, varName);
        } else {
            weight *= this->weight(*jetPtr);
        }
    }
    return weight  / getNormFactor_FlavorFilter(event, flavor, jecVariation, jesVarName);
}

/// help function for calculating normalization factors ///

std::map<int, double> ReweighterBTagShape::calcAverageOfWeights(const Sample &sample,
                                                                long unsigned numberOfEntries) const
{
    // calculate the average of b-tag weights in a given sample
    // the return type is a map of jet multiplicity to average of weights
    // input arguments:
    // - sample: a Sample object
    // - numberOfEntries: number of entries to consider for the average of weights
    //   note: defaults to 0, in which case all entries in the file are used
    // note: for the averaging, each entry in the input sample is counted as 1,
    //       regardless of lumi, cross-section, generator weight or other reweighting factors!

    // make a TreeReader
    std::string inputFilePath = sample.filePath();
    std::cout << "making TreeReader..." << std::endl;
    TreeReader treeReader;
    treeReader.initSampleFromFile(inputFilePath);

    // initialize the output map
    std::map<int, double> averageOfWeights;
    std::map<int, int> nEntries;

    // loop over events
    long unsigned availableEntries = treeReader.numberOfEntries();
    if (numberOfEntries == 0)
        numberOfEntries = availableEntries;
    else
        numberOfEntries = std::min(numberOfEntries, availableEntries);
    std::cout << "starting event loop for " << numberOfEntries << " events..." << std::endl;
    for (long unsigned entry = 0; entry < numberOfEntries; ++entry)
    {
        // if (entry == 1000) break;
        Event event = treeReader.buildEvent(entry);

        // do basic jet cleaning
        event.cleanJetsFromFOLeptons();
        event.jetCollection().selectGoodJets();
        if (event.numberOfFOLeptons() < 2) continue;

        // determine (nominal) b-tag reweighting and number of jets
        double btagreweight = this->weight(event);
        int njets = event.jetCollection().goodJetCollection().size();

        // add it to the map
        if (averageOfWeights.find(njets) == averageOfWeights.end())
        {
            averageOfWeights[njets] = btagreweight;
            nEntries[njets] = 1;
        }
        else
        {
            averageOfWeights[njets] += btagreweight;
            nEntries[njets] += 1;
        }
    }

    // divide sum by number to get average
    for (std::map<int, double>::iterator it = averageOfWeights.begin();
         it != averageOfWeights.end(); ++it)
    {
        averageOfWeights[it->first] = it->second / nEntries[it->first];
    }

    std::cout << "done with event loop" << std::endl;
    return averageOfWeights;
}
