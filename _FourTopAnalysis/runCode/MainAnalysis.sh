#!/bin/bash

cd /user/nivanden/CMSSW_10_6_27/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
## source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_102 x86_64-centos7-gcc11-opt
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER) 

cd /user/nivanden/ewkino/_FourTopAnalysis/
./analysisExec "../sampleLists/$@"
