#ifndef AIManagerH
#define AIManagerH

#include "Bot.h"
#include "BotDriver.h"
#include "WPManager.h"
#include "SimaforeSystem.h"

namespace Gibdd
{
    class PhysicManager;
    class Bot;
    class BotDriver;
    class SimaforeSystem;

    enum BotType
    {
        BOT_PEDESTRIAN,
        BOT_DRIVER
    };

    class AIManager
    {
        public:
            AIManager(Ogre::SceneManager* mgr, PhysicManager* physic);
            ~AIManager();

            void build();

            void simulate(float dt);

            void add(BotType type);
            void remove();

            void reload();

            uint16 getBotCount(){ return iBotPednum;}

            void killBots(bool kill);

        private:

            Ogre::SceneManager *pSceneMgr;
            WPManager *pWPMgr;
            SimaforeSystem *pSimaforeMgr;
            
            PhysicManager* pPhysicMgr;
            uint16 iBotPednum;
            uint16 iBotDrivenum;

            typedef stdext::hash_map<std::string, Bot*> BotPedestrianMap;
            BotPedestrianMap mBotPedestrianMap;
            typedef stdext::hash_map<std::string, BotDriver*> BotDriverMap;
            BotDriverMap mBotDriverMap;
    };
}


#endif