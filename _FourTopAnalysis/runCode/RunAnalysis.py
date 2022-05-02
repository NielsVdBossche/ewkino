#!/usr/bin/env python3
import sys
import os
from datetime import datetime

def PrepareJobDescription(additionalArguments):
    argstring = "$(Samplelist) $(Method)"

    for arg in additionalArguments:
        argstring += " " + arg
    
    with open("AnalysisJob.sub", 'w') as f:
        f.write('executable = MainAnalysis.sh\n')
        f.write('arguments = {}\n\n'.format(argstring))
        f.write('output = /user/nivanden/condor/output/analysis_exec_$(method)_$(ClusterId)_$(ProcId).out\n')
        f.write('error = /user/nivanden/condor/error/analysis_exec_$(method)_$(ClusterId)_$(ProcId).err\n')
        f.write('log = /user/nivanden/condor/logs/analysis_exec_$(method)_$(ClusterId)_$(ProcId).log\n\n')

    return


runTypesCode = ["MCAll", "MCPrompt", "ChargeDD", "nonPromptDD", "nonPromptDDControl", "Obs", "MCNoChargeMisID", "MCNoNP"]
runTypes = ["mcall", "dd", "npdd", "chdd", "npcontrol"]

matches = {"mcall" : ["MCAll"], "npdd" : ["nonPromptDD", "MCAll"], "chdd" : ["ChargeDD"], "dd" : ["MCPrompt", "ChargeDD", "nonPromptDD"], "npcontrol" : {"nonPromptDD", "nonPromptDDControl"}}

ddtypes = ["all", "np", "ch"]

