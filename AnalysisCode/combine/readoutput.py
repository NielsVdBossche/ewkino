######################################################################
# script to read output text files generated by the combine commands #
######################################################################
import os
import sys
import math

def readr(filename):
    ### read signal strength (r) from the output of a combine command directed to a file
    r = 0.
    uperror = 0.
    downerror = 0.
    with open(filename,'r') as f:
	for l in f.readlines():
	    if l[:11]=='Best fit r:':
		l = l.replace('Best fit r: ','').replace(' (68% CL)','')
		l = l.split(' ')
		r = l[0]
		uperror = l[2].split('/')[1].strip('+')
		downerror = l[2].split('/')[0].strip('-')
    return (float(r),float(uperror),float(downerror))

def readsigma(filename):
    ### read significance (sigma) from the output of a combine command directed to a file
    s = 0.
    with open(filename,'r') as f:
	for l in f.readlines():
	    if l[:13]=='Significance:':
		s = float(l.replace('Significance:','').strip(' '))
    return s

def formatline(title,strength=0,uperror=0,downerror=0,uperror_stat=0,downerror_stat=0,significance=0):
    titlelen = 25
    numlen = 8
    res = str('{:<'+str(titlelen)+'}').format(title)
    res += ':'
    if significance!=0:
	res += str('s = {:<'+str(numlen)+'}').format(str('{:.'+str(numlen-3)+'f}').format(significance))
    if strength!=0:
	res += str('r = {:<'+str(numlen)+'}').format(str('{:.'+str(numlen-3)+'f}').format(strength))
	res += '+'
	res += str('{:<'+str(numlen)+'}').format(str('{:.'+str(numlen-3)+'f}').format(uperror))
	if uperror_stat > 0:
	    res += '(stat: '
	    res += str('{:<'+str(numlen)+'}').format(str('{:.'+str(numlen-3)+'f}').format(uperror_stat))
	    res += ') '
	res += '-'
	res += str('{:<'+str(numlen)+'}').format(str('{:.'+str(numlen-3)+'f}').format(downerror))
	if downerror_stat > 0:
            res += '(stat: '
            res += str('{:<'+str(numlen)+'}').format(str('{:.'+str(numlen-3)+'f}').format(downerror_stat))
            res += ') '
    res += '\n'
    res += '-'*(titlelen+3*numlen)+'\n'
    return res

def formatlatextableentries(reslist,kind):
    tableentries = {}
    for res in reslist:
	key = res['card']
	value = ''
	if kind=='significance':
	    value = '{:.2f}'.format(res['s'])
	if kind=='signalstrength':
	    uperror = res['uperror']
	    upstat = res['uperror_stat']
	    upsys = math.sqrt(uperror**2-upstat**2)
	    downerror = res['downerror']
	    downstat = res['downerror_stat']
	    downsys = math.sqrt(downerror**2-downstat**2)
	    if upstat < 1e-12 or downstat < 1e-12:
		value = '${:.2f}_{{-{:.2f} }}^{{+{:.2f} }}$'.format(res['r'],res['downerror'],res['uperror'])
	    else:
		value = '${:.2f}_{{-{:.2f} ({:.2f} / {:.2f}) }}'
		value += '^{{+{:.2f} ({:.2f} / {:.2f}) }}$'
		value = value.format(res['r'],downerror,downsys,downstat,uperror,upsys,upstat)
	tableentries[key] = value
    return tableentries

def get(adict,akey):
    if akey in adict.keys(): return adict[akey]
    else: return '0'

