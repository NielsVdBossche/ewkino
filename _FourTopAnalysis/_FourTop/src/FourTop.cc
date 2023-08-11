#include "../interface/FourTop.h"

#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

#include "TObjString.h"

#include "../../../Tools/interface/systemTools.h"
#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

FourTop::FourTop(std::string outputName, std::vector<std::string>& argvString, int mode) {
    // Set settings according to input
    // prepare details of analysis in separate functions
    
    // First setting are samples to work through
    treeReader = new TreeReader(argvString[1], "/pnfs/iihe/cms/store/user/nivanden/skims/");

    Sample samp = treeReader->sampleVector()[0];

    std::string jecUncertaintyFile;
    if (samp.is2018()) {
        jecUncertaintyFile = "JECUncertaintyInputs/Summer19UL18_V5_MC/Summer19UL18_V5_MC_UncertaintySources_AK4PFchs.txt";
    } else if (samp.is2017()) {
        jecUncertaintyFile = "JECUncertaintyInputs/Summer19UL17_V5_MC/Summer19UL17_V5_MC_UncertaintySources_AK4PFchs.txt";
    } else if (samp.is2016PostVFP()) {
        jecUncertaintyFile = "JECUncertaintyInputs/Summer19UL16_V7_MC/Summer19UL16_V7_MC_UncertaintySources_AK4PFchs.txt";
    } else {
        jecUncertaintyFile = "JECUncertaintyInputs/Summer19UL16APV_V7_MC/Summer19UL16APV_V7_MC_UncertaintySources_AK4PFchs.txt";
    }
    selection = new EventFourTLoose(jecUncertaintyFile);

    if (mode < 2) {
        std::string outputFileName = "Output/" + outputName + "_";
        std::ostringstream oss;

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::string timestampOutputName = "";
        for (auto it : argvString) {
            if (it == "additionalNonPrompt") {
                infuseNonPrompt = true;
                outputFileName += "EnlargedNonprompt_";
            } else if (it == "CR") {
                outputFileName += "CR_";
                onlyCR = true;
            } else if (stringTools::stringContains(it, "timestamp=")) {
                timestampOutputName = stringTools::split(it, "=")[1];
            } else if (stringTools::stringContains(it, "tight") || stringTools::stringContains(it, "tight")) {
                leanEventSelection = false;
                outputFileName += "OriginalAn_";

                delete selection;
                selection = new EventFourT(jecUncertaintyFile);
            } else if (stringTools::stringContains(it, "region=")) {
                searchRegion = stringTools::split(it, "=")[1];
            } else if (stringTools::stringContains(it, "EventInfo")) {
                printEventTags = true;
            } else if (stringTools::stringContains(it, "TestCode")) {
                testRun = true;
            } else if (stringTools::stringContains(it, "plotString=")) {
                plotString = stringTools::split(it, "=")[1];
            } else if (stringTools::stringContains(it, "inclusiveClasses")) {
                overarchClasses = true;
                selection->setOverarchClasses(overarchClasses);
            }
        }


        std::string strippedSampleList = "";

        if (argvString[1] != "allSamples.txt") {
            strippedSampleList = stringTools::fileNameWithoutExtension(stringTools::splitDirectoryFileName(argvString[1]).second);
            std::string newYearString = "2018";
            Sample sampleZero = treeReader->sampleVector()[0];
            if (sampleZero.is2017()) {
                newYearString = "2017";
            } else if (sampleZero.is2016PostVFP()) {
                newYearString = "2016PostVFP";
            } else if (sampleZero.is2016PreVFP()) {
                newYearString = "2016PreVFP";
            }

            std::cout << "year string is: " << newYearString << std::endl;

            if (! analysisTools::checkYearStringNoErr( newYearString )) {
                std::cout << "force year string to 2018" << std::endl;
                newYearString = "2018";
            }
            setYearString(newYearString);
        }
        if (timestampOutputName != "") {
            oss << timestampOutputName;
        } else {
            oss << std::put_time(&tm, "%Y-%m-%d_%H-%M");
        }
        if (searchRegion != "All") {
            oss << "_" << searchRegion;
        }
        oss << "_" << strippedSampleList << ".root";

        outputFileName += oss.str();

        std::cout << outputFileName.c_str() << std::endl;

        outfile = new TFile(outputFileName.c_str(), "recreate");
        
        outfile->mkdir("Nominal");
        outfile->mkdir("Uncertainties");
        
        TH1F* intLuminosityMC = new TH1F("IntegratedLumiMC", "IntegratedLumiMC", 1, 0, 1);

        intLuminosityMC->SetBinContent(1, treeReader->getIntLumi());
        intLuminosityMC->Write("IntLumi", TObject::kOverwrite);
        
        // also write metadata here. This includes branch?
        #ifdef GIT_BRANCH
        std::string branchString = GIT_BRANCH;
        TObjString branchInfo(branchString.c_str());
        outfile->WriteObject(&branchInfo, "Branch");
        #endif

        #ifdef GIT_HASH
        std::string commithash = GIT_HASH;
        TObjString commitInfo(branchString.c_str());
        outfile->WriteObject(&commitInfo, "commit_hash");
        #endif
        
        std::stringstream time;

        if (timestampOutputName != "") {
            time << timestampOutputName;
        } else {
            time << std::put_time(&tm, "%Y_%m_%d-%H_%M");
        }
        //time << std::put_time(&tm, "%Y_%m_%d-%H_%M"); //);"%d_%m_%Y-%H_%M");
        TObjString timestamp(time.str().c_str());
        outfile->WriteObject(&timestamp, "Timestamp");
        if (argvString.size() > 2) {
            TObjString anType(argvString[2].c_str());
            outfile->WriteObject(&anType, "AN_Type");
        }

        TObjString eventSelectionType = "OriginalSelection";
        if (leanEventSelection) {
            eventSelectionType = "LooseSelection";
        }
        outfile->WriteObject(&eventSelectionType, "EventSelectionType");

        if (mode == 1) {
            createMVAHandlers();
        }
    } else {
        Sample samp = treeReader->sampleVector()[0];

        if (samp.is2018()) {
            yearString = "2018";
        } else if (samp.is2017()) {
            yearString = "2017";
        } else if (samp.is2016PostVFP()) {
            yearString = "2016PostVFP";
        } else {
            yearString = "2016PreVFP";
        }
    }
}

FourTop::~FourTop() {
    if (mva_DL) delete mva_DL;
    if (mva_ML) delete mva_ML;
    delete selection;

    if (MisIDRates) delete MisIDRates;
    if (FakeRatesElectron) delete FakeRatesElectron;
    if (FakeRatesMuon) delete FakeRatesMuon;

    if (currentEvent) delete currentEvent;
    //outfile->Close();
}

void FourTop::createMVAHandlers() {
    mva_DL = new MVAHandler_4T(MVAConfigs::TriClass_DL, selection, leanEventSelection);
    mva_ML = new MVAHandler_4T(MVAConfigs::TriClass_ML, selection, leanEventSelection);

    selection->setDLMVA(mva_DL);
    selection->setMLMVA(mva_ML);
}

