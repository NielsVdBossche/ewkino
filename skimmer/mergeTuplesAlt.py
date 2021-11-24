#!/usr/bin/env python

import os
import sys
import subprocess

inputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/rawSkims/"
outputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/"

skimVersion = ""
outSubdir = ""

if (sys.argv[1] == '0'):
    skimVersion = "2016_ULpreVFP"
    outSubdir = "2016PreVFP"
elif (sys.argv[1] == '1'):
    skimVersion = "2016_ULpostVFP"
    outSubdir = "2016PostVFP"
elif (sys.argv[1] == '2'):
    skimVersion = "2017_UL"
    outSubdir = "2017"
elif (sys.argv[1] == '3'):
    skimVersion = "2018_UL"
    outSubdir = "2018"

print(skimVersion)
print(sys.argv)

if (len(sys.argv) >= 3):
    if (sys.argv[2] == "TTTrainingSamples"):
        outSubdir += "/mvaSamples"

processes = ["TTWW", "TTWZ", "TTZZ", "TTHH", "TTWH", "TTZH"]

for dir in os.listdir(inputBase):
    print(dir)
    # catch version, decide outputfolder
    version = dir.split("_version_")[-1]

    if (version != skimVersion):
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
