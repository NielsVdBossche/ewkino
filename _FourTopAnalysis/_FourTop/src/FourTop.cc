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
    selection = new EventFourTLoose();

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
                selection = new EventFourT();
            } else if (stringTools::stringContains(it, "region=")) {
                searchRegion = stringTools::split(it, "=")[1];
            } else if (stringTools::stringContains(it, "EventInfo")) {
                printEventTags = true;
            } else if (stringTools::stringContains(it, "TestCode")) {
                testRun = true;
            } else if (stringTools::stringContains(it, "plotString=")) {
                plotString = stringTools::split(it, "=")[1];
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
        TFile* btagNormFactorFile = TFile::Open(sampleNormfactorsPath.c_str());

        for (int nLep = 2; nLep < 5; nLep++) {
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

void FourTop::generateBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normFilePath, std::string& var, bool jec, bool up) {
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

    std::string jecVariation = var;
    std::string bTagVar = var;
    if (jec) {
        if (up) {
            bTagVar = "up_jes" + var;  
            jecVariation = var + "Up";
        } else {
            bTagVar = "down_jes" + var;
            jecVariation = var + "Down";
        }
    }
    // initialize the output map
    std::shared_ptr<TH1D> averageOfWeights = std::make_shared<TH1D>(("bTagNormFactors" + samp.fileName() + bTagVar).c_str(), "bTagNormFactors;Jets;Factor", 30, 0, 30);
    std::shared_ptr<TH1D> nEntries = std::make_shared<TH1D>(("nEntries" + bTagVar).c_str(), "nEntries;Jets;Entries", 30, 0, 30);

    // loop over events
    long unsigned availableEntries = tempTree.numberOfEntries();

    std::cout << "starting event loop for " << availableEntries << " events..." << std::endl;

    averageOfWeights->Fill(0., 1.);
    nEntries->Fill(0., 1.);

    for (long unsigned entry = 0; entry < availableEntries; ++entry) {
        //if (entry>1000) break;
        Event event = tempTree.buildEvent(entry, false, false, false, true);

        // do basic selection
        event.removeTaus();
        event.selectLooseLeptons();
        event.cleanElectronsFromLooseMuons();
        event.cleanJetsFromFOLeptons();
        int njets = event.numberOfGoodJets();
        double ht = event.HT();

        if (jec) {
            njets = event.getJetCollection(jecVariation).size();
            ht = event.getJetCollection(jecVariation).scalarPtSum();
        }

        if (event.numberOfFOLeptons() != event.numberOfTightLeptons()) continue;
        event.selectTightLeptons();

        if (considerRegion == eventClass::dy || considerRegion == eventClass::ttbar) {
            //std::cout << "in alt sel" << std::endl;
            if (event.numberOfLeptons() == 2) continue;
            if (event.lepton(0).charge() == event.lepton(1).charge()) continue;
            if (njets < 2) continue;
        } else {
            //std::cout << "in nom sel" << std::endl;

            if (event.numberOfLeptons() < 2) continue;
            if (event.numberOfLeptons() == 2 && event.lepton(0).charge() != event.lepton(1).charge()) continue;

            if (njets < 2) continue;
            if (event.numberOfLeptons() < 4 && ht < 200) continue;
            //std::cout << "pass nom sel" << std::endl;

        }
        // determine (nominal) b-tag reweighting and number of jets
        double btagreweight = 1.5;
        if (jec) {
            btagreweight = reweighter->weightJecVar(event, jecVariation);
        } else {
            btagreweight = reweighter->weightVariation(event, bTagVar);
        }

        // add it to the map
        averageOfWeights->Fill(njets, btagreweight);
        nEntries->Fill(njets, 1.);
    }

    // divide sum by number to get average
    //for (int i = 1; i < averageOfWeights->GetNbinsX() + 1; i++) {
    //    std::cout << averageOfWeights->GetBinContent(i) << "/" << nEntries->GetBinContent(i) << std::endl;
    //}
    averageOfWeights->Divide(nEntries.get());
    //for (int i = 1; i < averageOfWeights->GetNbinsX() + 1; i++) {
    //    std::cout << " = " << averageOfWeights->GetBinContent(i) << std::endl;
    //}

    // write out to histogram
    //std::string outputFilePath = stringTools::formatDirectoryName(normDirectory) + stringTools::fileNameFromPath(samp.fileName());
    TFile* normFile = TFile::Open( normFilePath.c_str(), "UPDATE" );
    normFile->cd();
    averageOfWeights->Write(("bTagNormFactors_" + bTagVar).c_str(), TObject::kOverwrite);
    normFile->Close();
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
                bTagVar.push_back(jecVar);

                std::shared_ptr<TH1D> averageOfWeights = std::make_shared<TH1D>(("L" + nLepStr + "_bTagNormFactors" + samp.fileName() + jecVar).c_str(), "bTagNormFactors;Jets;Factor", 30, 0, 30);
                std::shared_ptr<TH1D> nEntries = std::make_shared<TH1D>(("L" + nLepStr + "_nEntries" + jecVar).c_str(), "nEntries;Jets;Entries", 30, 0, 30);

                averageOfWeights->Fill(0., 1.);
                nEntries->Fill(0., 1.);

                averageOfWeightsMapTmp[jecVar] = averageOfWeights;
                nEntriesMapTmp[jecVar] = nEntries;

                jecVar = "down_jes" + var;
                jecVarForSelection.push_back(var+"Down");
                bTagVar.push_back(jecVar);

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

            double btagreweight;
            if (jec && ! flavorQCD_Vars) {
                btagreweight = reweighter->weightJecVar(event, jecVarForSelection[i]);
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

            if (event.numberOfTightLeptons() > 4) {
                averageOfWeightsMap[2][bVar]->Fill(njets, btagreweight);
                nEntriesMap[2][bVar]->Fill(njets, 1.);
            } else {
                averageOfWeightsMap[event.numberOfTightLeptons()-2][bVar]->Fill(njets, btagreweight);
                nEntriesMap[event.numberOfTightLeptons()-2][bVar]->Fill(njets, 1.);
            }
        }
    }

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
