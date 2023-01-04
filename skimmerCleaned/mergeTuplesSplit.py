#!/usr/bin/env python

import os
import sys
import subprocess

sys.path.append(os.path.abspath('../'))
from jobSubmission.condorTools import submitCommandsAsCondorCluster



# input: /pnfs/iihe/cms/store/user/nivanden/SomeFolder/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/220630_155601_0000_singlelep_0.root
# output: /pnfs/iihe/cms/store/user/nivanden/SomeFolder/year/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2.root

inputBase = "/pnfs/iihe/cms/store/user/nivanden/skims_v6/rawSkims/"
outputBase = "/pnfs/iihe/cms/store/user/nivanden/skims_v6/"

skimVersion = []
outSubdir = ""

if (sys.argv[1] == '0'):
    skimVersion = ["2016_ULpreVFP", "RunIISummer20UL16MiniAODAPV", "MiniAOD2016preVFP", "2016_ULpreVFP_Nov"]
    outSubdir = "2016PreVFP"
elif (sys.argv[1] == '1'):
    skimVersion = ["2016_ULpostVFP", "RunIISummer20UL16MiniAOD-106X", "MiniAOD2016postVFP", "2016_ULpostVFP_Nov"]
    outSubdir = "2016PostVFP"
elif (sys.argv[1] == '4'):
    skimVersion = ["Run2016"]
    outSubdir = "2016"
elif (sys.argv[1] == '2'):
    skimVersion = ["2017_UL", "RunIISummer20UL17MiniAOD", "MiniAOD2017", "2017_Nov", "Run2017"]
    outSubdir = "2017"
elif (sys.argv[1] == '3'):
    skimVersion = ["2018_UL", "RunIISummer20UL18MiniAOD", "MiniAOD2018", "2018_Nov", "Run2018"]
    outSubdir = "2018"

print(skimVersion)
print(sys.argv)

if (len(sys.argv) >= 3):
    if (sys.argv[2] == "TTTrainingSamples"):
        outSubdir += "/mvaSamples"

processes = ["WZJJ"]

commands = []
for dir in os.listdir(inputBase):
    print(dir)

    if not any(skimver in dir for skimver in skimVersion):
        continue

    if not any(process in dir for process in processes):
        continue
    
    # join path outputbase + folder
    outputDir  = os.path.join(outputBase, outSubdir)
    print(outputDir)
    # decide outputname: strip inputdirname of useless stuff en same for individual files (singlelep, date, 0000/0001 stuff)
    # Handle different files in same dir.. Should be fixed at skimming level... Not because same identifier that they're allowed to be in same subdir. For this, add some uniqueness to naming?
    outputFileName = dir + ".root"

    # join outputname with outputfolder
    outputPath = os.path.join(outputDir, outputFileName)
    
    print(outputPath)
    print(os.path.join(inputBase, dir))

    # call hadd output inputs (star?)

    commands.append("hadd -f {} {}/*.root".format(outputPath, os.path.join(inputBase, dir)))

submitCommandsAsCondorCluster("hadd", commands)