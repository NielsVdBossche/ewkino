# Four top analysis
This framework expands on the ewkino framework to incorporate the four top analysis. Some important changes were made to the ewkino framework so don't expect too much compatibility in other analyses using the ewkino framework.

## Recipe
First, build the framework using:
```console
make -f makeCodeLibrary
```

in the base ewkino directory.

To build the analysiscode:

```console
cd _FourTopAnalysis
make -f makeBaseAnalysis
```

Make sure a directory called Output is present in _FourTopAnalysis.

To run the code from a m-machine, while still in _FourTopAnalysis directory, run:

```console
./analysisExec ../sampleLists/allSamples.txt <AnalysisMethod>
```
Several options are available. When requiring only MC results, use: MCAll

To run the code on HTCondor:

```console
cd runCode
condor_submit <.sub file>
```
A series of options is available for submission.

New option: using the submission script.

As long as my skims are available on T2B, it should run fine.
