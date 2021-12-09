#include "../interface/FourTop.h"

#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

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
}
