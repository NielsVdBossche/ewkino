#!/usr/bin/env python3
import sys
import os
from datetime import datetime
import ClearCondorFiles

def PrepareJobDescription(additionalArguments, exe="MainAnalysis"):
    argstring = "$(Samplelist) $(Method)"

    for arg in additionalArguments:
        argstring += " " + arg
    
    with open("AnalysisJob.sub", 'w') as f:
        f.write('executable = '+ exe +'.sh\n')
        f.write('arguments = {}\n\n'.format(argstring))
        f.write('output = /user/nivanden/condor/output/analysis_exec_$(method)_$(ClusterId)_$(ProcId).out\n')
        f.write('error = /user/nivanden/condor/error/analysis_exec_$(method)_$(ClusterId)_$(ProcId).err\n')
        f.write('log = /user/nivanden/condor/logs/analysis_exec_$(method)_$(ClusterId)_$(ProcId).log\n\n')

    return


runTypesCode = ["MCAll", "MCPrompt", "ChargeDD", "nonPromptDD", "nonPromptDDControl", "Obs", "MCNoChargeMisID", "MCNoNP"]
runTypes = ["mcall", "dd", "npdd", "chdd", "npcontrol", "mcnp", "mcprompt", "data_only"]

matches = {"mcall" : ["MCAll"], "npdd_ext" : ["nonPromptDD", "MCAll"], "chdd" : ["ChargeDD"], "dd" : ["MCPrompt", "ChargeDD", "nonPromptDD"], "npcontrol" : {"nonPromptDD", "nonPromptDDControl"}, "mcnp" : ["MCAllBJetTest"], "npdd" : ["nonPromptDD"], "mcprompt" : ["MCPrompt"], "data_only" : []}

ddtypes = ["all", "np", "ch"]

