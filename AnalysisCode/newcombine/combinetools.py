############################################################################
# tools to run the combine executable for several configurations and tasks #
############################################################################
import os
import sys

def cleandatacarddir(datacarddir,rmtxt=True,rmroot=True):
    ### remove all objects from datacarddir except original (elementary) datacards and histograms
    # depends a little on naming convention, maybe think of generalization later
    if rmtxt:
        alltxtfiles = [f for f in os.listdir(datacarddir) if f[-4:]=='.txt']
        for f in alltxtfiles:
            if '_out_' in f: os.system('rm '+os.path.join(datacarddir,f))
            elif 'combined' in f: os.system('rm '+os.path.join(datacarddir,f))
    if rmroot:
        allrootfiles = [f for f in os.listdir(datacarddir) if f[-5:]=='.root']
        for f in allrootfiles:
            if not f[:11]=='histograms_': os.system('rm '+os.path.join(datacarddir,f))
    allotherfiles = [f for f in os.listdir(datacarddir) if not (f[-5:]=='.root' or f[-4:]=='.txt')]
    for f in allotherfiles: os.system('rm -r '+os.path.join(datacarddir,f))

def makecombinedcards(datacarddir,combinationdict):
    ### call combineCards functionality on combinations of datacards
    # input arguments:
    # - datacarddir: directory containing the datacards
    # - combinationdict: a dictionary,
    #   key = filename of combined datacard to create
    #	value = dict mapping filenames of cards to combine to channelnames in combination  
    # output: a list of of combined datacard filenames
    #         (not necessarily equal to combinationdict.keys() due to
    #         possibly invalid entries (unexisting cards or zero cards to combine))

    # initializations
    cwd = os.getcwd()
    os.chdir(datacarddir)
    combinedcards = []
    # loop over requested combinations
    keys = sorted(list(combinationdict.keys()))
    for combcard in keys:
	cards = combinationdict[combcard].keys()
	# check if combination dict entry for this combination contains > 0 cards
        if len(cards)==0:
            print('WARNING in makecombinedcards: found empty list for combination '.format(combcard)
		    +', skipping it.')
            continue
	# check if each card for this combination exists
	allexist = True
	for card in cards:
	    if not os.path.exists(card):
		allexist = False
	if not allexist:
	    print('WARNING in makecombinedcards: some requested cards for combination '.format(combcard)
		    +' seem not to exist, skipping this combination.')
	    continue
	# make sure the combination has correct extension
	combcard = os.path.splitext(combcard)[0]+'.txt'
	# run the combinceCards script
	command = 'combineCards.py'
	for card in cards:
	    channelname = combinationdict[combcard][card]
	    command += ' '+channelname+'='+card
	command += ' &> '+combcard
	os.system(command)
	combinedcards.append(combcard)
    os.chdir(cwd)
    return combinedcards

def get_workspace_commands(datacarddir,card,workspace=None,options=''):
    ### get the command to turn a datacard into a workspace
    # input arguments:
    # - datacarddir: directory of the datacards
    # - card: filename of the datacard file in datacarddir
    # - workpace: name of the output workspace (default: equal to card)
    # - options: string that will be appended to text2workspace command
    commands = []
    cwd = os.getcwd()
    if workspace is None: workspace = card.replace('.txt','.root')
    commands.append('cd {}'.format(datacarddir))
    commands.append('text2workspace.py {} -o {} {}'.format(card,workspace,options))
    commands.append('cd {}'.format(cwd))
    return commands

