import os
from random import sample
import sys

#sys.path.append(os.path.abspath('../../'))

#from jobSubmission.condorTools import submitCommandsAsCondorJob

sampleListPath = "../../sampleLists/"
sampleLists = ["trainingSamples_UL"]
# sampleLists = ["Data16PreVFP", "Data16PostVFP", "Data17", "Data18"]

outputPath = sampleListPath + "TmpLists/"
outputSampleLists = []
for sl in sampleLists:
    filePath = sampleListPath + sl + ".txt"

    sampleDict = dict()
    #input
    with open(filePath) as f:
        process = ""

        for line in f:
            splitLine = line.split()
            
            if line == "\n": continue
            if line[0] == '#' or splitLine[0] == "#": continue
            
            if splitLine[0] != process:
                process = splitLine[0]
                count = 0
                processFilename = process + "_" + str(count)

            if not processFilename in sampleDict.keys():
                sampleDict[processFilename] = [line]
            else:
                curr = sampleDict[processFilename]
                if len(curr) >= 1:
                    count += 1
                    processFilename = process + "_" + str(count)
                    sampleDict[processFilename] = [line]
                else:
                    curr.append(line)
                    sampleDict[processFilename] = curr
    
    # output
    for key in sampleDict.keys():
        currOutPath = outputPath + sl + "_" + key + ".txt"
        outputSampleLists.append("TmpLists/" + sl + "_" + key + ".txt")
        with open(currOutPath, 'w') as of:
            samples = sampleDict[key]
            for samp in samples:
                of.write(samp)     

outputstring = ""
outputAlt = ""
outputTri = "["
for samp in outputSampleLists:
    outputstring += samp + ", "
    outputAlt += samp + "\n"
    outputTri += "\"" + samp + "\","


print(outputstring[:-2])
print(outputAlt)
print(outputTri[:-1] + "]")

