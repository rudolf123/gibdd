#include "Precompiled.h"
#include "AIManager.h"
#include "PhysicManager.h"
#include <fstream>

namespace Gibdd
{
    AIManager::AIManager(SceneManager* mgr, PhysicManager* physic):
    iBotPednum(NULL),
    iBotDrivenum(NULL),
    pSceneMgr(mgr),
    pPhysicMgr(physic),
    pWPMgr(0),
    pSimaforeMgr(0)
    {
        pWPMgr = new WPManager("PathfindingWpMgr");
        pWPMgr->setVisualDebug(false);

        pSimaforeMgr = new SimaforeSystem;
        pSimaforeMgr->loadAllSemaforesInScene(pSceneMgr);
    }

    AIManager::~AIManager()
    {
        if (pWPMgr)
            delete pWPMgr;

        if (!mBotPedestrianMap.empty())
        {
            BotPedestrianMap::iterator itr = mBotPedestrianMap.begin();
            while (itr!=mBotPedestrianMap.end())
            {
                delete (*itr).second;
                ++itr;
            }
        }

        if (!mBotDriverMap.empty())
        {
            BotDriverMap::iterator itr = mBotDriverMap.begin();
            while (itr!=mBotDriverMap.end())
            {
                delete (*itr).second;
                ++itr;
            }
        }

        if (pSimaforeMgr)
            delete pSimaforeMgr;
    }

    void AIManager::build()
    {

    }

    void AIManager::simulate(float dt)
    {
        if(!mBotPedestrianMap.empty())
        {
            BotPedestrianMap::const_iterator itr = mBotPedestrianMap.begin();
            for(; itr != mBotPedestrianMap.end(); ++itr)
            {
                itr->second->simulate(dt);
            }
        }

        if(!mBotDriverMap.empty())
        {
            BotDriverMap::const_iterator itr = mBotDriverMap.begin();
            for(; itr != mBotDriverMap.end(); ++itr)
            {
                itr->second->simulate(dt);
            }
        }

        pSimaforeMgr->simulate(dt);
    }

    void AIManager::add(BotType type)
    {
        if (type == BOT_PEDESTRIAN)
        {
            LOGS("1");
            std::string bot_name = "botPed_" + Ogre::StringConverter::toString(iBotPednum);
            Bot* bot = new Bot(pSceneMgr, pPhysicMgr, pWPMgr, pSimaforeMgr);
            LOGS("2");
            bot->create(bot_name, "Body01.mesh",Vector3(364,-47,-1));
            LOGS("3");
            mBotPedestrianMap[bot_name] = bot;
            ++iBotPednum;

            return;
        }

        if (type == BOT_DRIVER)
        {
            std::string bot_name = "botDriv_" + Ogre::StringConverter::toString(iBotDrivenum);
            BotDriver* bot = new BotDriver(pSceneMgr, pPhysicMgr, pWPMgr);
            bot->create(bot_name, "vaz_01.mesh", toNx(Vector3(364,-47,-1)), "VehicleConfig.cfg");
            mBotDriverMap[bot_name] = bot;
            ++iBotDrivenum;

            return;
        }

    }

    void AIManager::killBots(bool kill)
    {
        if(!mBotPedestrianMap.empty())
        {
            BotPedestrianMap::const_iterator itr = mBotPedestrianMap.begin();
            for(; itr != mBotPedestrianMap.end(); ++itr)
            {
                itr->second->SetRagDoll(kill);
            }
        }
    }

    void AIManager::remove()
    {

    }

    void AIManager::reload()
    {

    }
}