def get_significance_commands(datacarddir,card,workspace=None,usedata=False,pois=['r']):
    ### get the commands to calculate the significance
    # input arguments:	
    # - datacarddir: directory of the datacards
    # - card: filename of the datacard file in datacarddir
    # - workpace: name of the workspace (default: equal to card)
    # - usedata: boolean whether to use data (default: blind analysis)
    # - pois: list of parameters of interest (default: one poi called 'r')
    # WARNING: DOES NOT (YET) SEEM TO WORK FOR MULTIPLE POIS!
    # output:
    # - list of commands that, when executed, will produce:
    #	-- txt file similar to card but with extension _out_significance_exp (for blind analysis)
    #	   or _out_significance_obs (for unblinded analysis)
    #	-- output root file named higgsCombine<datacard>_<extension>.Significance.mH120.root

    if workspace is None: workspace = card.replace('.txt','.root')
    basename = os.path.splitext(card)[0]
    extension = '_out_significance_exp'
    if usedata: extension = '_out_significance_obs'
    name = basename + extension
    outtxtfile = name+'.txt'
    cwd = os.getcwd()
    defaultpoi = (len(pois)==1 and pois[0]=='r')

    commands = []
    commands.append('cd {}'.format(datacarddir))
    # define basic command
    sig_command = 'combine -M Significance '+workspace+' -n '+name+' --signif'
    sig_command += ' --cminDefaultMinimizerStrategy 0'
    # define options for blinding data
    toy_options = '-t -1'
    if defaultpoi: toy_options += ' --expectSignal=1'
    else:
	toy_options += ' --setParameters {}=1'.format(pois[0])
	for poi in pois[1:]: toy_options += ',{}=1'.format(poi)
    # set correct options
    if not usedata: sig_command += ' '+toy_options
    # add command
    commands.append( sig_command+' > '+outtxtfile+' 2> '+outtxtfile )
    commands.append('cd {}'.format(cwd))
    return commands
    
def get_fitdiagnostics_commands(datacarddir,card,workspace=None,
				dostatonly=False,usedata=False,
				pois=['r']):
    ### get the commands to calculate the signal strength with FitDiagnostics
    # input arguments:
    # - datacarddir: directory of the datacards
    # - card: filename of the datacard file in datacarddir
    # - workpace: name of the workspace (default: equal to card)
    # - dostatonly: boolean whether to do statistical uncertainty only analysis
    # - usedata: boolean whether to use data (default: blind analysis)
    # - pois: list of parameters of interest (default: one poi called 'r')
    # WARNING: DOES NOT (YET) WORK FOR MULTIPLE POIS!
    #          FITDIAGNOSTICS SEEMS TO BE INTRINSICALLY 1 POI ONLY
    #	       see get_multisignalstrength_commands below
    # output:
    # - list of commands that, when executed, will produce:
    #   -- txt file similar to card but with extension _out_fitdiagnostics_exp(_stat) 
    #      (for blind analysis) or _out_fitdiagnostics_obs(_stat) (for unblinded analysis)
    #   -- output root file named higgsCombine<datacard>_<extension>.FitDiagnostics.mH120.root
    #   -- output root file named fitDiagnostics<datacard>_<extension>.root

    if workspace is None: workspace = card.replace('.txt','.root')
    basename = os.path.splitext(card)[0]
    extension = '_out_fitdiagnostics_exp'
    if usedata: extension = '_out_fitdiagnostics_obs'
    if dostatonly: extension += '_stat'
    name = basename + extension
    outtxtfile = name+'.txt'
    cwd = os.getcwd()
    defaultpoi = (len(pois)==1 and pois[0]=='r')

    commands = []
    commands.append('cd {}'.format(datacarddir))
    # define basic command
    ss_command = 'combine -M FitDiagnostics '+workspace
    ss_command += ' -n '+name
    ss_command += ' --saveShapes --saveWithUncertainties'
    ss_command += ' --plots' # for covariance matrix
    ss_command += ' --cminDefaultMinimizerStrategy 0'
    #ss_command += ' --robustFit=1'
    if defaultpoi: ss_command += ' --rMin 0 --rMax 5'
    else: 
	ss_command += ' --redefineSignalPOIs {}'.format(pois[0])
	for poi in pois[1:]: ss_command += ',{}'.format(poi)
    # define options for stat only analysis
    stat_options = '--profilingMode=none'
    # define options for blinding data 
    toy_options = '-t -1'
    if defaultpoi: toy_options += ' --expectSignal=1'
    else:
	toy_options += ' --setParameters {}=1'.format(pois[0])
	for poi in pois[1:]: toy_options += ',{}=1'.format(poi)
    # set correct options
    if not usedata: ss_command += ' '+toy_options
    if dostatonly: ss_command += ' '+stat_options
    # add command
    commands.append( ss_command+' > '+outtxtfile+' 2> '+outtxtfile )
    commands.append('cd {}'.format(cwd))
    return commands

