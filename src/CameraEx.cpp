#include "Precompiled.h"
#include "CameraEx.h"
#include "Root.h"
#include "GameObject.h"

#define ROTATE 3.14

namespace Gibdd
{
    CameraEx::CameraEx(const std::string& name, Ogre::Camera* camera):
    pPitchNode(NULL),
    pYawNode(NULL),
    pObjConnected(NULL),
    mViewMode(VIEW_FREE)
    {
        pKeyboard = 0;
        pMouse = 0;
        fMoveScale = 0;
        fMoveSpeed = 100;
        mRotScale = 0;
        mRotateSpeed = 36;
        fTimeUntilNextToggle = 0;
        fCurrentSpeed = 0;

        sName = name;
        pCamera = camera;

        fTightness = 10;

        pCamera->setPosition(Ogre::Vector3::ZERO);
        pCamera->setOrientation(Ogre::Quaternion::IDENTITY);
        pCamera->setQueryFlags(CAMERA_MASK);

        pKeyboard = Root::GetInstance().GetKeyboard();
        pMouse = Root::GetInstance().GetMouse();
        pSceneMgr = Root::GetInstance().GetSceneManager();
        pYawNode = pSceneMgr->getRootSceneNode()->createChildSceneNode(name);

        pPitchNode = pYawNode->createChildSceneNode();
        pPitchNode->attachObject(pCamera);
        pYawNode->setInheritOrientation(false);

        fTargetYaw = 0;
        fTargetPitch = 0;
    }

    CameraEx::~CameraEx()
    {
    }

    void CameraEx::changeViewMode()
    {
        if(!pObjConnected)
            return;

        switch(mViewMode)
        {
            case VIEW_1ST:
                changeViewModeTo3rdPerson();
                break;
            case VIEW_3RD:
                changeViewModeTo1stPerson();
                break;
        }
    }

    void CameraEx::changeViewModeTo1stPerson()
    {
        if(mViewMode == VIEW_1ST)
            return;

        pCamera->setPosition(0, 0, 0);
        pYawNode->setPosition(0, 180, 0);

        mViewMode = VIEW_1ST;
    }

    void CameraEx::changeViewModeTo3rdPerson()
    {
        if(mViewMode == VIEW_3RD)
            return;

        CameraInfo& info = pObjConnected->getCameraInfo();
        info.minPitch /= 10;

        pCamera->setPosition(0, 0, pObjConnected->getCameraInfo().vCamPosition.z);
        pYawNode->setPosition(0, pObjConnected->getCameraInfo().vCamPosition.y, 0);

        mViewMode = VIEW_3RD;
    }

    void CameraEx::changeViewModeToFree()
    {
        canRotate = true;
        mViewMode = VIEW_FREE;
    }

    void CameraEx::changeViewModeToDebug()
    {
        canRotate = false;
        mViewMode = VIEW_DEBUG;
    }

    void CameraEx::connect(GameObject *gObj)
    {      
        pObjConnected = gObj;
        pPitchNode->setPosition(Ogre::Vector3::ZERO);
        Ogre::Quaternion quat;
        quat.FromAngleAxis(Ogre::Degree(180), Ogre::Vector3::UNIT_X);
        pPitchNode->setOrientation(/*gObj->getMainNode()->_getDerivedOrientation()**/quat);
        pYawNode->getParent()->removeChild(pYawNode);
        pObjConnected->getMainNode()->addChild(pYawNode);
        pYawNode->setPosition(0.1,0.7,-0.4);
    }

    GameObject *CameraEx::disconnect()
    {
        GameObject *obj = 0;
        if(!pObjConnected)
            return obj;

        obj = pObjConnected;

        Ogre::Vector3 pos = pObjConnected->getCameraInfo().pNode->_getDerivedPosition();
        pObjConnected->getCameraInfo().pNode->removeChild(pYawNode);

        pSceneMgr->getRootSceneNode()->addChild(pYawNode);

        pYawNode->setPosition(pos);
        pPitchNode->setPosition(Ogre::Vector3::ZERO);
        pObjConnected = 0;

        return obj;
    }

