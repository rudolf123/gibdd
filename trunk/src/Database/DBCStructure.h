#ifndef DBCSTRUCTURE_H
#define DBCSTRUCTURE_H

#include "DBCEnums.h"

namespace Gibdd
{
    struct LevelEntry
    {
        char* MeshName;
        float position[3];
        float orientation;

        float WorldRulesTriggers[20];
    };
}

#endif