def get_multidimfit_commands(datacarddir, card, workspace=None,
					dostatonly=False, usedata=False,
					pois=['r'], nuisance=''):
    ### get the commands to calculate the signal strength for multiple POIs
    # input arguments:
    # - datacarddir: directory of the datacards
    # - card: filename of the datacard file in datacarddir
    # - workpace: name of the workspace (default: equal to card)
    # - dostatonly: boolean whether do statistical uncertainty only analysis
    # - usedata: boolean whether to use data (default: blind analysis)
    # - pois: list of parameters of interest (default: one poi called 'r')
    # - nuisance: name of single nuisance parameter to keep, freeze the rest after initial fit
    #             note: still experimental, not yet verified
    # output:
    # - list of commands that, when executed, will produce:
    #   -- txt file similar to card but with extension _out_multidimfit_exp(_stat) 
    #      (for blind analysis) or _out_multidimfit_obs(_stat) (for unblinded analysis)
    #   -- output root file named higgsCombine<datacard>_<extension>.MultiDimFit.mH120.root
    #   -- output root file named multidimfit<datacard>_<extension>.root

    if workspace is None: workspace = card.replace('.txt','.root')
    basename = os.path.splitext(card)[0]
    extension = '_out_multidimfit_exp'
    if usedata: extension = '_out_multidimfit_obs'
    if dostatonly: extension += '_stat'
    dosinglenuisance = False if (nuisance=='' or dostatonly) else True
    if dosinglenuisance: extension += '_'+nuisance
    name = basename + extension
    outtxtfile = name+'.txt'
    cwd = os.getcwd()
    defaultpoi = (len(pois)==1 and pois[0]=='r')

    commands = []
    commands.append('cd {}'.format(datacarddir))
    # define basic command
    ss_command = 'combine -M MultiDimFit '+workspace
    ss_command += ' -n '+name
    ss_command += ' --algo singles --saveWorkspace --saveFitResult'
    ss_command += ' --cminDefaultMinimizerStrategy 0'
    #ss_command += ' --robustFit=1'
    if defaultpoi: ss_command += ' --rMin 0 --rMax 5'
    else:
        ss_command += ' --redefineSignalPOIs {}'.format(pois[0])
        for poi in pois[1:]: ss_command += ',{}'.format(poi)
    # define options for stat only analysis
    stat_options = '--freezeParameters allConstrainedNuisances'
    singlenuisance_options = '--floatParameters {}'.format(nuisance)
    # define options for blinding data
    toy_options = '-t -1'
    if defaultpoi: toy_options += ' --expectSignal=1'
    else:
        toy_options += ' --setParameters {}=1'.format(pois[0])
        for poi in pois[1:]: toy_options += ',{}=1'.format(poi)
    # set correct options
    if not usedata: ss_command += ' '+toy_options
    if dostatonly or dosinglenuisance:
	# special case as need to run two commands in sequence
	init_command = ss_command.replace(' -n '+name,' -n '+name+'_initfit')
	commands.append( init_command )
	stat_command = ss_command.replace(workspace,
			'higgsCombine'+name+'_initfit.MultiDimFit.mH120.root',1)
	stat_command += ' --snapshotName "MultiDimFit"'
	stat_command += ' '+stat_options
	if dosinglenuisance: stat_command += ' '+singlenuisance_options
	commands.append( stat_command+' > '+outtxtfile+' 2> '+outtxtfile )
    else:	
	commands.append( ss_command+' > '+outtxtfile+' 2> '+outtxtfile )
    commands.append('cd {}'.format(cwd))
    return commands