    void CameraEx::simulate(float elapsedTime)
    {
        if (mViewMode == VIEW_FREE)
        {
            fSpeedLimit = fMoveScale * elapsedTime;
            Ogre::Vector3 lastMotion = vTranslateVector;

            {
                if (fTimeUntilNextToggle >= 0)
                    fTimeUntilNextToggle -= elapsedTime;

                fMoveScale = fMoveSpeed * elapsedTime;
                mRotScale = mRotateSpeed * elapsedTime;

                mRotX = 0;
                mRotY = 0;
                vTranslateVector = Ogre::Vector3::ZERO;
            }

            if (vTranslateVector == Ogre::Vector3::ZERO)
            {
                fCurrentSpeed -= elapsedTime * 0.3;
                vTranslateVector = lastMotion;
            }
            else
            {
                fCurrentSpeed += elapsedTime;
            }

            if (fCurrentSpeed > 1.0)
                fCurrentSpeed = 1.0;
            if (fCurrentSpeed < 0.0)
                fCurrentSpeed = 0.0;

            vTranslateVector *= fCurrentSpeed;

            const OIS::MouseState &ms = pMouse->getMouseState();
            mRotX = Ogre::Degree(-ms.X.rel * 0.13);
            mRotY = Ogre::Degree(-ms.Y.rel * 0.13);

            if(pKeyboard->isKeyDown(OIS::KC_A))
                vTranslateVector.x = -fMoveScale;

            if(pKeyboard->isKeyDown(OIS::KC_D))
                vTranslateVector.x = fMoveScale;

            if(pKeyboard->isKeyDown(OIS::KC_W))
                vTranslateVector.z = -fMoveScale;

            if(pKeyboard->isKeyDown(OIS::KC_S))
                vTranslateVector.z = fMoveScale;

            moveFreeCamera();

        }

        if(mViewMode == VIEW_DEBUG)
        {
            fSpeedLimit = fMoveScale * elapsedTime;
            Ogre::Vector3 lastMotion = vTranslateVector;

            {
                if (fTimeUntilNextToggle >= 0)
                    fTimeUntilNextToggle -= elapsedTime;

                fMoveScale = fMoveSpeed * elapsedTime;
                mRotScale = mRotateSpeed * elapsedTime;

                mRotX = 0;
                mRotY = 0;
                vTranslateVector = Ogre::Vector3::ZERO;
            }

            if (vTranslateVector == Ogre::Vector3::ZERO)
            {
                fCurrentSpeed -= elapsedTime * 0.3;
                vTranslateVector = lastMotion;
            }
            else
            {
                fCurrentSpeed += elapsedTime;
            }

            if (fCurrentSpeed > 1.0)
                fCurrentSpeed = 1.0;
            if (fCurrentSpeed < 0.0)
                fCurrentSpeed = 0.0;

            vTranslateVector *= fCurrentSpeed;

            const OIS::MouseState &ms = pMouse->getMouseState();
            mRotX = Ogre::Degree(-ms.X.rel * 0.13);
            mRotY = Ogre::Degree(-ms.Y.rel * 0.13);

            if(pKeyboard->isKeyDown(OIS::KC_A))
                vTranslateVector.x = -fMoveScale;

            if(pKeyboard->isKeyDown(OIS::KC_D))
                vTranslateVector.x = fMoveScale;

            if(pKeyboard->isKeyDown(OIS::KC_W))
                vTranslateVector.z = -fMoveScale;

            if(pKeyboard->isKeyDown(OIS::KC_S))
                vTranslateVector.z = fMoveScale;

            moveFreeCamera();

        }

        if(pObjConnected)
        {
            pPitchNode->setOrientation(pObjConnected->getMainNode()->_getDerivedOrientation());
            if(fTargetYaw)
            {
                float yaw = fTargetYaw*elapsedTime;
                fTargetYaw = 0;
                Ogre::Quaternion quat = pYawNode->getOrientation()*Ogre::Quaternion(Ogre::Radian(yaw), Ogre::Vector3::UNIT_Y);
                float newYaw = quat.getYaw().valueRadians();
                if(newYaw < pObjConnected->getCameraInfo().maxYaw && newYaw > pObjConnected->getCameraInfo().minYaw)
                    pYawNode->yaw(Ogre::Radian(yaw));
            }
            if(fTargetPitch)
            {
                float pitch = fTargetPitch*elapsedTime;
                fTargetPitch = 0;
                Ogre::Quaternion quat = pPitchNode->getOrientation()*Ogre::Quaternion(Ogre::Radian(pitch), Ogre::Vector3::UNIT_X);
                float oldPitch = -pPitchNode->getOrientation().getPitch().valueRadians();
                float newPitch = -quat.getPitch().valueRadians();
                if((newPitch < pObjConnected->getCameraInfo().maxPitch && newPitch > pObjConnected->getCameraInfo().minPitch)
                    ||(newPitch < oldPitch && newPitch > pObjConnected->getCameraInfo().maxPitch)
                    ||(newPitch > oldPitch && newPitch < pObjConnected->getCameraInfo().minPitch))
                {
                    pPitchNode->pitch(Ogre::Radian(pitch));
                }
            }
        }
    }

    void CameraEx::mouseMoved(const OIS::MouseEvent &e)
    {
        if(pObjConnected)
        {
            pObjConnected->mouseMove(e);

           /* if(!pObjConnected->getCameraInfo().bFixedRotate)
            {
                fTargetYaw = -e.state.X.rel * ROTATE;
                fTargetPitch = e.state.Y.rel * ROTATE;
            }*/
        }
    }

    void CameraEx::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        if (mViewMode == VIEW_DEBUG && id == OIS::MB_Right)
        {
            canRotate = true;
        }
    }

    void CameraEx::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        if (mViewMode == VIEW_DEBUG && id == OIS::MB_Right)
        {
            canRotate = false;
        }
    }
    void CameraEx::keyReleased(const OIS::KeyEvent &e)
    {
        if(!pObjConnected)
            return;

        pObjConnected->keyReleased(e);
    }

    void CameraEx::keyPressed(const OIS::KeyEvent &e)
    {
        if(!pObjConnected)
            return;

        pObjConnected->keyPressed(e);
    }

    void CameraEx::moveFreeCamera()
    {
        if (mViewMode == VIEW_FREE)
        {
            pCamera->yaw(mRotX);
            pCamera->pitch(mRotY);
            pCamera->moveRelative(vTranslateVector);

            return;
        }

        if (mViewMode == VIEW_DEBUG && canRotate)
        {
            pCamera->yaw(mRotX);
            pCamera->pitch(mRotY);
            pCamera->moveRelative(vTranslateVector);

            return;
        }
        else
        {
            pCamera->moveRelative(vTranslateVector);

            return;
        }

    }
}