#ifndef CameraExH
#define CameraExH

namespace Gibdd
{
    class GameObject;

    enum ViemModeEnum
    {
        VIEW_1ST,
        VIEW_3RD,
        VIEW_FREE,
        VIEW_DEBUG,
        VIEW_CHARACTER_3D
    };

    enum QueryFlags
    {
        STATIC_MASK     = 1 << 0,
        WORLD_MASK      = 1 << 1,
        CHARACTER_MASK  = 1 << 2,
        CAMERA_MASK     = 1 << 3,
        WAYPOINT_MASK   = 1 << 4,
        TERRAIN_MASK    = 1 << 5,
        AXES_MASK       = 1 << 6,
        DEFAULT_MASK    = 1 << 7
    };

    class CameraEx
    {
        public:
            CameraEx(const std::string& name, Ogre::Camera* camera);
            ~CameraEx();

            void mouseMoved(const OIS::MouseEvent &e);
            void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            void keyReleased(const OIS::KeyEvent &e);
            void keyPressed(const OIS::KeyEvent &e);

            void connect(GameObject *gObj);
            GameObject *disconnect();
            void simulate(float elapsedTime);

            GameObject* getObject() { return pObjConnected; }

            Ogre::Camera* getCamera() { return pCamera; }
            Ogre::SceneNode* getYawNode() { return pYawNode; }

            void setPosition(Ogre::Vector3 pos){ pCamera->setPosition(pos);}

            void changeViewMode();
            void changeViewModeTo1stPerson();
            void changeViewModeTo3rdPerson();
            void changeViewModeToFree();
            void changeViewModeToDebug();

        private:

            void moveFreeCamera();

        private:
            Gibdd::GameObject *pObjConnected;
            Ogre::SceneNode *pPitchNode;
            Ogre::SceneNode *pYawNode;
            Ogre::Camera *pCamera;
            Ogre::RaySceneQuery  *p_pray_scene_query;
            Ogre::SceneManager* pSceneMgr;

            uint8 mViewMode;

            float fTargetYaw;
            float fTargetPitch;
            bool canRotate;

            std::string sName;

            OIS::Keyboard *pKeyboard;
            OIS::Mouse *pMouse;

            //free camera
            float fTightness;
            Ogre::Vector3 vTranslateVector;
            float fMoveScale;
            float fMoveSpeed;
            float fSpeedLimit;
            Ogre::Degree mRotScale;
            Ogre::Degree mRotateSpeed;
            float fTimeUntilNextToggle;
            float fCurrentSpeed;
            Ogre::Radian mRotX;
            Ogre::Radian mRotY;

    };
}

#endif