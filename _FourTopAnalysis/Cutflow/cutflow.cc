#include "../_FourTop/interface/FourTop.h"
#include "../../GenLevelStudies/interface/GenLeptonCollection.h"
#include "../../plotting/tdrStyle.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* getCutflowHist(std::string flag, bool genInfo) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;
    *histInfoVec = {
        HistInfo("Cutflow_" + flag, "", 14, -0.5, 13.5),
        HistInfo("Cutflow_" + flag + "_NotSel", "", 14, -0.5, 13.5),
        HistInfo("Cutflow_" + flag + "_DL", "", 14, -0.5, 13.5),
        HistInfo("Cutflow_" + flag + "_3L", "", 14, -0.5, 13.5),
        HistInfo("Cutflow_" + flag + "_4L", "", 14, -0.5, 13.5),
        HistInfo("nLooseLeptons_" + flag, "N_{l} loose", 8, -0.5, 7.5),
        HistInfo("nTightLeptons_" + flag, "N_{l} tight", 8, -0.5, 7.5),
        HistInfo("nJets_" + flag, "N_{jets}", 8, -0.5, 7.5),
        HistInfo("nLooseBjet_" + flag, "N_{b}, loose WP", 10, -0.5, 9.5),
        HistInfo("nMediumBjet_" + flag, "N_{b}, med. WP", 10, -0.5, 9.5),
        HistInfo("HT_" + flag, "H_{T} [GeV]", 25, 0, 500),
        HistInfo("pt_TrailingLooseLepton" + flag, "p_{T}(trailing loose lepton)", 9, 5, 50),
        HistInfo("lepMVATrailingLepton" + flag, "leptonMVA score trailing loose lepton", 40, -1., 1.),
    };

    // variables for selection

    return histInfoVec;
}

bool selectLeptonsPt(const Lepton& lepton) {
    return (lepton.pt() > 10);
}

bool selectLeptonsMVA(const LightLepton& lepton) {
    return (lepton.leptonMVATOP() > 0.40);
}