def get_initimpacts_commands( datacarddir, card, workspace=None, 
				dostatonly=False, usedata=False,
				pois=['r'] ):
    # attempt to try a third approach next to MultiDimFit and FitDiagnostics,
    # consisting of the initial step of making impact plots
    
    if workspace is None: workspace = card.replace('.txt','.root')
    basename = os.path.splitext(card)[0]
    extension = '_out_initimpacts_exp'
    if usedata: extension = '_out_initimpacts_obs'
    if dostatonly: extension += '_stat'
    name = basename + extension
    outtxtfile = name+'.txt'
    cwd = os.getcwd()
    defaultpoi = (len(pois)==1 and pois[0]=='r')

    commands = []
    commands.append('cd {}'.format(datacarddir))
    # define basic command
    ss_command = 'combineTool.py -M Impacts --doInitialFit -d '+workspace
    ss_command += ' -n '+name
    ss_command += ' -m 120'
    ss_command += ' --saveWorkspace'
    ss_command += ' --cminDefaultMinimizerStrategy 0'
    #ss_command += ' --robustFit=1'
    if defaultpoi: ss_command += ' --rMin 0 --rMax 5'
    else:
        ss_command += ' --redefineSignalPOIs {}'.format(pois[0])
        for poi in pois[1:]: ss_command += ',{}'.format(poi)
    # define options for stat only analysis
    stat_options = '--freezeParameters allConstrainedNuisances'
    # define options for blinding data
    toy_options = '-t -1'
    if defaultpoi: toy_options += ' --expectSignal=1'
    else:
        toy_options += ' --setParameters {}=1'.format(pois[0])
        for poi in pois[1:]: toy_options += ',{}=1'.format(poi)
    # set correct options
    if not usedata: ss_command += ' '+toy_options
    if dostatonly:
        # special case as need to run two commands in sequence
        init_command = ss_command.replace(' -n '+name,' -n '+name+'_initfit')
        commands.append( init_command )
        stat_command = ss_command.replace(workspace,
                        'higgsCombine_initialFit_'+name+'_initfit.MultiDimFit.mH120.root',1)
        stat_command += ' --snapshotName "MultiDimFit"'
        stat_command += ' '+stat_options
        commands.append( stat_command+' > '+outtxtfile+' 2> '+outtxtfile )
    else:
        commands.append( ss_command+' > '+outtxtfile+' 2> '+outtxtfile )
    commands.append('cd {}'.format(cwd))
    return commands

def get_channelcompatibility_commands( datacarddir, card, workspace=None, 
					dostatonly=False, usedata=False,
					channel_options = '' ):
    ### get the commands for a channel compatibility check
    # input arguments:
    # - datacarddir: directory of the datacards
    # - card: filename of the datacard file in datacarddir
    # - workpace: name of the workspace (default: equal to card)
    # - dostatonly: boolean whether to do statistical uncertainty only analysis
    # - usedata: boolean whether to use data (default: blind analysis)
    # - channel_options: options to specifiy what channels to consider
    # output:
    # - list of commands that, when executed, will produce:
    #   -- txt file similar to card but with extension _out_channelcompat_exp(_stat) 
    #      (for blind analysis) or _out_channelcompat_obs(_stat) (for unblinded analysis)
    #   -- output root file named 
    #      higgsCombine<datacard>_<extension>.ChannelCompatibilityCheck.mH120.root
    
    if workspace is None: workspace = card.replace('.txt','.root')
    basename = os.path.splitext(card)[0]
    extension = '_out_channelcompat_exp'
    if usedata: extension = '_out_channelcompat_obs'
    if dostatonly: extension += '_stat'
    name = basename + extension
    outtxtfile = name+'.txt'
    cwd = os.getcwd()

    commands = []
    commands.append('cd {}'.format(datacarddir))
    # define basic command
    cc_command = 'combine -M ChannelCompatibilityCheck '+workspace+' -n '+name
    cc_command += ' '+channel_options
    # define options for stat only analysis
    stat_options = '--profilingMode=none'
    # define options for blinding data
    toy_options = '-t -1'
    toy_options += ' --expectSignal=1'
    # set correct options
    if not usedata: cc_command += ' '+toy_options
    if dostatonly: cc_command += ' '+stat_options
    # add command
    commands.append( cc_command+' > '+outtxtfile+' 2> '+outtxtfile )
    commands.append('cd {}'.format(cwd))
    return commands

