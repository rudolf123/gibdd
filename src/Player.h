#ifndef PlayerH
#define PlayerH
#include "GameplaySettings.h"
#include "Profile.h"

namespace Gibdd
{
    class Vehicle;
    class PhysicManager;
    class CameraEx;

    class Player
    {
        public:
            Player(const std::string& name, Ogre::SceneManager *scnMgr, PhysicManager *physMgr, uint16 ID, ProfileInputMap inp);
            ~Player();

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

            Vehicle* CreateCar(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, const std::string& filename);

            Ogre::Viewport* GetViewport(){ return pVp;};
            void preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
            void postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
          //  void setOverlay (Ogre::Overlay* ovr); 
        private:

            void processTurningLight();
            void processInputs();

            std::string sName;
            uint16 playerID;

            PhysicManager *pPhysicMgr;
            Ogre::SceneManager *pSceneMgr;
            Ogre::Viewport *pVp;
            Ogre::Overlay* mTahometrOverlay; 
            Ogre::Overlay* mSpeedometrOverlay;
            Ogre::Overlay* mTurningLightsOverlay;

            CameraEx *pCameraEx;

            bool joyInput;
            //typedef stdext::hash_map<GameplayActions, PlayerInput> InputMap;
            ProfileInputMap mInput;
            OIS::Keyboard *inputKey;
            OIS::JoyStick *inputJoy;

            Vehicle *car; 
            float accelerate;
            float brake;
            float steering;

            bool isTurnLeftOn;
            bool isTurnRightOn;
            Ogre::Timer *mBlinkingTimer;

            int subentitynum;

    };

}

#endif