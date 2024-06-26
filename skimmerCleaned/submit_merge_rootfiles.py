import argparse
import os
import glob
"""
Walk folder structure until root files are found.
Start from a base folder, drop down until no more subfolders, for each launch a merging job.
Run in a CMSSW environment!
"""


def submission_file(njobs):
    return """
executable = tmp/merge_samples.sh
arguments = $(Process)
log = tmp/log/log_$(ClusterId)_$(Process).txt
output = tmp/out/out_$(ClusterId)_$(Process).txt
error = tmp/err/err_$(ClusterId)_$(Process).txt

queue {}
""".format(njobs)


def base_script():
    ret = "#!/bin/bash\n\n"
    ret += "source /cvmfs/cms.cern.ch/cmsset_default.sh\n"
    ret += f"cd {os.gentenv("CMSSW_BASE")}/src\n"
    ret += "eval `scram runtime -sh`\n"
    ret += "export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)\n\n"
    # ret += f"cd {os.getenv('PWD')}\n\n"
    return ret


def final_commands(pnfs_subdir=""):
    if pnfs_subdir != "" and pnfs_subdir[-1] != "/":
        pnfs_subdir += "/"
    ret = "\n"
    ret += "echo $DIR\n"
    ret += "echo $FILENAME\n"
    ret += "hadd -f $TMPDIR/$FILENAME $DIR/*NanoAOD*root\n"
    ret += f"cp $TMPDIR/$FILENAME /pnfs/iihe/cms/store/user/$USER/{pnfs_subdir}$FILENAME\n"
    return ret


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--inputfolder', required=True, help="Folder containing a set of samples.")
    parser.add_argument('-o', '--outputsubfolder', default='', help="Subfolder to use on PNFS for the outputfile. Don't provide the whole structure, just the subfolder")
    parser.add_argument('--filter', default='', help="Arguments to reduce the samples considered in the merging. e.g. a date-tag with a given day, or a specific set of sample parameters")
    args = parser.parse_args()

    print("Collecting samples, this will take a while.")
    samples = glob.glob(os.path.join(args.inputfolder, "*/*/NanoAOD_0.root"))
    scriptlines = ""
    njobs = 0
    for sample in samples:
        if args.filter not in sample:
            continue

        sampledir = sample.split("NanoAOD_0.root")[0]
        filename = sample.split(args.inputfolder)[-1].split("/")[0].replace("MiniAODandNanoAOD", "NanoAOD") + ".root"
        scriptlines += f"if [ $1 -eq {njobs} ]; then\n"
        njobs += 1
        scriptlines += f"  DIR={sampledir}\n"
        scriptlines += f"  FILENAME={filename}\n"
        scriptlines += "fi\n"

    if not os.path.exists("tmp/"):
        os.makedirs("tmp/")
    if not os.path.exists("tmp/log/"):
        os.makedirs("tmp/log")
        os.makedirs("tmp/out")
        os.makedirs("tmp/err")

    scriptname = "tmp/merge_samples.sh"
    with open(scriptname, 'w') as f:
        f.write(base_script())
        f.write(scriptlines)
        f.write(final_commands(args.outputsubfolder))

    subfile = "tmp/merge_samples.sub"
    with open(subfile, 'w') as f:
        f.write(submission_file(njobs))

    os.system('chmod +x tmp/merge_samples.sh')
    os.system('condor_submit tmp/merge_samples.sub -batch-name samplemerging')
