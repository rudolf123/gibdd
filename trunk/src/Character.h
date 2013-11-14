#ifndef CharacterH
#define CharacterH
#include "GameObject.h"
#include "PhysicManager.h"
#include "Ragdoll.h"

namespace Gibdd
{
    using namespace Ogre;
    #define NUM_ANIMS 5  
    #define CAM_HEIGHT 2           // height of camera above character's center of mass
    #define RUN_SPEED 1           // character running speed in units per second
    #define TURN_SPEED 500.0f      // character turning in degrees per second
    #define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
    #define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second

    enum AnimID
    {
        ANIM_WAIT,
        ANIM_WALK,
        ANIM_RUN_START,
        ANIM_RUN_LOOP,
        ANIM_RUN_STOP,

        ANIM_NONE
    };

    class PhysicManager;
    class Ragdoll;

    class Character: public GameObject, public Ragdoll
    {
        public:
            Character(Ogre::Camera *cam, PhysicManager *physicMgr, Ogre::Vector3 pos);
            ~Character();


            void mouseMove(const OIS::MouseEvent &e);
            void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

            void keyPressed(const OIS::KeyEvent &e);
            void keyReleased(const OIS::KeyEvent &e);

            void addTime(float deltaTime);

            void executeRagdoll(bool exec);

        private:
            void setupBody(Ogre::SceneManager* sceneMgr, Ogre::Vector3 pos);
            void setupRagdoll();
            void setupAnimations(std::string filename);
            void setupCamera(Camera* cam);

            void updateBody(float deltaTime);
            void setBaseAnimation(AnimID id, bool reset = false);
            void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);
            void updateCamera(Real deltaTime);
            void fadeAnimations(Real deltaTime);
            void updateAnimations(Real deltaTime);

            void setRunning(bool isRun){ isRunning = isRun; };
            bool getRunning(){ return isRunning;};

        private:
            Ogre::SceneManager *pSceneMgr;
            PhysicManager *pPhysicMgr;
            CharacterController *pController;

            Ogre::Camera* mCamera;
            Ogre::SceneNode* mBodyNode;
            Ogre::SceneNode* mCameraPivot;
            Ogre::SceneNode* mCameraGoal;
            Ogre::SceneNode* mCameraNode;
            Ogre::Real mPivotPitch;
            Ogre::Entity* mBodyEnt;
            Ogre::AnimationState* mAnims[NUM_ANIMS];    // master animation list
            AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
            bool mFadingIn[NUM_ANIMS];            // which animations are fading in
            bool mFadingOut[NUM_ANIMS];           // which animations are fading out
            Ogre::Vector3 mKeyDirection;      // player's local intended direction based on WASD keys
            Ogre::Vector3 mGoalDirection;     // actual intended direction in world-space
            Ogre::Real mVerticalVelocity;     // for jumping
            Ogre::Real mTimer;                // general timer to see how long animations have been playing

            bool isRunning;
            uint16 iRunSpeed;
    };

}

#endif