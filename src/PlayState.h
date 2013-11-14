#ifndef PlayStateH
#define PlayStateH

#include "State.h"
#include "ProfileManager.h"
#include "Profile.h"

namespace Gibdd
{
   // class CameraEx;
    class PhysicManager;
   // class Vehicle;
    class SimaforeSystem;
    class AIManager;
    class PlayerManager;
    class GameplaySettings;
    class ProfileManager;
    class Profile;

    class PlayState: public State
    {
    public:
        PlayState();
        ~PlayState();

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

        bool frameStarted(const Ogre::FrameEvent &evt);
        bool frameEnded(const Ogre::FrameEvent &evt);
        void preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
        void postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);

        void Hide();
        void Enter();
        void Restore();

    private:

        void loadGui();
        void loadLevel();
        void loadPlayers();
        void loadPhysic();
        void loadAI();

    private:
        Ogre::SceneManager *pSceneMgr;

        MyGUI::Gui *pGUI;
        MyGUI::WidgetPtr pWidget;
        MyGUI::StaticTextPtr pGearText;

        Ogre::Camera *pCamera;
        PhysicManager *pPhysicMgr;
        AIManager *pAIManager;
        PlayerManager *pPlayersMgr;

        GameplaySettings *gpSettings;

       // SimaforeSystem *simafore;

    };
}

#endif