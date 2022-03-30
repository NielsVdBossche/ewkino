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


runTypesCode = ["MCAll", "MCPrompt", "ChargeDD", "nonPromptDD", "Obs", "MCNoChargeMisID", "MCNoNP"]
runTypes = ["mcall", "dd", "npdd", "chdd"]

matches = {"mcall" : ["MCAll"], "npdd" : ["nonPromptDD", "MCAll"], "chdd" : ["ChargeDD"], "dd" : ["MCPrompt", "ChargeDD", "nonPromptDD"]}

ddtypes = ["all", "np", "ch"]

mcSL = ["TmpLists/2016PreVFP_Rare_0.txt", "TmpLists/2016PreVFP_Rare_1.txt", "TmpLists/2016PreVFP_Rare_2.txt", 
            "TmpLists/2016PreVFP_Rare_3.txt", "TmpLists/2016PreVFP_Rare_4.txt", "TmpLists/2016PreVFP_Rare_5.txt", 
            "TmpLists/2016PreVFP_Rare_6.txt", "TmpLists/2016PreVFP_Rare_7.txt", "TmpLists/2016PreVFP_Rare_8.txt", 
            "TmpLists/2016PreVFP_Rare_9.txt", "TmpLists/2016PreVFP_TTVV_0.txt", "TmpLists/2016PreVFP_TTVV_1.txt", 
            "TmpLists/2016PreVFP_Xgamma_0.txt", "TmpLists/2016PreVFP_Xgamma_1.txt", "TmpLists/2016PreVFP_ttbar_0.txt", 
            "TmpLists/2016PreVFP_TTW_0.txt", "TmpLists/2016PreVFP_TTZ_0.txt", "TmpLists/2016PreVFP_TTH_0.txt", 
            "TmpLists/2016PreVFP_TTTT_0.txt", "TmpLists/2016PostVFP_Rare_0.txt", "TmpLists/2016PostVFP_Rare_1.txt", 
            "TmpLists/2016PostVFP_Rare_2.txt", "TmpLists/2016PostVFP_Rare_3.txt", "TmpLists/2016PostVFP_Rare_4.txt", 
            "TmpLists/2016PostVFP_Rare_5.txt", "TmpLists/2016PostVFP_Rare_6.txt", "TmpLists/2016PostVFP_Rare_7.txt", 
            "TmpLists/2016PostVFP_Rare_8.txt", "TmpLists/2016PostVFP_Rare_9.txt", "TmpLists/2016PostVFP_TTVV_0.txt", 
            "TmpLists/2016PostVFP_TTVV_1.txt", "TmpLists/2016PostVFP_ttbar_0.txt", "TmpLists/2016PostVFP_Xgamma_0.txt", 
            "TmpLists/2016PostVFP_Xgamma_1.txt", "TmpLists/2016PostVFP_TTW_0.txt", "TmpLists/2016PostVFP_TTZ_0.txt", 
            "TmpLists/2016PostVFP_TTH_0.txt", "TmpLists/2016PostVFP_TTTT_0.txt", "TmpLists/2017_Rare_0.txt", "TmpLists/2017_Rare_1.txt", 
            "TmpLists/2017_Rare_2.txt", "TmpLists/2017_Rare_3.txt", "TmpLists/2017_Rare_4.txt", "TmpLists/2017_Rare_5.txt", 
            "TmpLists/2017_Rare_6.txt", "TmpLists/2017_Rare_7.txt", "TmpLists/2017_Rare_8.txt", "TmpLists/2017_TTVV_0.txt", 
            "TmpLists/2017_TTVV_1.txt", "TmpLists/2017_ttbar_0.txt", "TmpLists/2017_Xgamma_0.txt", "TmpLists/2017_Xgamma_1.txt", 
            "TmpLists/2017_TTW_0.txt", "TmpLists/2017_TTZ_0.txt", "TmpLists/2017_TTH_0.txt", "TmpLists/2017_TTTT_0.txt", 
            "TmpLists/2018_Rare_0.txt", "TmpLists/2018_Rare_1.txt", "TmpLists/2018_Rare_2.txt", "TmpLists/2018_Rare_3.txt", 
            "TmpLists/2018_Rare_4.txt", "TmpLists/2018_Rare_5.txt", "TmpLists/2018_Rare_6.txt", "TmpLists/2018_Rare_7.txt", 
            "TmpLists/2018_Rare_8.txt", "TmpLists/2018_Rare_9.txt", "TmpLists/2018_TTVV_0.txt", "TmpLists/2018_TTVV_1.txt", 
            "TmpLists/2018_ttbar_0.txt", "TmpLists/2018_Xgamma_0.txt", "TmpLists/2018_Xgamma_1.txt", "TmpLists/2018_TTW_0.txt", 
            "TmpLists/2018_TTZ_0.txt", "TmpLists/2018_TTH_0.txt", "TmpLists/2018_TTTT_0.txt"]

dataSL = ["Data16PostVFP.txt", "Data16PreVFP.txt", "Data17.txt", "Data18.txt"]


sets = {"MCAll" : [mcSL], "nonPromptDD" : [mcSL, dataSL], "ChargeDD" : [dataSL], "MCPrompt" : [mcSL]}


wd = "/user/nivanden/ewkino/_FourTopAnalysis"

if __name__ == "__main__":
    baseCommand = "./analysisExec $1 $2"
    
    dateTimeObj = datetime.now()
    timestamp = "timestamp=" + str(dateTimeObj.year) + "-" + str(dateTimeObj.month) + "-" + str(dateTimeObj.day) + "_" + str(dateTimeObj.hour) + "-" + str(dateTimeObj.minute)
 
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