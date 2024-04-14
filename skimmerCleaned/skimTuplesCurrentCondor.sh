# skimcommand [Sourcefolder] [tag of production, ie subfolder of sample name] [outout targetfolder] [skimcondition] [nFiles per job] [selection on sample name]

python skimTuplesCondor.py /pnfs/iihe/cms/store/user/nivanden/heavyNeutrino/ tttt_eft_v1 /pnfs/iihe/cms/store/user/nivanden/skims_v7/rawSkims/ lightDileptonSkim 20 TTTT_EFT_
# python skimTuplesCondor.py /pnfs/iihe/cms/store/user/nivanden/heavyNeutrinoGenPart/ MC_2016_ULpreVFP /pnfs/iihe/cms/store/user/nivanden/skims_v6/rawSkims/ lightDileptonSkim 150 WZJJ_EWK
# python skimTuplesCondor.py /pnfs/iihe/cms/store/user/nivanden/heavyNeutrinoGenPart/ MC_2017 /pnfs/iihe/cms/store/user/nivanden/skims_v6/rawSkims/ lightDileptonSkim 150 WZJJ_EWK
# python skimTuplesCondor.py /pnfs/iihe/cms/store/user/nivanden/heavyNeutrinoGenPart/ MC_2018 /pnfs/iihe/cms/store/user/nivanden/skims_v6/rawSkims/ lightDileptonSkim 150 WZJJ_EWK