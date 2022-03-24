import sys
import glob

base_dir = "/user/nivanden/condor/logs/"

if __name__ == "__main__":
    jobId = sys.argv[1]

    # list all files with job id in /user/nivanden/condor/logs
    # loop files until code 005 for termination and check for normal termination
    logFiles = glob.glob(base_dir + "*" + jobId + "*")

    for log in logFiles:
        with open(log, 'r') as f:
            for line in f:
                if (line[:2] != "00"): continue

                splitLine = line.split()

                if (splitLine[0] == "005"):
                    print("job " + log + " terminated with code 005. Should be nominal")
                    break
                if (splitLine[0] == "009"):
                    print("job " + log + " aborted")
                    break
                if not any(splitLine[0] in id for id in ["000", "001", "002", "003"]):
                    print("Job " + log + " has id" + splitLine[0] + ". Check manually.")
                 

