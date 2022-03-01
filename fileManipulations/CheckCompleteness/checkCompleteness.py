import os
import sys
import numpy as np
import glob

def missing_elements(L):
    start, end = L[0], L[-1]
    return sorted(set(range(start, end + 1)).difference(L))


if __name__ == '__main__':
    dir = sys.argv[1]
    dataset = sys.argv[1]

    #### ask a dataset as argument
    # list all folders matching this name in the dir
    out = glob.glob(dir + "*" + dataset + "*")

    print(out)

    for subdir in out:
        files = glob.glob(subdir + "/*.root")
        # list all folders for this dataset and check  if there is continuity in the numbering
        integers = np.zeros(len(files), dtype=np.int64)

        for i, file in enumerate(files):
            print(file[file.rfind('_'):-5])
            # integers[i] = int(file[:-5])

            

        

    

