#ifndef PlayerManagerH
#define PlayerManagerH
#include "GameplaySettings.h"

namespace Gibdd
{
    class Player;
    class PhysicManager;
    class Root;
    class Profile;

    class PlayerManager
    {
        public:
            PlayerManager(Ogre::SceneManager *scnMgr, PhysicManager *physMgr);
            ~PlayerManager();

            void mouseMove(const OIS::MouseEvent &e);
            void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

            void keyPressed(const OIS::KeyEvent &e);
            void keyReleased(const OIS::KeyEvent &e);

            void povMoved(const OIS::JoyStickEvent &e, int pov);
            void axisMoved(const OIS::JoyStickEvent &e, int axis);
            void sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
            void buttonPressed(const OIS::JoyStickEvent &e, int button);
            void buttonReleased(const OIS::JoyStickEvent &e, int button);

            void Update(float timeSinceLastFrame);
            bool loadFromFile(const std::string &file);
            void setToDefault();
            void addPlayer(PlayerInfo info);
            void addPlayer(Profile *prof);


        private:
            uint8 iPlayersCount;

            Ogre::SceneManager *pSceneMgr;
            PhysicManager *pPhysicMgr;

            typedef std::vector<Player*> PlayersMap;
            PlayersMap Players;
            PlayersMap::iterator PlayersItr;
    };
}
#endif