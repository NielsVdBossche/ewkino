#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /user/nivanden/CMSSW_10_6_27/src
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
cd /storage_mnt/storage/user/nivanden/ewkino/_FourTopAnalysis/SyncCheck
echo "stacker5.sh"
./analysisExec /user/nivanden/sync_objects_TTTT.root ../../sampleLists/TmpLists/2017_fullStat_TTTT_0.txt 
