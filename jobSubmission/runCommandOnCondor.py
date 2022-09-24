import condorTools as ct
import sys


if __name__ == "__main__":
    command = ""

    for item in sys.argv[1:]:
        command += item + " "
    
    ct.submitCommandAsCondorJob("stacker", command)