void FourTop::cutFlow(std::string& sortingMode) {
    bool sortOnGenerator = false;
    if (sortingMode == "GeneratorInfo") {
        sortOnGenerator = true;
    }

    std::string channelDL = "DL";
    std::vector<HistInfo>* infoDL = getCutflowHist(channelDL, sortOnGenerator);
    HistogramManager* DLManager = new HistogramManager(channelDL, infoDL);

    std::string channel3L = "3L";
    std::vector<HistInfo>* info3L = getCutflowHist(channel3L, sortOnGenerator);
    HistogramManager* TriLManager = new HistogramManager(channel3L, info3L);

    std::string channel4L = "4L";
    std::vector<HistInfo>* info4L = getCutflowHist(channel4L, sortOnGenerator);
    HistogramManager* FourLManager = new HistogramManager(channel4L, info4L);

    std::string channelGarbage = "garbage";
    std::vector<HistInfo>* infoGarbage = getCutflowHist(channelGarbage, sortOnGenerator);
    HistogramManager* garbageManager = new HistogramManager(channelGarbage, infoGarbage);

    std::cout << "event loop" << std::endl;
    currentEvent = new Event();

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        DLManager->newSample(uniqueName);
        TriLManager->newSample(uniqueName);
        FourLManager->newSample(uniqueName);
        garbageManager->newSample(uniqueName);

        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();

        std::vector<std::shared_ptr<TH1D>>* allDLHist = DLManager->getHistograms(false);
        std::vector<std::shared_ptr<TH1D>>* all3LHist = TriLManager->getHistograms(false);
        std::vector<std::shared_ptr<TH1D>>* all4LHist = FourLManager->getHistograms(false);
        std::vector<std::shared_ptr<TH1D>>* allGarbageHist = garbageManager->getHistograms(false);

        std::shared_ptr<TH1D> dlHist = allDLHist->at(0);
        std::shared_ptr<TH1D> trilepHist = all3LHist->at(0);
        std::shared_ptr<TH1D> fourlepHist = all4LHist->at(0);
        
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

            std::shared_ptr<TH1D> cutflowHist;
            std::vector<std::shared_ptr<TH1D>>* currentHistSet;
            std::shared_ptr<TH1D> cutflowHistSub;

            
            bool sameCharge = false;
            int nLeps = currentEvent->numberOfTightLeptons();
            int nTightLeps = nLeps;
            int recoSameCharge = sameCharge;
            if (sortOnGenerator) {
                GenLeptonCollection* genLeptons = new GenLeptonCollection(*treeReader);
                genLeptons->selectLightLeptons();
                nLeps = genLeptons->size();
                if (nLeps == 2) {
                    sameCharge = (genLeptons->at(0)->charge() == genLeptons->at(1)->charge());
                }
            } else if (nTightLeps >= 2) {         
                sameCharge = (tightLeps[0].charge() == tightLeps[1].charge());
            }

            if (nLeps == 2 && sameCharge) {
                cutflowHist = dlHist;
                currentHistSet = allDLHist;
            } else if (nLeps == 3) {
                cutflowHist = trilepHist;
                currentHistSet = all3LHist;
            } else if (nLeps >= 4) {
                cutflowHist = fourlepHist;
                currentHistSet = all4LHist;
            } else {
                currentHistSet = allGarbageHist;
            }

            // subHist determination
            if (nTightLeps == 2 && recoSameCharge) {
                cutflowHistSub = currentHistSet->at(2);
            } else if (nTightLeps == 3) {
                cutflowHistSub = currentHistSet->at(3);
            } else if (nTightLeps >= 4) {
                cutflowHistSub = currentHistSet->at(4);
            } else {
                cutflowHistSub = currentHistSet->at(1);
            }
            
            weight = currentEvent->weight();
            currentHistSet->at(5)->Fill(currentEvent->numberOfLooseLeptons(), weight);

            if (nLeps < 2 || (nLeps == 2 && !sameCharge)) continue;


            //std::cout << nLeps << " " << sameCharge << std::endl;
            cutflowHist->Fill(0., weight);
            cutflowHistSub->Fill(0., weight);

            if (currentEvent->numberOfLooseLeptons() < 2) continue;
            if (currentEvent->numberOfLooseLeptons() == 2 && currentEvent->looseLeptonCollection().hasOSPair()) continue;

            cutflowHist->Fill(1., weight);
            cutflowHistSub->Fill(1., weight);

            currentEvent->selectLooseLeptons();

            currentHistSet->at(6)->Fill(currentEvent->numberOfTightLeptons(), weight);
            currentHistSet->at(7)->Fill(currentEvent->numberOfGoodJets(), weight);
            currentHistSet->at(8)->Fill(currentEvent->numberOfLooseBTaggedJets(), weight);
            currentHistSet->at(9)->Fill(currentEvent->numberOfMediumBTaggedJets(), weight);
            currentHistSet->at(10)->Fill(currentEvent->HT(), weight);
            currentHistSet->at(11)->Fill(currentEvent->lepton(currentEvent->numberOfLooseLeptons() - 1).pt(), weight);
            currentHistSet->at(12)->Fill(currentEvent->lightLepton(currentEvent->numberOfLooseLeptons() - 1).leptonMVATOP(), weight);

            LeptonCollection looseLeps = currentEvent->looseLeptonCollection();
            looseLeps.selectObjects(selectLeptonsPt);
            if (looseLeps.size() < 2 || (looseLeps.size() == 2 && looseLeps.hasOSPair())) continue;
            cutflowHist->Fill(2., weight);
            cutflowHistSub->Fill(2., weight);

            LightLeptonCollection lightLeps = currentEvent->lightLeptonCollection();
            lightLeps.selectObjects(selectLeptonsMVA);
            if (lightLeps.size() < 2 || (lightLeps.size() == 2 && lightLeps[0].charge() != lightLeps[1].charge())) continue;
            cutflowHist->Fill(3., weight);
            cutflowHistSub->Fill(3., weight);

            currentEvent->selectTightLeptons();

            if (currentEvent->numberOfTightLeptons() < 2) continue;
            if (currentEvent->numberOfTightLeptons() == 2 && currentEvent->hasOSLeptonPair()) continue;

            cutflowHist->Fill(4., weight);
            cutflowHistSub->Fill(4., weight);
            currentEvent->sortLeptonsByPt();

            if (currentEvent->lepton(0).pt() < 25 || currentEvent->lepton(1).pt() < 20) continue;
            
            cutflowHist->Fill(5., weight);
            cutflowHistSub->Fill(5., weight);

            // Remove mass resonances
            selection->addNewEvent(currentEvent);
            if (! selection->passLowMassVeto()) continue;

            cutflowHist->Fill(6., weight);
            cutflowHistSub->Fill(6., weight);
            
            if (! selection->passZBosonVeto()) continue;
            cutflowHist->Fill(7., weight);
            cutflowHistSub->Fill(7., weight);


            if ((currentEvent->numberOfTightLeptons() < 4 && selection->numberOfJets() < 4) || (currentEvent->numberOfTightLeptons() == 4 && selection->numberOfJets() < 3)) continue;
            cutflowHist->Fill(8., weight);
            cutflowHistSub->Fill(8., weight);

            if ((currentEvent->numberOfTightLeptons() < 4 && selection->numberOfMediumBJets() < 2) || (currentEvent->numberOfTightLeptons() == 4 && selection->numberOfMediumBJets() < 1)) continue;
            cutflowHist->Fill(9., weight);
            cutflowHistSub->Fill(9., weight);

            if (currentEvent->numberOfTightLeptons() < 4 && selection->getHT() < 300) continue;
            cutflowHist->Fill(10., weight);
            cutflowHistSub->Fill(10., weight);
            if (currentEvent->numberOfTightLeptons() == 2) {
                cutflowHist->Fill(11., weight);
                cutflowHistSub->Fill(11., weight);
            }
            if (currentEvent->numberOfTightLeptons() == 3) {
                cutflowHist->Fill(12., weight);
                cutflowHistSub->Fill(12., weight);
            }
            if (currentEvent->numberOfTightLeptons() == 4) {
                cutflowHist->Fill(13., weight);
                cutflowHistSub->Fill(13., weight);
            }

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
        garbageManager->writeCurrentHistograms();
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    DLManager->writeNonpromptHistograms();
    TriLManager->writeNonpromptHistograms();
    FourLManager->writeNonpromptHistograms();
    garbageManager->writeNonpromptHistograms();


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
