#ifndef fourtop_h
#define fourtop_h

#include "../../../Tools/interface/HistInfo.h"
#include "../../../TreeReader/interface/TreeReader.h"

#include "../histogramSetups/histogramSetup.h"

class FourTop {
    private:
        std::vector<HistInfo>* histInfoVec_Global; // Histograms shared between DL and ML (if any?)
        std::vector<HistInfo>* histInfoVec_DL; // Histogram for DL kinematics
        std::vector<HistInfo>* histInfoVec_ML; // Histogram for ML kinematics


        TreeReader* treeReader;
        std::vector<Sample>* sampleVec;

        // General settings for analysis run

        
    public:
        // Loading settings for analysis, preparing trees, ...
        FourTop(std::vector< std::string > argvString);
        ~FourTop();

        // Prepare 
        void createHistInfoVec();
        
        // Main loop function
        void analyze();
};


#endif
