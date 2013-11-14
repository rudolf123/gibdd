#ifndef DBCSTORES_H
#define DBCSTORES_H

#include "DBCStore.h"
#include "DBCStructure.h"

namespace Gibdd
{
    extern DBCStorage<LevelEntry> sLevelStore;

    void LoadDBCStores(const std::string& dataPath);
}

#endif
