################################################################
# run a skim on a single test file (for testing and debugging) #
################################################################

import sys
import os

if len(sys.argv)==1:
    print('Use with following command line args:')
    print(' - path to input file')
    print(' - path to output file')
    print(' - skim condition')
    sys.exit()

# parse input arguments
input_file_path = sys.argv[1]
output_file_path = sys.argv[2]
skim_condition = sys.argv[3]
output_dir = os.path.dirname(output_file_path)

# check if input file exists
if not os.path.exists(input_file_path):
    raise Exception('ERROR: input file does not seem to exist.')

# check if skim condition is valid
if not skim_condition in (["noskim", "singlelepton", 
			"dilepton", "trilepton", 
			"fourlepton", "fakerate"]):
    raise Exception('ERROR: skim condition not recognized.')

# check if executable exists
exe = './skimmer'
if not os.path.exists(exe):
    raise Exception('ERROR: executable {} does not seem to exist'.format(exe))

# create the output directory if it does not exist
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# run the skim job
cmd = exe + ' ' + input_file_path + ' ' + output_file_path + ' ' + skim_condition
print('running command: {}'.format(cmd))
os.system(cmd)
