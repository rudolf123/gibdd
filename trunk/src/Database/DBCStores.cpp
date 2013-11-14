#include "Precompiled.h"
#include "DBCStores.h"
#include "DBCfmt.h"

#pragma warning(disable: 4996)

namespace Gibdd
{
    DBCStorage<LevelEntry> sLevelStore(Levelfmt);

    template<class T>
    inline void LoadDBC(DBCStorage<T>& storage, const std::string& dbc_path, const std::string& filename)
    {
        std::string dbc_filename = dbc_path + filename;
        if(storage.Load(dbc_filename.c_str()))
            storage.LoadStringsFrom(dbc_filename.c_str());
        else
        {
            FILE * f = fopen(dbc_filename.c_str(), "rb");
            if(f)
            {
                //Ogre::LogManager::getSingleton().logMessage(dbc_filename + " Wrong client version DBC file?");
                fclose(f);
            }
            //else
            //    Ogre::LogManager::getSingleton().logMessage(dbc_filename + " Cant find DBC file");
        }
    }

    void LoadDBCStores(const std::string& dataPath)
    {
        LoadDBC(sLevelStore, dataPath, "Level_0.dbc");
    }
}