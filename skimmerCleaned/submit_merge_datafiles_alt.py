import os
import sys
import uproot
import glob

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
        return good_files
    else:
        return files


if __name__ == "__main__":
