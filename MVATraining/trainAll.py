import sys
#sys.path.append("../jobSubmission/")
sys.path.insert(1, '/user/nivanden/ewkino/jobSubmission')

import condorTools as ct

commandList = []

setups = ["BIN_DL", "DL_BDT", "DL_NN", "BIN_ML", "ML_BDT", "ML_NN"]
trees = ["DL_tree", "ML_tree"]

for tree in trees:
    for setup in setups:
        commandList.append("./trainMVAMethodsExec sampleLists/DLTest.txt " + tree + " " + setup + " search")

ct.submitCommandsAsCondorCluster("trainAllMVAs", commandList)