void FourTop::addBTaggingNormFactors(ReweighterBTagShape* reweighter, std::string dir) {
    std::vector<Sample> sampleVector = treeReader->sampleVector();
    std::vector<std::string> variations = {"central", "up_hf", "up_lf", "up_hfstats1", "up_hfstats2",
                                           "up_lfstats1", "up_lfstats2", "up_cferr1", "up_cferr2",
                                           "down_hf", "down_lf", "down_hfstats1", "down_hfstats2",
                                           "down_lfstats1", "down_lfstats2", "down_cferr1", "down_cferr2"};

    std::vector<std::string> jec_variations_stat = {"Absolute_", "BBEC1_", "EC2_",
                                                    "HF_", "RelativeSample_"};// list of relevant JEC variations for each we want stuff added.

    std::vector<std::string> flavorQCDVariations = {"FlavorQCD_0", "FlavorQCD_4", "FlavorQCD_5"};
    
    for (auto samp : sampleVector) {
        std::vector<std::string> jec_variations = {"Absolute", "BBEC1", "EC2", "FlavorQCD", "HF", "RelativeBal"};
        if (samp.is2016() || samp.is2016PostVFP() || samp.is2016PreVFP()) {
            for (auto var : jec_variations_stat) {
                var += "2016";
                jec_variations.push_back(var);
            }
        } else if (samp.is2017()) {
            for (auto var : jec_variations_stat) {
                var += "2017";
                jec_variations.push_back(var);
            }
        } else if (samp.is2018()) {
            for (auto var : jec_variations_stat) {
                var += "2018";
                jec_variations.push_back(var);
            }
        }

        std::string sampleNormfactorsPath = stringTools::formatDirectoryName(dir) + stringTools::fileNameFromPath(samp.fileName());
        std::string sampleNormfactorsJecVarPath = stringTools::formatDirectoryName(dir) + stringTools::fileNameWithoutExtension(stringTools::fileNameFromPath(samp.fileName())) + "_JEC.root";
        std::string sampleNormfactorsFlavorQCDVarPath = stringTools::formatDirectoryName(dir) + stringTools::fileNameWithoutExtension(stringTools::fileNameFromPath(samp.fileName())) + "_FlQCDSplit.root";

        if( !systemTools::fileExists( sampleNormfactorsPath ) ){
            generateAllBTaggingNormFactorsSample(reweighter, samp, sampleNormfactorsPath, variations, false);
        }
        if( !systemTools::fileExists( sampleNormfactorsJecVarPath ) ){
            generateAllBTaggingNormFactorsSample(reweighter, samp, sampleNormfactorsJecVarPath, jec_variations, true);
        }
        if( !systemTools::fileExists( sampleNormfactorsFlavorQCDVarPath ) ){
            generateAllBTaggingNormFactorsSample(reweighter, samp, sampleNormfactorsFlavorQCDVarPath, flavorQCDVariations, true);
        }
        // loading in

        for (int nLep = 2; nLep < 5; nLep++) {
            TFile* btagNormFactorFile = TFile::Open(sampleNormfactorsPath.c_str());
            std::string nLepStr = std::to_string(nLep);
            for (auto var : variations) {
                std::shared_ptr<TH1> bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFile->Get((nLepStr + "L_bTagNormFactors_" + var).c_str())));
                int tr = 0;
                while (! bTagNormFactorsHist && tr < 10) {
                    btagNormFactorFile->Close();
                    btagNormFactorFile = TFile::Open(sampleNormfactorsPath.c_str());
                    bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFile->Get((nLepStr + "L_bTagNormFactors_" + var).c_str())));
                    tr++;
                }
                std::map<int, double> normFactors;

                for (int i=1; i<bTagNormFactorsHist->GetNbinsX() + 1; i++) {
                    double normFactor = bTagNormFactorsHist->GetBinContent(i);
                    if (fabs(normFactor) > 1e-6) {
                        normFactors[i-1] = normFactor;
                    }
                }

                reweighter->setNormFactors(samp, normFactors, var, nLep);
            }

            btagNormFactorFile->Close();

            TFile* btagNormFactorFileJec = TFile::Open(sampleNormfactorsJecVarPath.c_str());
            for (auto var : jec_variations) {
                for (std::string upOrDown : {"up", "down"}) {
                    std::string fullJecName = upOrDown + "_jes" + var;

                    std::shared_ptr<TH1> bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFileJec->Get((nLepStr + "L_bTagNormFactors_" + fullJecName).c_str())));
                    int tr = 0;
                    while (! bTagNormFactorsHist && tr < 10) {
                        btagNormFactorFileJec->Close();
                        btagNormFactorFileJec = TFile::Open(sampleNormfactorsJecVarPath.c_str());
                        bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFileJec->Get((nLepStr + "L_bTagNormFactors_" + var).c_str())));
                        tr++;
                    }

                    std::map<int, double> normFactors;

                    for (int i=1; i<bTagNormFactorsHist->GetNbinsX() + 1; i++) {
                        double normFactor = bTagNormFactorsHist->GetBinContent(i);
                        if (fabs(normFactor) > 1e-6) {
                            normFactors[i-1] = normFactor;
                        }
                    }
                    // up and down variations!!
                    reweighter->setNormFactors(samp, normFactors, fullJecName, nLep);
                }
            }
            btagNormFactorFileJec->Close();

            TFile* btagNormFactorFileFlavorQCD = TFile::Open(sampleNormfactorsFlavorQCDVarPath.c_str());
            for (auto var : flavorQCDVariations) {
                for (std::string upOrDown : {"up", "down"}) {
                    std::string fullJecName = upOrDown + "_jes" + var;

                    std::shared_ptr<TH1> bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFileFlavorQCD->Get((nLepStr + "L_bTagNormFactors_" + fullJecName).c_str())));
                    int tr = 0;
                    while (! bTagNormFactorsHist && tr < 10) {
                        btagNormFactorFileFlavorQCD->Close();
                        btagNormFactorFileFlavorQCD = TFile::Open(sampleNormfactorsFlavorQCDVarPath.c_str());
                        bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFileFlavorQCD->Get((nLepStr + "L_bTagNormFactors_" + var).c_str())));
                        tr++;
                    }

                    std::map<int, double> normFactors;

                    for (int i=1; i<bTagNormFactorsHist->GetNbinsX() + 1; i++) {
                        double normFactor = bTagNormFactorsHist->GetBinContent(i);
                        if (fabs(normFactor) > 1e-6) {
                            normFactors[i-1] = normFactor;
                        }
                    }
                    // up and down variations!!
                    reweighter->setNormFactors(samp, normFactors, fullJecName, nLep);
                }
            }
            btagNormFactorFileFlavorQCD->Close();
        }

    }
}


