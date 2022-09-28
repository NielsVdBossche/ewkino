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
    for (auto samp : sampleVector) {
        std::string sampleNormfactorsPath = stringTools::formatDirectoryName(dir) + stringTools::fileNameFromPath(samp.fileName());

        if( !systemTools::fileExists( sampleNormfactorsPath ) ){
            for (auto var : variations) {
                generateBTaggingNormFactorsSample( reweighter, samp, dir, var );
            }
        }

        // loading in
        TFile* btagNormFactorFile = TFile::Open(sampleNormfactorsPath.c_str());

        for (auto var : variations) {
            std::shared_ptr<TH1> bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFile->Get(("bTagNormFactors_" + var).c_str())));
            int tr = 0;
            while (! bTagNormFactorsHist && tr < 10) {
                btagNormFactorFile->Close();
                btagNormFactorFile = TFile::Open(sampleNormfactorsPath.c_str());
                bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFile->Get(("bTagNormFactors_" + var).c_str())));
                tr++;
            }
            std::map<int, double> normFactors;

            for (int i=1; i<bTagNormFactorsHist->GetNbinsX() + 1; i++) {
                double normFactor = bTagNormFactorsHist->GetBinContent(i);
                if (fabs(normFactor) > 1e-6) {
                    normFactors[i-1] = normFactor;
                }
            }

            reweighter->setNormFactors(samp, normFactors, var);
        }
        btagNormFactorFile->Close();
    }
}

void FourTop::generateBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normDirectory, std::string& var) {
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
    std::shared_ptr<TH1D> averageOfWeights = std::make_shared<TH1D>(("bTagNormFactors" + samp.fileName()).c_str(), "bTagNormFactors;Jets;Factor", 30, 0, 30);
    std::shared_ptr<TH1D> nEntries = std::make_shared<TH1D>("nEntries", "nEntries;Jets;Entries", 30, 0, 30);

    // loop over events
    long unsigned availableEntries = tempTree.numberOfEntries();

    std::cout << "starting event loop for " << availableEntries << " events..." << std::endl;

    averageOfWeights->Fill(0., 1.);
    nEntries->Fill(0., 1.);

    for (long unsigned entry = 0; entry < availableEntries; ++entry) {
        Event event = tempTree.buildEvent(entry);

        // do basic selection
        event.removeTaus();
        event.selectLooseLeptons();
        event.cleanElectronsFromLooseMuons();
        event.cleanJetsFromFOLeptons();
        
        event.selectGoodJets();

        if (event.numberOfFOLeptons() != event.numberOfTightLeptons()) continue;
        event.selectTightLeptons();

        if (considerRegion == eventClass::dy || considerRegion == eventClass::ttbar) {
            //std::cout << "in alt sel" << std::endl;
            if (event.numberOfLeptons() == 2) continue;
            if (event.lepton(0).charge() == event.lepton(1).charge()) continue;
            if (event.numberOfJets() < 2) continue;
        } else {
            //std::cout << "in nom sel" << std::endl;

            if (event.numberOfLeptons() < 2) continue;
            if (event.numberOfLeptons() == 2 && event.lepton(0).charge() != event.lepton(1).charge()) continue;

            if (event.numberOfJets() < 2) continue;
            if (event.numberOfLeptons() < 4 && event.HT() < 200) continue;
        }
        // determine (nominal) b-tag reweighting and number of jets
        double btagreweight = reweighter->weightVariation(event, var);
        int njets = event.jetCollection().goodJetCollection().size();

        // add it to the map
        averageOfWeights->Fill(njets, btagreweight);
        nEntries->Fill(njets, 1.);
    }

    // divide sum by number to get average
    averageOfWeights->Divide(nEntries.get());

    // write out to histogram
    std::string outputFilePath = stringTools::formatDirectoryName(normDirectory) + stringTools::fileNameFromPath(samp.fileName());
    TFile* normFile = TFile::Open( outputFilePath.c_str(), "UPDATE" );
    normFile->cd();
    averageOfWeights->Write(("bTagNormFactors_" + var).c_str(), TObject::kOverwrite);
    normFile->Close();
}
