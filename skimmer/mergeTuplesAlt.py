import os
import sys
import subprocess

inputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/rawSkims/"
outputBase = "/pnfs/iihe/cms/store/user/nivanden/skims/"

for dir in os.listdir(inputBase):
    print(dir)
    # catch version, decide outputfolder
    version = dir.split("_version_")[-1]

    outSubdir = ""
    if (version == "2016_ULpreVFPv3"):
        outSubdir = "2016PreVFP"
    elif (version == "2016_ULpostVFPv3"):
        outSubdir = "2016PostVFP"
    elif (version == "2017_ULv3"):
        outSubdir = "2017"
    elif (version == "2018_ULv3"):
        outSubdir = "2018"

    if (outSubdir == ""):
        print('ERROR: Version not found\n')
        exit()

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
    outputFileName = inputFileName.split("singlelep_")[0][:-17] + ".root" # should cut away date

    # join outputname with outputfolder
    outputPath = os.path.join(outputDir, outputFileName)
    
    print(outputPath)
    print(os.path.join(inputBase, dir))

    # call hadd output inputs (star?)
    subprocess.call("hadd -f {} {}/*.root".format(outputPath, os.path.join(inputBase, dir)), shell=True)

    break # temporary for testing