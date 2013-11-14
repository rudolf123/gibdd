#pragma once
#include "Wheel.h"
#include "Motor.h"
#include "Gear.h"
#include "VehicleDesc.h"
#include "PhysicManager.h"
#include "GameObject.h"

namespace Gibdd
{
    class Vehicle: public GameObject, Ogre::RenderTargetListener
    {
        public:
            Vehicle();
            ~Vehicle();

            static Vehicle* createVehicle(Ogre::SceneManager *pScnMgr, NxScene* scene, VehicleDesc* vehicleDesc);
     
            void updateVehicle(float timeSinceLastFrame);

            void mouseMove(const OIS::MouseEvent &e);
            void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

            void keyPressed(const OIS::KeyEvent &e);
            void keyReleased(const OIS::KeyEvent &e);

            void buttonPressed(const OIS::JoyStickEvent &e, int button);

            void control (float acceler, float brk, float steer);
            void gearUp(){ _vehicleGears->gearUp(); };
            void gearDown(){_vehicleGears->gearDown();};


            const Motor* getMotor() const { return _vehicleMotor; }
            const Gear* getGears() const { return _vehicleGears; }
            NxActor* getActor() { return mVehicleActor; }
            Ogre::SceneNode* getMainNode() { return pBodyModel->getParentSceneNode(); }
            Ogre::Entity* getMainModel() { return pBodyModel; }
            void setVisible(bool visible){ pBodyModel->setVisible(visible) ;}
            bool getVisible(){ return pBodyModel->getVisible() ;}
            uint8 getClosestWheelToPoint(Ogre::Vector3 vPoint);
            void setControlByBot(bool bot) { botInput = bot;};
            void setJoyInput(bool inp) { joyInput = inp;};

            void* userData;
            float wrpm;
        private:
            NxWheelShape* addWheelToActor(NxActor* actor, NxWheelDesc* wheelDesc);
            NxActor* createBodyActor(const NxVec3& pos, const NxVec3& boxDim, const NxReal density);

            void updatePhysicVehicle(float timeSinceLastFrame); 

            void updateGraphicVehicle(float timeSinceLastFrame); 
            void createCubeMap(Ogre::SceneManager *mSceneMgr);
            void preRenderTargetUpdate(const RenderTargetEvent& evt);
            void postRenderTargetUpdate(const RenderTargetEvent& evt);

            NxScene* _nxScene;
            NxActor* mVehicleActor;
            NxBoxShape* mBodyShape;
            NxArray<Wheel*> _wheels;
            std::vector<Ogre::SceneNode*> _GpahicWheels;

            Motor* _vehicleMotor;
            Gear* _vehicleGears;
            float vel;         //линейная скорость движения танка
            float ratio;
            
            float Mf;
            float torque;
            float btorque;

            Ogre::Entity *pBodyModel;
           /* Ogre::SceneNode *pWheelNode_1;
            Ogre::SceneNode *pWheelNode_2;
            Ogre::SceneNode *pWheelNode_3;
            Ogre::SceneNode *pWheelNode_4;*/

            NxReal _steeringWheelState;

            float steering;
            float accelerate;
            float brake;

            NxReal _digitalSteeringDelta;
            NxVec3 _steeringTurnPoint;
            NxVec3 _steeringSteerPoint;
            NxReal _steeringMaxAngleRad;
            NxReal _motorForce;
            NxReal _transmissionEfficiency;
            NxReal _differentialRatio;

            NxVec3 _localVelocity;
            NxReal _maxVelocity;
            NxReal _maxBrakeForce;
            NxMaterial *_carMaterial;

            NxActor* _mostTouchedActor;

            NxMaterial* wsm;

            int mMotorTorque;

            bool joyInput;
            bool botInput;

            RenderTarget* mTargets[6];
            RenderTexture* mRearView;
            Ogre::Camera *pCubeCamera;
    };
}