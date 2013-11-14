#ifndef GameplaySettingsH
#define GameplaySettingsH

#include <hash_map>

namespace Gibdd
{
    enum GameplayActions
    {
        ACCELERATE,
        BRAKE,
        STEERLEFT,
        STEERRIGHT,
        GEARUP,
        GEARDOWN,
        LEFTLIGHT,
        RIGHTLIGHT,

        ACTIONS_NUM
    };

    struct PlayerInput
    {
        OIS::Type inputType;
        int deviceID;
        OIS::ComponentType compType;
        OIS::KeyCode key;
        int val;
    };

    typedef stdext::hash_map<GameplayActions, PlayerInput> InputMap;

    struct PlayerInfo
    {
        std::string sName;
        uint16 ID;
        InputMap mInputMap;
    };

    typedef stdext::hash_map<std::string, PlayerInfo> PlayersList;

    class GameplaySettings
    {
        public:
            GameplaySettings();
            ~GameplaySettings();
            void setPedsNum(uint16 num){numPeds = num; };
            void setDrivsNum(uint16 num){numDrivs = num; };
            void setPlayersNum(uint8 num){numPlayers = num; };

            void setDefaultSettings();
            void changePlayerInputSettings(std::string name, GameplayActions action, OIS::Type inputDevice, int deviceID, OIS::KeyCode key, OIS::ComponentType compType, int val);
            void changePlayerInfo(std::string name, PlayerInfo info);
            void addPlayerInfo(std::string name, PlayerInfo info);
            PlayerInfo getPlayerInfo(std::string name) { return mPlayers[name]; };
            bool loadSettingsFromFile(std::string filename);
            bool saveSettingsToFile(std::string filename);
            InputMap getPlayerInput(uint16 playerID) { return mInputMaps[playerID]; };
            PlayersList getPlayersList() { return mPlayers;};
            uint16 getNumPeds(){ return numPeds;};
            uint16 getNumDrivs(){ return numDrivs;};
            uint16 getNumPlayers(){ return numPlayers;};
            
        private:

            uint16 numPeds;
            uint16 numDrivs;
            uint16 numPlayers;
            uint16 currPlayerID;
            stdext::hash_map<uint16, InputMap> mInputMaps;
            PlayersList mPlayers;
    };
}
#endif