mcSL = ["TmpLists/2016PreVFP_SSWW_0.txt","TmpLists/2016PreVFP_ttbar_0.txt","TmpLists/2016PreVFP_ttbar_1.txt","TmpLists/2016PreVFP_ttG_0.txt","TmpLists/2016PreVFP_ttG_1.txt","TmpLists/2016PreVFP_TTH_0.txt","TmpLists/2016PreVFP_TTTT_0.txt","TmpLists/2016PreVFP_TTVV_0.txt","TmpLists/2016PreVFP_TTVV_1.txt","TmpLists/2016PreVFP_TTVV_2.txt","TmpLists/2016PreVFP_TTVV_3.txt","TmpLists/2016PreVFP_TTVV_4.txt","TmpLists/2016PreVFP_TTVV_5.txt","TmpLists/2016PreVFP_TTW_0.txt","TmpLists/2016PreVFP_TTZ_0.txt","TmpLists/2016PreVFP_THQ_0.txt","TmpLists/2016PreVFP_TTX_0.txt","TmpLists/2016PreVFP_TTX_1.txt","TmpLists/2016PreVFP_TTX_2.txt","TmpLists/2016PreVFP_VVV_0.txt","TmpLists/2016PreVFP_VVV_1.txt","TmpLists/2016PreVFP_VVV_2.txt","TmpLists/2016PreVFP_WZ_0.txt","TmpLists/2016PreVFP_Z_0.txt","TmpLists/2016PreVFP_Z_1.txt","TmpLists/2016PreVFP_ZG_0.txt","TmpLists/2016PreVFP_ZZ-H_0.txt","TmpLists/2016PreVFP_ZZ-H_1.txt","TmpLists/2016PreVFP_ZZ-H_2.txt","TmpLists/2016PreVFP_ZZ-H_3.txt","TmpLists/2016PreVFP_ZZ-H_4.txt","TmpLists/2016PreVFP_ZZ-H_5.txt","TmpLists/2016PreVFP_ZZ-H_6.txt","TmpLists/2016PostVFP_SSWW_0.txt","TmpLists/2016PostVFP_ttbar_0.txt","TmpLists/2016PostVFP_ttbar_1.txt","TmpLists/2016PostVFP_ttG_0.txt","TmpLists/2016PostVFP_ttG_1.txt","TmpLists/2016PostVFP_TTH_0.txt","TmpLists/2016PostVFP_TTTT_0.txt","TmpLists/2016PostVFP_TTVV_0.txt","TmpLists/2016PostVFP_TTVV_1.txt","TmpLists/2016PostVFP_TTVV_2.txt","TmpLists/2016PostVFP_TTVV_3.txt","TmpLists/2016PostVFP_TTVV_4.txt","TmpLists/2016PostVFP_TTVV_5.txt","TmpLists/2016PostVFP_TTW_0.txt","TmpLists/2016PostVFP_TTZ_0.txt","TmpLists/2016PostVFP_THQ_0.txt","TmpLists/2016PostVFP_TTX_0.txt","TmpLists/2016PostVFP_TTX_1.txt","TmpLists/2016PostVFP_TTX_2.txt","TmpLists/2016PostVFP_VVV_0.txt","TmpLists/2016PostVFP_VVV_1.txt","TmpLists/2016PostVFP_VVV_2.txt","TmpLists/2016PostVFP_WZ_0.txt","TmpLists/2016PostVFP_Z_0.txt","TmpLists/2016PostVFP_Z_1.txt","TmpLists/2016PostVFP_ZG_0.txt","TmpLists/2016PostVFP_ZZ-H_0.txt","TmpLists/2016PostVFP_ZZ-H_1.txt","TmpLists/2016PostVFP_ZZ-H_2.txt","TmpLists/2016PostVFP_ZZ-H_3.txt","TmpLists/2016PostVFP_ZZ-H_4.txt","TmpLists/2016PostVFP_ZZ-H_5.txt","TmpLists/2016PostVFP_ZZ-H_6.txt","TmpLists/2016PostVFP_ZZ-H_7.txt","TmpLists/2017_SSWW_0.txt","TmpLists/2017_ttbar_0.txt","TmpLists/2017_ttbar_1.txt","TmpLists/2017_ttG_0.txt","TmpLists/2017_ttG_1.txt","TmpLists/2017_TTH_0.txt","TmpLists/2017_TTTT_0.txt","TmpLists/2017_TTVV_0.txt","TmpLists/2017_TTVV_1.txt","TmpLists/2017_TTVV_2.txt","TmpLists/2017_TTVV_3.txt","TmpLists/2017_TTVV_4.txt","TmpLists/2017_TTVV_5.txt","TmpLists/2017_TTW_0.txt","TmpLists/2017_TTZ_0.txt","TmpLists/2017_THQ_0.txt","TmpLists/2017_TTX_0.txt","TmpLists/2017_TTX_1.txt","TmpLists/2017_TTX_2.txt","TmpLists/2017_VVV_0.txt","TmpLists/2017_VVV_1.txt","TmpLists/2017_VVV_2.txt","TmpLists/2017_WZ_0.txt","TmpLists/2017_Z_0.txt","TmpLists/2017_Z_1.txt","TmpLists/2017_ZG_0.txt","TmpLists/2017_ZZ-H_0.txt","TmpLists/2017_ZZ-H_1.txt","TmpLists/2017_ZZ-H_2.txt","TmpLists/2017_ZZ-H_3.txt","TmpLists/2017_ZZ-H_4.txt","TmpLists/2017_ZZ-H_5.txt","TmpLists/2017_ZZ-H_6.txt","TmpLists/2017_ZZ-H_7.txt","TmpLists/2017_ZZ-H_8.txt","TmpLists/2018_SSWW_0.txt","TmpLists/2018_ttbar_0.txt","TmpLists/2018_ttbar_1.txt","TmpLists/2018_ttG_0.txt","TmpLists/2018_ttG_1.txt","TmpLists/2018_TTH_0.txt","TmpLists/2018_TTTT_0.txt","TmpLists/2018_TTVV_0.txt","TmpLists/2018_TTVV_1.txt","TmpLists/2018_TTVV_2.txt","TmpLists/2018_TTVV_3.txt","TmpLists/2018_TTVV_4.txt","TmpLists/2018_TTVV_5.txt","TmpLists/2018_TTW_0.txt","TmpLists/2018_TTZ_0.txt","TmpLists/2018_THQ_0.txt","TmpLists/2018_TTX_0.txt","TmpLists/2018_TTX_1.txt","TmpLists/2018_TTX_2.txt","TmpLists/2018_VVV_0.txt","TmpLists/2018_VVV_1.txt","TmpLists/2018_VVV_2.txt","TmpLists/2018_WZ_0.txt","TmpLists/2018_Z_0.txt","TmpLists/2018_Z_1.txt","TmpLists/2018_ZG_0.txt","TmpLists/2018_ZZ-H_0.txt","TmpLists/2018_ZZ-H_1.txt","TmpLists/2018_ZZ-H_2.txt","TmpLists/2018_ZZ-H_3.txt","TmpLists/2018_ZZ-H_4.txt","TmpLists/2018_ZZ-H_5.txt","TmpLists/2018_ZZ-H_6.txt","TmpLists/2018_ZZ-H_7.txt","TmpLists/2018_ZZ-H_8.txt"]
mcSL_fullStat = ["TmpLists/2016PreVFP_fullStat_SSWW_0.txt","TmpLists/2016PreVFP_fullStat_ttbar_0.txt","TmpLists/2016PreVFP_fullStat_ttbar_1.txt","TmpLists/2016PreVFP_fullStat_ttG_0.txt","TmpLists/2016PreVFP_fullStat_ttG_1.txt","TmpLists/2016PreVFP_fullStat_TTH_0.txt","TmpLists/2016PreVFP_fullStat_TTTT_0.txt","TmpLists/2016PreVFP_fullStat_TTVV_0.txt","TmpLists/2016PreVFP_fullStat_TTVV_1.txt","TmpLists/2016PreVFP_fullStat_TTVV_2.txt","TmpLists/2016PreVFP_fullStat_TTVV_3.txt","TmpLists/2016PreVFP_fullStat_TTVV_4.txt","TmpLists/2016PreVFP_fullStat_TTVV_5.txt","TmpLists/2016PreVFP_fullStat_TTW_0.txt","TmpLists/2016PreVFP_fullStat_TTW_1.txt","TmpLists/2016PreVFP_fullStat_TTZ_0.txt","TmpLists/2016PreVFP_fullStat_THQ_0.txt","TmpLists/2016PreVFP_fullStat_TTX_0.txt","TmpLists/2016PreVFP_fullStat_TTX_1.txt","TmpLists/2016PreVFP_fullStat_TTX_2.txt","TmpLists/2016PreVFP_fullStat_VVV_0.txt","TmpLists/2016PreVFP_fullStat_VVV_1.txt","TmpLists/2016PreVFP_fullStat_WZ_0.txt","TmpLists/2016PreVFP_fullStat_Z_0.txt","TmpLists/2016PreVFP_fullStat_Z_1.txt","TmpLists/2016PreVFP_fullStat_ZG_0.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_0.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_1.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_2.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_3.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_4.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_5.txt","TmpLists/2016PreVFP_fullStat_ZZ-H_6.txt","TmpLists/2016PostVFP_fullStat_SSWW_0.txt","TmpLists/2016PostVFP_fullStat_ttbar_0.txt","TmpLists/2016PostVFP_fullStat_ttbar_1.txt","TmpLists/2016PostVFP_fullStat_ttG_0.txt","TmpLists/2016PostVFP_fullStat_ttG_1.txt","TmpLists/2016PostVFP_fullStat_TTH_0.txt","TmpLists/2016PostVFP_fullStat_TTTT_0.txt","TmpLists/2016PostVFP_fullStat_TTVV_0.txt","TmpLists/2016PostVFP_fullStat_TTVV_1.txt","TmpLists/2016PostVFP_fullStat_TTVV_2.txt","TmpLists/2016PostVFP_fullStat_TTVV_3.txt","TmpLists/2016PostVFP_fullStat_TTVV_4.txt","TmpLists/2016PostVFP_fullStat_TTW_0.txt","TmpLists/2016PostVFP_fullStat_TTW_1.txt","TmpLists/2016PostVFP_fullStat_TTZ_0.txt","TmpLists/2016PostVFP_fullStat_THQ_0.txt","TmpLists/2016PostVFP_fullStat_TTX_0.txt","TmpLists/2016PostVFP_fullStat_TTX_1.txt","TmpLists/2016PostVFP_fullStat_TTX_2.txt","TmpLists/2016PostVFP_fullStat_VVV_0.txt","TmpLists/2016PostVFP_fullStat_VVV_1.txt","TmpLists/2016PostVFP_fullStat_VVV_2.txt","TmpLists/2016PostVFP_fullStat_WZ_0.txt","TmpLists/2016PostVFP_fullStat_Z_0.txt","TmpLists/2016PostVFP_fullStat_Z_1.txt","TmpLists/2016PostVFP_fullStat_ZG_0.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_0.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_1.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_2.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_3.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_4.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_5.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_6.txt","TmpLists/2016PostVFP_fullStat_ZZ-H_7.txt","TmpLists/2017_fullStat_SSWW_0.txt","TmpLists/2017_fullStat_ttbar_0.txt","TmpLists/2017_fullStat_ttbar_1.txt","TmpLists/2017_fullStat_ttG_0.txt","TmpLists/2017_fullStat_ttG_1.txt","TmpLists/2017_fullStat_TTH_0.txt","TmpLists/2017_fullStat_TTTT_0.txt","TmpLists/2017_fullStat_TTVV_0.txt","TmpLists/2017_fullStat_TTVV_1.txt","TmpLists/2017_fullStat_TTVV_2.txt","TmpLists/2017_fullStat_TTVV_3.txt","TmpLists/2017_fullStat_TTVV_4.txt","TmpLists/2017_fullStat_TTVV_5.txt","TmpLists/2017_fullStat_TTW_0.txt","TmpLists/2017_fullStat_TTW_1.txt","TmpLists/2017_fullStat_TTZ_0.txt","TmpLists/2017_fullStat_THQ_0.txt","TmpLists/2017_fullStat_TTX_0.txt","TmpLists/2017_fullStat_TTX_1.txt","TmpLists/2017_fullStat_TTX_2.txt","TmpLists/2017_fullStat_VVV_0.txt","TmpLists/2017_fullStat_VVV_1.txt","TmpLists/2017_fullStat_VVV_2.txt","TmpLists/2017_fullStat_WZ_0.txt","TmpLists/2017_fullStat_Z_0.txt","TmpLists/2017_fullStat_Z_1.txt","TmpLists/2017_fullStat_ZG_0.txt","TmpLists/2017_fullStat_ZZ-H_0.txt","TmpLists/2017_fullStat_ZZ-H_1.txt","TmpLists/2017_fullStat_ZZ-H_2.txt","TmpLists/2017_fullStat_ZZ-H_3.txt","TmpLists/2017_fullStat_ZZ-H_4.txt","TmpLists/2017_fullStat_ZZ-H_5.txt","TmpLists/2017_fullStat_ZZ-H_6.txt","TmpLists/2017_fullStat_ZZ-H_7.txt","TmpLists/2017_fullStat_ZZ-H_8.txt","TmpLists/2018_fullStat_SSWW_0.txt","TmpLists/2018_fullStat_ttbar_0.txt","TmpLists/2018_fullStat_ttbar_1.txt","TmpLists/2018_fullStat_ttG_0.txt","TmpLists/2018_fullStat_ttG_1.txt","TmpLists/2018_fullStat_TTH_0.txt","TmpLists/2018_fullStat_TTTT_0.txt","TmpLists/2018_fullStat_TTVV_0.txt","TmpLists/2018_fullStat_TTVV_1.txt","TmpLists/2018_fullStat_TTVV_2.txt","TmpLists/2018_fullStat_TTVV_3.txt","TmpLists/2018_fullStat_TTVV_4.txt","TmpLists/2018_fullStat_TTW_0.txt","TmpLists/2018_fullStat_TTW_1.txt","TmpLists/2018_fullStat_TTZ_0.txt","TmpLists/2018_fullStat_THQ_0.txt","TmpLists/2018_fullStat_TTX_0.txt","TmpLists/2018_fullStat_TTX_1.txt","TmpLists/2018_fullStat_VVV_0.txt","TmpLists/2018_fullStat_VVV_1.txt","TmpLists/2018_fullStat_VVV_2.txt","TmpLists/2018_fullStat_WZ_0.txt","TmpLists/2018_fullStat_Z_0.txt","TmpLists/2018_fullStat_Z_1.txt","TmpLists/2018_fullStat_ZG_0.txt","TmpLists/2018_fullStat_ZZ-H_0.txt","TmpLists/2018_fullStat_ZZ-H_1.txt","TmpLists/2018_fullStat_ZZ-H_2.txt","TmpLists/2018_fullStat_ZZ-H_3.txt","TmpLists/2018_fullStat_ZZ-H_4.txt","TmpLists/2018_fullStat_ZZ-H_5.txt","TmpLists/2018_fullStat_ZZ-H_6.txt","TmpLists/2018_fullStat_ZZ-H_7.txt","TmpLists/2018_fullStat_ZZ-H_8.txt"]
mcSL_GP = ["TmpLists/GP_Samples_TTH_0.txt","TmpLists/GP_Samples_TTH_1.txt","TmpLists/GP_Samples_TTH_2.txt","TmpLists/GP_Samples_TTH_3.txt","TmpLists/GP_Samples_TTZ_0.txt","TmpLists/GP_Samples_TTZ_1.txt","TmpLists/GP_Samples_TTZ_2.txt","TmpLists/GP_Samples_TTZ_3.txt","TmpLists/GP_Samples_TTW_0.txt","TmpLists/GP_Samples_TTW_1.txt","TmpLists/GP_Samples_TTW_2.txt","TmpLists/GP_Samples_TTW_3.txt","TmpLists/GP_Samples_TTTT_0.txt","TmpLists/GP_Samples_TTTT_1.txt","TmpLists/GP_Samples_TTTT_2.txt","TmpLists/GP_Samples_TTTT_3.txt"]
mc_miss = ["TmpLists/2018_fullStat_TTTT_0.txt", "TmpLists/2017_fullStat_ttG_1.txt", "TmpLists/2016PreVFP_fullStat_ttbar_0.txt", "TmpLists/2016PostVFP_fullStat_ttbar_0.txt", "TmpLists/2017_fullStat_ZG_0.txt", "TmpLists/2016PreVFP_fullStat_TTH_0.txt"]

