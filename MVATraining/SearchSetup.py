import sys
#sys.path.append("../jobSubmission/")
sys.path.insert(1, '/user/nivanden/ewkino/jobSubmission')

import condorTools as ct
import numpy as np
import itertools

commandList = []

#setups = [["DL_BDT", "DL_NN"], ["ML_BDT", "ML_NN"]]
setups = [["DL_BDT"], ["ML_BDT"]]

trees = ["DL_tree_loose", "ML_tree_loose"]

ntrees = np.arange(500, 1500, 100)
depths = [3, 4, 5, 6, 7, 8]
nCuts = [10, 20, 30]
shrinkages = np.arange(0.01, 0.1, 0.01)
minNodeSizes = np.arange(1, 10, 1)
baggedFractions = np.arange(0.25, 1, 0.25)


for setup, tree in zip(setups, trees):
    for subsetup in setup:
        for mix in itertools.product(ntrees, depths, nCuts, shrinkages, minNodeSizes, baggedFractions):
            interstring = ""
            for el in mix:
                interstring += " " + str(el)
            command = "./trainMVAMethodsExec sampleLists/DLTest.txt " + tree + " " + subsetup + " search" + interstring
            ct.submitCommandAsCondorJob("trainAllMVA", command)
