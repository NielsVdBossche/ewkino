
#import python library classes
import os
import sys
import subprocess
import time


#import other code from framework
sys.path.append(os.path.abspath('../'))
from jobSubmission.condorTools import submitCommandsAsCondorJob
from fileListing import *


## Is this compatible with UL directory structure?
# Might need to rework this a bit

def is2017( sample_path ):
	return ( ( 'MiniAOD2017' in sample_path )
		or ( 'Run2017' in sample_path )
		or ( 'Fall17' in sample_path )
	)


def is2018( sample_path ):
	return ( ( 'MiniAOD2018' in sample_path )
		or ( 'Run2018' in sample_path )
		or ( 'Autumn18' in sample_path )
	)


def yearIdentifierFromPath( sample_path ):
    if is2018( sample_path ):
        return 'Autumn18'
    elif is2017( sample_path ):
        return 'Fall17'
    else :
        return 'Summer16'



if __name__ == '__main__' :

    #WARNING : it is assumed this script is run from the 'skimmer' directory
    current_directory = os.path.dirname( os.path.abspath( __file__ ) )
    
    input_directory = sys.argv[1] 
    version_name = sys.argv[2] 
    output_directory_base = sys.argv[3] 
    skim_condition = sys.argv[4]
    files_per_job = int(sys.argv[5]) if len( sys.argv ) >= 6 else 50
    processes = sys.argv[6] if len( sys.argv ) >= 7 else ''
    if len( sys.argv ) <= 4:
        print( 'Error: skimTuples.py requires additional command-line arguments.' )
        print( 'Usage: python skimTuples.py < input_directory > < ntuple_version > < output_directory > < skim_condition > < files_per_job > < wall_time >' )
        print( 'files_per_job and wall_time have default values of 50' )
        sys.exit()
    
    #make a list of samples (the main directories) and the subdirectories with the latest version of the ntuples ( one for each main directory )
    sample_directories = []
    sample_sub_directories = []
    for sample_directory, subdirectory in listSampleDirectories( input_directory, version_name ):
        if processes != '' and not processes in sample_directory: 
            continue
        
        sample_directories.append( sample_directory )
        sample_sub_directories.append( subdirectory )
    
    #output directory for each sample
    sample_names = [ directory.rstrip( os.path.sep ).split( os.path.sep )[-1] + '_' + subdirectory for directory, subdirectory in zip( sample_directories, sample_sub_directories ) ]
    sample_output_directories = []
    for sample in sample_names:
        output_directory = os.path.join( output_directory_base, 'ntuples_skimmed_{}_version_{}'.format( sample, version_name ) )
        if not os.path.exists( output_directory ):
            os.makedirs( output_directory )
        sample_output_directories.append( output_directory )

    for sample_directory, sub_directory, output_directory in zip( sample_directories, sample_sub_directories, sample_output_directories ):
    
        #identify locations of files to process for a given sample 
        root_files = list( listFiles( os.path.join( sample_directory, sub_directory ), '.root' ) )
        
        #split_files in lists of files_per_job
        for chunk in listParts( root_files, files_per_job ):
        
            #make a job script 
            script_name = 'skimmer.sh'

            commands = []
            for f in chunk :
                skim_command = './skimmer {} {} {}\n'.format( f, output_directory, skim_condition )
                commands.append(skim_command)
            
            submitCommandsAsCondorJob(script_name, commands)


