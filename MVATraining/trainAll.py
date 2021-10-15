import sys
#sys.path.append("../jobSubmission/")
sys.path.insert(1, '/user/nivanden/ewkino/jobSubmission')

import condorTools as ct

commandList = []

#setups = [["DL_BDT", "DL_NN"], ["ML_BDT", "ML_NN"]]
setups = [["DL_BDT"], ["ML_BDT"]]

trees = ["DL_tree", "ML_tree"]

for setup, tree in zip(setups, trees):
    for subsetup in setup:
        commandList.append("./trainMVAMethodsExec sampleLists/DLTest.txt " + tree + " " + subsetup + " search")

ct.submitCommandsAsCondorCluster("trainAllMVAs", commandList)