import os
from random import sample
import sys

sys.path.append(os.path.abspath('../../'))

#from jobSubmission.condorTools import submitCommandsAsCondorJob

sampleListPath = "../../sampleLists/"
sampleLists = ["2016PreVFP", "2016PostVFP", "2017", "2018"]

outputPath = sampleListPath + "TmpLists/"
outputSampleLists = []
for sl in sampleLists:
    filePath = sampleListPath + sl + ".txt"

    sampleDict = dict()

    with open(filePath) as f:
        for line in f:
            splitLine = line.split()
            if line == "\n": continue
            if line[0] == '#' or splitLine[0] == "#": continue
            
            if not splitLine[0] in sampleDict.keys():
                sampleDict[splitLine[0]] = [line]
            else:
                curr = sampleDict[splitLine[0]]
                curr.append(line)
                sampleDict[splitLine[0]] = curr
    
    for key in sampleDict.keys():
        currOutPath = outputPath + sl + "_" + key + ".txt"
        outputSampleLists.append("TmpLists/" + sl + "_" + key + ".txt")
        with open(currOutPath, 'w') as of:
            samples = sampleDict[key]
            for samp in samples:
                of.write(samp)     

outputstring = ""

for samp in outputSampleLists:
    outputstring += samp + ", "

print(outputstring[:-2])
