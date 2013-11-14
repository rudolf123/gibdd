#ifndef ProfileManagerH
#define ProfileManagerH
#include "GameplaySettings.h"

namespace Gibdd
{
    class Profile;

    typedef stdext::hash_map<std::string, Profile*> ProfilesList;

    class ProfileManager
    {
        public:
            ProfileManager();
            ~ProfileManager();

            Profile* CreateProfile();
            void DestroyProfile(Profile* prof);
            bool loadProfiles(std::string filename);
            bool saveProfiles(std::string filename);
            Profile* getProfile(std::string name) { return mProfileList[name]; }; 
            Profile* addProfile(std::string name);
            bool addProfile(Profile *prof);
            void removeProfile(std::string name);
            Profile* GetFirstProfile();
            Profile* GetNextProfile();
            bool HasProfile(std::string name);
            bool isEmpty(){ return mProfileList.empty(); };


        private:

            ProfilesList mProfileList;
            ProfilesList::iterator mListItr;
    };
}

#endif