void cleanLastBins(std::shared_ptr<TH1D> num, std::shared_ptr<TH1D> denom) {
    for (int i = num->GetNbinsX()+1; i > 1; i--) {
        if (num->GetBinContent(i) < 1e-5) continue;
        if (num->GetBinError(i) * 10 < num->GetBinContent(i)) return;

        double errNum = sqrt(num->GetBinError(i) * num->GetBinError(i) + num->GetBinError(i-1) * num->GetBinError(i-1));
        num->SetBinContent(i-1, num->GetBinContent(i-1) + num->GetBinContent(i));
        num->SetBinError(i-1, errNum);
        
        num->SetBinContent(i, 0.);
        num->SetBinError(i, 0.);

        double errDenom = sqrt(denom->GetBinError(i) * denom->GetBinError(i) + denom->GetBinError(i-1) * denom->GetBinError(i-1));
        denom->SetBinContent(i-1, denom->GetBinContent(i-1) + denom->GetBinContent(i));
        denom->SetBinError(i-1, errDenom);

        denom->SetBinContent(i, 0.);
        denom->SetBinError(i, 0.);
    }
}


void FourTop::generateAllBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normFilePath, std::vector<std::string>& variations, bool jec) {
    // calculate the average of b-tag weights in a given sample
    // the return type is a map of jet multiplicity to average of weights
    // input arguments:
    // - sample: a Sample object
    // - numberOfEntries: number of entries to consider for the average of weights
    //   note: defaults to 0, in which case all entries in the file are used
    // note: for the averaging, each entry in the input sample is counted as 1, 
    //       regardless of lumi, cross-section, generator weight or other reweighting factors!

    // make a TreeReader
    std::string inputFilePath = samp.filePath();
    std::cout << "making TreeReader..." << std::endl;
    TreeReader tempTree;
    tempTree.initSampleFromFile( inputFilePath );

    // initialize the output map
    std::vector<std::map<std::string, std::shared_ptr<TH1D>>> averageOfWeightsMap;
    std::vector<std::map<std::string, std::shared_ptr<TH1D>>> nEntriesMap;

    std::vector<std::string> jecVarForSelection;
    std::vector<std::string> bTagVar;
    std::vector<unsigned> flavors = {0, 0, 4, 4, 5, 5};
    bool flavorQCD_Vars = false;
    for (int nLep = 2; nLep < 5; nLep++) {
        std::string nLepStr = std::to_string(nLep);
        std::map<std::string, std::shared_ptr<TH1D>> averageOfWeightsMapTmp;
        std::map<std::string, std::shared_ptr<TH1D>> nEntriesMapTmp;

        for (std::string var : variations) {
            if (stringTools::stringContains(var, "FlavorQCD_") && jec) {
                flavorQCD_Vars = true;
            }
            if (jec) {
                std::string jecVar = "up_jes" + var;
                jecVarForSelection.push_back(var+"Up");
                if (nLep == 2) bTagVar.push_back(jecVar);

                std::shared_ptr<TH1D> averageOfWeights = std::make_shared<TH1D>(("L" + nLepStr + "_bTagNormFactors" + samp.fileName() + jecVar).c_str(), "bTagNormFactors;Jets;Factor", 30, 0, 30);
                std::shared_ptr<TH1D> nEntries = std::make_shared<TH1D>(("L" + nLepStr + "_nEntries" + jecVar).c_str(), "nEntries;Jets;Entries", 30, 0, 30);

                averageOfWeights->Fill(0., 1.);
                nEntries->Fill(0., 1.);

                averageOfWeightsMapTmp[jecVar] = averageOfWeights;
                nEntriesMapTmp[jecVar] = nEntries;

                jecVar = "down_jes" + var;
                jecVarForSelection.push_back(var+"Down");
                if (nLep == 2) bTagVar.push_back(jecVar);

                std::shared_ptr<TH1D> averageOfWeights_down = std::make_shared<TH1D>(("L" + nLepStr + "_bTagNormFactors" + samp.fileName() + jecVar).c_str(), "bTagNormFactors;Jets;Factor", 30, 0, 30);
                std::shared_ptr<TH1D> nEntries_down = std::make_shared<TH1D>(("L" + nLepStr + "_nEntries" + jecVar).c_str(), "nEntries;Jets;Entries", 30, 0, 30);

                averageOfWeights_down->Fill(0., 1.);
                nEntries_down->Fill(0., 1.);

                averageOfWeightsMapTmp[jecVar] = averageOfWeights_down;
                nEntriesMapTmp[jecVar] = nEntries_down;
            } else {
                std::shared_ptr<TH1D> averageOfWeights = std::make_shared<TH1D>(("L" + nLepStr + "_bTagNormFactors" + samp.fileName() + var).c_str(), "bTagNormFactors;Jets;Factor", 30, 0, 30);
                std::shared_ptr<TH1D> nEntries = std::make_shared<TH1D>(("L" + nLepStr + "_nEntries" + var).c_str(), "nEntries;Jets;Entries", 30, 0, 30);

                averageOfWeights->Fill(0., 1.);
                nEntries->Fill(0., 1.);

                averageOfWeightsMapTmp[var] = averageOfWeights;
                nEntriesMapTmp[var] = nEntries;
            }
        }
        averageOfWeightsMap.push_back(averageOfWeightsMapTmp);
        nEntriesMap.push_back(nEntriesMapTmp);
    }
    
    if (! jec) bTagVar = variations;


    // loop over events
    long unsigned availableEntries = tempTree.numberOfEntries();

    std::cout << "starting event loop for " << availableEntries << " events..." << std::endl;

    for (long unsigned entry = 0; entry < availableEntries; ++entry) {
        //if (entry>1000) break;
        Event event;
        if (jec) event = tempTree.buildEvent(entry, false, false, false, true);
        else event = tempTree.buildEvent(entry);

        // do basic selection
        event.removeTaus();
        event.selectLooseLeptons();
        event.cleanElectronsFromLooseMuons();
        event.cleanJetsFromFOLeptons();

        if (event.numberOfFOLeptons() != event.numberOfTightLeptons()) continue;
        event.selectTightLeptons();

        if (considerRegion == eventClass::dy || considerRegion == eventClass::ttbar) {
            if (event.numberOfLeptons() == 2) continue;
            if (event.lepton(0).charge() == event.lepton(1).charge()) continue;
        } else {
            if (event.numberOfLeptons() < 2) continue;
            if (event.numberOfLeptons() == 2 && event.lepton(0).charge() != event.lepton(1).charge()) continue;
        }

        int njets = event.numberOfGoodJets();
        double ht = event.HT();

        for (unsigned i=0; i < bTagVar.size(); i++) {
            std::string bVar = bTagVar[i];

            JetCollection currentJets;
            if (jec && ! flavorQCD_Vars) {
                currentJets = event.getJetCollection(jecVarForSelection[i]);
                njets = currentJets.size();
                ht = currentJets.scalarPtSum();
            } else if (jec && flavorQCD_Vars) {
                bool up = i % 2 == 0;
                if (up) {
                    currentJets = event.jetCollection().JECUpGroupedFlavorQCD(flavors[i]);
                } else {
                    currentJets = event.jetCollection().JECDownGroupedFlavorQCD(flavors[i]);
                }
                njets = currentJets.size();
                ht = currentJets.scalarPtSum();
            }

            if (njets < 2) continue;
            if (considerRegion != eventClass::dy || considerRegion != eventClass::ttbar) {
                if (event.numberOfLeptons() < 4 && ht < 200) continue;
            }
            //std::cout << "before weights" << std::endl;

            double btagreweight;
            if (jec && ! flavorQCD_Vars) {
                std::string varname;
                if (stringTools::stringEndsWith(jecVarForSelection[i], "Up")) {
                    varname = jecVarForSelection[i].substr(0, jecVarForSelection[i].size() - 2);
                }
                else if (stringTools::stringEndsWith(jecVarForSelection[i], "Down")) {
                    varname = jecVarForSelection[i].substr(0, jecVarForSelection[i].size() - 4);
                }
                btagreweight = reweighter->weightJecVar(event, jecVarForSelection[i], true, event.jetInfo().groupedJECVariationsMap()->at(varname));

            } else if (jec && flavorQCD_Vars) {
                bool up = i % 2 == 0;
                if (up) {
                    btagreweight = reweighter->weightJecVar_FlavorFilter(event, "FlavorQCDUp", flavors[i]);
                } else {
                    btagreweight = reweighter->weightJecVar_FlavorFilter(event, "FlavorQCDDown", flavors[i]);
                }
            } else {
                btagreweight = reweighter->weightVariation(event, bVar);
            }
            //std::cout << "got weights: " << event.numberOfTightLeptons();


            if (event.numberOfTightLeptons() > 4) {
                averageOfWeightsMap[2][bVar]->Fill(njets, btagreweight);
                nEntriesMap[2][bVar]->Fill(njets, 1.);
            } else {
                averageOfWeightsMap[event.numberOfTightLeptons()-2][bVar]->Fill(njets, btagreweight);
                nEntriesMap[event.numberOfTightLeptons()-2][bVar]->Fill(njets, 1.);
            }
            //std::cout << "filled maps" << std::endl;

        }
    }
    //std::cout << "flling file" << std::endl;

    TFile* normFile = TFile::Open( normFilePath.c_str(), "RECREATE" );

    for (int nLep = 2; nLep<5; nLep++) {
        for (unsigned i=0; i < bTagVar.size(); i++) {
            normFile->cd();
            std::string bVar = bTagVar[i];
            std::string nLepStr = std::to_string(nLep);
            cleanLastBins(averageOfWeightsMap[nLep-2][bVar], nEntriesMap[nLep-2][bVar]);
            averageOfWeightsMap[nLep-2][bVar]->Divide(nEntriesMap[nLep-2][bVar].get());
            averageOfWeightsMap[nLep-2][bVar]->Write((nLepStr + "L_bTagNormFactors_" + bVar).c_str(), TObject::kOverwrite);
        }
    }

    normFile->Close();
}

