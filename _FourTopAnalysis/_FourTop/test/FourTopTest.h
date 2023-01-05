#include "../interface/FourTop.h"

class FourTopTest : public FourTop {
    private:
        /* data */
    public:
        FourTopTest(std::string outputName, std::vector<std::string>& argvString) : FourTop(outputName, argvString) {};
        ~FourTopTest();

        void TestBTagging();

        // btagging test helpers
};

