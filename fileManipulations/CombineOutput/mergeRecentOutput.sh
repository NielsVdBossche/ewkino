#!/bin/bash

cd /user/nivanden/CMSSW_10_6_20/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER) 

cd /user/nivanden/ewkino/_FourTopAnalysis/
file_16Pre=$(ls -t Output/*2016Pre* | head -1)
file_16Post=$(ls -t Output/*2016Post* | head -1)
file_17=$(ls -t Output/*2017* | head -1)
file_18=$(ls -t Output/*2018* | head -1)

./../fileManipulations/CombineOutput/combineOutputExec $file_18 $file_17 $file_16Pre $file_16Post
