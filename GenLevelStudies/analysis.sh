#!/bin/bash

cd /user/nivanden/CMSSW_10_6_20/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER) 

cd /user/nivanden/ewkino/_FourTopAnalysis/
./genLevelExec ../sampleLists/FourTopsOnly.txt 

