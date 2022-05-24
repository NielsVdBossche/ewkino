import glob
import os, time, sys

def deleteOldFiles(path):
    now = time.time()

    for f in glob.glob(path+"*analysis*"):
        if os.stat(f).st_mtime < now - 4 * 86400:
            if os.path.isfile(f):
                os.remove(os.path.join(path, f))


def deleteAllOldCondorfiles():
    subdirs = ["output", "error", "log"]
    path = "/user/nivanden/condor/"
    for sub in subdirs:
        deleteOldFiles(path+sub)