mcSL = ["TmpLists/2016PreVFP_WZ_0.txt","TmpLists/2016PreVFP_WZ_1.txt","TmpLists/2016PreVFP_ZZ-H_0.txt","TmpLists/2016PreVFP_ZZ-H_1.txt","TmpLists/2016PreVFP_ZZ-H_2.txt","TmpLists/2016PreVFP_ZZ-H_3.txt","TmpLists/2016PreVFP_ZZ-H_4.txt","TmpLists/2016PreVFP_ZZ-H_5.txt","TmpLists/2016PreVFP_ZZ-H_6.txt","TmpLists/2016PreVFP_ZZ-H_7.txt","TmpLists/2016PreVFP_ZZ-H_8.txt","TmpLists/2016PreVFP_ZZ-H_9.txt","TmpLists/2016PreVFP_VVV_0.txt","TmpLists/2016PreVFP_VVV_1.txt","TmpLists/2016PreVFP_VVV_2.txt","TmpLists/2016PreVFP_VVV_3.txt","TmpLists/2016PreVFP_VVV_4.txt","TmpLists/2016PreVFP_TTX_0.txt","TmpLists/2016PreVFP_TTX_1.txt","TmpLists/2016PreVFP_TTX_2.txt","TmpLists/2016PreVFP_TTX_3.txt","TmpLists/2016PreVFP_TTX_4.txt","TmpLists/2016PreVFP_TTX_5.txt","TmpLists/2016PreVFP_TTX_6.txt","TmpLists/2016PreVFP_TTX_7.txt","TmpLists/2016PreVFP_TTX_8.txt","TmpLists/2016PreVFP_TTVV_0.txt","TmpLists/2016PreVFP_TTVV_1.txt","TmpLists/2016PreVFP_TTVV_2.txt","TmpLists/2016PreVFP_TTVV_3.txt","TmpLists/2016PreVFP_TTVV_4.txt","TmpLists/2016PreVFP_TTVV_5.txt","TmpLists/2016PreVFP_Xgamma_0.txt","TmpLists/2016PreVFP_Xgamma_1.txt","TmpLists/2016PreVFP_Xgamma_2.txt","TmpLists/2016PreVFP_Xgamma_3.txt","TmpLists/2016PreVFP_Xgamma_4.txt","TmpLists/2016PreVFP_Xgamma_5.txt","TmpLists/2016PreVFP_Xgamma_6.txt","TmpLists/2016PreVFP_ttbar_0.txt","TmpLists/2016PreVFP_ttbar_1.txt","TmpLists/2016PreVFP_ttbar_2.txt","TmpLists/2016PreVFP_TTW_0.txt","TmpLists/2016PreVFP_TTW_1.txt","TmpLists/2016PreVFP_TTZ_0.txt","TmpLists/2016PreVFP_TTZ_1.txt","TmpLists/2016PreVFP_TTH_0.txt","TmpLists/2016PreVFP_TTH_1.txt","TmpLists/2016PreVFP_TTTT_0.txt","TmpLists/2016PostVFP_WZ_0.txt","TmpLists/2016PostVFP_WZ_1.txt","TmpLists/2016PostVFP_ZZ-H_0.txt","TmpLists/2016PostVFP_ZZ-H_1.txt","TmpLists/2016PostVFP_ZZ-H_2.txt","TmpLists/2016PostVFP_ZZ-H_3.txt","TmpLists/2016PostVFP_ZZ-H_4.txt","TmpLists/2016PostVFP_ZZ-H_5.txt","TmpLists/2016PostVFP_ZZ-H_6.txt","TmpLists/2016PostVFP_ZZ-H_7.txt","TmpLists/2016PostVFP_ZZ-H_8.txt","TmpLists/2016PostVFP_ZZ-H_9.txt","TmpLists/2016PostVFP_ZZ-H_10.txt","TmpLists/2016PostVFP_ZZ-H_11.txt","TmpLists/2016PostVFP_ZZ-H_12.txt","TmpLists/2016PostVFP_ZZ-H_13.txt","TmpLists/2016PostVFP_VVV_0.txt","TmpLists/2016PostVFP_VVV_1.txt","TmpLists/2016PostVFP_VVV_2.txt","TmpLists/2016PostVFP_VVV_3.txt","TmpLists/2016PostVFP_VVV_4.txt","TmpLists/2016PostVFP_TTX_0.txt","TmpLists/2016PostVFP_TTX_1.txt","TmpLists/2016PostVFP_TTX_2.txt","TmpLists/2016PostVFP_TTX_3.txt","TmpLists/2016PostVFP_TTX_4.txt","TmpLists/2016PostVFP_TTX_5.txt","TmpLists/2016PostVFP_TTX_6.txt","TmpLists/2016PostVFP_TTX_7.txt","TmpLists/2016PostVFP_TTX_8.txt","TmpLists/2016PostVFP_TTVV_0.txt","TmpLists/2016PostVFP_TTVV_1.txt","TmpLists/2016PostVFP_TTVV_2.txt","TmpLists/2016PostVFP_TTVV_3.txt","TmpLists/2016PostVFP_TTVV_4.txt","TmpLists/2016PostVFP_TTVV_5.txt","TmpLists/2016PostVFP_ttbar_0.txt","TmpLists/2016PostVFP_ttbar_1.txt","TmpLists/2016PostVFP_ttbar_2.txt","TmpLists/2016PostVFP_Xgamma_0.txt","TmpLists/2016PostVFP_Xgamma_1.txt","TmpLists/2016PostVFP_Xgamma_2.txt","TmpLists/2016PostVFP_Xgamma_3.txt","TmpLists/2016PostVFP_Xgamma_4.txt","TmpLists/2016PostVFP_Xgamma_5.txt","TmpLists/2016PostVFP_Xgamma_6.txt","TmpLists/2016PostVFP_Xgamma_7.txt","TmpLists/2016PostVFP_TTW_0.txt","TmpLists/2016PostVFP_TTW_1.txt","TmpLists/2016PostVFP_TTZ_0.txt","TmpLists/2016PostVFP_TTZ_1.txt","TmpLists/2016PostVFP_TTH_0.txt","TmpLists/2016PostVFP_TTH_1.txt","TmpLists/2016PostVFP_TTTT_0.txt","TmpLists/2017_WZ_0.txt","TmpLists/2017_WZ_1.txt","TmpLists/2017_ZZ-H_0.txt","TmpLists/2017_ZZ-H_1.txt","TmpLists/2017_ZZ-H_2.txt","TmpLists/2017_ZZ-H_3.txt","TmpLists/2017_ZZ-H_4.txt","TmpLists/2017_ZZ-H_5.txt","TmpLists/2017_ZZ-H_6.txt","TmpLists/2017_ZZ-H_7.txt","TmpLists/2017_ZZ-H_8.txt","TmpLists/2017_ZZ-H_9.txt","TmpLists/2017_ZZ-H_10.txt","TmpLists/2017_ZZ-H_11.txt","TmpLists/2017_ZZ-H_12.txt","TmpLists/2017_ZZ-H_13.txt","TmpLists/2017_ZZ-H_14.txt","TmpLists/2017_ZZ-H_15.txt","TmpLists/2017_ZZ-H_16.txt","TmpLists/2017_VVV_0.txt","TmpLists/2017_VVV_1.txt","TmpLists/2017_VVV_2.txt","TmpLists/2017_VVV_3.txt","TmpLists/2017_VVV_4.txt","TmpLists/2017_TTX_0.txt","TmpLists/2017_TTX_1.txt","TmpLists/2017_TTX_2.txt","TmpLists/2017_TTX_3.txt","TmpLists/2017_TTX_4.txt","TmpLists/2017_TTX_5.txt","TmpLists/2017_TTX_6.txt","TmpLists/2017_TTX_7.txt","TmpLists/2017_TTX_8.txt","TmpLists/2017_TTVV_0.txt","TmpLists/2017_TTVV_1.txt","TmpLists/2017_TTVV_2.txt","TmpLists/2017_TTVV_3.txt","TmpLists/2017_TTVV_4.txt","TmpLists/2017_TTVV_5.txt","TmpLists/2017_ttbar_0.txt","TmpLists/2017_ttbar_1.txt","TmpLists/2017_ttbar_2.txt","TmpLists/2017_Xgamma_0.txt","TmpLists/2017_Xgamma_1.txt","TmpLists/2017_Xgamma_2.txt","TmpLists/2017_Xgamma_3.txt","TmpLists/2017_Xgamma_4.txt","TmpLists/2017_Xgamma_5.txt","TmpLists/2017_Xgamma_6.txt","TmpLists/2017_TTW_0.txt","TmpLists/2017_TTW_1.txt","TmpLists/2017_TTZ_0.txt","TmpLists/2017_TTZ_1.txt","TmpLists/2017_TTH_0.txt","TmpLists/2017_TTH_1.txt","TmpLists/2017_TTTT_0.txt","TmpLists/2018_WZ_0.txt","TmpLists/2018_WZ_1.txt","TmpLists/2018_ZZ-H_0.txt","TmpLists/2018_ZZ-H_1.txt","TmpLists/2018_ZZ-H_2.txt","TmpLists/2018_ZZ-H_3.txt","TmpLists/2018_ZZ-H_4.txt","TmpLists/2018_ZZ-H_5.txt","TmpLists/2018_ZZ-H_6.txt","TmpLists/2018_ZZ-H_7.txt","TmpLists/2018_ZZ-H_8.txt","TmpLists/2018_ZZ-H_9.txt","TmpLists/2018_ZZ-H_10.txt","TmpLists/2018_ZZ-H_11.txt","TmpLists/2018_ZZ-H_12.txt","TmpLists/2018_ZZ-H_13.txt","TmpLists/2018_ZZ-H_14.txt","TmpLists/2018_ZZ-H_15.txt","TmpLists/2018_VVV_0.txt","TmpLists/2018_VVV_1.txt","TmpLists/2018_VVV_2.txt","TmpLists/2018_VVV_3.txt","TmpLists/2018_VVV_4.txt","TmpLists/2018_TTX_0.txt","TmpLists/2018_TTX_1.txt","TmpLists/2018_TTX_2.txt","TmpLists/2018_TTX_3.txt","TmpLists/2018_TTX_4.txt","TmpLists/2018_TTX_5.txt","TmpLists/2018_TTX_6.txt","TmpLists/2018_TTX_7.txt","TmpLists/2018_TTX_8.txt","TmpLists/2018_TTVV_0.txt","TmpLists/2018_TTVV_1.txt","TmpLists/2018_TTVV_2.txt","TmpLists/2018_TTVV_3.txt","TmpLists/2018_TTVV_4.txt","TmpLists/2018_TTVV_5.txt","TmpLists/2018_ttbar_0.txt","TmpLists/2018_ttbar_1.txt","TmpLists/2018_ttbar_2.txt","TmpLists/2018_Xgamma_0.txt","TmpLists/2018_Xgamma_1.txt","TmpLists/2018_Xgamma_2.txt","TmpLists/2018_Xgamma_3.txt","TmpLists/2018_Xgamma_4.txt","TmpLists/2018_Xgamma_5.txt","TmpLists/2018_Xgamma_6.txt","TmpLists/2018_TTW_0.txt","TmpLists/2018_TTW_1.txt","TmpLists/2018_TTZ_0.txt","TmpLists/2018_TTZ_1.txt","TmpLists/2018_TTH_0.txt","TmpLists/2018_TTH_1.txt","TmpLists/2018_TTTT_0.txt"]

