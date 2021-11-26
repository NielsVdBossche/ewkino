#include "interface/GenLeptonCollection.h"
#include "../TreeReader/interface/TreeReader.h"
#include "../plotting/tdrStyle.h"

#include "../Tools/interface/HistInfo.h"
#include "../_FourTopAnalysis/additionalTools/interface/histHelper.h"


std::vector<HistInfo>* histInfoGen(std::string flag, bool trilep, bool fourlep) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    
    *histInfoVec = {
        HistInfo( "leptonPtLeading_" + flag, "p_{T}(l1) [GeV]", 50, 0, 400),
        HistInfo( "leptonEtaLeading_" + flag, "#eta (l1)", 14, -2.8, 2.8), // Might switch to det segmentation
        HistInfo( "leptonPhiLeading_" + flag, "#phi (l1)", 12, - M_PI, M_PI),
        HistInfo( "leptonELeading_" + flag, "E(l1) [GeV]", 50, 0, 500),
        HistInfo( "leptonPtSecond_" + flag, "p_{T}(l2) [GeV]", 40, 0, 150),
        HistInfo( "leptonEtaSecond_" + flag, "#eta (l2)", 14, -2.8, 2.8), // Might switch to det segmentation
        HistInfo( "leptonPhiSecond_" + flag, "#phi (l2)", 12, - M_PI, M_PI),
        HistInfo( "leptonESecond_" + flag, "E(l2) [GeV]", 50, 0, 500),

        HistInfo( "MET_" + flag, "p_{T}^{miss} [GeV]", 20, 0, 500),
        HistInfo( "LT_" + flag, "L_{T} [GeV]", 20, 0, 500),
        HistInfo( "Nr_of_leps_" + flag, "N_{l}", 5, 0, 5)
    };

    if (trilep) {
        histInfoVec->push_back( HistInfo( "leptonPtThird_" + flag, "p_{T}(l3) [GeV]", 40, 0, 100) );
        histInfoVec->push_back( HistInfo( "leptonEtaThird_" + flag, "#eta (l3)", 14, -2.8, 2.8) );
        histInfoVec->push_back( HistInfo( "leptonPhiThird_" + flag, "#phi (l3)", 12, - M_PI, M_PI) );
        histInfoVec->push_back( HistInfo( "leptonEThird_" + flag, "E(l3) [GeV]", 50, 0, 500) );

        if (fourlep) {
            histInfoVec->push_back( HistInfo( "leptonPtFour_" + flag, "p_{T}(l4) [GeV]", 40, 0, 60) );
            histInfoVec->push_back( HistInfo( "leptonEtaFour_" + flag, "#eta (l4)", 14, -2.8, 2.8) );
            histInfoVec->push_back( HistInfo( "leptonPhiFour_" + flag, "#phi (l4)", 12, - M_PI, M_PI) );
            histInfoVec->push_back( HistInfo( "leptonEFour_" + flag, "E(l4) [GeV]", 50, 0, 500) );
        }
    }

    return histInfoVec;
}

std::vector<double> getFillVec(GenLeptonCollection* lepCol, TreeReader* treeReader, double trilep, double fourlep) {
    std::vector<double> fillVal = {
        lepCol->at(0)->pt(),
        lepCol->at(0)->eta(),
        lepCol->at(0)->phi(),
        lepCol->at(0)->energy(),
        lepCol->at(1)->pt(),
        lepCol->at(1)->eta(),
        lepCol->at(1)->phi(),
        lepCol->at(1)->energy(),

        treeReader->_gen_met,
        lepCol->getLT(),
        double(lepCol->size())
    };

    if (trilep) {
        fillVal.push_back(lepCol->at(2)->pt());
        fillVal.push_back(lepCol->at(2)->eta());
        fillVal.push_back(lepCol->at(2)->phi());
        fillVal.push_back(lepCol->at(2)->energy());

        if (fourlep) {
            fillVal.push_back(lepCol->at(3)->pt());
            fillVal.push_back(lepCol->at(3)->eta());
            fillVal.push_back(lepCol->at(3)->phi());
            fillVal.push_back(lepCol->at(3)->energy());
        }
    }

    return fillVal;
}


int main(int argc, char const *argv[]) {
    setTDRStyle();
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    TreeReader* treeReader = new TreeReader(argvStr[1], "/pnfs/iihe/cms/store/user/nivanden/skims/");

    std::vector<HistInfo> *histInfoDL, *histInfo3L, *histInfo4L;
    histInfoDL = histInfoGen("DL", false, false);
    histInfo3L = histInfoGen("3L", true, false);
    histInfo4L = histInfoGen("4L", true, true);

    std::vector< Sample > sampleVec = treeReader->sampleVector();

    std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_DL = histHelper::initHistograms(histInfoDL, sampleVec);
    std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_3L = histHelper::initHistograms(histInfo3L, sampleVec);
    std::vector< std::vector<std::shared_ptr<TH1D>>>* hists_4L = histHelper::initHistograms(histInfo4L, sampleVec);

    std::cout << "event loop" << std::endl;

    TFile* outfile = new TFile("GeneratorInfo.root", "recreate");
    outfile->cd();
    outfile->mkdir("Nominal");

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            treeReader->GetEntry(entry);
            GenLeptonCollection* genLeptons = new GenLeptonCollection(*treeReader);

            genLeptons->selectLightLeptons();
            genLeptons->sortByPt();

            if (! genLeptons->passSelection()) continue;

            if (genLeptons->size() == 2) {
                std::vector<double> fillVec = getFillVec(genLeptons, treeReader, false, false);
                histHelper::histFiller(fillVec, &(hists_DL->at(sampleIndex)), treeReader->_scaledWeight);
            } else if (genLeptons->size() == 3) {
                std::vector<double> fillVec = getFillVec(genLeptons, treeReader, true, false);
                histHelper::histFiller(fillVec, &(hists_3L->at(sampleIndex)), treeReader->_scaledWeight);
            } else if (genLeptons->size() == 4) {
                std::vector<double> fillVec = getFillVec(genLeptons, treeReader, true, true);
                histHelper::histFiller(fillVec, &(hists_4L->at(sampleIndex)), treeReader->_scaledWeight);
            }
        }

        outfile->cd();
        outfile->cd("Nominal");
        const char* processName = treeReader->currentSample().processName().c_str();
        if (! gDirectory->GetDirectory(processName)) {
            gDirectory->mkdir(processName);
        }
        gDirectory->cd(processName);

        std::string outdir = stringTools::fileNameFromPath(treeReader->currentSample().fileName());
        gDirectory->mkdir(outdir.c_str()); // got to switch to gDirectory. Otherwise keeps working as if we're on level of file
        gDirectory->cd(outdir.c_str());

        for( size_t dist = 0; dist < histInfoDL->size(); ++dist ) {
            hists_DL->at(sampleIndex)[dist]->Write(TString(histInfoDL->at(dist).name()), TObject::kOverwrite);
        }
        for( size_t dist = 0; dist < histInfo3L->size(); ++dist ) {
            hists_3L->at(sampleIndex)[dist]->Write(TString(histInfo3L->at(dist).name()), TObject::kOverwrite);
        }

        for( size_t dist = 0; dist < histInfo4L->size(); ++dist ) {
            hists_4L->at(sampleIndex)[dist]->Write(TString(histInfo4L->at(dist).name()), TObject::kOverwrite);
        }
    }

    outfile->Close();

    return 0;
}
