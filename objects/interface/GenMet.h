#ifndef GenMet_H
#define GenMet_H

// include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "PhysicsObject.h"

class TreeReader;

class GenMet : public PhysicsObject {

    public:
        GenMet( const TreeReader& );

    private:

};


#endif