def get_default_commands( datacarddir, card, method='multidimfit', includesignificance=False, 
			    includestatonly=False, includedata=False ):
    ### get all default commands for a simple, inclusive measurement with 1 signal
    # input arguments: see above
    # - method: 'multidimfit', 'fitdiagnostics' or 'initimpacts'
    # - includesignificance: boolean whether to include significance measurement
    #   additional to signal strength measurement
    # - includestatonly: boolean whether to calculate stat-only uncertainty 
    #   additional to total uncertainty
    # - includedata: boolean whether to use data additional to blind analysis
    commands = []
    for c in get_workspace_commands( datacarddir, card ): commands.append(c)
    # get commands for multidimfit
    if method=='multidimfit':
	for c in get_multidimfit_commands( datacarddir, card, usedata=False,
	    dostatonly=False): commands.append(c)
	if includestatonly:
	    for c in get_multidimfit_commands( datacarddir, card, usedata=False,
		dostatonly=True): commands.append(c)
	if includedata:
	    for c in get_multidimfit_commands( datacarddir, card, usedata=True,
                dostatonly=False): commands.append(c)
	if( includedata and includestatonly):
	    for c in get_multidimfit_commands( datacarddir, card, usedata=True,
                dostatonly=True): commands.append(c)
    # get commands for fitdiagnostics
    elif method=='fitdiagnostics':
	for c in get_fitdiagnostics_commands( datacarddir, card, dostatonly=False, 
	    usedata=False ): commands.append(c)
	if includestatonly:
            for c in get_fitdiagnostics_commands( datacarddir, card, usedata=False,
                dostatonly=True): commands.append(c)
        if includedata:
            for c in get_fitdiagnostics_commands( datacarddir, card, usedata=True,
                dostatonly=False): commands.append(c)
        if( includedata and includestatonly):
            for c in get_fitdiagnostics_commands( datacarddir, card, usedata=True,
                dostatonly=True): commands.append(c)
    elif method=='initimpacts':
	for c in get_initimpacts_commands( datacarddir, card, dostatonly=False, 
            usedata=False ): commands.append(c)
        if includestatonly:
            for c in get_initimpacts_commands( datacarddir, card, usedata=False,
                dostatonly=True): commands.append(c)
        if includedata:
            for c in get_initimpacts_commands( datacarddir, card, usedata=True,
                dostatonly=False): commands.append(c)
        if( includedata and includestatonly):
            for c in get_initimpacts_commands( datacarddir, card, usedata=True,
                dostatonly=True): commands.append(c)
    # get significance commands
    if includesignificance:
	for c in get_significance_commands( datacarddir, card, usedata=False ):
            commands.append(c)
	if includedata:
	    for c in get_significance_commands( datacarddir, card, usedata=True ):
		commands.append(c)
    return commands

def get_gof_commands( datacarddir, card, workspace=None, algo='saturated', ntoys=10 ):
    ### get commands to do a goodness-of-fit test
    # input arguments:
    # - datacarddir: directory of the datacards
    # - card: name of the datacard file in datacarddir (should not contain the full path)
    # - workspace: name of the workspace (default: equal to card)
    # - algo: see http://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/part3/
    #             commonstatsmethods/#goodness-of-fit-tests
    # - ntoys: number of toys to use
    # furthere processing and interpretation of output: see same link!

    # initializations
    name = card.replace('.txt','')
    if workspace is None: workspace = card.replace('.txt','.root')
    cwd = os.getcwd()
    
    # check algo
    if algo not in ['saturated','KS','AD']: 
	raise Exception('ERROR in get_gof_commands: algo {}'.format(algo)
			    +' not recognized...')

    commands = []
    commands.append('cd {}'.format(datacarddir))
    # run GoodnessOfFit using data
    gof_command = 'combine -M GoodnessOfFit '+workspace+' -n '+name+' --algo='+algo
    commands.append(gof_command)
    # run GoodnessOfFit using toys
    toy_options = '-t {}'.format(ntoys)
    if algo=='saturated': toy_options += ' --toysFreq'
    commands.append( gof_command+' '+toy_options )
    commands.append('cd {}'.format(cwd))
    return commands
