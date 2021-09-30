#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

FourTop::FourTop(std::vector< std::string > argvString, int mode) {
    // Set settings according to input
    // prepare details of analysis in separate functions
    
    // First setting are samples to work through
    treeReader = new TreeReader(argvString[1], "/pnfs/iihe/cms/store/user/nivanden/skims/");

    if (mode == 0) {
        outfile = new TFile("testOutput.root", "recreate");
        
        outfile->mkdir("Nominal");
        outfile->mkdir("Uncertainties");
        
        TH1F* intLuminosityMC = new TH1F("IntegratedLumiMC", "IntegratedLumiMC", 1, 0, 1);

        intLuminosityMC->SetBinContent(1, treeReader->getIntLumi());
        intLuminosityMC->Write("IntLumi", TObject::kOverwrite);
        
        createHistInfoVec();
    }

    selection = new EventSelection4T();
}

FourTop::~FourTop() {

    //outfile->Close();

    // Delete histograms

    // Delete histogramInfo
    delete histInfoVec_DL;
    delete histInfoVec_ML;
    delete histInfoVec_CRZ;    
    delete histInfoVec_CRW;
    //delete histInfoVec_Global; // If even assigned?
}

// Prepare 
void FourTop::createHistInfoVec() {

    // Temp solution
    histInfoVec_DL = fourTopHists::allHistsDL();
    histInfoVec_ML = fourTopHists::allHistsML();
    histInfoVec_CRZ = fourTopHists::crzInfo();
    histInfoVec_CRW = fourTopHists::crwInfo();

    // Go over settings, voor elke setting, add de passende histosetup bij het totaal
    // Niet te veel om easy te houden, wil niet voor elke fill kei veel settings checken

    // Set hier mss ook setting voor lengte -> fill vector zelfde lengte, anders teveel checks/constructing voor vector?

    // Init histograms
    std::vector< Sample > sampleVec = treeReader->sampleVector();

    hists_DL = histHelper::initHistograms(histInfoVec_DL, sampleVec);
    hists_ML = histHelper::initHistograms(histInfoVec_ML, sampleVec);
    hists_CRZ = histHelper::initHistograms(histInfoVec_CRZ, sampleVec);
    hists_CRW = histHelper::initHistograms(histInfoVec_CRW, sampleVec);

}
