#!/usr/bin/env python

import os
import sys
import subprocess

inputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/rawSkims/"
outputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/"

skimVersion = []
outSubdir = ""

if (sys.argv[1] == '0'):
    skimVersion = ["2016_ULpreVFP", "RunIISummer20UL16MiniAODAPV", "MiniAOD2016preVFP", "2016_ULpreVFP_Nov"]
    outSubdir = "2016PreVFP"
elif (sys.argv[1] == '1'):
    skimVersion = ["2016_ULpostVFP", "RunIISummer20UL16MiniAOD", "MiniAOD2016postVFP", "2016_ULpostVFP_Nov"]
    outSubdir = "2016PostVFP"
elif (sys.argv[1] == '2'):
    skimVersion = ["2017_UL", "RunIISummer20UL17MiniAOD", "MiniAOD2017", "2018_Nov"]
    outSubdir = "2017"
elif (sys.argv[1] == '3'):
    skimVersion = ["2018_UL", "RunIISummer20UL18MiniAOD", "MiniAOD2018", "2017_Nov"]
    outSubdir = "2018"

print(skimVersion)
print(sys.argv)

if (len(sys.argv) >= 3):
    if (sys.argv[2] == "TTTrainingSamples"):
        outSubdir += "/mvaSamples"

processes = ["TTTJ", "TTTW", "ST_tW", "GluGluToContin", "THQ", "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX", "DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM", "ST_t-channel", "ST_s-channel", "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8", "WZ", "ZZ", "TTWJetsToQQ"]
for dir in os.listdir(inputBase):
    print(dir)
    # catch version, decide outputfolder
    version = dir.split("_version_")[-1]

    if not any(skimver in dir for skimver in skimVersion):
        continue

    if not any(process in dir for process in processes):
        continue
    
    # join path outputbase + folder
    outputDir  = os.path.join(outputBase, outSubdir)
    print(outputDir)
    # decide outputname: strip inputdirname of useless stuff en same for individual files (singlelep, date, 0000/0001 stuff)
    # Handle different files in same dir.. Should be fixed at skimming level... Not because same identifier that they're allowed to be in same subdir. For this, add some uniqueness to naming?
    inputFileName = ""
    i = -1
    while not ".root" in inputFileName:
        i += 1
        inputFileName = os.listdir(os.path.join(inputBase, dir))[i]

    # Strip names
    #dir = dir[15:] # NOT NECESSARY -> all information in filename itself 
    if ("pnfsiihecmsstoreusergmestdacheavyNeutrinoUL" in inputFileName) :
        inputFileName = inputFileName[len("pnfsiihecmsstoreusergmestdacheavyNeutrinoUL"):]

    #if ("singlelepton" in inputFileName):
    #    outputFileName = inputFileName.split("singlelepton_")[0][:-17] + ".root"
    if ("singlelep" in inputFileName):
        outputFileName = inputFileName.split("singlelep_")[0][:-17] + ".root" # should cut away date
    elif ("ssdilep" in inputFileName):
        outputFileName = inputFileName.split("ssdilep_")[0][:-17] + ".root" # should cut away date

    # join outputname with outputfolder
    outputPath = os.path.join(outputDir, outputFileName)
    
    print(outputPath)
    print(os.path.join(inputBase, dir))

    # call hadd output inputs (star?)
    subprocess.call("hadd -f {} {}/*.root".format(outputPath, os.path.join(inputBase, dir)), shell=True)
    #break # temporary for testing