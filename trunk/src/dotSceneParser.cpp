#include "Precompiled.h"
#include "dotSceneParser.h"
#include "Root.h"

namespace Gibdd
{
    dotSceneParser::dotSceneParser()
    {

    }

    dotSceneParser::~dotSceneParser()
    {

    }

    bool dotSceneParser::ExtractPosition(std::string str, ObjectDescription& obj)
    {
        int pos_x = str.find("x=\"");
        int pos_y = str.find("y=\"");
        int pos_z = str.find("z=\"");

        if(pos_x == str.npos || pos_y == str.npos || pos_z == str.npos)
        {
            return false;
        }

        std::string str_x = str.substr(pos_x + 3);
        std::string str_y = str.substr(pos_y + 3);
        std::string str_z = str.substr(pos_z + 3);

        obj.pos_x = atof(str_x.substr(0, str_x.find("\"")).c_str());
        obj.pos_y = atof(str_y.substr(0, str_y.find("\"")).c_str());
        obj.pos_z = atof(str_z.substr(0, str_z.find("\"")).c_str());

        return true;
    }

    bool dotSceneParser::ExtractScale(std::string str, ObjectDescription& obj)
    {
        int pos_x = str.find("x=\"");
        int pos_y = str.find("y=\"");
        int pos_z = str.find("z=\"");

        if(pos_x == str.npos || pos_y == str.npos || pos_z == str.npos)
        {
            return false;
        }

        std::string str_x = str.substr(pos_x + 3);
        std::string str_y = str.substr(pos_y + 3);
        std::string str_z = str.substr(pos_z + 3);

        obj.scale_x = atof(str_x.substr(0, str_x.find("\"")).c_str());
        obj.scale_y = atof(str_y.substr(0, str_y.find("\"")).c_str());
        obj.scale_z = atof(str_z.substr(0, str_z.find("\"")).c_str());

        return true;
    }

    bool dotSceneParser::ExtractRotation(std::string str, ObjectDescription& obj)
    {
        int pos_x = str.find("qx=\"");
        int pos_y = str.find("qy=\"");
        int pos_z = str.find("qz=\"");
        int pos_w = str.find("qw=\"");

        if(pos_x == str.npos || pos_y == str.npos || pos_z == str.npos || pos_w == str.npos)
        {
            return false;
        }

        std::string str_x = str.substr(pos_x + 4);
        std::string str_y = str.substr(pos_y + 4);
        std::string str_z = str.substr(pos_z + 4);
        std::string str_w = str.substr(pos_w + 4);

        obj.rot_x = atof(str_x.substr(0, str_x.find("\"")).c_str());
        obj.rot_y = atof(str_y.substr(0, str_y.find("\"")).c_str());
        obj.rot_z = atof(str_z.substr(0, str_z.find("\"")).c_str());
        obj.rot_w = atof(str_w.substr(0, str_w.find("\"")).c_str());

        return true;
    }

    bool dotSceneParser::ExtractMaterial(std::string str, ObjectDescription& obj)
    {
        int name = str.find("name=\"");
        int mesh = str.find("meshFile=\"");
        int shadow = str.find("castShadows=\"");

        if(mesh == str.npos)
        {
            return false;
        }

        std::string str_name = str.substr(name + 6);
        std::string str_mesh = str.substr(mesh + 10);
        std::string str_shadow = str.substr(shadow + 13);

        obj.entry_name = str_name.substr(0, str_name.find("\""));
        obj.entry_size = obj.entry_name.size();

        obj.mesh_name = str_mesh.substr(0, str_mesh.find("\""));
        obj.mesh_size = obj.mesh_name.size();

        std::string shad = str_shadow.substr(0, str_shadow.find("\""));
        std::transform(shad.begin(), shad.end(), shad.begin(), std::tolower);

        if(shad == "true" || shad == "1" || shad == "yes")
            obj.shadows = 1;
        else
            obj.shadows = 0;

        return true;
    }

    bool dotSceneParser::CheckInputLine(const std::string& str)
    {
        if(str.find(BAD_WORD_1) != str.npos || str.find(BAD_WORD_2) != str.npos || str.find(BAD_WORD_3) != str.npos || str.find(BAD_WORD_4) != str.npos || str.find(BAD_WORD_5) != str.npos)
            return false;

        return true;
    }

    bool dotSceneParser::ParseFile(const std::string& file)
    {
        std::ifstream ifs(file.c_str());
        if(!ifs.is_open())
            return false;

        ObjectDescription obj;
        int value = 0;

        bool found = false;
        std::string str;
        while(!ifs.eof())
        {
            std::getline(ifs, str);
            if(str.empty())
                continue;

            if(!CheckInputLine(str))
                continue;

            int pos = str.find(FIND_FIRST);
            if(pos > 0)
            {
                ++value;

                std::getline(ifs, str);
                if(!ExtractPosition(str, obj))
                {
                    printf("Object with line index %i, without position data. Skipped\n\n", value);
                    continue;
                }

                std::getline(ifs, str);
                if(!ExtractRotation(str, obj))
                {
                    printf("Object with line index %i, without rotation data. Skipped\n\n", value);
                    continue;
                }

                std::getline(ifs, str);
                if(!ExtractScale(str, obj))
                {
                    printf("Object with line index %i, without scale data. Skipped\n\n", value);
                    continue;
                }

                std::getline(ifs, str);
                if(!ExtractMaterial(str, obj))
                {
                    printf("Object with line index %i, without material. Skipped\n\n", value);
                    continue;
                }

                ObjectVector.push_back(obj);
            }
        }

        if(ObjectVector.empty())
            return false;

        return true;
    }

    void dotSceneParser::LoadScene()
    {
        pSceneMgr = Root::GetInstance().GetSceneManager();
        ObjectDescription obj;
        Ogre::Vector3 objPos;
        Ogre::Quaternion objRot;
        Ogre::Vector3 objScl;
        bool shadows;
        std::vector<ObjectDescription>::iterator itr = ObjectVector.begin();
        for(; itr != ObjectVector.end(); ++itr)
        {
            obj = *itr;
            objPos = Ogre::Vector3(obj.pos_x, obj.pos_y, obj.pos_z);
            objRot = Ogre::Quaternion(obj.rot_w, obj.rot_x, obj.rot_y, obj.rot_z);
            objScl = Ogre::Vector3(obj.scale_x, obj.scale_y, obj.scale_z);
            if(obj.shadows == 1)
                shadows = true;
            else
                shadows = false;
            CreateObject(obj.entry_name, obj.mesh_name, objPos, objScl, objRot, shadows);
        }
    }

    void dotSceneParser::CreateObject(const std::string& name, const std::string& mesh, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& rotation, bool shadows)
    {
        Ogre::Entity* entity = pSceneMgr->createEntity(name, mesh);
        size_t pos = name.find("simaforezone");
        if (pos != std::string::npos)
            entity->setVisible(false);
        pos = name.find("start_point");
        if (pos != std::string::npos)
            entity->setVisible(false);

        entity->setCastShadows(shadows);

        Ogre::SceneNode* node;

        node = pSceneMgr->getRootSceneNode()->createChildSceneNode(name);
        node->setScale(scale);
        node->setOrientation(rotation);
        node->setPosition(position);
        node->attachObject(entity);
    }
}