def formatlatextable(resdict_obs, resdict_exp):
    t = r'\begin{table}[h]'+'\n'
    t += '\t'+r'\begin{center}'+'\n'
    t += '\t\t'+r'\caption{}'+'\n'
    t += '\t\t'+r'\label{}'+'\n'
    t += '\t\t'+r'\begin{tabular}{|l|c|c|c|}'+'\n'
    t += '\t\t\t'+r'\hline'+'\n'
    t += '\t\t\t'+r'channel & 2016 & 2017 & 2018 \\'+'\n'
    t += '\t\t\t'+r'\hline \hline'+'\n'
    t += '\t\t\t'+r'1 b-jet, 2-3 jets & '+'{} ({}) & {} ({}) & {} ({}) '.format(
		    get(resdict_obs,'signalregion_1_2016'),get(resdict_exp,'signalregion_1_2016'),
		    get(resdict_obs,'signalregion_1_2017'),get(resdict_exp,'signalregion_1_2017'),
		    get(resdict_obs,'signalregion_1_2018'),get(resdict_exp,'signalregion_1_2018')
	 )+r'\\'+'\n'
    t += '\t\t\t'+r'\hline'+'\n'
    t += '\t\t\t'+r'1 b-jet, $\geq$ 4 jets & '+'{} ({}) & {} ({}) & {} ({}) '.format(
                    get(resdict_obs,'signalregion_2_2016'),get(resdict_exp,'signalregion_2_2016'),
                    get(resdict_obs,'signalregion_2_2017'),get(resdict_exp,'signalregion_2_2017'),
                    get(resdict_obs,'signalregion_2_2018'),get(resdict_exp,'signalregion_2_2018')
         )+r'\\'+'\n'
    t += '\t\t\t'+r'\hline'+'\n'
    t += '\t\t\t'+r'$\geq$ 2 b-jets & '+'{} ({}) & {} ({}) & {} ({}) '.format(
                    get(resdict_obs,'signalregion_3_2016'),get(resdict_exp,'signalregion_3_2016'),
                    get(resdict_obs,'signalregion_3_2017'),get(resdict_exp,'signalregion_3_2017'),
                    get(resdict_obs,'signalregion_3_2018'),get(resdict_exp,'signalregion_3_2018')
         )+r'\\'+'\n'
    t += '\t\t\t'+r'\hline'+'\n'
    t += '\t\t\t'+r'combination (+ control regions) & '+'{} ({}) & {} ({}) & {} ({}) '.format(
                    get(resdict_obs,'_2016'),get(resdict_exp,'_2016'),
                    get(resdict_obs,'_2017'),get(resdict_exp,'_2017'),
                    get(resdict_obs,'_2018'),get(resdict_exp,'_2018')
         )+r'\\'+'\n'
    t += '\t\t\t'+r'\hline'+'\n'
    t += '\t\t\t'+r'total combination (+ control regions) & \multicolumn{3}{c|}{'+'{} ({})'.format(
		    get(resdict_obs,'_all'),get(resdict_exp,'_all')
	 )+r'} \\'+'\n'
    t += '\t\t\t'+r'\hline'+'\n'
    t += '\t\t'+r'\end{tabular}'+'\n'
    t += '\t'+r'\end{center}'+'\n'
    t += r'\end{table}'
    return t

if __name__=='__main__':

    datacarddir = ''
    if len(sys.argv)==2:
	datacarddir = sys.argv[1]
    else:
	print('### ERROR ###: need one command line argument.')
	print('               normal use: python readoutput.py <datacard directory>')

    tags = ['significance_exp','significance_obs','signalstrength_exp','signalstrength_obs']
    resdict = {}
    for tag in tags:
	outputfiles = sorted([f for f in os.listdir(datacarddir) if '_out_'+tag+'.txt' in f])
	print('---------------------------------------------')
	print('--- '+tag+' ---')
	print('---------------------------------------------')
	reslist = []
	for f in outputfiles:
	    card = f.replace('datacard_','').replace('dc_combined','').replace('_out_'+tag+'.txt','')
	    # read significance
	    if 'significance' in tag:
		s = readsigma(os.path.join(datacarddir,f))
                reslist.append({'card':card,'r':0,'uperror':0,'downerror':0,'s':s,
				    'uperror_stat':0,'downerror_stat':0})
	    # read signal strength
	    if 'signalstrength' in tag:
		(r,uperror,downerror) = readr(os.path.join(datacarddir,f))
		reslist.append({'card':card,'r':r,'uperror':uperror,'downerror':downerror,'s':0,
				    'uperror_stat':0,'downerror_stat':0})
		# check if stat only file exists and if so, read it as well
		fstat = f.replace('.txt','_stat.txt')
		if os.path.exists(os.path.join(datacarddir,fstat)):
		    (rstat,uperror,downerror) = readr(os.path.join(datacarddir,fstat))
		    if abs(r-rstat) > 1e-12:
			print('### WARNING ###: best fit signal strength does not agree.')
		    else:
			reslist[-1]['uperror_stat'] = uperror
			reslist[-1]['downerror_stat'] = downerror
	for res in reslist:
	    print(formatline(res['card'],strength=res['r'],
			    uperror=res['uperror'],downerror=res['downerror'],
			    uperror_stat=res['uperror_stat'],downerror_stat=res['downerror_stat'],
			    significance=res['s']))
	resdict[tag] = formatlatextableentries(reslist,tag.split('_')[0])
    print(formatlatextable(resdict['significance_obs'],resdict['significance_exp']))
    print(formatlatextable(resdict['signalstrength_obs'],resdict['signalstrength_exp']))
