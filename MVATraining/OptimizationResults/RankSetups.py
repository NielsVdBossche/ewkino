import ROOT
import numpy as np
import matplotlib.pyplot as plt
import itertools
import csv

ntrees = [1000, 1500, 2000]
depths = [3, 4, 5, 6]
nCuts = [20]
shrinkages = [0.01, 0.05, 0.10]
minNodeSizes = [1, 5, 10]
baggedFractions = [0.25, 0.5, 1.]

classes = ["Signal", "Background", "TTV"]
resultType = ["Train", "Test"]

basename = "/user/nivanden/ewkino/MVATraining/Classifiers/FourTopClassification_LeanSel_Search_nTrees_"

def extractParameters(file, method):
    # load a file, extract the relevant output
    f = ROOT.TFile(file, "READ")
    f.cd("VeryEpicLoader")
    ROOT.gDirectory.cd("Method_"+method+"/"+method)
    # list elements in current directory, print and extract useful stuff -> all tgraphs basically
    keyList = ROOT.gDirectory.GetListOfKeys()

    keys = []
    for key in keyList:
        if key.GetClassName() == "TGraph":
            keyName = str(key.GetName())
            keyName = keyName.replace("MVA_" + method, "").replace("_Train_", "").replace("_Test_", "")
            keys.append(keyName)

    f.Close()
    return set(keys)

def extractResultFromFile(file, method, key):
    # load a file, extract the relevant output
    f = ROOT.TFile(file, "READ")
    f.cd("VeryEpicLoader")
    ROOT.gDirectory.cd("Method_"+method)
    ROOT.gDirectory.cd(method)

    mvaName = "MVA_" + method
    output = []

    for el in resultType:
        graph = ROOT.gDirectory.Get(mvaName + "_" + el + "_" + key)
        output.append(graph.Integral())

    return output

def extractResultFromFiles(key):
    result = dict()
    for mix in itertools.product(ntrees, depths, nCuts, shrinkages, minNodeSizes, baggedFractions):
        filename = basename + str(mix[0]) + "_Depth_" + str(mix[1]) + "_nCuts_" + str(mix[2]) + "_shrink_" + str(mix[3]) + "_minNodeSize" + str(mix[4]) + "_baggedFraction_" + str(mix[5]) + ".root"
        if mix[5] == 1.:
            methodname = "DL_BDTG_B_" + str(mix[0]) + "_" + str(mix[1]) + "_" + str(mix[2]) + "_" + str(mix[3]) + "_" + str(mix[4]) + "_1" 
        else:
            methodname = "DL_BDTG_B_" + str(mix[0]) + "_" + str(mix[1]) + "_" + str(mix[2]) + "_" + str(mix[3]) + "_" + str(mix[4]) + "_" + str(mix[5])

        result[methodname] = extractResultFromFile(filename, methodname, key)

    return result

def plotResultsAlongVariable(results, varNb, testkeys):
    # create scatter plot for variable; order is based on methodName -> ntrees, depths, cuts, shrink, minNode, baggedFrac


    return

def plot2DResultsAlongVariable(results, varNbX, varNbY, testkey):
    # get best result and plot on 2D canvas


    return

def sortByBestPerformer(results, testkey):
    resultsNew = []
    for res in results.items():
        tmp = [res[0], res[1][0], res[1][1]]
        resultsNew.append(tmp)

    with open(testkey+".csv", "wb") as f:
        writer = csv.writer(f)
        writer.writerows(resultsNew)
    
    return




if __name__ == "__main__":
    # we can parse output files or read classifiers root files
    # glob outputfiles, read last 100 lines or so. 

    filename = "/user/nivanden/ewkino/MVATraining/Classifiers/FourTopClassification_LeanSel_Search_nTrees_1000_Depth_3_nCuts_20_shrink_0.1_minNodeSize5_baggedFraction_0.5.root"
    method = "DL_BDTG_B_1000_3_20_0.1_5_0.5"
    
    listOfKeys = extractParameters(filename, method)
    #print(listOfKeys)
    #for key in listOfKeys:
    #    extractResultFromFile(filename, method, key)
    for key in listOfKeys:
        results = extractResultFromFiles(key)
        sortByBestPerformer(results, key)



