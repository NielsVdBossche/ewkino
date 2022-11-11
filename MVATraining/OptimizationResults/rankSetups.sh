#!/bin/bash
source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_98python3 x86_64-centos7-gcc10-opt

export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)

cd /storage_mnt/storage/user/nivanden/ewkino/MVATraining/OptimizationResults

python3 RankSetups.py
