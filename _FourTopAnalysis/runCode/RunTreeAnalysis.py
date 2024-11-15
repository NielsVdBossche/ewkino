#!/usr/bin/env python3
import sys
import os
from datetime import datetime
import argparse


methods = ["mcall", "dd", "npdd", "chdd", "npcontrol", "mcnp", "mcprompt", "data_only"]
method_matching = {
    "mcall": ["MCAll"],
    "full": ["MCPrompt", "nonPromptDD", "ChargeDD"],
    "dd": ["MCPrompt", "nonPromptDD", "ChargeDD"],
    "mcprompt": ["MCPrompt"],
    "npdd": ["nonPromptDD"],
    "chdd": ["ChargeDD"],
    "data_only": []
}


def load_samplelist(filename):
    ret = []
    with open(filename, 'r') as f:
        for line in f:
            if line[0] == "#":
                continue
            if line == "\n":
                continue
            ret.append(line.replace("\n", ""))
    return ret


def jobsubmission_base(timestamp, additional_arguments: list = []):
    ret = ""
    ret += "executable = TreeAnalysis.sh\n"
    ret += f"arguments = $(Samplelist) $(Method) $(Syst) {timestamp} condorrun $(MiniOrNano)"

    for arg in additional_arguments:
        ret += f" {arg}"
    ret += "\n\n"

    if not os.path.exists(f"/user/{os.getenv('USER')}/condor"):
        os.makedirs(f"/user/{os.getenv('USER')}/condor")
        os.makedirs(f"/user/{os.getenv('USER')}/condor/output")
        os.makedirs(f"/user/{os.getenv('USER')}/condor/error")
        os.makedirs(f"/user/{os.getenv('USER')}/condor/logs")

    ret += f"output = /user/{os.getenv('USER')}/condor/output/Analysis_$(Method)_$(Syst)_$(SLShort)_$(ClusterId).$(ProcId).out\n"
    ret += f"error = /user/{os.getenv('USER')}/condor/error/Analysis_$(Method)_$(Syst)_$(SLShort)_$(ClusterId).$(ProcId).err\n"
    ret += f"log = /user/{os.getenv('USER')}/condor/logs/Analysis_$(Method)_$(Syst)_$(SLShort)_$(ClusterId).$(ProcId).log\n\n"

    ret += "should_transfer_files = NO\n\n"

    ret += "queue 1 SLShort, Samplelist, Method, Syst, MiniOrNano from (\n"
    return ret


def parse_arguments():
    parser = argparse.ArgumentParser(description='Description of your program')

    # Add arguments
    parser.add_argument('-m','--method', dest="method", default="full",
                        help='Method to run the analysis with.')

    # truly optional
    parser.add_argument("--unc", "--uncertainty", dest="uncertainty", default=None,
                        help="If this argument is given, only the chosen uncertainty is used")
    parser.add_argument('-sl', '--samplelist', dest="samplelist", default=None,
                        help="Provide a specific samplelist to use in the run.")
    parser.add_argument('-y', '--year', dest="year", default=None,
                        help="Filter to only run samples for a chosen year.")
    parser.add_argument('-p', '--process', dest="process", default=None,
                        help="Filter to only run samples for a chosen process.")
    # switches
    parser.add_argument("--nominal", dest="only_nominal", action='store_true',
                        help="Switch to swap between full analysis run with JECs, JERs, ... or only the nominal run")
    parser.add_argument('--data', dest="data", action='store_true',
                        help="Switch to turn on the processing of the data.")
    parser.add_argument('--onlydata', dest="onlydata", action='store_true',
                        help="Switch to only process the data.")
    parser.add_argument('--bsm', dest="bsm", action='store_true',
                        help="Switch to turn on the processing of the BSM samples.")
    parser.add_argument('--onlybsm', dest="onlybsm", action='store_true',
                        help="Switch to only process BSM samples.")
    parser.add_argument('--eft', dest="eft", action='store_true',
                        help="Switch to turn on the processing of the EFT samples.")
    parser.add_argument('--onlyeft', dest="onlyeft", action='store_true',
                        help="Switch to only process EFT samples.")
    parser.add_argument('--debug', '--dryrun', dest="debug", action='store_true',
                        help="Switch to turn on debug mode.")
    # Parse arguments
    args = parser.parse_args()
    
    return args


