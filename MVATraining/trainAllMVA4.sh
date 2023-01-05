#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /user/nivanden/CMSSW_10_6_27/src
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
cd /storage_mnt/storage/user/nivanden/ewkino/MVATraining
./trainMVAMethodsExec $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
