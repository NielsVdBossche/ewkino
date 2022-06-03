#ifndef globalsettings_h
#define globalsettings_h

#define MEMLEAK false

// WP's of MVAs,...
enum eventClass {
    fail,
    // dy, // new one whieeee
    crwz,
    cr_conv,
    crz3L,
    crz4L,
    cro,
    cro3L,
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

#endif