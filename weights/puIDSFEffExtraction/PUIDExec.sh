#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /user/nivanden/el9_cmssws/CMSSW_13_3_3/src
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
cd /user/nivanden/ewkino/weights/puIDSFEffExtraction
echo "PUIDExec.sh"
echo $(pwd)

./computePUIDEfficienciesMC "sampleLists/fourtop_puidEff_${1}.txt" $1 FOLeptons nanoAOD
