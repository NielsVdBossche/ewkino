#!/bin/bash

cd /user/nivanden/CMSSW_10_6_27/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER) 

cd /user/nivanden/ewkino/_FourTopAnalysis/

echo "$@"

./analysisTreeExec "../sampleLists/$1" $2 $3 "timestamp=$4" $5

echo "Analysis code finished! Now copying files to /pnfs"
cp $TMPDIR/*.root /pnfs/iihe/cms/store/user/nivanden/AnalysisOutput/ReducedTuples/$4/
