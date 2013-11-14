#ifndef GameObjH
#define GameObjH

namespace Gibdd
{
    struct CameraInfo
    {
        bool bFixedRotate;
        Ogre::SceneNode* pNode;
        Ogre::Vector3 vCamPosition;
        float minYaw;
        float maxYaw;
        float minPitch;
        float maxPitch;
        float aimingDistance;
        float targetDistance;
        void clear()
        {
            bFixedRotate = false;
            pNode = 0;
            vCamPosition = Ogre::Vector3::ZERO;
            minYaw = 0;
            maxYaw = 0;
            minPitch = 0;
            maxPitch = 0;
            aimingDistance = 200;
            targetDistance = 200;
        }
    };
    class GameObject
    {
        public:
            GameObject() { }
            virtual ~GameObject() { }

            virtual void mouseMove(const OIS::MouseEvent &e) = 0;
            virtual void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;
            virtual void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;

            virtual void keyPressed(const OIS::KeyEvent &e) = 0;
            virtual void keyReleased(const OIS::KeyEvent &e) = 0;

            virtual CameraInfo& getCameraInfo() { return mCameraInfo; }
            virtual Ogre::SceneNode* getMainNode() { return pMainNode; }
            virtual void setYaw(float Yaw) { fYaw = Yaw; }

        protected:
            CameraInfo mCameraInfo;

            Ogre::SceneNode *pMainNode;
            float fYaw;

    };

}
#endif