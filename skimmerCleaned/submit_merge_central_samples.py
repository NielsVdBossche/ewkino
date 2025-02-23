#import python library classes
import os
import sys
import argparse
import glob
import re
import uproot

#import other code from framework
sys.path.append(os.path.abspath('../'))
from jobSubmission.condorTools import submitCommandsAsCondorJob, submitCommandsAsCondorCluster, submitCommandsetsAsCondorCluster
from fileListing import *


def makeUnique(fname):
    ### make a file name unique by appending a number to it,
    ### e.g. test.txt -> test1.txt (in case test.txt already exists)
    if not os.path.exists(fname): return fname
    [name,ext] = os.path.splitext(fname)
    app = 1
    while app < 2500:
        tryname = name+str(app)+ext
        if not os.path.exists(tryname): return tryname
        app += 1
    print('### ERROR ###: already 2500 files named {} exist.'.format(fname))
    print(' consider choosing more specific names, splitting in folders, etc.')
    sys.exit()


def submission_file(scriptname, njobs):
    return """
executable = {}
arguments = $(Process)
log = /user/nivanden/condor/logs/mergedata_$(ClusterId)_$(Process).txt
output = /user/nivanden/condor/output/mergedata_$(ClusterId)_$(Process).txt
error = /user/nivanden/condor/error/mergedata_$(ClusterId)_$(Process).txt

queue {}
""".format(scriptname, njobs)


def base_script():
    ret = "#!/bin/bash\n\n"
    ret += "source /cvmfs/cms.cern.ch/cmsset_default.sh\n"
    ret += f"cd {os.getenv('CMSSW_BASE')}/src\n"
    ret += "eval `scram runtime -sh`\n"
    ret += "export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)\n\n"
    # ret += f"cd {os.getenv('PWD')}\n\n"
    return ret


def final_commands(pnfs_subdir="", individual_hadds=False):
    if pnfs_subdir != "" and pnfs_subdir[-1] != "/":
        pnfs_subdir += "/"
    ret = "\n"
    ret += "echo $FILENAME\n"
    if not individual_hadds: 
        ret += "echo $DIR\n"
        ret += "hadd -f $TMPDIR/$FILENAME $DIR/*.root\n"
    ret += f"cp $TMPDIR/$FILENAME /pnfs/iihe/cms/store/user/$USER/{pnfs_subdir}$FILENAME\n"
    return ret


def check_if_file_contains_shit(filename):
    # open file, get the Events Tree, check if Tree contains branch "Electron_mvaTOP"
    with uproot.open(filename) as file:
        tree = file["Events"]
        if "Electron_mvaTOP" in tree.keys():
            return True

    return False


def get_good_files(folder):
    files = os.listdir(folder)
    good_files = []
    for file in files:
        path_to_file = os.path.join(folder, file)
        if not os.path.isfile(path_to_file):
            continue
        if check_if_file_contains_shit(path_to_file):
            good_files.append(file)

    rate = len(good_files)/(len(files)-1)
    if rate < 1.:
        return False, good_files
    else:
        return True, files



if __name__ == '__main__' :
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--inputfolder', required=True, help="Folder containing a set of samples.", nargs="+")
    parser.add_argument('-o', '--outputsubfolder', default='', help="Subfolder to use on PNFS for the outputfile. Don't provide the whole structure, just the subfolder")
    parser.add_argument('--filter', default='', help="Arguments to reduce the samples considered in the merging. e.g. a date-tag with a given day, or a specific set of sample parameters")
    parser.add_argument('--version', default='', help="Arguments to match a specific version based on the subfolder")
    parser.add_argument('--CheckEventsTree', default=False, action="store_true")
    args = parser.parse_args()

    if len(args.inputfolder) == 1:
        print("Collecting samples, this will take a while.")
        samples = glob.glob(os.path.join(args.inputfolder[0], f"*/*{args.version}*/*"))
    else:
        samples = []
        for sample in args.inputfolder:
            if args.version in sample:
                samples.append(sample)
    print("Found samples: ", samples)
    scriptlines = ""
    njobs = 0

    for sample in samples:
        print(sample)
        if args.filter not in sample and not bool(re.search(args.filter, sample)):
            continue

        # complete path by walking down through the directory tree
        full_sample_path = sample
        subpath = ""
        while subpath != "log" and os.path.isdir(os.path.join(full_sample_path, subpath)):
            full_sample_path = os.path.join(full_sample_path, subpath)
            subpath = os.listdir(full_sample_path)[0]

        # generate the filename by concatening the last two directories in the sample name:
        date_info = full_sample_path.split("/0000")[0].split("/")[-1]
        filename = "_".join(sample.split("/")[-3:]) + ".root"
        filename = filename.replace("_crab", "")
        filename = filename.replace("crab_", "")
        processname = filename.split("_")[0]
        # replace second occurence of process name in filename if it occurs twice:
        filename = filename.replace("_" + processname, "")


        scriptlines += f"if [ $1 -eq {njobs} ]; then\n"
        njobs += 1
        scriptlines += f"  FILENAME={filename}\n"
        if args.CheckEventsTree:
            everything_ok, files_to_include = get_good_files(full_sample_path)
            if everything_ok:       
                scriptlines += "  exit\n"
                scriptlines += "fi\n"
                continue
            scriptlines += "  hadd -f $TMPDIR/$FILENAME"
            for inputfile in files_to_include:
                scriptlines += " \\\n"
                scriptlines += f"  {os.path.join(full_sample_path, inputfile)}"
            scriptlines += " \n"
        else:
            scriptlines += f"  DIR={full_sample_path}\n"
        
        scriptlines += "fi\n"


    if not os.path.exists("tmp/"):
        os.makedirs("tmp/")
    if not os.path.exists("tmp/log/"):
        os.makedirs("tmp/log")
        os.makedirs("tmp/out")
        os.makedirs("tmp/err")

    scriptname = "tmp/merge_samples.sh"
    scriptname = makeUnique(scriptname)
    with open(scriptname, 'w') as f:
        f.write(base_script())
        f.write(scriptlines)
        f.write(final_commands(args.outputsubfolder, args.CheckEventsTree))

    subfile = "tmp/merge_samples.sub"
    subfile = makeUnique(subfile)
    with open(subfile, 'w') as f:
        f.write(submission_file(scriptname, njobs))

    os.system(f'chmod +x {scriptname}')
    os.system(f'condor_submit {subfile} -batch-name samplemerging')
