#include "../_FourTop/interface/FourTop.h"
#include "../../GenLevelStudies/interface/GenLeptonCollection.h"
#include "../../plotting/tdrStyle.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* getCutflowHist(std::string flag) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    *histInfoVec = {HistInfo("Cutflow_" + flag, "", 11, -0.5, 10.5)};
    return histInfoVec;
}

bool selectLeptonsPt(const Lepton& lepton) {
    return (lepton.pt() > 10);
}

bool selectLeptonsMVA(const LightLepton& lepton) {
    return (lepton.leptonMVATOP() > 0.40);
}

void FourTop::cutFlow(std::string& sortingMode) {

    std::string channelDL = "DL";
    std::vector<HistInfo>* infoDL = getCutflowHist(channelDL);
    HistogramManager* DLManager = new HistogramManager(channelDL, infoDL);

    std::string channel3L = "3L";
    std::vector<HistInfo>* info3L = getCutflowHist(channel3L);
    HistogramManager* TriLManager = new HistogramManager(channel3L, info3L);

    std::string channel4L = "4L";
    std::vector<HistInfo>* info4L = getCutflowHist(channel4L);
    HistogramManager* FourLManager = new HistogramManager(channel4L, info4L);
    std::cout << "event loop" << std::endl;
    currentEvent = new Event();

    std::vector<Sample> sampleVec = treeReader->sampleVector();

    bool sortOnGenerator = false;
    if (sortingMode == "GeneratorInfo") {
        sortOnGenerator = true;
    }

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        DLManager->newSample(uniqueName);
        TriLManager->newSample(uniqueName);
        FourLManager->newSample(uniqueName);

        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        std::shared_ptr<TH1D> dlHist = DLManager->getHistograms(false)->at(0);
        std::shared_ptr<TH1D> trilepHist = TriLManager->getHistograms(false)->at(0);
        std::shared_ptr<TH1D> fourlepHist = FourLManager->getHistograms(false)->at(0);
        
        // at end of sel, count extra if it flows to other channels (3l and then ssdl?)
        double weight = 0;

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );
            if (! currentEvent->passTTGOverlap(ttgOverlapCheck)) continue; // TTG overlap, double check "working points"

            currentEvent->removeTaus();
            currentEvent->cleanElectronsFromLooseMuons();
            currentEvent->cleanJetsFromLooseLeptons();
            currentEvent->selectGoodJets();
            
            LeptonCollection tightLeps = currentEvent->TightLeptonCollection();
            tightLeps.sortByPt();

            std::shared_ptr<TH1D> currentHist;
            
            bool sameCharge = false;
            int nLeps = currentEvent->numberOfTightLeptons();
            if (sortOnGenerator) {
                GenLeptonCollection* genLeptons = new GenLeptonCollection(*treeReader);
                genLeptons->selectLightLeptons();
                nLeps = genLeptons->size();
                if (nLeps == 2) {
                    sameCharge = (genLeptons->at(0)->charge() == genLeptons->at(1)->charge());
                }
            }
            
            if (nLeps < 2) continue;

            if (! sortOnGenerator) {
                sameCharge = tightLeps[0].charge() == tightLeps[1].charge();
            }

            if (nLeps == 2 && sameCharge) {
                currentHist = dlHist;
            } else if (nLeps == 3) {
                currentHist = trilepHist;
            } else if (nLeps == 4) {
                currentHist = fourlepHist;
            } else {
                continue;
            }

            weight = currentEvent->weight();

            currentHist->Fill(0., weight);

            if (currentEvent->numberOfLooseLeptons() < 2) continue;
            if (currentEvent->numberOfLooseLeptons() == 2 && currentEvent->looseLeptonCollection().hasOSPair()) continue;

            currentHist->Fill(1., weight);

            currentEvent->selectLooseLeptons();

            LeptonCollection looseLeps = currentEvent->looseLeptonCollection();
            looseLeps.selectObjects(selectLeptonsPt);
            if (looseLeps.size() < 2 || (looseLeps.size() == 2 && looseLeps.hasOSPair())) continue;
            currentHist->Fill(2., weight);

            LightLeptonCollection lightLeps = currentEvent->lightLeptonCollection();
            lightLeps.selectObjects(selectLeptonsMVA);
            if (lightLeps.size() < 2 || (lightLeps.size() == 2 && lightLeps[0].charge() != lightLeps[1].charge())) continue;
            currentHist->Fill(3., weight);

            currentEvent->selectTightLeptons();

            if (currentEvent->numberOfTightLeptons() < 2) continue;
            if (currentEvent->numberOfTightLeptons() == 2 && currentEvent->hasOSLeptonPair()) continue;

            currentHist->Fill(4., weight);
            currentEvent->sortLeptonsByPt();

            if (currentEvent->lepton(0).pt() < 25 || currentEvent->lepton(1).pt() < 20) continue;
            
            currentHist->Fill(5., weight);

            // Remove mass resonances
            selection->addNewEvent(currentEvent);
            if (! selection->passLowMassVeto()) continue;

            currentHist->Fill(6., weight);
            
            if (! selection->passZBosonVeto()) continue;
            currentHist->Fill(7., weight);


            if (selection->numberOfJets() < 4) continue;
            currentHist->Fill(8., weight);

            if (selection->numberOfMediumBJets() < 2) continue;
            currentHist->Fill(9., weight);

            if (selection->getHT() < 300) continue;
            currentHist->Fill(10., weight);
        }
        
        // Output management: save histograms to a ROOT file.
        // Processes were named in samplelist. Should use this to make directory for process and subdir for sample

        // Might interface with Stacker to create desired output plots as well... Or at least already have the stacked process ready instead of individual components. Then a "getDirectory" in stacker could be handy to see if it exists.
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

        // Rewrite this to a dedicated function maybe, or something where we don't have to call sampleIndex each time?
        //for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
        //    hists_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        //}
        DLManager->writeCurrentHistograms();
        TriLManager->writeCurrentHistograms();
        FourLManager->writeCurrentHistograms();
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    DLManager->writeNonpromptHistograms();
    TriLManager->writeNonpromptHistograms();
    FourLManager->writeNonpromptHistograms();

    outfile->Close();
}

int main(int argc, char* argv[]) {
    setTDRStyle();

    if (argc < 3) {
        std::cout << "Two argument required: <sampleList.txt> <GeneratorInfo available>" << std::endl;
    }
    // zet makkelijk te verwerken string op
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    // Settings splitsen if necessary

    // main func call
    FourTop analysisObject("Cutflow", argvStr, 0);
    
    if (argvStr.size() < 3) {
        argvStr.push_back("noGen");
    }
    analysisObject.cutFlow(argvStr[2]);
}
