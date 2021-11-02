#!/bin/bash

cd /user/nivanden/CMSSW_10_6_20/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER) 

hadd -f /pnfs/iihe/cms/store/user/nivanden/skims/TTTTUnskimmed_2016PreVFP.root /pnfs/iihe/cms/store/user/nivanden/heavyNeutrino/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/crab_RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_2016_ULpreVFPv3/210920_092026/0000/*.root
hadd -f /pnfs/iihe/cms/store/user/nivanden/skims/TTTTUnskimmed_2016PostVFP.root /pnfs/iihe/cms/store/user/nivanden/heavyNeutrino/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/crab_RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_2016_ULpostVFPv3/210920_091651/0000/*.root
hadd -f /pnfs/iihe/cms/store/user/nivanden/skims/TTTTUnskimmed_2017.root /pnfs/iihe/cms/store/user/nivanden/heavyNeutrino/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/crab_RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_2017_ULv3/210920_092353/0000/*.root
hadd -f /pnfs/iihe/cms/store/user/nivanden/skims/TTTTUnskimmed_2018.root /pnfs/iihe/cms/store/user/nivanden/heavyNeutrino/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/crab_RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_2018_ULv3/210920_092739/0000/*.root
