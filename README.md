# Four top analysis
This framework expands on the ewkino framework to incorporate the four top analysis. Some important changes were made to the ewkino framework so don't expect too much compatibility in other analyses using the ewkino framework.

## Recipe
Recommended to use a EL7 machine on T2B to run the analysis

First, build the framework using:
make -f makeCodeLibrary

in the base directory.

To build the analysiscode:
cd _FourTopAnalysis
make -f makeBaseAnalysis

Make sure a directory called Output is present in _FourTopAnalysis.

To run the code from a m-machine, while still in _FourTopAnalysis directory, run:

./analysisExec ../sampleLists/allSamples.txt 

To run the code on HTCondor:

cd runCode
condor_submit analysis.sub

As long as my skims are available on T2B, it should run fine.
