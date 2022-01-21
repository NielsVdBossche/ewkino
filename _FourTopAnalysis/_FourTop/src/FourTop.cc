#include "../interface/FourTop.h"

#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

#include "../../../Tools/interface/systemTools.h"
#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

FourTop::FourTop(std::string outputName, std::vector<std::string>& argvString, int mode) {
    // Set settings according to input
    // prepare details of analysis in separate functions
    
    // First setting are samples to work through
    treeReader = new TreeReader(argvString[1], "/pnfs/iihe/cms/store/user/nivanden/skims/");
    selection = new EventFourT();

    if (mode < 2) {
        std::string outputFileName = "Output/" + outputName + "_";
        std::ostringstream oss;

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        if (argvString.size() >= 3) {
            if (argvString[2] == "additionalNonPrompt") {
                infuseNonPrompt = true;
                outputFileName += "EnlargedNonprompt_";
            }
        }

        if (argvString[1] != "allSamples.txt") {
            std::string newYearString = stringTools::fileNameWithoutExtension(stringTools::splitDirectoryFileName(argvString[1]).second);
            //if (! analysisTools::checkYearStringNoErr( newYearString )) {
            //    newYearString = "2018";
            //}
            setYearString(newYearString);
        }

        //outputFileName += yearString + "_";
        
        oss << std::put_time(&tm, "%d_%m_%Y-%H_%M") << "_" << yearString << ".root";

        outputFileName += oss.str();

        std::cout << outputFileName.c_str() << std::endl;
        outfile = new TFile(outputFileName.c_str(), "recreate");
        
        outfile->mkdir("Nominal");
        outfile->mkdir("Uncertainties");
        
        TH1F* intLuminosityMC = new TH1F("IntegratedLumiMC", "IntegratedLumiMC", 1, 0, 1);

        intLuminosityMC->SetBinContent(1, treeReader->getIntLumi());
        intLuminosityMC->Write("IntLumi", TObject::kOverwrite);
        
        //createHistInfoVec();

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

// Prepare 
void FourTop::createHistInfoVec() {

    // Temp solution
    //histInfoVec_DL = fourTopHists::allHists("DL", false);
    //histInfoVec_3L = fourTopHists::allHists("3L", true);
    //histInfoVec_4L = fourTopHists::allHists("4L", true, true);
    //histInfoVec_CRZ = fourTopHists::allHists("CRZ", false);
    //histInfoVec_CRW = fourTopHists::allHists("CRW", false);
    //histInfoVec_Other = fourTopHists::allHists("CRO", false);

    // Go over settings, voor elke setting, add de passende histosetup bij het totaal
    // Niet te veel om easy te houden, wil niet voor elke fill kei veel settings checken

    // Set hier mss ook setting voor lengte -> fill vector zelfde lengte, anders teveel checks/constructing voor vector?

    // Init histograms
    //std::vector< Sample > sampleVec = treeReader->sampleVector();

    //hists_DL = histHelper::initHistograms(histInfoVec_DL, sampleVec);
    //hists_3L = histHelper::initHistograms(histInfoVec_3L, sampleVec);
    //hists_4L = histHelper::initHistograms(histInfoVec_4L, sampleVec);
    //hists_CRZ = histHelper::initHistograms(histInfoVec_CRZ, sampleVec);
    //hists_CRW = histHelper::initHistograms(histInfoVec_CRW, sampleVec);
    //hists_Other = histHelper::initHistograms(histInfoVec_Other, sampleVec);
}

void FourTop::createMVAHandlers() {
    mva_DL = new MVAHandler_4T(MVAConfigs::TriClass_DL, selection);
    mva_ML = new MVAHandler_4T(MVAConfigs::TriClass_ML, selection);

    selection->setDLMVA(mva_DL);
    selection->setMLMVA(mva_ML);
}

void FourTop::addBTaggingNormFactors(ReweighterBTagShape* reweighter, std::string dir) {
    std::vector<Sample> sampleVector = treeReader->sampleVector();

    for (auto samp : sampleVector) {
        std::string sampleNormfactorsPath = stringTools::formatDirectoryName(dir) + stringTools::fileNameFromPath(samp.fileName());

        if( !systemTools::fileExists( sampleNormfactorsPath ) ){
            generateBTaggingNormFactorsSample( reweighter, samp, dir );
        }

        // loading in
        TFile* btagNormFactorFile = TFile::Open(sampleNormfactorsPath.c_str());

        std::shared_ptr<TH1> bTagNormFactorsHist = std::shared_ptr<TH1>(dynamic_cast<TH1*>(btagNormFactorFile->Get("bTagNormFactors")));

        std::map<int, double> normFactors;

        for (int i=1; i<bTagNormFactorsHist->GetNbinsX() + 1; i++) {
            double normFactor = bTagNormFactorsHist->GetBinContent(i);
            if (fabs(normFactor) > 1e-6) {
                normFactors[i-1] = normFactor;
            }
        }

        for (auto it : normFactors) {
            std::cout << it.first << "\t" << it.second << std::endl;
        }

        reweighter->setNormFactors(samp, normFactors);
    }
}

void FourTop::generateBTaggingNormFactorsSample(ReweighterBTagShape* reweighter, Sample& samp, std::string& normDirectory) {
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

    std::map< int, double > averageOfWeightsOld;
    std::map< int, int > nEntriesOld;

    // loop over events
    long unsigned availableEntries = tempTree.numberOfEntries();

    std::cout << "starting event loop for " << availableEntries << " events..." << std::endl;

    for (long unsigned entry = 0; entry < availableEntries; ++entry) {
        Event event = tempTree.buildEvent(entry);

        // do basic selection
        event.cleanJetsFromFOLeptons();
        event.jetCollection().selectGoodJets();
        event.removeTaus();
        event.cleanElectronsFromLooseMuons();
        event.selectTightLeptons();

        if (event.numberOfLeptons() < 2) continue;
        if (event.numberOfLeptons() == 2 && event.lepton(0).charge() != event.lepton(1).charge()) continue;
        if (event.numberOfJets() < 3) continue;


        // determine (nominal) b-tag reweighting and number of jets
        double btagreweight = reweighter->weight(event);
        int njets = event.jetCollection().goodJetCollection().size();

        // add it to the map
        averageOfWeights->Fill(njets, btagreweight);
        nEntries->Fill(njets, 1);

        if(averageOfWeights.find(njets)==averageOfWeightsOld.end()) { 
            averageOfWeightsOld[njets] = btagreweight;
            nEntriesOld[njets] = 1;
        } else {
            averageOfWeightsOld[njets] += btagreweight;
            nEntriesOld[njets] += 1;
        }
    }

    for( std::map<int,double>::iterator it = averageOfWeightsOld.begin(); it != averageOfWeightsOld.end(); ++it){
        averageOfWeightsOld[it->first] = it->second / nEntriesOld[it->first];
        std::cout << it->first << "\t" << averageOfWeights[it->first] << std::endl;
    }

    // divide sum by number to get average
    averageOfWeights->Divide(nEntries.get());

    std::cout << "done with event loop" << std::endl;
    
    // write out to histogram
    std::string outputFilePath = stringTools::formatDirectoryName(normDirectory) + stringTools::fileNameFromPath(samp.fileName());
    TFile* normFile = TFile::Open( outputFilePath.c_str(), "RECREATE" );
    normFile->cd();
    averageOfWeights->Write("bTagNormFactors", TObject::kOverwrite);
    normFile->Close();
}
