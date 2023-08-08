#include "../../GenLevelStudies/interface/GenLeptonCollection.h"
#include "../../plotting/tdrStyle.h"
#include "../_FourTop/interface/FourTop.h"

#if MEMLEAK
#include "../../../memleak/debug_new.h"
#endif

std::vector<HistInfo>* getCutflowHist(std::string flag, bool genInfo) {
    std::vector<HistInfo>* histInfoVec = new std::vector<HistInfo>;
    *histInfoVec = {
        HistInfo("Cutflow_" + flag, "", 17, -0.5, 16.5),
        HistInfo("Cutflow_NotSel_" + flag, "", 17, -0.5, 16.5),
        HistInfo("Cutflow_DL_" + flag, "", 17, -0.5, 16.5),
        HistInfo("Cutflow_3L_" + flag, "", 17, -0.5, 16.5),
        HistInfo("Cutflow_4L_" + flag, "", 17, -0.5, 16.5),
        HistInfo("nLooseLeptons_" + flag, "N_{l} loose", 8, -0.5, 7.5),
        HistInfo("nTightLeptons_" + flag, "N_{l} tight", 8, -0.5, 7.5),
        HistInfo("nJets_" + flag, "N_{jets}", 8, -0.5, 7.5),
        HistInfo("nLooseBjet_" + flag, "N_{b}, loose WP", 10, -0.5, 9.5),
        HistInfo("nMediumBjet_" + flag, "N_{b}, med. WP", 10, -0.5, 9.5),
        HistInfo("HT_" + flag, "H_{T} [GeV]", 25, 0, 500),
        HistInfo("pt_TrailingLooseLepton_" + flag, "p_{T}(trailing loose lepton)", 18, 5, 50),
        HistInfo("lepMVATrailingLepton_" + flag, "leptonMVA score trailing loose lepton", 40, -1., 1.),
        HistInfo("nJets_AtSel_" + flag, "N_{jets}", 8, -0.5, 7.5),
        HistInfo("nLooseBjet_AtSel_" + flag, "N_{b}, loose WP", 10, -0.5, 9.5),
        HistInfo("nMediumBjet_AtSel_" + flag, "N_{b}, med. WP", 10, -0.5, 9.5),
        HistInfo("nJets_At2LooseBs_" + flag, "N_{jets}", 11, -0.5, 10.5),
        HistInfo("nJets_At2MediumBs_" + flag, "N_{jets}", 11, -0.5, 10.5),
        HistInfo("HT_AtSel_" + flag, "H_{T} [GeV]", 25, 0, 500),
        HistInfo("nJets_At3LooseBs_" + flag, "N_{jets}", 11, -0.5, 10.5),
        HistInfo("nJets_At2LooseBs_AfterCuts_" + flag, "N_{jets}", 11, -0.5, 10.5),
        HistInfo("nJets_At3LooseBs_AfterCuts_" + flag, "N_{jets}", 11, -0.5, 10.5),
        HistInfo("nJets_At2MediumBs_AfterCuts_" + flag, "N_{jets}", 11, -0.5, 10.5),
        HistInfo("nLepsAfterChargeConsistency_" + flag, "N_{l}", 8, -0.5, 7.5),
        HistInfo("nLepsAfterConversionVeto_" + flag, "N_{l}", 8, -0.5, 7.5),
        HistInfo("nLepsAfterLepMVA_" + flag, "N_{l}", 8, -0.5, 7.5),
        // HistInfo("LowestLepMVAScore_" + flag, "score", 100, -1., 1.),
        HistInfo("lepMVALeadingLepton_" + flag, "leptonMVA score leading loose lepton", 40, -1., 1.),
        HistInfo("lepMVASubleadingLepton_" + flag, "leptonMVA score sublead loose lepton", 40, -1., 1.),
        HistInfo("lepMVASubSubleadinfLepton_" + flag, "leptonMVA score subsublead loose lepton", 40, -1., 1.),
        HistInfo("lepMVATrailingLeptonNew_" + flag, "leptonMVA score trailing loose lepton", 40, -1., 1.),
        HistInfo("LowestLepMVAScore_" + flag, "score", 40, -1., 1.),
        HistInfo("LeptonsAtLooseWP_" + flag, "N_{l}", 8, -0.5, 7.5),
        HistInfo("nEventsWith3T1Looser_" + flag, "N_{l}", 2, -0.5, 1.5),
        HistInfo("nLepsNotTightButLooseWP_" + flag, "N_{l}", 4, -0.5, 3.5)
    };

    // variables for selection

    return histInfoVec;
}

