#include "../interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

FourTop::FourTop(std::vector< std::string > argvString) {
    // Set settings according to input
    // prepare details of analysis in separate functions
    
    // First setting are samples to work through
    treeReader = new TreeReader("samples.txt", "/pnfs/iihe/cms/store/user/nivanden/skims/");

    outfile = new TFile("testOutput.root", "recreate");

    createHistInfoVec();
}

FourTop::~FourTop() {

    outfile->Close();

    // Delete histograms

    // Delete histogramInfo
    delete histInfoVec_DL;
    delete histInfoVec_ML;
    //delete histInfoVec_Global; // If even assigned?
}

// Prepare 
void FourTop::createHistInfoVec() {

    // Temp solution
    histInfoVec_DL = new std::vector<HistInfo> (fourTopHists::lepInfoDL());
    histInfoVec_ML = new std::vector<HistInfo> (fourTopHists::lepInfoML());

    // Go over settings, voor elke setting, add de passende histosetup bij het totaal
    // Niet te veel om easy te houden, wil niet voor elke fill kei veel settings checken

    // Set hier mss ook setting voor lengte -> fill vector zelfde lengte, anders teveel checks/constructing voor vector?

    // Init histograms
    std::vector< Sample > sampleVec = treeReader->sampleVector();

    hists_DL = new std::vector< std::vector< std::shared_ptr< TH1D > > >( histInfoVec_DL->size(), std::vector< std::shared_ptr< TH1D > >( sampleVec.size() + 1 )  );
    for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ){
        for( size_t p = 0; p < sampleVec.size() + 1; ++p ){
            if( p < sampleVec.size() ){
                (*hists_DL)[ dist ][ p ] = histInfoVec_DL->at(dist).makeHist( histInfoVec_DL->at(dist).name() + "_" + sampleVec[p].uniqueName() );
            } else {
                //(*hists_DL)[ dist ][ p ] = histInfoVec_DL->at(dist).makeHist( histInfoVec_DL->at(dist).name() + "_nonprompt" );
            }
        }
    }

    hists_ML = new std::vector< std::vector< std::shared_ptr< TH1D > > >( histInfoVec_ML->size(), std::vector< std::shared_ptr< TH1D > >( sampleVec.size() + 1 )  );
    for( size_t dist = 0; dist < histInfoVec_ML->size(); ++dist ){
        for( size_t p = 0; p < sampleVec.size() + 1; ++p ){
            if( p < sampleVec.size() ){
                (*hists_ML)[ dist ][ p ] = histInfoVec_ML->at(dist).makeHist( histInfoVec_ML->at(dist).name() + "_" + sampleVec[p].uniqueName() );
            } else {
                //(*hists_ML)[ dist ][ p ] = histInfoVec_ML->at(dist).makeHist( histInfoVec_ML->at(dist).name() + "_nonprompt" );
            }
        }
    }
}
