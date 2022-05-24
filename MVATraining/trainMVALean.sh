#!/bin/bash

cd /user/nivanden/CMSSW_10_6_27/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER) 

cd /user/nivanden/ewkino/MVATraining/

./trainMVAMethodsExec sampleLists/DLTest.txt DL_tree_loose DL_BDT no
./trainMVAMethodsExec sampleLists/DLTest.txt ML_tree_loose ML_BDT no
