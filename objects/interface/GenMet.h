#ifndef GenMet_H
#define GenMet_H

//include other parts of framework
#include "PhysicsObject.h"

class TreeReader;
class NanoReader;

class GenMet : public PhysicsObject {

    public:
        GenMet( const TreeReader& );
        GenMet( const NanoReader& );

    private:

};


#endif
