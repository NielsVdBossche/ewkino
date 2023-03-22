#!/usr/bin/env python
import subprocess,ROOT,glob,os,sys
ROOT.gROOT.SetBatch(True)
ROOT.gErrorIgnoreLevel = ROOT.kWarning

# System command and retrieval of its output
def system(command):
    return subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)

# Saves the mean and RMS from the histos and branches in a dictionary
def extractDir(name, data):
    if (name != ""):
        ROOT.gDirectory.cd(name)

    for key in ROOT.gDirectory.GetListOfKeys():
        object = ROOT.TKey(key)
        if object.GetClassName() == "TDirectory":
            extractDir(object.GetName(), data)
        else:
            readObj = key.ReadObj()
            data[key.GetName()] = (readObj.GetMean(), readObj.GetRMS())

    return data


def extractFile(name):
    f = ROOT.TFile(name)
    data = dict()
    extractDir("", data)
    return data

# Comparing the two ROOT files using the dictionaries
def compare(logger, refFile, newFile):
    newData = extractFile(newFile)
    refData = extractFile(refFile)

    def compatible(tuple1, tuple2):
        return all([(abs(x-y)/x < 0.0001 if abs(x) > 0 else (abs(x-y)<0.0001)) for x,y in zip(tuple1, tuple2)])

    new     = sorted([i for i in newData if i not in refData])
    removed = sorted([i for i in refData if i not in newData])
    changed = sorted([i for i in refData if i in newData and not compatible(refData[i], newData[i])])
    if len(new):      logger.write('   New: '     + ','.join(new) + '\n')
    if len(removed):  logger.write('   Removed: ' + ','.join(removed) + '\n')
    if len(changed):  logger.write('   Changed:\n')
    for c in changed:
        logger.write('      %-50s mean: %-25s rms: %-25s\n' % (c, '%8.4f --> %8.4f' % (refData[c][0], newData[c][0]), '%8.4f --> %8.4f' % (refData[c][1], newData[c][1])))


with open('tests.log', 'w') as logFile:
    refPath = "/pnfs/iihe/cms/store/user/nivanden/AnalysisOutput/TTTT/2022-10-03_18-08/"
    outputPath = "/user/nivanden/ewkino/_FourTopAnalysis/Output/"
    usefiles = ["2018_TTTT", "2018_TTZ", "2018_TTW", "2017_TTTT", "2017_TTZ", "2017_TTW", "2016PreVFP_TTTT", "2016PreVFP_TTZ", "2016PreVFP_TTW", "2016PostVFP_TTTT", "2016PostVFP_TTZ", "2016PostVFP_TTW"]
    newFilesDate = sys.argv[1]

    refFilesTmp = glob.glob(refPath + "*")
    newFilesTmp = glob.glob(outputPath + "*" + newFilesDate + "*")

    filePairs = []
    

    for fileComp in usefiles:
        refFile = ""
        newFile = ""
        for refEl in refFilesTmp:
            if fileComp in refEl:
                refFile = refEl
                break
        
        for newEl in newFilesTmp:
            if fileComp in newEl:
                refFile = newEl
                break

        logFile.write('\n--------------------------------------------------------------------------------------------------\n\n')
        logFile.write("comparing ", refFile, newFile)
        
        compare(logFile, refFile, newFile)
    
