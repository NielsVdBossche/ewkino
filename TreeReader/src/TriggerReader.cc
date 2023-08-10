#include "../interface/NanoReader.h"

// helpers
#include "../../Tools/interface/stringTools.h"

NanoReader::TriggerReader::TriggerReader(NanoReader& nanoReader, TTree* treePtr, bool resetTriggersAndFilters) {
    initFlags(nanoReader);

    // add all individually stored triggers
    if (resetTriggersAndFilters || _triggerMap.empty()) {
        initializeTriggerMap(treePtr);
    }
    //setMapBranchAddresses(treePtr, _triggerMap, b__triggerMap);

    //// add all individually stored MET filters
    //if (resetTriggersAndFilters || _METFilterMap.empty()) {
    //    initializeMETFilterMap(treePtr);
    //}
    //setMapBranchAddresses(treePtr, _METFilterMap, b__METFilterMap);
}

void NanoReader::TriggerReader::initFlags(NanoReader& nanoReader) {
    // No overlap can exist between various triggernames! Will fuck up the mapping.
    if (nanoReader.is2018()) {
        triggerNames["passTrigger_FR"] = {"HLT_Mu3_PFJet40", "HLT_Mu8", "HLT_Mu17", "HLT_Mu20",
                                          "HLT_Mu27", "HLT_Ele8_CaloIdM_TrackIdM_PFJet30",
                                          "HLT_Ele17_CaloIdM_TrackIdM_PFJet30",
                                          "HLT_Ele23_CaloIdM_TrackIdM_PFJet30"};
        triggerNames["passTrigger_FR_iso"] = {"HLT_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL",
                                              "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              //"HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"};
        triggerNames["passTrigger_m"] = {"HLT_IsoMu24", "HLT_IsoMu27", "HLT_Mu50",
                                         "HLT_OldMu100", "HLT_TkMu100"};
        triggerNames["passTrigger_e"] = {"HLT_Ele32_WPTight_Gsf", "HLT_Ele115_CaloIdVT_GsfTrkIdT",
                                         "HLT_Photon200"};
        triggerNames["passTrigger_mm"] = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8",
                                          "HLT_Mu37_TkMu27"};
        triggerNames["passTrigger_em"] = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
                                          "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
                                          "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
                                          "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
                                          "HLT_Mu27_Ele37_CaloIdL_MW", "HLT_Mu37_Ele27_CaloIdL_MW"};
        triggerNames["passTrigger_ee"] = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL",
                                          "HLT_DoubleEle25_CaloIdL_MW"};
        triggerNames["passTrigger_mmm"] = {"HLT_TripleMu_10_5_5_DZ",
                                           //"HLT_TripleMu_5_3_3_Mass3p8to60_DZ",
                                           "HLT_TripleMu_5_3_3_Mass3p8_DZ",
                                           //"TripleMu_12_10_5",
                                           "HLT_TripleMu_12_10_5"};
        // special case: some 2018A files (but not all?)
        // seem to contain "HLT_TripleMu_5_3_3_Mass3p8to60_DZ",
        // instead of "HLT_TripleMu_5_3_3_Mass3p8_DZ"
        if (stringTools::stringContains(nanoReader.currentSamplePtr()->filePath(), "Run2018A")) {
            if (_triggerMap.count("HLT_TripleMu_5_3_3_Mass3p8_DZ") == 0) {
                triggerNames["passTrigger_mmm"][1] = "HLT_TripleMu_5_3_3_Mass3p8to60_DZ";
            }
        }
        triggerNames["passTrigger_emm"] = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ"};
        triggerNames["passTrigger_eem"] = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_eee"] = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_ref"] = {"HLT_CaloMET350_HBHECleaned", "HLT_CaloJet500_NoJetID",
                                           "HLT_AK8PFJet500", "HLT_AK8PFJet400_TrimMass30",
                                           "HLT_DiJet110_35_Mjj650_PFMET110", "HLT_PFHT800_PFMET75_PFMHT75_IDTight",
                                           "HLT_PFHT700_PFMET85_PFMHT85_IDTight",
                                           "HLT_PFHT500_PFMET100_PFMHT100_IDTight", "HLT_PFHT1050", "HLT_PFJet500",
                                           "HLT_PFMET120_PFMHT120_IDTight",
                                           "HLT_PFMET250_HBHECleaned", "HLT_PFMET200_HBHE_BeamHaloCleaned",
                                           "HLT_PFMETTypeOne140_PFMHT140_IDTight",
                                           "HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned",
                                           "HLT_TripleJet110_35_35_Mjj650_PFMET110"};

    } else if (nanoReader.is2017()) {
        triggerNames["passTrigger_FR"] = {"HLT_Mu3_PFJet40", "HLT_Mu8", "HLT_Mu17", "HLT_Mu20", "HLT_Mu27",
                                          "HLT_Ele8_CaloIdM_TrackIdM_PFJet30", "HLT_Ele17_CaloIdM_TrackIdM_PFJet30",
                                          "HLT_Ele23_CaloIdM_TrackIdM_PFJet30"};
        triggerNames["passTrigger_FR_iso"] = {"HLT_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL",
                                              "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              //"HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"};
        triggerNames["passTrigger_m"] = {"HLT_IsoMu24", "HLT_IsoMu24_eta2p1", "HLT_IsoMu27", "HLT_Mu50", "HLT_OldMu100", "HLT_TkMu100"};
        triggerNames["passTrigger_e"] = {"HLT_Ele32_WPTight_Gsf", "HLT_Ele35_WPTight_Gsf", "HLT_Ele115_CaloIdVT_GsfTrkIdT", "HLT_Photon200"};
        triggerNames["passTrigger_mm"] = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", "HLT_Mu37_TkMu27",
                                          "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"};
        triggerNames["passTrigger_em"] = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
                                          "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
                                          "HLT_Mu27_Ele37_CaloIdL_MW", "HLT_Mu37_Ele27_CaloIdL_MW"};
        triggerNames["passTrigger_ee"] = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", "HLT_DoubleEle33_CaloIdL_MW"};
        triggerNames["passTrigger_mmm"] = {"HLT_TripleMu_10_5_5_DZ",
                                           //"HLT_TripleMu_5_3_3_Mass3p8to60_DZ",
                                           "HLT_TripleMu_5_3_3_Mass3p8_DZ",
                                           //"TripleMu_12_10_5",
                                           "HLT_TripleMu_12_10_5"};
        triggerNames["passTrigger_emm"] = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ"};
        triggerNames["passTrigger_eem"] = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_eee"] = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_ref"] = {"HLT_PFJet500", "HLT_PFMET140_PFMHT140_IDTight", "HLT_PFHT500_PFMET100_PFMHT100_IDTight",
                                           "HLT_PFHT700_PFMET85_PFMHT85_IDTight", "HLT_PFHT800_PFMET75_PFMHT75_IDTight", "HLT_CaloJet500_NoJetID",
                                           "HLT_AK8PFJet500"};

    } else {
        triggerNames["passTrigger_FR"] = {"HLT_Mu3_PFJet40", "HLT_Mu8", "HLT_Mu17", "HLT_Mu20", "HLT_Mu27",
                                          "HLT_Ele8_CaloIdM_TrackIdM_PFJet30", "HLT_Ele12_CaloIdM_TrackIdM_PFJet30",
                                          "HLT_Ele17_CaloIdM_TrackIdM_PFJet30", "HLT_Ele23_CaloIdM_TrackIdM_PFJet30"};
        triggerNames["passTrigger_FR_iso"] = {"HLT_Mu8_TrkIsoVVL",
                                              "HLT_Mu17_TrkIsoVVL",
                                              "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              //"HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30",
                                              "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"};
        triggerNames["passTrigger_m"] = {"HLT_IsoMu24", "HLT_IsoTkMu24", "HLT_Mu50", "HLT_TkMu50", "HLT_Mu45_eta2p1"};
        triggerNames["passTrigger_e"] = {"HLT_Ele27_WPTight_Gsf", "HLT_Ele105_CaloIdVT_GsfTrkIdT", "HLT_Ele115_CaloIdVT_GsfTrkIdT", "HLT_Photon175"};
        triggerNames["passTrigger_mm"] = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",
                                          "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",
                                          "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL", "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",
                                          "HLT_Mu30_TkMu11"};
        triggerNames["passTrigger_em"] = {"HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
                                          "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL", "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ",
                                          "HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL", "HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL"};
        triggerNames["passTrigger_ee"] = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL",
                                          "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW"};
        triggerNames["passTrigger_mmm"] = {"HLT_TripleMu_12_10_5"};
        triggerNames["passTrigger_emm"] = {"HLT_DiMu9_Ele9_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_eem"] = {"HLT_Mu8_DiEle12_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_eee"] = {"HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL"};
        triggerNames["passTrigger_ref"] = {"HLT_MET200", "HLT_PFMET300", "HLT_PFMET170_HBHECleaned", "HLT_PFMET120_PFMHT120_IDTight",
                                           "HLT_PFHT300_PFMET110", "HLT_PFHT350_DiPFJetAve90_PFAlphaT0p53", "HLT_PFHT400_DiPFJetAve90_PFAlphaT0p52",
                                           "HLT_PFHT400_SixJet30_DoubleBTagCSV_p056", "HLT_PFHT900", "HLT_PFHT650_WideJetMJJ900DEtaJJ1p5",
                                           "HLT_CaloJet500_NoJetID"};
    }
}