std::vector<HistInfo>* getSelectHist(std::string flag, bool genInfo) {
    std::vector<HistInfo>* histInfoVec = new std::vector<HistInfo>;
    *histInfoVec = {
        HistInfo("electrons_" + flag, "", 2, -0.5, 1.5),
        HistInfo("muons_" + flag, "", 2, -0.5, 1.5)
    };

    // variables for selection

    return histInfoVec;
}


bool selectLeptonsPt(const Lepton& lepton) {
    return (lepton.pt() > 10);
}

bool selectLeptonsBase(const LightLepton& lepton) {
    if (! lepton.isPrompt()) return false;
    if (lepton.uncorrectedPt() < 10) return false;
    if (lepton.absEta() > 2.5) return false;
    if (fabs(lepton.dxy()) > 0.05) return false;
    if (fabs(lepton.dz()) > 0.1) return false;
    if (lepton.sip3d() >= 8) return false;
    return true;
}

bool selectElectronsBase(const Electron& electron) {
    if( electron.numberOfMissingHits() >= 2 ) return false;
    return true;
}


bool selectMuonsBase(const Muon& muon) {
    if( !muon.isMediumPOGMuon() ) return false;
    return true;
}

bool selectLeptonsIrel(const LightLepton& lepton) {
    return (lepton.miniIso() < 0.4);
}

bool selectLeptonsLooseMVA(const LightLepton& lepton) {
    return (lepton.leptonMVATOP() > 0.);
}

bool selectLeptonsMVA(const LightLepton& lepton) {
    if (lepton.isElectron()) {
        return (lepton.leptonMVATOPUL() > 0.81);
    } else {
        return (lepton.leptonMVATOPUL() > 0.64);
    }
}

bool selectLeptonsChargeConsistency(const LightLepton& lepton) {
    return lepton.isTightCharge();
}

bool selectElectronsConversionVeto(const LightLepton& lepton) {
    if (!lepton.isElectron()) return true;
    const Electron& el = (Electron&)lepton;
    return el.passConversionVeto();
}

