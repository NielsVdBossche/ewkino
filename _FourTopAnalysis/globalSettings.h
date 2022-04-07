#ifndef globalsettings_h
#define globalsettings_h

#define MEMLEAK false

// WP's of MVAs,...
enum eventClass {
    fail,
    crwInvHT,
    crwOSLeps,
    crzInvHT,
    crzNoB,
    crzInvBAndJets,
    crLowB,
    crwLowHT,
    cro3L,
    cr_1b,
    crz,
    cro,
    crw,
    ssdl,
    trilep,
    fourlep
};

enum selectionType {
    MCAll,
    MCPrompt,
    MCNoChargeMisID,
    MCNoNP,
    ChargeMisDD,
    NPDD,
    Data
};

// others
#define USE_SYSTEMATICS false
#define NONPROMPT_ESTIMATION false

#endif