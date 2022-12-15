import numpy as np
import matplotlib.pyplot as plt

evNbNiels = "EventTags_TTTT_TuneCP5_13TeV-amcatnlo-pythia8_RunIISummer20UL18MiniAODv2_singlelepton_MC_2018_UL.txt"
evNbMayram = "2l-SS-events.txt"

### compare both lists, find what is in 1 but not 2 and vice versa
evsNiels = []
with open(evNbNiels, 'r') as f:
    for line in f:
        evsNiels.append(int(line))

evsMayram = []
with open(evNbMayram, 'r') as f:
    for line in f:
        evsMayram.append(int(line))

print(list(np.setdiff1d(evsMayram,evsNiels))) # els in mayrams version -> els mayram selects but I dont
print(np.setdiff1d(evsNiels,evsMayram)) # els in my version -> els I selects but Mayram dont