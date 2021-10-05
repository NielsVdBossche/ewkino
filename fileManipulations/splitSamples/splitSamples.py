#!/usr/bin/env python

# Script to split samples randomly as training and analysis samples

import glob
import os
import random
import shutil
import subprocess

base_directory = "/pnfs/iihe/cms/store/user/nivanden/skims/rawSkims/"
analysisSubDir = "analysisSamples"
mvaSubDir = "mvaSamples"

finalOutputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/"

def mergeTuples(mergableDir):
    version = mergableDir.split("_version_")[-1]

    outSubdir = ""
    if (version == "2016_ULpreVFPv3"):
        outSubdir = "2016PreVFP"
    elif (version == "2016_ULpostVFPv3"):
        outSubdir = "2016PostVFP"
    elif (version == "2017_ULv3"):
        outSubdir = "2017"
    elif (version == "2018_ULv3"):
        outSubdir = "2018"

    # join path outputbase + folder
    outSubSubdir = ""
    if (analysisSubDir in mergableDir):
        outSubSubdir = analysisSubDir
    elif (mvaSubDir in mergableDir):
        outSubSubdir = mvaSubDir

    outputDir  = os.path.join(finalOutputBase, outSubdir, outSubSubdir)
    print(outputDir)

    try:
        os.mkdir(outputDir) 
    except OSError as error: 
        print(outputDir + " already exists")

    inputFileName = ""
    i = -1
    while not ".root" in inputFileName:
        i += 1
        inputFileName = os.listdir(mergableDir)[i]

    outputFileName = inputFileName.split("singlelep_")[0][:-17] + ".root" # should cut away date

    # join outputname with outputfolder
    outputPath = os.path.join(outputDir, outputFileName)

    # call hadd output inputs (star?)
    subprocess.call("hadd -f {} {}/*.root".format(outputPath, mergableDir), shell=True)

    return


processes = ["TTZToLLNuNu", "TTWJetsToLNu", "ttHJetToNonbb", "TTToSemiLeptonic", "TTTo2L2Nu", "TTTT", "TTGamma_Dilept", "TTGamma_SingleLept"]

folders = [element for process in processes for element in glob.glob(base_directory + "*" + process + "*")]

for folder in folders:
    # make output dirs (direct copy of dir)
    
    outputAnalysis = os.path.join(base_directory, analysisSubDir, folder.split('/')[-1])
    try:
        os.mkdir(outputAnalysis) 
    except OSError as error: 
        print("Analysis sub folder already exists for " + folder)

    outputMVA = os.path.join(base_directory, mvaSubDir, folder.split('/')[-1])
    try:
        os.mkdir(outputMVA) 
    except OSError as error: 
        print("MVA sub folder already exists for " + folder)

    # list files in folder, copy (by random choice to right subdir)
    for file in os.listdir(folder):
        r = random.random()

        if (r < 0.9):
            shutil.copy2(os.path.join(folder, file), outputAnalysis)
        else:
            shutil.copy2(os.path.join(folder, file), outputMVA)

    # once files are moved, merge samples from these new folders to subfolders in respective years.

    mergeTuples(outputAnalysis)
    mergeTuples(outputMVA)
