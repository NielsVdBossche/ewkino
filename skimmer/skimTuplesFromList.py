##################################
# skim tuples from a sample list #
##################################
# Functionality is similar to skimTuples.py, 
# but with following updates/extensions:
# - usage of a sample list instead of automatically running over all samples in a directory.
# - possibility to read samples directly from DAS instead of from a local directory.
# Note: might not work anymore for reading local files after modifications to allow DAS;
#       the expected use case however is only reading from DAS.

import sys
import os
import argparse
sys.path.append('python')
from fileListing import listSampleDirectories, listSampleFiles, listParts
from dasTools import get_sample_files 
from skimTuples import yearIdentifierFromPath
sys.path.append(os.path.abspath('../jobSubmission'))
import condorTools as ct
from jobSettings import CMSSW_VERSION
CMSSW_VERSION = '~/CMSSW_12_4_6'
sys.path.append(os.path.abspath('../Tools/python'))
from samplelisttools import readsamplelist
import argparsetools as apt

# define allowed skim conditions
skims = (["noskim", "singlelepton", "dilepton",
        "trilepton", "fourlepton", "fakerate",
	"dilightlepton", "multilightlepton"])

# parse arguments
parser = argparse.ArgumentParser('Skimming')
parser.add_argument('--samplelist', required=True, type=os.path.abspath)
parser.add_argument('--outputdir', required=True, type=os.path.abspath)
parser.add_argument('--skim', required=True, choices=skims)
parser.add_argument('--files_per_job', default=10, type=int)
parser.add_argument('--walltime_hours', default=24, type=int)
parser.add_argument('--filemode', default='das', choices=['das','local'])
parser.add_argument('--inputdir', default=None, type=apt.path_or_none)
parser.add_argument('--proxy', default=None, type=apt.path_or_none)
parser.add_argument('--istest', default=False, action='store_true')
parser.add_argument('--max_files_per_sample', default=-1, type=int)
parser.add_argument('--readmode', default='remote', choices=['remote','copy'])
parser.add_argument('--runmode', default='condor', choices=['condor','local'])
args = parser.parse_args()

# print arguments
print('Running with following configuration:')
for arg in vars(args):
    print('  - {}: {}'.format(arg,getattr(args,arg)))

# check if executable exists
exe = './skimmer'
if not os.path.exists(exe):
    raise Exception('ERROR: executable {} does not exist.'.format(exe))

# check if input directory exists
if args.inputdir is not None:
    if not os.path.exists(args.inputdir):
	raise Exception('ERROR: input directory {} does not exist.'.format(args.inputdir))

# check if sample list exist
if not os.path.exists(args.samplelist):
    raise Exception('ERROR: sample list {} does not exist.'.format(args.samplelist))

# check if output directory is empty and ask permission to clean it
if os.path.exists(args.outputdir):
    if not len(os.listdir(args.outputdir))==0:
	print('WARNING: output directory {} not empty.'.format(args.outputdir)
		+' Permission to clean it? (y/n)')
	go = raw_input()
	if not go=='y': sys.exit()
	os.system('rm -r {}'.format(os.path.join(args.outputdir,'*')))
else: os.makedirs(args.outputdir)

# parse walltime
walltime = '{}:00:00'.format(args.walltime_hours)

# make a list of sample names
print('Extracting sample names from the provided sample list...')
dasformat = (args.filemode=='das')
sample_collection = readsamplelist(args.samplelist, dasformat=dasformat)
sample_names = [s.name for s in sample_collection.get_samples()]

# print sample names
print('Extracted {} sample names from the sample list:'.format(len(sample_names)))
for s in sample_names: print('  - {}'.format(s))

# get the files for each sample
print('Finding number of files to process...')
sample_files = {}
nfiles = []
for s in sample_names:
    maxfiles = None
    if args.max_files_per_sample > 0:
	maxfiles = args.max_files_per_sample
    this_sample_files = get_sample_files(s,
                      filemode=args.filemode,
                      istest=args.istest,
                      maxfiles=maxfiles)
    # check if sample was found correctly
    allgood = True
    if len(this_sample_files)==0: allgood = False
    for this_sample_file in this_sample_files:
	if not this_sample_file.endswith('.root'): allgood = False
    if not allgood:
	msg = 'ERROR: something seems wrong with sample {}'.format(s)
	msg += ' query to retrieve files for this sample'
	msg += ' returned the following: {}'.format(this_sample_files)
	raise Exception(msg)
    print('sample: {} -> {} files found'.format(s,len(this_sample_files)))
    sample_files[s] = this_sample_files
    nfiles.append(len(this_sample_files))
nfiles = sum(nfiles)
njobs = max(1,int(nfiles/args.files_per_job))
print('Found a total of {} files, which will result in approximately {} jobs.'.format(
	nfiles,njobs))
print('Continue with the submission? (y/n)')
go = raw_input()
if not go=='y': sys.exit()

# make output directory for each sample
print('Making output directories...')
sample_output_directories = []
for sample_name in sample_names:
    _, shortname, version, _ = sample_name.split('/')
    yearid = yearIdentifierFromPath( version )
    shortname += '_' + yearid
    output_directory = os.path.join( args.outputdir, 'ntuples_skimmed_{}_version_{}'
					.format( shortname, version ) )
    if not os.path.exists( output_directory ):
        os.makedirs( output_directory )
    sample_output_directories.append( output_directory )

# loop over samples and submit skimming jobs
print('Starting submission...')
cwd = os.getcwd()
itlist = zip(sample_names, sample_output_directories)
for sample_name, sample_output_directory in itlist:
    print('----------------------------')
    print('Now processing the following sample:')
    print('  {}'.format(sample_name))
    print('  Number of root files: {}'.format(len(sample_files[sample_name])))
    # split files in lists of files_per_job
    chunks = list(listParts( sample_files[sample_name], args.files_per_job ))
    for chunk in chunks:
	# make the commands to execute for this chunk
	commands = []
	commands.append( 'cd {}'.format(cwd) )
	# loop over files in this chunk
	for f in chunk:
	    # define output file
	    output_file = f.split('/')[-1]
	    output_file = os.path.join(sample_output_directory,output_file)
	    # define commands to skim this file
	    thiscommands = []
	    if args.readmode=='remote':
		# read remote file directly
		thiscommands.append('./skimmer {} {} {}'.format(f,output_file,args.skim))
	    elif args.readmode=='copy':
		# copy remote file to local before running skimmer
		output_file_unskimmed = output_file.replace('.root','_raw.root')
		thiscommands.append('xrdcp {} {}'.format(f,output_file_unskimmed))
		thiscommands.append('./skimmer {} {} {}'.format(
		    output_file_unskimmed,output_file,args.skim))
		thiscommands.append('rm -f {}'.format(output_file_unskimmed))
	    for c in thiscommands: commands.append(c)
        # run in local
	if( args.runmode=='local' ):
	    for cmd in commands: os.system(cmd)
	# submission via condor
	if( args.runmode=='condor' ): 
	    ct.submitCommandsAsCondorJob( 
		'cjob_skimTuplesFromList', commands,
		cmssw_version=CMSSW_VERSION, proxy=args.proxy )
