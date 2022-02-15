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
    if any(skimver in mergableDir for skimver in ["2016_ULpreVFP", "RunIISummer20UL16MiniAODAPV", "MiniAOD2016preVFP", "2016_ULpreVFP_Nov"]):
        outSubdir = "2016PreVFP"
    elif any(skimver in mergableDir for skimver in ["2016_ULpostVFP", "RunIISummer20UL16MiniAOD-106X", "MiniAOD2016postVFP", "2016_ULpostVFP_Nov"]):
        outSubdir = "2016PostVFP"
    elif any(skimver in mergableDir for skimver in ["2017_UL", "RunIISummer20UL17MiniAOD", "MiniAOD2017", "2017_Nov"]):
        outSubdir = "2017"
    elif any(skimver in mergableDir for skimver in ["2018_UL", "RunIISummer20UL18MiniAOD", "MiniAOD2018", "2018_Nov"]):
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

    if ("singlelep_" in inputFileName):
        outputFileName = inputFileName.split("singlelep_")[0][:-17] + ".root" # should cut away date
    elif ("dilep_" in inputFileName):
        outputFileName = inputFileName.split("dilep_")[0][:-17] + ".root" # should cut away date
    elif ("ssdilep_" in inputFileName):
        outputFileName = inputFileName.split("ssdilep_")[0][:-17] + ".root" # should cut away date

    # join outputname with outputfolder
    outputPath = os.path.join(outputDir, outputFileName)

    # call hadd output inputs (star?)
    subprocess.call("hadd -f {} {}/*.root".format(outputPath, mergableDir), shell=True)

    return


#processes = ["TTZToLLNuNu", "TTWJetsToLNu", "ttHJetToNonbb", "TTToSemiLeptonic_TuneCP5_13TeV", "TTTo2L2Nu_TuneCP5_13TeV", "TTTT", "TTGamma_Dilept", "TTGamma_SingleLept"]
#fractions = [0.2, 0.2, 0.2, 0.2, 0.2, 0.1, 0.2, 0.2]

processes = ["TTGamma_Dilept", "TTGamma_SingleLept"]
fractions = [0.2, 0.2]

folders = [(element, frac) for process, frac in zip(processes, fractions) for element in glob.glob(base_directory + "*" + process + "*")]

for folder, currFrac in folders:
    # make output dirs (direct copy of dir)
    print("Currently working on " + folder.split('/')[-1])
    
    outputAnalysis = os.path.join(base_directory, analysisSubDir, folder.split('/')[-1])
    try:
        os.mkdir(outputAnalysis)
    except OSError as error: 
        print("Analysis sub folder already exists for " + folder + "... Cleaning folder")
        files = glob.glob(outputAnalysis)
        for f in files:
            os.remove(f)

    outputMVA = os.path.join(base_directory, mvaSubDir, folder.split('/')[-1])
    try:
        os.mkdir(outputMVA) 
    except OSError as error: 
        print("MVA sub folder already exists for " + folder + "... Cleaning folder")
        files = glob.glob(outputMVA)
        for f in files:
            os.remove(f)

    # list files in folder, copy (by random choice to right subdir)
    for file in os.listdir(folder):
        r = random.random()

        if (r < 1 - currFrac):
            shutil.copy2(os.path.join(folder, file), outputAnalysis)
        else:
            shutil.copy2(os.path.join(folder, file), outputMVA)

    # once files are moved, merge samples from these new folders to subfolders in respective years.

    mergeTuples(outputAnalysis)
    mergeTuples(outputMVA)
