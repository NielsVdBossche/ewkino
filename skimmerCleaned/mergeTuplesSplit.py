#!/usr/bin/env python

import os
import sys
import subprocess
import random

sys.path.append(os.path.abspath('../'))
from jobSubmission.condorTools import submitCommandsAsCondorCluster



# input: /pnfs/iihe/cms/store/user/nivanden/SomeFolder/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/220630_155601_0000_singlelep_0.root
# output: /pnfs/iihe/cms/store/user/nivanden/SomeFolder/year/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2.root

inputBase = "/pnfs/iihe/cms/store/user/nivanden/skims_v6/rawSkims"
outputBase = "/pnfs/iihe/cms/store/user/nivanden/skims_v6/"

skimVersion = []
outSubdir = ""

if (sys.argv[1] == '0'):
    skimVersion = ["2016_ULpreVFP", "RunIISummer20UL16MiniAODAPV", "MiniAOD2016preVFP", "2016_ULpreVFP_Nov", "Summer20UL16MiniAODAPV"]
    outSubdir = "2016PreVFP"
elif (sys.argv[1] == '1'):
    skimVersion = ["2016_ULpostVFP", "RunIISummer20UL16MiniAOD-106X", "MiniAOD2016postVFP", "2016_ULpostVFP_Nov", "Summer20UL16MiniAOD_MC"]
    outSubdir = "2016PostVFP"
elif (sys.argv[1] == '4'):
    skimVersion = ["Run2016"]
    outSubdir = "2016"
elif (sys.argv[1] == '2'):
    skimVersion = ["2017_UL", "RunIISummer20UL17MiniAOD", "MiniAOD2017", "2017_Nov", "Run2017", "Summer20UL17"]
    outSubdir = "2017"
elif (sys.argv[1] == '3'):
    skimVersion = ["2018_UL", "RunIISummer20UL18MiniAOD", "MiniAOD2018", "2018_Nov", "Run2018", "Summer20UL18"]
    outSubdir = "2018"

print(skimVersion)
print(sys.argv)

if (len(sys.argv) >= 3):
    if (sys.argv[2] == "TTTrainingSamples"):
        outSubdir += "/mvaSamples"

processes = [""]
fraction = 1. # if not 1 -> not all samples will be used
commands = []

for dir in os.listdir(inputBase):
    if not any(skimver in dir for skimver in skimVersion):
        continue

    #if not any(process in dir for process in processes):
    #    continue

    if not any(dir.startswith(process) for process in processes):
        continue
    
    print(dir)
    # join path outputbase + folder
    outputDir  = os.path.join(outputBase, outSubdir)
    print(outputDir)
    # decide outputname: strip inputdirname of useless stuff en same for individual files (singlelep, date, 0000/0001 stuff)
    # Handle different files in same dir.. Should be fixed at skimming level... Not because same identifier that they're allowed to be in same subdir. For this, add some uniqueness to naming?
    if (fraction == 1):
        outputFileName = dir + ".root"
    else: 
        outputFileName = dir + "_" + str(fraction)[2:] + "pp.root"

    # join outputname with outputfolder
    outputPath = os.path.join(outputDir, outputFileName)
    
    print(outputPath)
    print(os.path.join(inputBase, dir))

    # call hadd output inputs (star?)
    if (fraction == 1):
        commands.append("hadd -f {} {}/*.root".format(outputPath, os.path.join(inputBase, dir)))
    else:
        accepted = []
        max_files = len(os.listdir(os.path.join(inputBase, dir)))
        cut = int(fraction * max_files)
        print(max_files)
        print(cut)
        for i, file in enumerate(os.listdir(os.path.join(inputBase, dir))):
            if (i >= cut): break
            #r = random.random()
            #if (r < fraction):
            accepted.append(os.path.join(inputBase, dir, file))

        print(len(accepted))
        #print(len(accepted))
        # print(accepted)
        commands.append("hadd -f {} {}".format(outputPath, " ".join(accepted)))

# print(commands)
# print(commands[0])
submitCommandsAsCondorCluster("hadd", commands)