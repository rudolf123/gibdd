#include "Precompiled.h"
#include "PlayerManager.h"
#include "Player.h"
#include "PhysicManager.h"
#include "Root.h"
#include "Profile.h"

namespace Gibdd
{
    PlayerManager::PlayerManager(Ogre::SceneManager *scnMgr, PhysicManager *physMgr):
    pSceneMgr(scnMgr),
    pPhysicMgr(physMgr),
    iPlayersCount(0)
    {

    }

    PlayerManager::~PlayerManager()
    {
        if(!Players.empty())
            for (PlayersItr = Players.begin(); PlayersItr!= Players.end(); ++PlayersItr)
                delete (*PlayersItr);
    }

    void PlayerManager::setToDefault()
    {
      //  iPlayersCount = 1;
      //  Player *plr = new Player("Player1", pSceneMgr, pPhysicMgr, iPlayersCount - 1, gpSettings->getPlayerInput(iPlayersCount - 1));
      //  Players[iPlayersCount - 1] = plr;
    }

    void PlayerManager::mouseMove(const OIS::MouseEvent &e)
    {
        if (!Players.empty())
            for (PlayersItr = Players.begin(); PlayersItr!= Players.end(); ++PlayersItr)
                (*PlayersItr)->mouseMove(e);
    }
    void PlayerManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }
    void PlayerManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
    }

    void PlayerManager::keyPressed(const OIS::KeyEvent &e)
    {
        if (!Players.empty())
            for (PlayersItr = Players.begin(); PlayersItr!= Players.end(); ++PlayersItr)
                (*PlayersItr)->keyPressed(e);



        if (e.key == OIS::KC_F2)
        {
            PlayersItr = Players.begin();
            while(PlayersItr!=Players.end())
            {
                Ogre::CompositorManager::getSingleton().addCompositor((*PlayersItr)->GetViewport(), "Glow");
                Ogre::CompositorManager::getSingleton().addCompositor((*PlayersItr)->GetViewport(), "Bloom");
                Ogre::CompositorManager::getSingleton().setCompositorEnabled((*PlayersItr)->GetViewport(), "Glow", true);
                Ogre::CompositorManager::getSingleton().setCompositorEnabled((*PlayersItr)->GetViewport(), "Bloom", true);

                ++PlayersItr;
            }
        }
        if (e.key == OIS::KC_F3)
        {
            PlayersItr = Players.begin();
            while(PlayersItr!=Players.end())
            {
                Ogre::CompositorManager::getSingleton().setCompositorEnabled((*PlayersItr)->GetViewport(), "Glow", false);
                Ogre::CompositorManager::getSingleton().addCompositor((*PlayersItr)->GetViewport(), "Bloom");
                Ogre::CompositorManager::getSingleton().removeCompositor((*PlayersItr)->GetViewport(), "Glow");
                Ogre::CompositorManager::getSingleton().setCompositorEnabled((*PlayersItr)->GetViewport(), "Bloom", true);

                ++PlayersItr;
            }
        }
    }
    void PlayerManager::keyReleased(const OIS::KeyEvent &e)
    {

    }

    void PlayerManager::povMoved(const OIS::JoyStickEvent &e, int pov)
    {

    }
    void PlayerManager::axisMoved(const OIS::JoyStickEvent &e, int axis)
    {

    }
    void PlayerManager::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
    {

    }
    void PlayerManager::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {
        if (!Players.empty())
            for (PlayersItr = Players.begin(); PlayersItr!= Players.end(); ++PlayersItr)
                (*PlayersItr)->buttonPressed(e, button);
    }
    void PlayerManager::buttonReleased(const OIS::JoyStickEvent &e, int button)
    {

    }

    void PlayerManager::Update(float timeSinceLastFrame)
    {
        if (!Players.empty())
            for (PlayersItr = Players.begin(); PlayersItr!= Players.end(); ++PlayersItr)
                (*PlayersItr)->Update(timeSinceLastFrame);
    }

    void PlayerManager::addPlayer(PlayerInfo info)
    {
        /*++iPlayersCount;
        Player *plr = new Player(info.sName, pSceneMgr, pPhysicMgr, iPlayersCount - 1, info.mInputMap);
        Players.push_back(plr);

        if (iPlayersCount == 2)
        {
            Players[0]->GetViewport()->setDimensions(0,0,1,0.5);
            Players[1]->GetViewport()->setDimensions(0,0.5,1,0.5);
        }
        if (iPlayersCount == 3)
        {
            Players[0]->GetViewport()->setDimensions(0,0,1,0.5);
            Players[1]->GetViewport()->setDimensions(0,0.5,0.5,1);
            Players[2]->GetViewport()->setDimensions(0.5,0.5,1,1);
        }*/
    }

    void PlayerManager::addPlayer(Profile *prof)
    {
        ++iPlayersCount;
        Player *plr = new Player(prof->GetName(), pSceneMgr, pPhysicMgr, iPlayersCount - 1, prof->GetInputMap());
        Players.push_back(plr);

        if (iPlayersCount == 2)
        {
            Players[0]->GetViewport()->setDimensions(0,0,1,0.5);
            Players[1]->GetViewport()->setDimensions(0,0.5,1,0.5);
        }
        if (iPlayersCount == 3)
        {
            Players[0]->GetViewport()->setDimensions(0,0,1,0.5);
            Players[1]->GetViewport()->setDimensions(0,0.5,0.5,1);
            Players[2]->GetViewport()->setDimensions(0.5,0.5,1,1);
        }
    }

    bool PlayerManager::loadFromFile(const std::string &file)
    {
      /*  std::string line;

        std::ifstream ifs(file);

        while(!ifs.eof())
        {
            std::getline(ifs, line);
            if(line.empty())
                continue;

            size_t found = line.find("numPlayers");
            if (found != std::string::npos)
            {
                found = line.find("=");
                iPlayersCount = atoi(line.substr(found + 1).c_str());
                continue;
            }

            found = line.find("#Player1");
            if (found != std::string::npos)
            {
                std::getline(ifs, line);
                found = line.find("input");
                found = line.find("=");
                if (line.substr(found + 1) == "keyboard")
                {
                    Player *plr = new Player("Player1", pSceneMgr, pPhysicMgr, iPlayersCount, false);
                    Players[0] = plr;
                }
                else
                {
                    Player *plr = new Player("Player1", pSceneMgr, pPhysicMgr, iPlayersCount, true);
                    Players[0] = plr;
                }

                continue;
            }

            found = line.find("#Player2");
            if (found != std::string::npos)
            {
                std::getline(ifs, line);
                found = line.find("input");
                found = line.find("=");
                if (line.substr(found + 1) == "keyboard")
                {
                    Player *plr = new Player("Player2", pSceneMgr, pPhysicMgr, iPlayersCount, false);
                    Players[1] = plr;
                }
                else
                {
                    Player *plr = new Player("Player2", pSceneMgr, pPhysicMgr, iPlayersCount, true);
                    Players[1] = plr;
                }

                continue;
            }


            found = line.find("#Player3");
            if (found != std::string::npos)
            {
                std::getline(ifs, line);
                found = line.find("input");
                found = line.find("=");
                if (line.substr(found + 1) == "keyboard")
                {
                    Player *plr = new Player("Player3", pSceneMgr, pPhysicMgr, iPlayersCount, false);
                    Players[2] = plr;
                }
                else
                {
                    Player *plr = new Player("Player3", pSceneMgr, pPhysicMgr, iPlayersCount, true);
                    Players[2] = plr;
                }

                continue;
            }
        }

        ifs.close();
        */
        return true;
    }
}