dataSL = ["TmpLists/Data16PreVFP_Obs_0.txt","TmpLists/Data16PreVFP_Obs_1.txt","TmpLists/Data16PreVFP_Obs_2.txt","TmpLists/Data16PreVFP_Obs_3.txt","TmpLists/Data16PreVFP_Obs_4.txt","TmpLists/Data16PostVFP_Obs_0.txt","TmpLists/Data16PostVFP_Obs_1.txt","TmpLists/Data16PostVFP_Obs_2.txt","TmpLists/Data17_Obs_0.txt","TmpLists/Data17_Obs_1.txt","TmpLists/Data17_Obs_2.txt","TmpLists/Data17_Obs_3.txt","TmpLists/Data17_Obs_4.txt","TmpLists/Data18_Obs_0.txt","TmpLists/Data18_Obs_1.txt","TmpLists/Data18_Obs_2.txt","TmpLists/Data18_Obs_3.txt"]
#dataSL = ["DATALIAM2017.txt"]


sets = {"MCAll" : [mcSL], "nonPromptDD" : [dataSL, mcSL], "ChargeDD" : [dataSL], "MCPrompt" : [mcSL], "nonPromptDDControl" : [mcSL]}

wd = "/user/nivanden/ewkino/_FourTopAnalysis"

if __name__ == "__main__":
    baseCommand = "./analysisExec $1 $2"
    
    dateTimeObj = datetime.now()
    timestamp = "timestamp=" + dateTimeObj.strftime("%Y-%m-%d_%H-%M")
 
    additionalArgs = [timestamp]
    customSamplelist = False
    runData = False
    customSamplelistName = ""
    specificEra = "1"
    
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
        else:
            print("Unrecognized option: {}. Usage: ./RunAnalysis.py <AnalysisType> [-CR] [-LEAN] [path/to/samplelist] [era=[16Pre, 16Post, 17, 18]]".format(option))

    
    

    # add some default arguments depening on contents of argv
    # add methods to specify queueing in htcondor
    # generally: change descripton file generation compared to usual stuff -> possible to use arguments
    # arguments should be included in queue part -> write it in a way that works

    # kinda works kinda don't
    # idk work more on this

    # add more complete options for runTypes, more similar to runcode. Based on the option, figure out which samplelists to use
    # Based on supplied runtype, the actual runtype for the code should be obtained.
    

    # submissionscript should be same as now basically -> Makes this way less complicated and just use 1 script for all submissions?
    # AnalysisSubmission.sh with ./analysisExec that takes arbitrary number of inputs -> possible: https://stackoverflow.com/questions/10307280/how-to-define-a-shell-script-with-variable-number-of-arguments
    # Then all add these in the command basically. Main code: make sure CR can be wherever we want
    # Static things in job description:
    # arguments = $(samplelist) $(method)
    # additionally provide other arguments like CR, like LooseEventSelection
    # queue samplelist, method from (
    # x.txt, y
    # z.txt, y
    # )
    # ^ general structure for the queue command -> only thing we really have to change inbetween is queue command and arguments
    # Possible runtypes: mcall -> just everything mc; ddXXX: datadriven backgrounds -> run all background estimation as datadriven! (maybe: ddAll, ddNP, ddCh); NPDD only datadriven np bkg estimation (no other things are running); 
    # chargeDD: same as NPDD but for chargemisID; 
    # add extra option: -Data: run data in obs mode alongside it

    PrepareJobDescription(additionalArgs)
    
    queueString = "queue 1 Samplelist, Method from (\n"
    requiredMethods = matches[runType]

    if not customSamplelist:
        requiredSamplelists = sets
        
        # if not dd x -> do stuff different to npdd, chdd

        for method in requiredMethods:
            for x in requiredSamplelists[method]:
                for sl in x:
                    if not specificEra in sl: continue

                    queueString += "\t" 
                    queueString += sl + ", " + method
                    queueString += "\n"

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

    # print queuestring at end of job description
    os.system('condor_submit AnalysisJob.sub')


# future plans: also submit job to condor waiting on previous job to finish