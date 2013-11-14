#include "Precompiled.h"
#include "LevelLoader.h"
#include "Level.h"
#include "dotSceneParser.h"

#pragma warning(disable: 4996)

INSTANTIATE_SINGLETON_1(Gibdd::LevelLoader);

namespace Gibdd
{
    uint32 LevelLoader::iMapVersion = 1;

    LevelLoader::LevelLoader()
    {

    }

    LevelLoader::~LevelLoader()
    {
    }

    Level* LevelLoader::LoadLevel(const std::string& filename)
    {
        FILE* file = fopen(filename.c_str(), "rb");
        if(!file)
        {
            return 0;
        }

        uint32 ver;
        fread(&ver, 4, 1, file);

        if(ver != iMapVersion)
        {
            return 0;
        }

        Level* pLevel = new Level(filename);

        uint32 objectCount = 0;
        fread(&objectCount, 4, 1, file);
        fseek(file, 0x12, 0);

        std::string mesh_name;
        std::string entity_name;
        uint32 size;

        int shadows;
        float rw, rx, ry, rz;
        float x, y, z;
        float sx, sy, sz;

        for(uint32 i = 0; i < objectCount; ++i)
        {
            fread(&size, 4, 1, file);
            char* entity = new char[size];
            fread(entity, size, 1, file);
            {
                entity_name = entity;
                entity_name.resize(size);

                delete [] entity;
            }

            fread(&size, 4, 1, file);
            char* mesh = new char[size];
            fread(mesh, size, 1, file);
            {
                mesh_name = mesh;
                mesh_name.resize(size);

                delete [] mesh;
            }

            fread(&x, 4, 1, file);
            fread(&y, 4, 1, file);
            fread(&z, 4, 1, file);

            fread(&rx, 4, 1, file);
            fread(&ry, 4, 1, file);
            fread(&rz, 4, 1, file);
            fread(&rw, 4, 1, file);

            fread(&sx, 4, 1, file);
            fread(&sy, 4, 1, file);
            fread(&sz, 4, 1, file);

            fread(&shadows, 4, 1, file);
            pLevel->CreateObject(entity_name, mesh_name, Ogre::Vector3(x, y, z), Ogre::Vector3(sx, sy, sz), Ogre::Quaternion(rw, rx, ry, rz), shadows >= 1 ? true:false);
        }
        fclose(file);

        //pLevel->BuildLevel();
        return pLevel;
    }

    void LevelLoader::LoadLevelFromdotScene(const std::string& filename)
    {
       dotSceneParser *dotScene = new dotSceneParser();
       if (dotScene->ParseFile(filename))
           dotScene->LoadScene();
       delete dotScene;
    }
}