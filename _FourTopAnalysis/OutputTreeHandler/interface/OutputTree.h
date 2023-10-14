#ifndef FOURTOP_OUTPUTTREE_H
#define FOURTOP_OUTPUTTREE_H

// C++ includes
#include <vector>
#include <string>

// root includes
#include <TTree.h>

// base class for output management
// contains methods such as initialize, fill, ...
// also should handle metadata in outputfile
// and analysis level data such as which variations are saved, but that should be done on a global file level, not in subsequent ttree
// or is it too much to let one class handle the output tree and the metadata?
class EventFourT;

class OutputTree {
    private:
        // vector with uncertainties stored?
        // could be split in weight variations and event variations
        // do we just build different trees for variations? In the end that's kinda what happens?
        // but that requires storage of unrelated variables...
        // maybe branch per variation so we only store variations for things that actually vary? 
        // on the other hand, could just have branch per variable, for each variation we'd have to fill it though... 
        // in the end that makes the structure more complex, wayyy more branches! So no? 
        // But the other approach brings a whole load of branches into calculation while the first approach might not be aproblem due to compressions
        // also base class identifier map -> 0 = SR-2L, 1 = SR-3L, ...

        TFile* outputfile;
        std::shared_ptr<TTree> tree;
        // 

        // base tree structure, this should contain all helper functions, derived classes for specific content, first one just having all variables, then one for syst weight changes which is the nominal one, one for syst var changes which only has nom weight
        // entry per event, should be straight forward enough
    public:
        OutputTree(TFile* outputfile, std::string& treeName);
        ~OutputTree();

        std::shared_ptr<TTree> GetTree() {return tree;}
        virtual void FillTree(EventFourT*, double) = 0;

        void AddEntry();
        void SaveToFile(TFile* file);
};

// open question: multiple processes per sample -> based on fr, etcetc


#endif