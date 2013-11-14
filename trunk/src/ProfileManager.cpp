#include "Precompiled.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "tinyxml/tinyxml.h"

namespace Gibdd
{
    ProfileManager::ProfileManager()
    {

    }

    ProfileManager::~ProfileManager()
    {
        stdext::hash_map<std::string, Profile*>::iterator itr;
        for (itr = mProfileList.begin(); itr != mProfileList.end(); ++itr)
        {
            LOGS(itr->second->GetName());
            delete itr->second;
        }
    }

    Profile* ProfileManager::CreateProfile()
    {
        Profile *prof = new Profile();

        return prof;
    }

    void ProfileManager::DestroyProfile(Profile* prof)
    {
        if (prof)
        {
            delete prof;
            prof = 0;
        }
    }

    Profile* ProfileManager::addProfile(std::string name)
    {
        stdext::hash_map<std::string, Profile*>::iterator itr = mProfileList.find(name);
        if(itr != mProfileList.end())
            return 0;

        Profile *prof = new Profile();
        prof->SetName(name);
        mProfileList[name] = prof;

        return prof;
    }

    bool ProfileManager::addProfile(Profile *prof)
    {
        stdext::hash_map<std::string, Profile*>::iterator itr = mProfileList.find(prof->GetName());
        if(itr != mProfileList.end())
            return false;

        mProfileList[prof->GetName()] = prof;

        return true;
    }

    void ProfileManager::removeProfile(std::string name)
    {
        if(mProfileList.empty())
            return;

        stdext::hash_map<std::string, Profile*>::iterator itr = mProfileList.find(name);
        if(itr == mProfileList.end())
            return;

        Profile* prof = itr->second;
        delete prof;

        mProfileList.erase(itr);
    }

    bool ProfileManager::HasProfile(std::string name)
    {
        stdext::hash_map<std::string, Profile*>::iterator itr = mProfileList.find(name);
        if(itr == mProfileList.end())
            return false;

        return true;
    }

    bool ProfileManager::loadProfiles(std::string filename)
    {
        stdext::hash_map<std::string, Profile*>::iterator itr;
        for (itr = mProfileList.begin(); itr != mProfileList.end(); ++itr)
        {
            delete itr->second;
        }

        mProfileList.clear();

        TiXmlDocument xml_file(filename);
        if(!xml_file.LoadFile())
            return false;

        TiXmlElement *xml_profiles = 0;
        xml_profiles = xml_file.FirstChildElement("Profiles");
        TiXmlElement *xml_profile = 0;
        xml_profile = xml_profiles->FirstChildElement("Profile");
        while (xml_profile != 0)
        {
            Profile *prof = addProfile(xml_profile->Attribute("name"));
            if (prof)
            {
                TiXmlElement *xml_action = 0;
                xml_action = xml_profile->FirstChildElement("Action");
                while (xml_action!=0)
                {
                    if ((OIS::Type)atoi(xml_action->Attribute("inputtype")) == OIS::OISKeyboard)
                    {
                        prof->bindAction((GameplayActions)atoi(xml_action->Attribute("ID")), (OIS::KeyCode)atoi(xml_action->Attribute("key")));
                    }
                    else
                    {
                        prof->bindAction((GameplayActions)atoi(xml_action->Attribute("ID")),
                            atoi(xml_action->Attribute("deviceID")),
                            (OIS::ComponentType)atoi(xml_action->Attribute("compType")),
                            atoi(xml_action->Attribute("val")), atoi(xml_action->Attribute("direction")));
                    }

                    xml_action = xml_action->NextSiblingElement("Action");
                }
            }

            xml_profile = xml_profile->NextSiblingElement("Profile");
        }

        return true;
    }

    bool ProfileManager::saveProfiles(std::string filename)
    {
        TiXmlDocument xml_file(filename);
        TiXmlDeclaration decl( "1.0", "", "" );
        TiXmlElement xml_profiles( "Profiles" );
        stdext::hash_map<std::string, Profile*>::iterator itr;
        for (itr = mProfileList.begin(); itr != mProfileList.end(); ++itr)
        {
            Profile* prof = itr->second;
            TiXmlElement xml_profile( "Profile" );
            xml_profile.SetAttribute("name", prof->GetName());
            for (uint16 i = 0; i < ACTIONS_NUM; ++i)
            {
                TiXmlElement xml_action( "Action" );
                xml_action.SetAttribute( "ID", TO_STRING(i) );
                xml_action.SetAttribute( "inputtype", TO_STRING((int)prof->GetInputMap()[(GameplayActions)i].inputType));
                if (prof->GetInputMap()[(GameplayActions)i].inputType == OIS::OISKeyboard)
                {
                    xml_action.SetAttribute( "key", TO_STRING((int)prof->GetInputMap()[(GameplayActions)i].key));
                    xml_profile.InsertEndChild(xml_action);

                    continue;
                }
                xml_action.SetAttribute( "deviceID", TO_STRING((int)prof->GetInputMap()[(GameplayActions)i].deviceID));
                xml_action.SetAttribute( "compType", TO_STRING((int)prof->GetInputMap()[(GameplayActions)i].compType));
                xml_action.SetAttribute( "val", TO_STRING((int)prof->GetInputMap()[(GameplayActions)i].val));
                xml_action.SetAttribute( "direction", TO_STRING((int)prof->GetInputMap()[(GameplayActions)i].direction));
                xml_profile.InsertEndChild(xml_action);
            }
            xml_profiles.InsertEndChild(xml_profile);
        }

        xml_file.InsertEndChild(xml_profiles);

        if(!xml_file.SaveFile())
            return false;

        return true;
    }
    Profile* ProfileManager::GetFirstProfile()
    {
        if (mProfileList.empty())
            return 0;

        mListItr = mProfileList.begin();
        return mListItr->second;
    }

    Profile* ProfileManager::GetNextProfile()
    {
        ++mListItr;
        if (mListItr == mProfileList.end())
            return 0;

        return mListItr->second;        
    }
}