mcSL_OSDL = ["TmpLists/OSDL_2016PreVFP_DY_0.txt","TmpLists/OSDL_2016PreVFP_DY_1.txt","TmpLists/OSDL_2016PreVFP_ttG_0.txt","TmpLists/OSDL_2016PreVFP_ttG_1.txt","TmpLists/OSDL_2016PreVFP_ttG_2.txt","TmpLists/OSDL_2016PreVFP_ttbar_0.txt","TmpLists/OSDL_2016PreVFP_ttbar_1.txt","TmpLists/OSDL_2016PreVFP_ttbar_incl_0.txt","TmpLists/OSDL_2016PreVFP_W_0.txt","TmpLists/OSDL_2016PreVFP_ZG_0.txt","TmpLists/OSDL_2016PreVFP_WW_0.txt","TmpLists/OSDL_2016PreVFP_ZZ_0.txt","TmpLists/OSDL_2016PreVFP_TTZ_0.txt","TmpLists/OSDL_2016PostVFP_DY_0.txt","TmpLists/OSDL_2016PostVFP_DY_1.txt","TmpLists/OSDL_2016PostVFP_ttG_0.txt","TmpLists/OSDL_2016PostVFP_ttG_1.txt","TmpLists/OSDL_2016PostVFP_ttG_2.txt","TmpLists/OSDL_2016PostVFP_ttbar_0.txt","TmpLists/OSDL_2016PostVFP_ttbar_1.txt","TmpLists/OSDL_2016PostVFP_ttbar_incl_0.txt","TmpLists/OSDL_2016PostVFP_W_0.txt","TmpLists/OSDL_2016PostVFP_WW_0.txt","TmpLists/OSDL_2016PostVFP_ZG_0.txt","TmpLists/OSDL_2016PostVFP_ZZ_0.txt","TmpLists/OSDL_2016PostVFP_ZZ_1.txt","TmpLists/OSDL_2016PostVFP_TTZ_0.txt","TmpLists/OSDL_2017_DY_0.txt","TmpLists/OSDL_2017_DY_1.txt","TmpLists/OSDL_2017_ttG_0.txt","TmpLists/OSDL_2017_ttG_1.txt","TmpLists/OSDL_2017_ttG_2.txt","TmpLists/OSDL_2017_ttbar_0.txt","TmpLists/OSDL_2017_ttbar_1.txt","TmpLists/OSDL_2017_ttbar_incl_0.txt","TmpLists/OSDL_2017_W_0.txt","TmpLists/OSDL_2017_ZG_0.txt","TmpLists/OSDL_2017_WW_0.txt","TmpLists/OSDL_2017_ZZ_0.txt","TmpLists/OSDL_2017_TTZ_0.txt","TmpLists/OSDL_2018_DY_0.txt","TmpLists/OSDL_2018_DY_1.txt","TmpLists/OSDL_2018_ttG_0.txt","TmpLists/OSDL_2018_ttG_1.txt","TmpLists/OSDL_2018_ttG_2.txt","TmpLists/OSDL_2018_ttbar_0.txt","TmpLists/OSDL_2018_ttbar_1.txt","TmpLists/OSDL_2018_ttbar_incl_0.txt","TmpLists/OSDL_2018_W_0.txt","TmpLists/OSDL_2018_ZG_0.txt","TmpLists/OSDL_2018_WW_0.txt","TmpLists/OSDL_2018_ZZ_0.txt","TmpLists/OSDL_2018_TTZ_0.txt"]
dataSL = ["TmpLists/Data16PreVFP_Obs_0.txt","TmpLists/Data16PreVFP_Obs_1.txt","TmpLists/Data16PreVFP_Obs_2.txt","TmpLists/Data16PreVFP_Obs_3.txt","TmpLists/Data16PreVFP_Obs_4.txt","TmpLists/Data16PostVFP_Obs_0.txt","TmpLists/Data16PostVFP_Obs_1.txt","TmpLists/Data16PostVFP_Obs_2.txt","TmpLists/Data17_Obs_0.txt","TmpLists/Data17_Obs_1.txt","TmpLists/Data17_Obs_2.txt","TmpLists/Data17_Obs_3.txt","TmpLists/Data17_Obs_4.txt","TmpLists/Data18_Obs_0.txt","TmpLists/Data18_Obs_1.txt","TmpLists/Data18_Obs_2.txt","TmpLists/Data18_Obs_3.txt"]
#dataSL = ["DATALIAM2017.txt"]