void NanoReader::TriggerReader::initializeTriggerMap(TTree* treePtr) {
    // loop over trigger map, per trigger name, loop content and add 
    for (auto& triggerName : triggerNames) {
        _triggerMap[triggerName.first] = std::vector<Bool_t*>(triggerName.second.size());
        
        for (unsigned i=0; i<_triggerMap[triggerName.first].size(); i++) *_triggerMap[triggerName.first][i] = false;
        unsigned i = 0;
        for (auto& trigger : triggerName.second) {
            b__triggerMap[trigger] = nullptr;

            if (! treeHasBranchWithName(treePtr, trigger)) {
                std::string msg = "ERROR in TriggerReader initializer:";
                msg += " trigger " + trigger + " not found in NanoFile.";
                throw std::runtime_error(msg);
            }

            treePtr->SetBranchAddress(trigger.c_str(), _triggerMap[triggerName.first][i], &b__triggerMap[trigger]);
            i++;
        }
    }
    // // change structure: make it so that the trigger address is part of an array?
    // auto triggerMaps = buildBranchMap(treePtr, {"HLT_"});
    // _triggerMap = triggerMaps.first;
    // b__triggerMap = triggerMaps.second;
}

// void NanoReader::TriggerReader::initializeMETFilterMap(TTree* treePtr) {
//     auto filterMaps = buildBranchMap(treePtr, {"Flag_"});
//     _METFilterMap = filterMaps.first;
//     b__METFilterMap = filterMaps.second;
// }