void FourTop::generateMatrix() {
    inverted_matrix = new std::vector<std::vector<double>>();
    *inverted_matrix = {
        {35.99999999999888, -53.999999999997826, -53.999999999997954, -53.99999999999805, -53.999999999997954, -53.99999999999791, -53.99999999999792, -53.9999999999977, 17.99999999999917, 35.999999999998515, 35.99999999999851, 35.99999999999845, 35.99999999999844, 35.99999999999842, 35.99999999999836, 17.99999999999929, 35.999999999998565, 35.99999999999856, 35.99999999999855, 35.99999999999857, 35.99999999999844, 17.999999999999282, 35.99999999999861, 35.99999999999856, 35.999999999998536, 35.999999999998494, 17.999999999999304, 35.99999999999856, 35.999999999998565, 35.99999999999848, 17.99999999999926, 35.999999999998494, 35.99999999999842, 17.99999999999925, 35.99999999999846, 17.999999999999165},
        {-53.99999999999803, 233.99999999999656, 80.99999999999646, 80.99999999999653, 80.9999999999965, 80.99999999999645, 80.99999999999636, 80.99999999999602, -107.99999999999869, -125.99999999999754, -125.9999999999975, -125.99999999999746, -125.9999999999974, -125.9999999999974, -125.9999999999973, -26.999999999998728, -53.99999999999749, -53.99999999999747, -53.999999999997456, -53.99999999999741, -53.99999999999726, -26.999999999998725, -53.99999999999754, -53.99999999999752, -53.99999999999739, -53.99999999999732, -26.99999999999877, -53.99999999999752, -53.999999999997414, -53.999999999997335, -26.999999999998735, -53.999999999997385, -53.99999999999728, -26.99999999999864, -53.99999999999721, -26.999999999998536},
        {-53.99999999999786, 80.99999999999585, 233.99999999999642, 80.99999999999622, 80.99999999999596, 80.99999999999594, 80.99999999999594, 80.99999999999558, -26.99999999999844, -125.99999999999734, -53.999999999997165, -53.99999999999705, -53.99999999999707, -53.999999999997016, -53.99999999999692, -107.99999999999883, -125.99999999999743, -125.99999999999727, -125.99999999999731, -125.99999999999739, -125.99999999999717, -26.999999999998625, -53.99999999999722, -53.99999999999723, -53.99999999999716, -53.999999999997094, -26.999999999998558, -53.99999999999714, -53.999999999997094, -53.99999999999704, -26.999999999998526, -53.99999999999707, -53.99999999999699, -26.999999999998536, -53.99999999999699, -26.999999999998376},
        {-53.999999999997875, 80.99999999999586, 80.99999999999625, 233.99999999999648, 80.99999999999629, 80.99999999999619, 80.99999999999605, 80.99999999999575, -26.99999999999843, -53.99999999999719, -125.99999999999727, -53.99999999999712, -53.999999999997065, -53.999999999997044, -53.99999999999689, -26.999999999998654, -125.99999999999739, -53.99999999999735, -53.999999999997314, -53.99999999999729, -53.99999999999709, -107.99999999999874, -125.9999999999974, -125.99999999999743, -125.99999999999731, -125.99999999999726, -26.999999999998703, -53.999999999997364, -53.999999999997286, -53.99999999999715, -26.999999999998643, -53.99999999999721, -53.99999999999707, -26.999999999998607, -53.99999999999712, -26.999999999998444},
        {-53.99999999999796, 80.99999999999582, 80.99999999999629, 80.9999999999964, 233.99999999999676, 80.99999999999635, 80.99999999999622, 80.9999999999959, -26.99999999999839, -53.99999999999714, -53.999999999997186, -125.99999999999723, -53.99999999999713, -53.99999999999707, -53.999999999996945, -26.99999999999864, -53.999999999997364, -125.99999999999746, -53.99999999999734, -53.9999999999973, -53.99999999999716, -26.999999999998717, -125.9999999999976, -53.999999999997435, -53.99999999999736, -53.99999999999725, -107.99999999999896, -125.9999999999976, -125.99999999999753, -125.99999999999737, -26.999999999998682, -53.99999999999734, -53.99999999999721, -26.999999999998654, -53.99999999999721, -26.9999999999985},
        {-53.99999999999786, 80.99999999999567, 80.99999999999613, 80.99999999999626, 80.99999999999604, 233.99999999999596, 80.99999999999608, 80.99999999999584, -26.99999999999832, -53.99999999999703, -53.99999999999711, -53.99999999999694, -125.99999999999689, -53.99999999999697, -53.99999999999689, -26.999999999998582, -53.999999999997264, -53.99999999999715, -125.99999999999713, -53.99999999999719, -53.999999999997065, -26.999999999998668, -53.999999999997286, -125.99999999999723, -53.99999999999725, -53.999999999997186, -26.999999999998554, -125.99999999999713, -53.99999999999719, -53.99999999999709, -107.99999999999856, -125.99999999999709, -125.99999999999699, -26.9999999999986, -53.99999999999714, -26.999999999998504},
        {-53.999999999997534, 80.99999999999528, 80.99999999999561, 80.99999999999564, 80.99999999999564, 80.99999999999564, 233.99999999999503, 80.999999999995, -26.999999999998206, -53.999999999996724, -53.999999999996774, -53.99999999999671, -53.9999999999967, -125.99999999999643, -53.99999999999641, -26.999999999998398, -53.99999999999687, -53.999999999996874, -53.99999999999689, -125.99999999999662, -53.999999999996575, -26.999999999998426, -53.99999999999693, -53.99999999999693, -125.99999999999658, -53.99999999999664, -26.999999999998444, -53.999999999996945, -125.99999999999665, -53.99999999999664, -26.999999999998437, -125.99999999999662, -53.99999999999664, -107.9999999999982, -125.99999999999638, -26.99999999999813},
        {-53.99999999999767, 80.99999999999535, 80.99999999999581, 80.9999999999959, 80.99999999999582, 80.99999999999575, 80.99999999999581, 233.9999999999952, -26.99999999999823, -53.999999999996824, -53.999999999996874, -53.99999999999679, -53.999999999996746, -53.999999999996774, -125.99999999999649, -26.999999999998483, -53.99999999999703, -53.99999999999703, -53.99999999999699, -53.999999999997016, -125.99999999999675, -26.999999999998536, -53.9999999999971, -53.999999999997044, -53.99999999999703, -125.99999999999677, -26.999999999998504, -53.999999999997044, -53.99999999999705, -125.99999999999679, -26.99999999999845, -53.99999999999699, -125.99999999999672, -26.999999999998515, -125.9999999999968, -107.99999999999828},
        {17.999999999999307, -107.99999999999878, -26.99999999999873, -26.999999999998753, -26.99999999999875, -26.999999999998746, -26.999999999998664, -26.99999999999859, 53.99999999999955, 53.99999999999914, 53.99999999999912, 53.99999999999912, 53.9999999999991, 53.9999999999991, 53.99999999999906, 8.999999999999538, 17.9999999999991, 17.999999999999094, 17.99999999999909, 17.99999999999906, 17.99999999999903, 8.999999999999542, 17.99999999999911, 17.99999999999911, 17.999999999999062, 17.999999999999037, 8.999999999999552, 17.999999999999115, 17.99999999999906, 17.999999999999037, 8.999999999999547, 17.999999999999062, 17.999999999999027, 8.999999999999506, 17.999999999998995, 8.999999999999472 },
        {35.99999999999853, -125.99999999999714, -125.99999999999741, -53.99999999999736, -53.99999999999724, -53.999999999997264, -53.999999999997165, -53.999999999997016, 53.99999999999895, 143.99999999999812, 71.99999999999805, 71.99999999999801, 71.999999999998, 71.99999999999795, 71.99999999999791, 53.99999999999907, 71.99999999999815, 71.9999999999981, 71.99999999999808, 71.99999999999805, 71.99999999999798, 17.999999999999034, 35.99999999999808, 35.99999999999811, 35.99999999999801, 35.99999999999797, 17.999999999999012, 35.99999999999807, 35.99999999999799, 35.99999999999793, 17.99999999999901, 35.999999999997996, 35.99999999999792, 17.999999999998966, 35.99999999999788, 17.999999999998877},
        {35.999999999998536, -125.99999999999719, -53.99999999999741, -125.99999999999746, -53.9999999999974, -53.99999999999736, -53.99999999999725, -53.99999999999706, 53.999999999998934, 71.99999999999808, 143.9999999999981, 71.99999999999804, 71.99999999999802, 71.99999999999801, 71.9999999999979, 17.99999999999906, 71.99999999999815, 35.99999999999813, 35.9999999999981, 35.99999999999808, 35.99999999999799, 53.99999999999908, 71.99999999999817, 71.99999999999817, 71.9999999999981, 71.99999999999802, 17.99999999999907, 35.99999999999816, 35.99999999999808, 35.99999999999799, 17.999999999999037, 35.99999999999806, 35.999999999997996, 17.999999999998998, 35.99999999999794, 17.9999999999989 },
        {35.99999999999857, -125.99999999999707, -53.999999999997385, -53.99999999999745, -125.99999999999757, -53.99999999999744, -53.999999999997286, -53.999999999997115, 53.99999999999889, 71.99999999999801, 71.99999999999805, 143.9999999999981, 71.999999999998, 71.99999999999797, 71.9999999999979, 17.99999999999904, 35.99999999999813, 71.99999999999818, 35.999999999998124, 35.99999999999809, 35.99999999999802, 17.999999999999073, 71.99999999999824, 35.99999999999818, 35.9999999999981, 35.99999999999804, 53.99999999999919, 71.99999999999825, 71.99999999999814, 71.99999999999808, 17.999999999999073, 35.9999999999981, 35.999999999998025, 17.99999999999902, 35.999999999997975, 17.99999999999893 },
        {35.999999999998515, -125.99999999999709, -53.99999999999732, -53.999999999997385, -53.99999999999727, -125.99999999999727, -53.99999999999725, -53.999999999997115, 53.99999999999888, 71.99999999999798, 71.99999999999801, 71.99999999999793, 143.99999999999793, 71.99999999999793, 71.99999999999787, 17.99999999999902, 35.9999999999981, 35.999999999998046, 71.99999999999805, 35.99999999999804, 35.99999999999797, 17.99999999999906, 35.9999999999981, 71.99999999999811, 35.99999999999806, 35.999999999998025, 17.99999999999901, 71.99999999999807, 35.999999999998025, 35.99999999999797, 53.99999999999902, 71.99999999999801, 71.99999999999794, 17.999999999999, 35.99999999999796, 17.999999999998938},
        {35.99999999999837, -125.9999999999969, -53.99999999999708, -53.99999999999709, -53.99999999999709, -53.99999999999711, -125.99999999999675, -53.99999999999672, 53.99999999999882, 71.99999999999783, 71.99999999999785, 71.99999999999783, 71.99999999999783, 143.99999999999767, 71.99999999999763, 17.999999999998938, 35.99999999999792, 35.99999999999792, 35.99999999999794, 71.99999999999775, 35.99999999999774, 17.999999999998945, 35.99999999999795, 35.999999999997954, 71.99999999999773, 35.999999999997776, 17.999999999998956, 35.99999999999797, 71.99999999999777, 35.99999999999777, 17.999999999998966, 71.99999999999778, 35.99999999999778, 53.99999999999879, 71.9999999999976, 17.999999999998764},
        {35.99999999999844, -125.99999999999694, -53.99999999999718, -53.999999999997215, -53.999999999997186, -53.99999999999717, -53.999999999997144, -125.9999999999968, 53.99999999999884, 71.99999999999788, 71.99999999999791, 71.99999999999787, 71.99999999999785, 71.99999999999784, 143.99999999999767, 17.999999999998977, 35.999999999997996, 35.99999999999799, 35.99999999999799, 35.999999999997975, 71.99999999999781, 17.999999999998998, 35.99999999999802, 35.99999999999801, 35.99999999999797, 71.99999999999783, 17.999999999998984, 35.99999999999801, 35.99999999999798, 71.99999999999783, 17.999999999998973, 35.99999999999796, 71.99999999999781, 17.999999999998977, 71.99999999999781, 53.99999999999882 },
        {17.999999999999215, -26.999999999998458, -107.9999999999987, -26.999999999998586, -26.999999999998487, -26.999999999998497, -26.99999999999848, -26.999999999998384, 8.999999999999428, 53.99999999999905, 17.999999999998945, 17.999999999998906, 17.999999999998913, 17.999999999998888, 17.999999999998852, 53.99999999999959, 53.999999999999076, 53.99999999999901, 53.99999999999901, 53.999999999999034, 53.99999999999898, 8.999999999999487, 17.999999999998952, 17.99999999999897, 17.999999999998927, 17.999999999998902, 8.999999999999451, 17.99999999999894, 17.999999999998902, 17.99999999999888, 8.999999999999451, 17.999999999998902, 17.999999999998874, 8.999999999999451, 17.999999999998867, 8.999999999999398},
        {35.999999999998444, -53.99999999999699, -125.99999999999739, -125.9999999999973, -53.99999999999715, -53.99999999999711, -53.99999999999706, -53.99999999999684, 17.999999999998856, 71.99999999999801, 71.99999999999797, 35.99999999999786, 35.99999999999784, 35.999999999997826, 35.99999999999773, 53.999999999999055, 143.99999999999807, 71.99999999999802, 71.99999999999801, 71.999999999998, 71.9999999999979, 53.99999999999901, 71.99999999999801, 71.99999999999805, 71.99999999999794, 71.9999999999979, 17.99999999999898, 35.999999999997975, 35.999999999997925, 35.999999999997826, 17.999999999998963, 35.99999999999791, 35.999999999997826, 17.999999999998938, 35.9999999999978, 17.999999999998835},
        {35.99999999999847, -53.999999999996966, -125.99999999999739, -53.999999999997286, -125.99999999999734, -53.99999999999719, -53.99999999999713, -53.999999999996916, 17.99999999999884, 71.99999999999797, 35.999999999997925, 71.9999999999979, 35.99999999999786, 35.99999999999784, 35.99999999999775, 53.999999999999055, 71.99999999999808, 143.9999999999981, 71.99999999999804, 71.99999999999801, 71.99999999999794, 17.99999999999903, 71.99999999999808, 35.99999999999807, 35.99999999999798, 35.99999999999792, 53.99999999999909, 71.99999999999808, 71.99999999999802, 71.99999999999793, 17.999999999998984, 35.99999999999797, 35.999999999997875, 17.999999999998966, 35.99999999999787, 17.999999999998863},
        {35.99999999999842, -53.99999999999688, -125.9999999999973, -53.999999999997215, -53.99999999999703, -125.999999999997, -53.999999999997065, -53.99999999999691, 17.999999999998813, 71.99999999999793, 35.99999999999788, 35.99999999999777, 71.99999999999774, 35.99999999999779, 35.99999999999772, 53.999999999999034, 71.99999999999804, 71.99999999999794, 143.99999999999793, 71.99999999999798, 71.99999999999788, 17.999999999999, 35.99999999999796, 71.99999999999798, 35.99999999999794, 35.99999999999788, 17.99999999999892, 71.99999999999788, 35.99999999999788, 35.99999999999782, 53.99999999999891, 71.99999999999785, 71.99999999999777, 17.99999999999894, 35.999999999997826, 17.999999999998863},
        {35.99999999999828, -53.99999999999671, -125.99999999999707, -53.99999999999692, -53.99999999999687, -53.999999999996874, -125.99999999999658, -53.99999999999652, 17.999999999998753, 71.99999999999778, 35.99999999999774, 35.99999999999768, 35.999999999997684, 71.99999999999753, 35.99999999999752, 53.99999999999894, 71.99999999999785, 71.99999999999781, 71.99999999999783, 143.9999999999977, 71.99999999999766, 17.999999999998888, 35.999999999997804, 35.99999999999783, 71.99999999999761, 35.999999999997655, 17.999999999998867, 35.9999999999978, 71.99999999999763, 35.99999999999762, 17.99999999999887, 71.99999999999763, 35.99999999999762, 53.99999999999874, 71.99999999999748, 17.999999999998696},
        {35.99999999999834, -53.99999999999677, -125.99999999999716, -53.999999999997044, -53.99999999999695, -53.99999999999694, -53.99999999999697, -125.9999999999966, 17.99999999999877, 71.99999999999781, 35.99999999999778, 35.99999999999771, 35.9999999999977, 35.99999999999771, 71.99999999999753, 53.99999999999898, 71.99999999999793, 71.99999999999788, 71.99999999999788, 71.9999999999979, 143.99999999999773, 17.999999999998945, 35.999999999997875, 35.999999999997875, 35.999999999997854, 71.9999999999977, 17.999999999998895, 35.99999999999783, 35.99999999999783, 71.99999999999767, 17.999999999998884, 35.99999999999782, 71.99999999999764, 17.99999999999892, 71.99999999999768, 53.99999999999874},
        {17.99999999999921, -26.999999999998472, -26.999999999998636, -107.9999999999987, -26.999999999998636, -26.999999999998597, -26.999999999998547, -26.99999999999844, 8.999999999999417, 17.99999999999896, 53.999999999999005, 17.99999999999894, 17.99999999999892, 17.999999999998913, 17.999999999998852, 8.999999999999506, 53.99999999999904, 17.99999999999902, 17.999999999999005, 17.999999999998984, 17.99999999999893, 53.99999999999955, 53.99999999999906, 53.99999999999906, 53.99999999999901, 53.99999999999899, 8.999999999999515, 17.999999999999023, 17.999999999998987, 17.999999999998945, 8.999999999999497, 17.99999999999898, 17.999999999998924, 8.99999999999948, 17.999999999998916, 8.99999999999942 },
        {35.99999999999849, -53.99999999999698, -53.99999999999731, -125.99999999999739, -125.99999999999751, -53.99999999999729, -53.999999999997215, -53.99999999999699, 17.999999999998835, 35.99999999999793, 71.99999999999798, 71.99999999999794, 35.99999999999789, 35.99999999999787, 35.99999999999776, 17.99999999999902, 71.9999999999981, 71.99999999999812, 35.99999999999805, 35.99999999999802, 35.99999999999792, 53.99999999999908, 143.99999999999818, 71.99999999999814, 71.99999999999805, 71.99999999999801, 53.999999999999154, 71.99999999999817, 71.9999999999981, 71.99999999999801, 17.999999999999023, 35.99999999999803, 35.99999999999793, 17.999999999998987, 35.999999999997925, 17.99999999999888 },
        {35.99999999999844, -53.99999999999691, -53.99999999999725, -125.99999999999736, -53.999999999997186, -125.99999999999714, -53.999999999997144, -53.99999999999696, 17.99999999999881, 35.99999999999788, 71.99999999999797, 35.999999999997826, 71.99999999999777, 35.99999999999782, 35.99999999999774, 17.99999999999899, 71.99999999999804, 35.99999999999799, 71.99999999999797, 35.999999999997975, 35.999999999997875, 53.999999999999076, 71.99999999999807, 143.99999999999807, 71.99999999999805, 71.999999999998, 17.999999999998977, 71.99999999999798, 35.99999999999797, 35.999999999997875, 53.99999999999897, 71.99999999999794, 71.99999999999781, 17.999999999998973, 35.99999999999789, 17.999999999998884},
        {35.999999999998295, -53.999999999996724, -53.999999999996994, -125.99999999999702, -53.999999999996994, -53.99999999999696, -125.99999999999667, -53.99999999999659, 17.999999999998757, 35.99999999999773, 71.9999999999978, 35.99999999999772, 35.999999999997705, 71.99999999999757, 35.99999999999752, 17.999999999998906, 71.99999999999787, 35.999999999997854, 35.99999999999785, 71.9999999999977, 35.999999999997655, 53.99999999999894, 71.9999999999979, 71.9999999999979, 143.9999999999977, 71.99999999999771, 17.999999999998927, 35.999999999997875, 71.99999999999771, 35.999999999997684, 17.999999999998913, 71.9999999999977, 35.999999999997684, 53.99999999999879, 71.99999999999754, 17.999999999998725},
        {35.99999999999836, -53.99999999999677, -53.999999999997094, -125.99999999999716, -53.99999999999709, -53.99999999999702, -53.99999999999704, -125.99999999999669, 17.999999999998767, 35.99999999999778, 71.99999999999784, 35.99999999999776, 35.99999999999773, 35.99999999999775, 71.99999999999754, 17.999999999998945, 71.99999999999794, 35.99999999999792, 35.9999999999979, 35.999999999997904, 71.99999999999773, 53.99999999999899, 71.99999999999798, 71.99999999999795, 71.99999999999793, 143.99999999999778, 17.999999999998952, 35.99999999999792, 35.999999999997925, 71.99999999999774, 17.999999999998916, 35.99999999999788, 71.99999999999771, 17.99999999999895, 71.99999999999775, 53.999999999998785},
        {17.999999999999257, -26.999999999998444, -26.99999999999863, -26.999999999998682, -107.99999999999889, -26.999999999998675, -26.99999999999863, -26.999999999998508, 8.999999999999398, 17.99999999999893, 17.99999999999896, 53.999999999999, 17.99999999999894, 17.999999999998916, 17.99999999999887, 8.999999999999492, 17.999999999999027, 53.99999999999909, 17.999999999999023, 17.999999999999005, 17.99999999999896, 8.99999999999953, 53.99999999999915, 17.999999999999066, 17.99999999999903, 17.999999999998998, 53.99999999999966, 53.99999999999916, 53.99999999999912, 53.99999999999906, 8.999999999999522, 17.99999999999904, 17.999999999998984, 8.99999999999951, 17.99999999999898, 8.999999999999453},
        {35.99999999999848, -53.99999999999688, -53.99999999999724, -53.999999999997335, -125.99999999999739, -125.99999999999721, -53.99999999999723, -53.99999999999703, 17.99999999999879, 35.99999999999786, 35.99999999999791, 71.99999999999784, 71.99999999999783, 35.99999999999781, 35.99999999999775, 17.999999999998977, 35.999999999998046, 71.99999999999804, 71.99999999999798, 35.99999999999798, 35.999999999997904, 17.99999999999905, 71.99999999999812, 71.99999999999807, 35.99999999999804, 35.99999999999799, 53.99999999999908, 143.9999999999981, 71.99999999999807, 71.99999999999797, 53.999999999999, 71.99999999999798, 71.9999999999979, 17.999999999998998, 35.99999999999796, 17.999999999998924},
        {35.99999999999833, -53.99999999999671, -53.99999999999701, -53.99999999999706, -125.99999999999724, -53.999999999997065, -125.99999999999677, -53.99999999999667, 17.99999999999874, 35.999999999997726, 35.99999999999777, 71.99999999999777, 35.99999999999773, 71.99999999999758, 35.99999999999754, 17.999999999998906, 35.99999999999787, 71.99999999999793, 35.99999999999788, 71.99999999999774, 35.999999999997684, 17.99999999999894, 71.99999999999798, 35.99999999999793, 71.99999999999773, 35.999999999997755, 53.999999999999055, 71.99999999999801, 143.99999999999784, 71.9999999999978, 17.99999999999895, 71.99999999999778, 35.99999999999775, 53.99999999999883, 71.99999999999761, 17.999999999998757},
        {35.9999999999984, -53.99999999999676, -53.999999999997115, -53.99999999999717, -125.99999999999733, -53.99999999999713, -53.99999999999713, -125.99999999999675, 17.999999999998753, 35.99999999999777, 35.99999999999782, 71.99999999999781, 35.999999999997755, 35.99999999999777, 71.99999999999757, 17.99999999999894, 35.99999999999795, 71.999999999998, 35.99999999999794, 35.99999999999794, 71.99999999999777, 17.999999999998995, 71.99999999999807, 35.99999999999799, 35.999999999997975, 71.99999999999781, 53.99999999999908, 71.99999999999805, 71.99999999999805, 143.99999999999787, 17.99999999999896, 35.99999999999796, 71.99999999999778, 17.999999999998984, 71.99999999999783, 53.99999999999882 },
        {17.999999999999194, -26.999999999998362, -26.999999999998536, -26.999999999998604, -26.9999999999985, -107.9999999999985, -26.99999999999855, -26.999999999998476, 8.999999999999364, 17.99999999999887, 17.99999999999891, 17.99999999999884, 53.99999999999882, 17.999999999998863, 17.999999999998835, 8.999999999999458, 17.99999999999897, 17.99999999999892, 53.99999999999892, 17.999999999998945, 17.999999999998906, 8.999999999999504, 17.999999999998977, 53.99999999999897, 17.999999999998977, 17.99999999999896, 8.999999999999448, 53.99999999999892, 17.999999999998945, 17.99999999999891, 53.99999999999946, 53.99999999999891, 53.99999999999887, 8.999999999999483, 17.99999999999894, 8.999999999999453 },
        {35.99999999999828, -53.99999999999663, -53.99999999999692, -53.99999999999699, -53.99999999999689, -125.99999999999683, -125.9999999999967, -53.99999999999664, 17.999999999998703, 35.99999999999767, 35.999999999997726, 35.999999999997634, 71.99999999999761, 71.99999999999753, 35.999999999997506, 17.99999999999887, 35.99999999999781, 35.99999999999776, 71.99999999999777, 71.99999999999767, 35.99999999999764, 17.99999999999892, 35.99999999999784, 71.99999999999783, 71.99999999999767, 35.999999999997726, 17.99999999999887, 71.99999999999778, 71.99999999999767, 35.99999999999767, 53.99999999999887, 143.99999999999764, 71.99999999999763, 53.99999999999879, 71.99999999999758, 17.99999999999876},
        {35.99999999999836, -53.99999999999669, -53.99999999999704, -53.999999999997115, -53.999999999996994, -125.99999999999694, -53.99999999999707, -125.99999999999677, 17.999999999998725, 35.99999999999773, 35.999999999997776, 35.999999999997684, 71.99999999999764, 35.99999999999772, 71.99999999999756, 17.999999999998913, 35.999999999997904, 35.999999999997854, 71.99999999999784, 35.9999999999979, 71.99999999999774, 17.999999999998977, 35.999999999997925, 71.9999999999979, 35.999999999997925, 71.9999999999978, 17.999999999998902, 71.99999999999784, 35.999999999997904, 71.99999999999775, 53.999999999998884, 71.99999999999784, 143.9999999999977, 17.999999999998966, 71.99999999999781, 53.99999999999884},
        {17.99999999999902, -26.999999999998145, -26.999999999998256, -26.999999999998266, -26.999999999998277, -26.999999999998288, -107.999999999998, -26.99999999999802, 8.999999999999297, 17.999999999998707, 17.999999999998728, 17.99999999999871, 17.99999999999871, 53.99999999999857, 17.999999999998575, 8.999999999999362, 17.999999999998757, 17.999999999998764, 17.999999999998774, 53.99999999999863, 17.999999999998636, 8.999999999999375, 17.99999999999878, 17.99999999999879, 53.99999999999861, 17.999999999998668, 8.999999999999385, 17.9999999999988, 53.99999999999865, 17.999999999998668, 8.99999999999939, 53.99999999999865, 17.99999999999868, 53.99999999999926, 53.99999999999853, 8.99999999999925 },
        {35.99999999999821, -53.99999999999651, -53.99999999999681, -53.99999999999683, -53.99999999999682, -53.99999999999681, -125.9999999999966, -125.99999999999638, 17.99999999999867, 35.99999999999759, 35.99999999999763, 35.99999999999758, 35.99999999999757, 71.99999999999747, 71.99999999999734, 17.999999999998835, 35.99999999999772, 35.999999999997726, 35.999999999997726, 71.99999999999763, 71.99999999999751, 17.999999999998863, 35.99999999999776, 35.99999999999776, 71.99999999999761, 71.99999999999754, 17.999999999998852, 35.99999999999776, 71.99999999999766, 71.99999999999756, 17.999999999998845, 71.99999999999764, 71.99999999999754, 53.99999999999878, 143.99999999999747, 53.999999999998664},
        {17.99999999999909, -26.999999999998185, -26.999999999998366, -26.999999999998394, -26.999999999998376, -26.99999999999834, -26.999999999998376, -107.99999999999814, 8.999999999999307, 17.999999999998753, 17.999999999998778, 17.99999999999875, 17.99999999999873, 17.999999999998746, 53.99999999999862, 8.999999999999403, 17.999999999998842, 17.999999999998842, 17.999999999998824, 17.999999999998842, 53.99999999999873, 8.999999999999432, 17.99999999999887, 17.99999999999885, 17.999999999998852, 53.99999999999874, 8.999999999999414, 17.999999999998845, 17.999999999998863, 53.99999999999875, 8.999999999999394, 17.999999999998835, 53.99999999999872, 8.999999999999428, 53.99999999999876, 53.99999999999933 }
    };

    combinations = new std::map<int, std::vector<int>>();
    *combinations = {{0, {}}, {1,  {0}}, {2,  {1}}, {3,  {2}}, {4,  {3}}, {5,  {4}}, {6,  {5}}, {7,  {6}},
                    {8,  {0, 0}}, {9,  {0, 1}}, {10, {0, 2}}, {11, {0, 3}}, {12, {0, 4}}, {13, {0, 5}}, {14, {0, 6}}, {15, {1, 1}},
                    {16, {1, 2}}, {17, {1, 3}}, {18, {1, 4}}, {19, {1, 5}}, {20, {1, 6}}, {21, {2, 2}}, {22, {2, 3}}, {23, {2, 4}},
                    {24, {2, 5}}, {25, {2, 6}}, {26, {3, 3}}, {27, {3, 4}}, {28, {3, 5}}, {29, {3, 6}}, {30, {4, 4}}, {31, {4, 5}},
                    {32, {4, 6}}, {33, {5, 5}}, {34, {5, 6}}, {35, {6, 6}}};
    param_points = new std::vector<std::vector<double>>();
    *param_points = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                     {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                     {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                     {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                     {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
                     {2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                     {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                     {1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
                     {1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                     {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                     {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                     {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
                     {0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                     {0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0},
                     {0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                     {0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                     {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                     {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0},
                     {0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0},
                     {0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0},
                     {0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0},
                     {0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0},
                     {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
                     {0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0},
                     {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0},
                     {0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0},
                     {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0},
                     {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0},
                     {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0},
                     {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0}                
    };
}

std::vector<double> FourTop::transformWeights(unsigned nWeights, double* eftWeights) {
    std::vector<double> transformed_vector;
    for (unsigned i=0; i<36; i++) {
        transformed_vector.push_back(calcEntry(nWeights, eftWeights, (*combinations)[i]));
    }
    // elke rij van inverse matrix overlopen en element per element vermenigvuldigen en optellen met transformed_vector
    std::vector<double> final_weights;
    for (auto row : *inverted_matrix) {
        double new_elem = 0.;
        for (unsigned j=0; j<row.size(); j++){
            new_elem += row[j] * transformed_vector[j];
        }
        final_weights.push_back(new_elem);
    }
    return final_weights;
}

double FourTop::calcEntry(unsigned nWeights, double* eftWeights, std::vector<int>& combination) {
    double sum = 0.;
    for (unsigned i=0; i<nWeights; i++) {
        double prod = 1.;
        for (auto param : combination) {
            prod *= (*param_points)[i][param];
        }
        sum += eftWeights[i] * prod;
    }
    return sum / 36.;
}