sets = {"MCAll" : [mcSL], "nonPromptDD" : [dataSL, mcSL], "ChargeDD" : [dataSL], "MCPrompt" : [mcSL], "nonPromptDDControl" : [mcSL], "MCAllBJetTest" : [mcSL]}
sets_OSDL = {"MCAll" : [mcSL_OSDL]}

wd = "/user/nivanden/ewkino/_FourTopAnalysis"

if __name__ == "__main__":
    baseCommand = "./analysisExec $1 $2"
    
    dateTimeObj = datetime.now()
    time = dateTimeObj.strftime("%Y-%m-%d_%H-%M")
    timestamp = "timestamp=" + time
 
    additionalArgs = [timestamp]
    customSamplelist = False
    runData = False
    useTree = False
    customSamplelistName = ""
    specificSample = ""
    specificEra = ""

    useOSDL = False
    
    runType = sys.argv[1].lower()
    
    if not any([runType == rt for rt in runTypes]):
        print("Unrecognized runtype. Usage: ./RunAnalysis.py <AnalysisType> [-CR] [-LEAN] [path/to/samplelist] [era=[16Pre, 16Post, 17, 18]]")

    for option in sys.argv[2:]:
        optionLower = option.lower()
        if optionLower == "-cr":
            additionalArgs.append("CR")
        elif optionLower == "-lean":
            additionalArgs.append("leanEventSelection")
        elif ".txt" in option:
            customSamplelist = True
            customSamplelistName = option
            print("Samplelist {} will be used.".format(option))
        elif "era=" in optionLower:
            specificEra = option.split('=')[1]
        elif optionLower == "-data":
            runData = True
        elif "region=" in optionLower:
            additionalArgs.append(option)
        elif "samples=" in optionLower:
            specificSample = option.split('=')[1]
        elif "osdl" in optionLower:
            useOSDL = True
        elif "fullstat" in optionLower:
            sets = {"MCAll" : [mcSL_fullStat], "nonPromptDD" : [dataSL, mcSL_fullStat], "ChargeDD" : [dataSL], "MCPrompt" : [mcSL_fullStat], "nonPromptDDControl" : [mcSL_fullStat], "MCAllBJetTest" : [mcSL_fullStat]}
        elif "plotstring=" in optionLower:
            additionalArgs.append(option)
        elif "gensamples" in optionLower:
            sets = {"MCAll" : [mcSL_GP], "MCPrompt" : [mcSL_GP]}
        elif "miss" in optionLower:
            sets = {"MCAll" : [mc_miss], "nonPromptDD" : [mc_miss], "ChargeDD" : [dataSL], "MCPrompt" : [mc_miss], "nonPromptDDControl" : [mc_miss], "MCAllBJetTest" : [mc_miss]}
        elif "inclusiveclasses" in optionLower:
            additionalArgs.append(option)
        elif "tree" in optionLower:
            useTree = True
        else:
            print("Unrecognized option: {}. Usage: ./RunAnalysis.py <AnalysisType> [-CR] [-LEAN] [path/to/samplelist] [era=[16Pre, 16Post, 17, 18]]".format(option))

    
    if useTree:
        PrepareJobDescription(additionalArgs, "TreeAnalysis")
    else:
        PrepareJobDescription(additionalArgs)
    
    queueString = "queue 1 Samplelist, Method from (\n"
    requiredMethods = matches[runType]
    i = 0
    if not customSamplelist:
        requiredSamplelists = sets
        
        if useOSDL:
            requiredSamplelists = sets_OSDL

        # if not dd x -> do stuff different to npdd, chdd

        for method in requiredMethods:
            for x in requiredSamplelists[method]:
                for sl in x:
                    if not specificEra in sl: continue
                    if not specificSample in sl: continue
                    i += 1
                    queueString += "\t" 
                    queueString += sl + ", " + method
                    queueString += "\n"
        print("submitting {} samples".format(i))

    else:
        for method in requiredMethods:
            queueString += "\t" 
            queueString += customSamplelistName + ", " + method
            queueString += "\n"


    if runData:
        for sl in dataSL:
            if not specificEra in sl: continue

            queueString += "\t" 
            queueString += sl + ", " + "Obs"
            queueString += "\n"


    queueString += ")\n\n"

    with open("AnalysisJob.sub", 'a') as f:
        f.write(queueString)

    # outputfolder preparation:
    foldername = time
    if "CR" in additionalArgs:
        foldername += "_CR"

    folder = os.path.join("Output", foldername)
    os.makedirs(folder)

    # print queuestring at end of job description
    os.system('condor_submit AnalysisJob.sub')

# future plans: also submit job to condor waiting on previous job to finish