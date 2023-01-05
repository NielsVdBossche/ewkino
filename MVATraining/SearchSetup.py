import sys
#sys.path.append("../jobSubmission/")
sys.path.insert(1, '/user/nivanden/ewkino/jobSubmission')

import condorTools as ct
import numpy as np
import itertools

commandList = []

#setups = [["DL_BDT", "DL_NN"], ["ML_BDT", "ML_NN"]]
setups = [["BDT_VAR_DL"], ["BDT_VAR_ML"]]

trees = ["DL_tree_loose", "ML_tree_loose"]

ntrees = [1000, 1250, 1500, 2000]
depths = [3, 4, 5, 6]
nCuts = [20]
shrinkages = [0.01, 0.05, 0.10]
minNodeSizes = [1, 5, 10, 20]
baggedFractions = [0.25, 0.5, 1.]


for setup, tree in zip(setups, trees):
    for subsetup in setup:
        for mix in itertools.product(ntrees, depths, nCuts, shrinkages, minNodeSizes, baggedFractions):
            interstring = ""
            for el in mix:
                interstring += " " + str(el)

            if subsetup == "BDT_VAR_DL":
                variables = "Settingfiles/VariableSelectionDL.txt"
            else:
                variables = "Settingfiles/VariableSelectionML.txt"


            #print("./trainMVAMethodsExec sampleLists/DLTest.txt " + tree + " " + subsetup + " search" + interstring)
            commandList.append("./trainMVAMethodsExec sampleLists/SL_UL.txt " + tree + " " + subsetup + " search" + interstring + " " + variables)
            
#print(commandList)
ct.submitCommandsAsCondorCluster("trainAllMVA", commandList)
