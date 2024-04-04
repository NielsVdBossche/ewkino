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
            ret.append(line.replace("\n", ""))
    return ret


def jobsubmission_base(timestamp, additional_arguments: list = []):
    ret = ""
    ret += "executable = TreeAnalysis.sh\n"
    ret += f"arguments = $(Samplelist) $(Method) $(Syst) timestamp={timestamp}"

    for arg in additional_arguments:
        ret += f" {arg}"
    ret += "\n\n"

    ret += "output = /user/nivanden/condor/output/Analysis_$(Method)_$(ClusterId).$(ProcId).out\n"
    ret += "error = /user/nivanden/condor/error/Analysis_$(Method)_$(ClusterId).$(ProcId).out\n"
    ret += "log = /user/nivanden/condor/logs/Analysis_$(Method)_$(ClusterId).$(ProcId).log\n\n"

    ret += "queue 1 Samplelist, Method, Syst from (\n"
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
    # Parse arguments
    args = parser.parse_args()
    
    return args


def provide_dataruns(data_samples):
    ret = []
    for sample in data_samples:
        line = f"\tTmpLists/{sample} Obs nominal"
        ret.append(line)
    return ret


def provide_runs(samplelist, method, syst):
    ret = [f"\tTmpLists/{sample} {method} {syst}" for sample in samplelist]
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
    else:
        systs = ["nominal", "jec_single", "jer", "met", "hem"]

    mc_samplelist = []
    data_samplelist = []
    if args.samplelist:
        print("SL")
        if "Data" in args.samplelist:
            data_samplelist.append(args.samplelist)
        else:
            mc_samplelist.append(args.samplelist)
    else:
        mc_samplelist = load_samplelist("mc_samples.txt")
        if "nonPromptDD" in methods or "ChargeDD" in methods or args.data:
            data_samplelist = load_samplelist("data_samples.txt")

    sample_filters = []
    if args.year:
        year = str(args.year).replace("20", "")
        sample_filters.append(year)
    if args.process:
        sample_filters.append(args.process)

    mc_samplelist = filter_samplelist(mc_samplelist, sample_filters)
    data_samplelist = filter_samplelist(data_samplelist, sample_filters)

    for method in methods:
        for syst in systs:
            if syst != "nominal" and method != "MCPrompt":
                continue
            run_entries.extend(provide_runs(mc_samplelist, method, syst))
            if method == "nonPromptDD" or method == "ChargeDD":
                run_entries.extend(provide_runs(data_samplelist, method, syst))

    if args.data:
        run_entries.extend(provide_dataruns(data_samplelist))
        # fill data samplelist as well

    submission_file = jobsubmission_base(timestamp)
    submission_file += "\n".join(run_entries)
    submission_file += "\n)\n"

    with open("AnalysisJob.sub", 'w') as f:
        f.write(submission_file)

    outputfolder = os.path.join("../Output", timestamp)
    os.makedirs(outputfolder)
    os.system("condor_submit AnalysisJob.sub")