void FourTop::cutFlow(std::string& sortingMode) {
    bool sortOnGenerator = false;
    if (sortingMode == "GeneratorInfo") {
        sortOnGenerator = true;
    }

    // reweighting
    std::shared_ptr<ReweighterFactory> reweighterFactory(new FourTopReweighterFactory());
    ReweighterBTagShape** btagReweighter;
    CombinedReweighter reweighter;
    CombinedSampleReweighter* sampleReweighter = nullptr;
    //if (!sortOnGenerator) {
    //    std::cout << "building reweighter" << std::endl;
    //    btagReweighter = new ReweighterBTagShape*();
    //    reweighter = reweighterFactory->buildReweighter("../weights/", yearString, treeReader->sampleVector(), btagReweighter, testRun);
    //    addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");
    //}

    std::string channelDL = "DL";
    std::vector<HistInfo>* infoDL = getSelectHist(channelDL, sortOnGenerator);
    HistogramManager* DLManager = new HistogramManager(channelDL, infoDL);

    std::string channel3L = "3L";
    std::vector<HistInfo>* info3L = getSelectHist(channel3L, sortOnGenerator);
    HistogramManager* TriLManager = new HistogramManager(channel3L, info3L);

    std::string channel4L = "4L";
    std::vector<HistInfo>* info4L = getSelectHist(channel4L, sortOnGenerator);
    HistogramManager* FourLManager = new HistogramManager(channel4L, info4L);

    std::string channelGarbage = "garbage";
    std::vector<HistInfo>* infoGarbage = getSelectHist(channelGarbage, sortOnGenerator);
    HistogramManager* garbageManager = new HistogramManager(channelGarbage, infoGarbage);

    std::cout << "event loop" << std::endl;
    currentEvent = new Event();

    //std::vector<unsigned long> evNbs = {282124, 282864, 283586, 283905, 283936, 548982, 615684, 682301, 712028, 712057, 
    //        712199, 712800, 712849, 770093, 891544, 896198, 896314, 896382, 927158, 927395, 
    //        958217, 958304, 958608, 959203, 961251, 961485, 1018199, 1018319, 1019000, 1019519, 
    //        1050902, 1093192, 1093833, 1598281, 1598570, 1906027, 1906159, 1906316, 1906652, 
    //        12372968, 13845208, 13845310, 13994079};

    //std::vector<unsigned long> evNbs = {961444, 682726, 282104, 13780131, 282240, 1050612, 1598220, 13994387, 13845072, 770996, 959904, 927881, 282334, 614633, 614109, 1906054, 896603, 1050821, 13780097, 891625};
    //std::vector<unsigned long> evNbs = {282124, 282864, 283778, 549380, 615684, 682266, 712057, 771721, 771997, 896314, 927998, 958217, 961986, 1018993, 1019000, 1019519, 1019955, 1050779, 1050902, 1093833, 1598056, 1598552, 1906027, 1906316, 1906652, 12372685, 13994079};
    //std::vector<unsigned long> evNbs = {1019519, 1019955, 1050902, 283413, 549513, 615684, 682266};

    std::vector<unsigned long> evaluatedEvNbs;

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    for (unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex) {
        treeReader->initSample();

        std::cout << treeReader->currentSample().fileName() << std::endl;

        std::string uniqueName = sampleVec[sampleIndex].uniqueName();
        DLManager->newSample(uniqueName);
        TriLManager->newSample(uniqueName);
        FourLManager->newSample(uniqueName);
        garbageManager->newSample(uniqueName);

        ttgOverlapCheck = treeReader->currentSamplePtr()->ttgOverlap();
        zgOverlapCheck = treeReader->currentSamplePtr()->zgOverlap();

        std::vector<std::shared_ptr<TH1D>>* allDLHist = DLManager->getHistograms(false);
        std::vector<std::shared_ptr<TH1D>>* all3LHist = TriLManager->getHistograms(false);
        std::vector<std::shared_ptr<TH1D>>* all4LHist = FourLManager->getHistograms(false);
        std::vector<std::shared_ptr<TH1D>>* allGarbageHist = garbageManager->getHistograms(false);

        std::shared_ptr<TH1D> dlHist = allDLHist->at(0);
        std::shared_ptr<TH1D> trilepHist = all3LHist->at(0);
        std::shared_ptr<TH1D> fourlepHist = all4LHist->at(0);
        std::shared_ptr<TH1D> garbageHist = allGarbageHist->at(0);

        if (sortOnGenerator) std::cout << "SORT ON GEN PARTICLES" << std::endl;
        // at end of sel, count extra if it flows to other channels (3l and then ssdl?)
        double weight = 1.;

        std::ofstream eventTagsOutput;        
        std::ofstream eventTagsNBS;
        std::string samplename = treeReader->sampleVector()[0].uniqueName();
        eventTagsOutput.open("Output/EventTags_" + samplename + ".txt");        
        eventTagsNBS.open("Output/EventTagsNBS_" + samplename + ".txt");
        
        for (long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry) {
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr(entry);
            // check if eventnumber matches any in evNbs
            unsigned long evNb = currentEvent->eventNumber();

            //if (std::find(evNbs.begin(), evNbs.end(), evNb) == evNbs.end()) continue;
            //evaluatedEvNbs.push_back(evNb);
            //eventTagsOutput << std::endl << std::endl;
            //eventTagsOutput << evNb << std::endl;
            //std::cout << evNb << std::endl;

            // selection->addNewEvent(currentEvent); 
            // if (! currentEvent->passMetFilters()) continue;

            // if (! eventPassesTriggers()) continue;
            // if (! selection->leptonsArePrompt()) continue;
            //eventTagsOutput << "has 3 leptons: " << std::endl;
            //if (selection->numberOfLeps() != 3) continue;
            //eventTagsOutput << "yes " << std::endl;
            currentEvent->removeTaus();
            // currentEvent->selectLooseLeptons();
            currentEvent->sortLeptonsByPt();

            //currentEvent->selectLeptons(selectLeptonsPt);
            currentEvent->selectLightLeptons(selectLeptonsBase);
            currentEvent->selectElectrons(selectElectronsBase);
            currentEvent->selectMuons(selectMuonsBase);
            ElectronCollection electrons = currentEvent->electronCollection();
            MuonCollection muons = currentEvent->muonCollection();
            allDLHist->at(0)->Fill(0., electrons.size());
            allDLHist->at(1)->Fill(0., muons.size());

            electrons.selectObjects(selectElectronsBase);
            muons.selectObjects(selectMuonsBase);

            allDLHist->at(0)->Fill(1., electrons.size());
            allDLHist->at(1)->Fill(1., muons.size());



            //for (auto lep : currentEvent->lightLeptonCollection()) {
            //    int nMissingHits = 0;
            //    bool passMVA = false;
            //    if (lep->isElectron()) {
            //        eventTagsOutput << "electron, ";
            //        Electron* el = (Electron*) lep.get();
            //        nMissingHits = el->numberOfMissingHits();
            //        passMVA = el->passElectronMVAFall17NoIsoLoose();
            //    } else {
            //        eventTagsOutput << "muon, ";   
            //    }
            //    eventTagsOutput << lep->pt() << "," << lep->eta() << "," << lep->phi()<< "," << lep->dxy()<< "," << lep->dz()<< "," << lep->sip3d()<< "," << nMissingHits << "," << lep->miniIso() << "," << lep->closestJetDeepFlavor() << "," << lep->ptRatio() << "," << passMVA << "," << lep->isFO() << "," << lep->leptonMVATOPUL();
            //    eventTagsOutput << std::endl;
            //}
            //continue;
            //eventTagsOutput << "pass lepton selection: " << std::endl;
            //if (selection->numberOfLeps() < 2) continue;
            ////if (!selection->passLeptonSelection()) continue;

            //eventTagsOutput << "true" << std::endl;
            //std::shared_ptr<TH1D> cutflowHist;
            //std::vector<std::shared_ptr<TH1D>>* currentHistSet;

            //int nLeps = selection->numberOfLeps();
            //int nTightLeps = nLeps;

            //eventTagsOutput << "nLeps == " << nLeps << std::endl;
            ////if (nLeps > 2) continue;

            ////if (nLeps < 2) continue;

            //if (nLeps == 2) {
            //    cutflowHist = dlHist;
            //    currentHistSet = allDLHist;
            //} else if (nLeps == 3) {
            //    cutflowHist = trilepHist;
            //    currentHistSet = all3LHist;
            //} else if (nLeps >= 4) {
            //    cutflowHist = fourlepHist;
            //    currentHistSet = all4LHist;
            //}

            //weight = 1.;// currentEvent->weight();
            ////weight *= reweighter.totalWeight(*currentEvent);
            //selection->classifyEvent();
            //eventClass nominalClass = selection->getCurrentClass();

            ////std::cout << nLeps << std::endl;
            //cutflowHist->Fill(0., weight);
            //

            //cutflowHist->Fill(1., weight);
            //

            //currentHistSet->at(7)->Fill(currentEvent->numberOfGoodJets(), weight);
            //currentHistSet->at(8)->Fill(currentEvent->numberOfLooseBTaggedJets(), weight);
            //currentHistSet->at(9)->Fill(currentEvent->numberOfMediumBTaggedJets(), weight);
            //currentHistSet->at(10)->Fill(currentEvent->HT(), weight);

            //cutflowHist->Fill(6., weight);
            //
            //currentEvent->sortLeptonsByPt();
            //
            //eventTagsOutput << "Lepton pts for trigger: " << selection->getLepton(0)->pt() << " " << selection->getLepton(1)->pt() << std::endl;

            //if (selection->getLepton(0)->pt() < 25 || selection->getLepton(1)->pt() < 20) continue;

            //cutflowHist->Fill(7., weight);
            //
            //eventTagsOutput << "pass low mass veto: " << std::endl;

            //// Remove mass resonances
            //if (! selection->passLowMassVeto()) continue;

            //eventTagsOutput << "true" << std::endl;


            //cutflowHist->Fill(8., weight);
            //
            //eventTagsOutput << "Z veto: " << std::endl;

            //if (!selection->passZBosonVeto()) continue;
            //eventTagsOutput << "Passed" << std::endl;

            //cutflowHist->Fill(9., weight);
            //

            //currentHistSet->at(13)->Fill(currentEvent->numberOfJets(), weight);
            //eventTagsOutput << "NJets " << selection->numberOfJets() << std::endl;


            //if ((nLeps == 2 && selection->numberOfJets() < 3) || (nLeps == 3 && selection->numberOfJets() < 3) || (nLeps == 4 && selection->numberOfJets() < 2)) continue;
            //cutflowHist->Fill(10., weight);
            //

            //if (selection->numberOfLooseBJets() == 2) currentHistSet->at(16)->Fill(selection->numberOfJets(), weight);
            //if (selection->numberOfLooseBJets() == 3) currentHistSet->at(19)->Fill(selection->numberOfJets(), weight);
            //if (selection->numberOfMediumBJets() == 2) currentHistSet->at(17)->Fill(selection->numberOfJets(), weight);

            //currentHistSet->at(14)->Fill(selection->numberOfLooseBJets(), weight);
            //currentHistSet->at(15)->Fill(selection->numberOfMediumBJets(), weight);

            //eventTagsOutput << "Loose Bs " << selection->numberOfLooseBJets() << std::endl;

            //if ((nLeps < 4 && selection->numberOfLooseBJets() < 2) || (nLeps == 4 && selection->numberOfLooseBJets() < 1)) continue;
            //cutflowHist->Fill(11., weight);
            //


            ////if ((nLeps == 2 && selection->numberOfLooseBJets() == 2 && selection->numberOfJets() < 6)) continue;
            ////cutflowHist->Fill(12., weight);
            //
            //eventTagsOutput << "HT " << selection->getHT() << std::endl;


            //currentHistSet->at(18)->Fill(selection->getHT(), weight);

            //if ((nLeps == 2 && selection->getHT() < 200) || (nLeps == 3 && selection->getHT() < 200)) continue;
            //cutflowHist->Fill(13., weight);
            ////if (nominalClass == eventClass::trilep || nominalClass == eventClass::ssdl) eventTagsNBS << currentEvent->eventNumber() << std::endl;
            //if (! selection->passPhotonOverlapRemoval()) continue;

            //eventTagsNBS << currentEvent->eventNumber() << std::endl;
            //if (selection->numberOfLooseBJets() == 2) currentHistSet->at(20)->Fill(selection->numberOfJets(), weight);
            //if (selection->numberOfLooseBJets() == 3) currentHistSet->at(21)->Fill(selection->numberOfJets(), weight);
            //if (selection->numberOfMediumBJets() == 2) currentHistSet->at(22)->Fill(selection->numberOfJets(), weight);

            //if (nominalClass == eventClass::ssdl) cutflowHist->Fill(14., weight);
            //if (nominalClass == eventClass::trilep) cutflowHist->Fill(15., weight);
            //if (nominalClass == eventClass::fourlep) cutflowHist->Fill(16., weight);
        }

        // Output management: save histograms to a ROOT file.
        // Processes were named in samplelist. Should use this to make directory for process and subdir for sample

        // Might interface with Stacker to create desired output plots as well... Or at least already have the stacked process ready instead of individual components. Then a "getDirectory" in stacker could be handy to see if it exists.
        outfile->cd();
        outfile->cd("Nominal");
        const char* processName = treeReader->currentSample().processName().c_str();
        if (!gDirectory->GetDirectory(processName)) {
            gDirectory->mkdir(processName);
        }
        gDirectory->cd(processName);

        std::string outdir = stringTools::fileNameFromPath(treeReader->currentSample().fileName());
        gDirectory->mkdir(outdir.c_str());  // got to switch to gDirectory. Otherwise keeps working as if we're on level of file
        gDirectory->cd(outdir.c_str());

        // Rewrite this to a dedicated function maybe, or something where we don't have to call sampleIndex each time?
        // for( size_t dist = 0; dist < histInfoVec_DL->size(); ++dist ) {
        //    hists_DL->at(sampleIndex)[dist]->Write(TString(histInfoVec_DL->at(dist).name()), TObject::kOverwrite);
        //}
        DLManager->writeCurrentHistograms();
        TriLManager->writeCurrentHistograms();
        FourLManager->writeCurrentHistograms();
        garbageManager->writeCurrentHistograms();
        eventTagsOutput.close();
        eventTagsNBS.close();
    }

    // Don't forget non-prompt contributions
    outfile->cd("Nominal");
    gDirectory->mkdir("nonPrompt");
    gDirectory->cd("nonPrompt");

    DLManager->writeNonpromptHistograms();
    TriLManager->writeNonpromptHistograms();
    FourLManager->writeNonpromptHistograms();
    garbageManager->writeNonpromptHistograms();

    //std::cout << "evaluated " << evaluatedEvNbs.size() << "/" << evNbs.size() << std::endl;
    for (auto el : evaluatedEvNbs) {
        std::cout << el << std::endl;
    }

    outfile->Close();
}

int main(int argc, char* argv[]) {
    setTDRStyle();

    if (argc < 3) {
        std::cout << "Two argument required: <sampleList.txt> <GeneratorInfo available>" << std::endl;
    }
    // zet makkelijk te verwerken string op
    std::vector<std::string> argvStr(&argv[0], &argv[0] + argc);

    // Settings splitsen if necessary

    // main func call
    FourTop analysisObject("Cutflow", argvStr, 0);

    if (argvStr.size() < 3) {
        argvStr.push_back("noGen");
    }
    analysisObject.cutFlow(argvStr[2]);
}