def provide_dataruns(data_samples, miniOrNano="MiniAOD"):
    ret = []
    for sample in data_samples:
        line = f"\t{sample} TmpLists/{sample} Obs nominal {miniOrNano}"
        ret.append(line)
    return ret


def provide_runs(samplelist, method, syst, miniOrNano="MiniAOD"):
    sl_short = lambda sample: "_".join(sample.split("/")[-1].split(".txt")[0].split("_fullStat_"))
    ret = [f"\t{sl_short(sample)} TmpLists/{sample} {method} {syst} {miniOrNano}" for sample in samplelist]
    return ret


def filter_samplelist(samplelist, filters):
    new_sl = []
    for sample in samplelist:
        if all([filt in sample for filt in filters]):
            new_sl.append(sample)
    return new_sl

if __name__ == "__main__":
    args = parse_arguments()
    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M")

    run_entries = []

    methods = method_matching[args.method]
    if args.only_nominal:
        systs = ["nominal"]
    elif args.uncertainty:
        systs = [args.uncertainty]
        methods = ["MCPrompt"]
    elif args.onlydata:
        methods = []
    else:
        systs = ["nominal", "jec_single", "jec_grouped", "jer", "met", "hem"]

    mc_samplelist = []
    data_samplelist = []
    bsm_samplelist = []
    if args.samplelist:
        print("SL")
        if "Data" in args.samplelist:
            data_samplelist.append(args.samplelist)
        else:
            mc_samplelist.append(args.samplelist)
    else:
        mc_samplelist = load_samplelist("mc_samples.txt")
        bsm_samplelist = load_samplelist("bsm_mc_samples.txt")
        eft_samplelist = load_samplelist("eft_mc_samples.txt")
        if "nonPromptDD" in methods or "ChargeDD" in methods or args.data or args.onlydata:
            data_samplelist = load_samplelist("data_samples_nano.txt")

    sample_filters = []
    if args.year:
        year = str(args.year).replace("20", "")
        sample_filters.append(year)
    if args.process:
        sample_filters.append(args.process)

    mc_samplelist = filter_samplelist(mc_samplelist, sample_filters)
    data_samplelist = filter_samplelist(data_samplelist, sample_filters)
    bsm_samplelist = filter_samplelist(bsm_samplelist, sample_filters)
    eft_samplelist = filter_samplelist(eft_samplelist, sample_filters)

    for method in methods:
        for syst in systs:
            if syst != "nominal" and method != "MCPrompt":
                continue
            if method != "ChargeDD":
                if not args.onlybsm and not args.onlyeft:
                    run_entries.extend(provide_runs(mc_samplelist, method, syst, miniOrNano="NanoAOD"))
                if args.bsm or args.onlybsm:
                    run_entries.extend(provide_runs(bsm_samplelist, method, syst, miniOrNano="NanoAOD"))
                if args.eft or args.onlyeft:
                    run_entries.extend(provide_runs(eft_samplelist, method, syst, miniOrNano="NanoAOD"))
            if (method == "nonPromptDD" or method == "ChargeDD") and (not args.onlybsm and not args.onlyeft):
                run_entries.extend(provide_runs(data_samplelist, method, syst, miniOrNano="NanoAOD"))

    if args.data or args.onlydata:
        run_entries.extend(provide_dataruns(data_samplelist))
        # fill data samplelist as well

    submission_file = jobsubmission_base(timestamp)
    submission_file += "\n".join(run_entries)
    submission_file += "\n)\n"

    with open("AnalysisJob.sub", 'w') as f:
        f.write(submission_file)

    if args.debug:
        exit()

    outputfolder = os.path.join(f"/pnfs/iihe/cms/store/user/{os.getenv('USER')}/AnalysisOutput/ReducedTuples/", timestamp)
    os.makedirs(outputfolder)
    os.system("condor_submit AnalysisJob.sub")
