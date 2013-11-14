#ifndef ProfileH
#define ProfileH
#include "GameplaySettings.h"

namespace Gibdd
{
    struct ProfileInput
    {
        OIS::Type inputType;
        int deviceID;
        OIS::ComponentType compType;
        OIS::KeyCode key;
        int val;
        int direction;
    };

    typedef stdext::hash_map<GameplayActions, ProfileInput> ProfileInputMap;

    class Profile
    {
        public:
            Profile();
            ~Profile();
            void bindAction(GameplayActions action, OIS::KeyCode key);
            void bindAction(GameplayActions action, int joyID, OIS::ComponentType joyCompType, int val, int direction);
            std::string GetName() {return sName; };
            void SetName(std::string name) {sName = name; };
            ProfileInputMap GetInputMap() { return mInputMap; };
        private:
            std::string sName;
            uint16 ID;
            ProfileInputMap mInputMap;
    };
}
#endif