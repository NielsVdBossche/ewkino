#include "histogramSetup.h"
#include <math.h>

#if MEMLEAK
#include "../../memleak/debug_new.h" 
#endif

std::vector<HistInfo>* fourTopHists::testHists(eventClass evClass) {
    std::vector< HistInfo >* histInfoVec = new std::vector<HistInfo>;

    std::map<eventClass, std::string> flagMapping = {
            {fail, "fail"},
            {crwz, "CRWZ"},
            {crzz, "CRZZ"},
            {crz3L, "CRZ"},
            {crz4L, "CRZ-4L"},
            {cro, "CRO"},
            {cro3L, "CRO-3L"},
            {crw, "CRW"},
            {ssdl, "DL"},
            {trilep, "3L"},
            {fourlep, "4L"}
    };
    std::string flag = flagMapping[evClass];

    *histInfoVec = {
        HistInfo("NLeps_After_TightCharge" + flag, "N_{L}", 6, -0.5, 5.5),
    };

    return histInfoVec;
}
