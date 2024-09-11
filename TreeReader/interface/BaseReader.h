#ifndef BASEREADER_H
#define BASEREADER_H

//include ROOT classes
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TLorentzVector.h"

//include other parts of code
#include "../../Tools/interface/Sample.h"

// Class: BaseReader
// Contains basic functionality that can be shared between mini and nano AOD.
bool treeHasBranchWithName( TTree* treePtr, const std::string& nameToFind );
std::pair<std::map<std::string, bool>, std::map<std::string, TBranch*> > buildBranchMap( TTree* treePtr, 
                              const std::vector<std::string> nameIdentifiers, const std::string& antiIdentifier="");
template <typename T>
void setMapBranchAddresses(TTree* treePtr, std::map<std::string, T>& variableMap, std::map<std::string, TBranch*> branchMap);
template <typename T>
void setMapBranchAddressesWithNameMap(TTree* treePtr, std::map<std::string, T>& variableMap, std::map<std::string, TBranch*> branchMap, std::map<std::string, std::string> nameMap);

class Event;

class BaseReader {
    public:
        BaseReader() = default;
        BaseReader(const std::string&, const std::string&);
        ~BaseReader() = default;

        // SAMPLES:
        // Read samples from a list and a directory
        void readSamples2016PreVFP(const std::string&, const std::string&);
        void readSamples2016PostVFP(const std::string&, const std::string&);
        void readSamples2016(const std::string&, const std::string&);
        void readSamples2017(const std::string&, const std::string&);
        void readSamples2018(const std::string&, const std::string&);
        void readSamples(const std::string& list, const std::string& directory);

        // Initializing sample:
        void initSample();
        void initSample(const Sample&);

        // Initialization from root file of sample:
        void initSampleFromFile( const std::string& pathToFile,
                                const bool is2016, 
                                const bool is2016PreVFP, 
                                const bool is2016PostVFP,
                                const bool is2017, const bool is2018, 
                                const bool resetTriggersAndFilters = true );
        void initSampleFromFile( const std::string& pathToFile, 
                                const bool resetTriggersAndFilters = true );

        // Helpers for samples
        const Sample& currentSample() const{ return *_currentSamplePtr; }
        const Sample* currentSamplePtr() const{ return _currentSamplePtr.get(); }
        std::vector< Sample >::size_type numberOfSamples() const{ return samples.size(); }
        std::vector< Sample > sampleVector() const{ return samples; }
        void removeBSMSignalSamples();
        void keepOnlySignalsWithName( const std::string& );

        // Access current file and tree 
        std::shared_ptr< TFile > currentFilePtr(){ return _currentFilePtr; }

        // Get things from current file
        TObject* getFromCurrentFile( const std::string& name ) const;
        std::vector< std::shared_ptr< TH1 > > getHistogramsFromCurrentFile() const;

        // Helpers that should be overridden in derived classes:
        virtual bool containsGeneratorInfo() const = 0;
        virtual bool containsLheInfo() const = 0;
        virtual bool containsEFTInfo() const = 0;
        bool hasPL() const { return _hasPLInfo;};
        bool hasEFT() const {return _hasEFTInfo;};
        bool hasGenLvl() const {return _hasGenLevelInfo;};

        // Getters and setters for configuration booleans
        bool getReadSourcesJECVariations() const { return _readSourcesJECVariations; }
        bool getReadGroupedJECVariations() const { return _readGroupedJECVariations; }
        void setReadSourcesJECVariations( const bool readSourcesJECVariations ) { _readSourcesJECVariations = readSourcesJECVariations; }
        void setReadGroupedJECVariations( const bool readGroupedJECVariations ) { _readGroupedJECVariations = readGroupedJECVariations; }

        // Generic helpers:
        bool isData() const;
        bool isMC() const;
        bool isSMSignal() const;
        bool isNewPhysicsSignal() const;
        bool isSusy() const{ return _isSusy; }

        bool is2016() const;
        bool is2016PreVFP() const;
        bool is2016PostVFP() const;
        bool is2017() const;
        bool is2018() const;

        double getIntLumi() const;
        unsigned long numberOfEntries() const;
    
        // Tree methods:
        virtual void initTree(const bool resetTriggersAndFilters = true) = 0;
        virtual void setOutputTree( TTree* ) = 0;
        TTree* GetTree() {return _currentTreePtr;}

        //Get entry from Tree, should not be used except for test purposes
        void GetEntry(const Sample&, long unsigned );
        void GetEntry(long unsigned );

        // 1 entry should be shared between all classes: the scaled weight:
        double _scaledWeight;

        // Event building: should be overriddden to ensure correct call to Event constructor
        virtual Event buildEvent( const Sample&, long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false,
                const bool readGroupedJECVariations = false ) = 0;

        virtual Event* buildEventPtr( const Sample&, long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false,
                const bool readGroupedJECVariations = false ) = 0;

        virtual Event buildEvent( long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false, 
                const bool readGroupedJECVariations = false ) = 0;

        virtual Event* buildEventPtr( long unsigned, 
                const bool readIndividualTriggers = false, 
                const bool readIndividualMetFilters = false,
                const bool readAllJECVariations = false, 
                const bool readGroupedJECVariations = false ) = 0;
    protected:
        //list of samples to loop over 
        std::vector<Sample> samples;
        std::vector<Sample> samples2016PreVFP;
        std::vector<Sample> samples2016PostVFP;
        std::vector<Sample> samples2016;
        std::vector<Sample> samples2017;
        std::vector<Sample> samples2018;

        //current sample
        std::shared_ptr< const Sample > _currentSamplePtr;

        //TFile associated to current sample
        std::shared_ptr< TFile > _currentFilePtr;

        //TTree associated to current sample 
        TTree* _currentTreePtr = nullptr;

        //cache whether current sample is SUSY to avoid having to check the branch names for each event
        bool _isSusy = false;
        // same for PL info
        bool _hasPLInfo = false;
        bool _hasGenLevelInfo = false;
        bool _hasEFTInfo = false;

        // Booleans for configuration details:
        bool _readGroupedJECVariations = false;
        bool _readSourcesJECVariations = false;

        //current index in samples vector
        int currentSampleIndex = -1;

        //luminosity scaling
        double scale = 0;

        // Methods
        // Check whether current sample/tree/file is initialized, throw an error if it is not 
        void checkCurrentSample() const;
        void checkCurrentTree() const;
        void checkCurrentFile() const;

        //some safety-checks for errors 
        void checkSampleEraConsistency() const; //make sure a sample is not is2016() AND 2017() 
        void checkEraOrthogonality() const; //make sure no sample from the wrong era is being used (i.e. no 2016 sample in the list of 2017 samples) 

        //general function to read a list of samples
        void readSamples(const std::string&, const std::string&, std::vector<Sample>&);

        // Functions to be overridden
        virtual double getSumSimulatedEventWeights() = 0;
        virtual TTree* getTreePtr() = 0;
        virtual double getWeight() = 0;
    private:
};



#endif