#include <string>
#include <vector>
#include <TTree.h>

class SyncTree {
    public:
        SyncTree(std::string& pathToFile);

        ULong_t _eventNumber;
        std::vector<bool> electrons_is_loose;
        std::vector<bool> electrons_is_fakeable;
        std::vector<bool> electrons_is_tight;
        std::vector<bool> muons_is_loose;
        std::vector<bool> muons_is_fakeable;
        std::vector<bool> muons_is_tight;

        TTree* GetTree() {return tree;}
        
    private:
        TTree* tree;

        TBranch* b__eventNumber;
        TBranch* b__muons_is_loose;
        TBranch* b__muons_is_fakeable;
        TBranch* b__muons_is_tight;
        TBranch* b__electrons_is_loose;
        TBranch* b__electrons_is_fakeable;
        TBranch* b__electrons_is_tight;
};