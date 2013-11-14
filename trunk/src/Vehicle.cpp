#include "Precompiled.h"
#include "Vehicle.h"
#include "UserData.h"
#include "Root.h"

namespace Gibdd
{
    Vehicle::Vehicle(): _steeringWheelState(0), _nxScene(NULL), _carMaterial(NULL), joyInput(false), botInput(false)
    {

    }
    Vehicle::~Vehicle()
    {
        if (_carMaterial)
            _nxScene->releaseMaterial(*_carMaterial);

        if (mVehicleActor)
            _nxScene->releaseActor(*mVehicleActor);

        for (NxU32 i = 0; i < _wheels.size(); i++) 
        {
            if (_wheels[i]) 
            {
                delete _wheels[i];
                _wheels[i] = NULL;
            }
        }
        Ogre::TextureManager::getSingleton().remove("dyncubemap");
        Ogre::TextureManager::getSingleton().remove("rearview");
    }

    Vehicle* Vehicle::createVehicle(Ogre::SceneManager *pScnMgr, NxScene* scene, VehicleDesc* vehicleDesc) 
    {  
        if (vehicleDesc == NULL)
            return NULL;

        Vehicle* vehicle = new Vehicle;
        vehicle->userData = vehicleDesc->userData;

        if (!vehicleDesc->isValid()) {
            LOGS("Vehicle Desc not valid!!");
            return NULL;
        }

        vehicle->_nxScene = scene;

        if (vehicle->_carMaterial == NULL) {
            NxMaterialDesc carMaterialDesc;
            carMaterialDesc.dynamicFriction = 0.4f;
            carMaterialDesc.staticFriction = 0.4f;
            carMaterialDesc.restitution = 0;
            carMaterialDesc.frictionCombineMode = NX_CM_MULTIPLY;
            vehicle->_carMaterial = scene->createMaterial(carMaterialDesc);
        }

        NxActorDesc actorDesc;
        for (NxU32 i = 0; i < vehicleDesc->carShapes.size(); i++) {
            actorDesc.shapes.pushBack(vehicleDesc->carShapes[i]);
            if (actorDesc.shapes[i]->materialIndex == 0)
                actorDesc.shapes[i]->materialIndex = vehicle->_carMaterial->getMaterialIndex();
        }

        NxBodyDesc bodyDesc;
        bodyDesc.mass = vehicleDesc->mass;
        actorDesc.body = &bodyDesc;
        actorDesc.globalPose.t = vehicleDesc->position;

        vehicle->mVehicleActor = scene->createActor(actorDesc);
        if (vehicle->mVehicleActor == NULL) {
            delete vehicle;
            return NULL;
        }
        vehicle->mVehicleActor->userData = vehicle;

        if (vehicleDesc->motorDesc != NULL) 
        {
            vehicle->_vehicleMotor = Motor::createMotor(vehicleDesc->motorDesc);
            if (vehicle->_vehicleMotor == NULL) 
            {
                delete vehicle;
                return NULL;
            }
            vehicle->_motorForce = 0;
        } 
        else 
        {
            vehicle->_vehicleMotor = NULL;
            vehicle->_motorForce = vehicleDesc->motorForce;
        }

        if (vehicleDesc->gearDesc != NULL) 
        {
            vehicle->_vehicleGears = Gear::createGear(vehicleDesc->gearDesc);
            if (vehicle->_vehicleGears == NULL) 
            {
                LOGS("Vehicle gear-creation failed");
                delete vehicle;
                return NULL;
            }
        } 
        else 
        {
            vehicle->_vehicleGears = NULL;
        }

        for (NxU32 i = 0; i < vehicleDesc->carWheels.size(); i++) 
        {
            Wheel* wheel = new Wheel(vehicle->mVehicleActor, vehicleDesc->carWheels[i]);
            if (wheel) 
            {
                vehicle->_wheels.pushBack(wheel);
            }
            else 
            {
                delete vehicle;
                return NULL;
            }
        }

        vehicle->joyInput                   = vehicleDesc->joyInput;
        vehicle->_digitalSteeringDelta		= vehicleDesc->digitalSteeringDelta;
        vehicle->_steeringSteerPoint		= vehicleDesc->steeringSteerPoint;
        vehicle->_steeringTurnPoint			= vehicleDesc->steeringTurnPoint;
        vehicle->_steeringMaxAngleRad		= NxMath::degToRad(vehicleDesc->steeringMaxAngle);
        vehicle->_transmissionEfficiency	= vehicleDesc->transmissionEfficiency;
        vehicle->_differentialRatio			= vehicleDesc->differentialRatio;
        vehicle->_maxVelocity				= vehicleDesc->maxVelocity;
        vehicle->_maxBrakeForce             = vehicleDesc->maxBrakeForce;
        vehicle->mVehicleActor->setCMassOffsetLocalPosition(vehicleDesc->centerOfMass);

        vehicle->pBodyModel = vehicleDesc->pBodyModel;
        vehicle->pMainNode = vehicleDesc->pBodyModel->getParentSceneNode();
        
        //Ogre::Entity *e;
        Ogre::SceneNode *n;
        for (uint8 i = 0; i < 4; ++i)
        {
            n = pScnMgr->getRootSceneNode()->createChildSceneNode(vehicle->pMainNode->getName() + "_Wheel_" + TO_STRING(i));
            n->attachObject(pScnMgr->createEntity(vehicle->pMainNode->getName() + "_Wheel_" + TO_STRING(i), vehicleDesc->sWheelmeshName));
            vehicle->_GpahicWheels.push_back(n);
        }


        vehicle->mVehicleActor->wakeUp(1e10);

        //vehicle->createCubeMap(pScnMgr);
        //vehicle->pBodyModel->getSubEntity(0)->setMaterialName("DynamicCubeMap");
       // vehicle->pBodyModel->getSubEntity(6)->setMaterialName("RearViewMaterial");

        return vehicle;
    }

    NxActor* Vehicle::createBodyActor(const NxVec3& pos, const NxVec3& boxDim, const NxReal density)
    {
        NxBoxShapeDesc boxDesc;
        boxDesc.dimensions.set(boxDim.x, boxDim.y, boxDim.z);
        boxDesc.localPose.t = NxVec3(0, boxDim.y, 0);

        NxActorDesc actorDesc;
        actorDesc.shapes.pushBack(&boxDesc);
        actorDesc.globalPose.t = pos;

        NxBodyDesc bodyDesc;
        actorDesc.body = &bodyDesc;
        actorDesc.density = density;
        actorDesc.body = 0;
    
        NxActor *actor = _nxScene->createActor(actorDesc);	
        mBodyShape = static_cast<NxBoxShape *>(actor->getShapes()[0]);

        return actor;
    }

    NxWheelShape* Vehicle::addWheelToActor(NxActor* actor, NxWheelDesc* wheelDesc)
    {
        NxWheelShapeDesc wheelShapeDesc;

        if (!wsm)
        {
            NxMaterialDesc m;
            m.flags |= NX_MF_DISABLE_FRICTION;
            wsm = actor->getScene().createMaterial(m);
        }
        wheelShapeDesc.materialIndex = wsm->getMaterialIndex();

        wheelShapeDesc.localPose.t = wheelDesc->position;
        NxQuat q;
        q.fromAngleAxis(90, NxVec3(0,1,0));
        wheelShapeDesc.localPose.M.fromQuat(q);

        NxReal heightModifier = (wheelDesc->wheelSuspension + wheelDesc->wheelRadius) / wheelDesc->wheelSuspension;

        wheelShapeDesc.suspension.spring = wheelDesc->springRestitution*heightModifier;
        wheelShapeDesc.suspension.damper = wheelDesc->springDamping*heightModifier;
        wheelShapeDesc.suspension.targetValue = wheelDesc->springBias*heightModifier;

        wheelShapeDesc.radius = wheelDesc->wheelRadius;
        wheelShapeDesc.suspensionTravel = wheelDesc->wheelSuspension; 
        wheelShapeDesc.inverseWheelMass = 0.1;	

        wheelShapeDesc.lateralTireForceFunction = wheelDesc->lateralTireForceFunction;	
        wheelShapeDesc.longitudalTireForceFunction = wheelDesc->longitudalTireForceFunction;

        NxWheelShape* wheelShape = NULL;
        wheelShape = static_cast<NxWheelShape *>(actor->createShape(wheelShapeDesc));
        return wheelShape;
    }

    void Vehicle::control (float acceler, float brk, float steer)
    {
       /* OIS::Keyboard *input = Root::GetInstance().GetKeyboard();
        if(!input)
            return;*/

        steering = steer;
        accelerate = acceler;
        brake = brk;

       /* if(joyInput)
        {
            steering = (float)Root::GetInstance().GetJoystick()->getJoyStickState().mAxes[3].abs/Root::GetInstance().GetJoystick()->MAX_AXIS;
            if(Root::GetInstance().GetJoystick()->getJoyStickState().mButtons[1])
                accelerate = true;
            else if(Root::GetInstance().GetJoystick()->getJoyStickState().mButtons[0])
                brake = true;
            else
            {
                accelerate = false;
                brake = false;
            }
        }
        else
        {
            if(input->isKeyDown(OIS::KC_LEFT) && input->isKeyDown(OIS::KC_RIGHT))
                steering = 0;
            else if(input->isKeyDown(OIS::KC_LEFT) && steering > -1)
                steering -= 0.001;
            else if(input->isKeyDown(OIS::KC_RIGHT)&& steering < 1)
                steering += 0.001;
            else
                steering = 0;

            if(input->isKeyDown(OIS::KC_UP))
                accelerate = true;
            else if(input->isKeyDown(OIS::KC_DOWN))
                brake = true;
            else
            {
                accelerate = false;
                brake = false;
            }
        }*/

    }
    
    void Vehicle::updateVehicle(float timeSinceLastFrame)
    {
        updatePhysicVehicle(timeSinceLastFrame);
        updateGraphicVehicle(timeSinceLastFrame);

    }

    void Vehicle::updatePhysicVehicle(float timeSinceLastFrame) 
    {
        ratio = _vehicleGears->getCurrentRatio() * _vehicleGears->getDifferentialRatio();
        torque = 0;
        btorque = 0;

        if(_vehicleMotor->_rpm < _vehicleMotor->_minRpm)
            _vehicleMotor->_rpm = _vehicleMotor->_minRpm;
        if(_vehicleMotor->_rpm > _vehicleMotor->_maxRpm)
            _vehicleMotor->_rpm = _vehicleMotor->_maxRpm;

       // control();
        Ogre::Quaternion currentOrientation(toOgre(mVehicleActor->getGlobalOrientationQuat()));
        bool ltouch = _wheels[0]->hasGroundContact(), rtouch = _wheels[1]->hasGroundContact();

        if(brake>0.01)
        {
            torque = 0;
            btorque = _maxBrakeForce * brake;
            {
                _vehicleMotor->_rpm -= _vehicleMotor->_maxRpm * timeSinceLastFrame;
                _vehicleMotor->_rpm = NxMath::max(_vehicleMotor->_rpm, _vehicleMotor->_minRpm);
            }
        }


        if(accelerate > 0.1)
        {
            torque = _vehicleMotor->getTorque()*ratio;
            wrpm =(Ogre::Math::Abs(_wheels[0]->getRpm()) + Ogre::Math::Abs(_wheels[1]->getRpm()))*ratio;
            if((wrpm >= _vehicleMotor->_minRpm) &&(_wheels[0]->hasGroundContact() || _wheels[1]->hasGroundContact()))    //Обороты двигателя корректируем от оборотов колес
            {
                _vehicleMotor->_rpm += wrpm - _vehicleMotor->_rpm;
            }
            torque = _vehicleMotor->getTorque()*ratio * accelerate;
        }
        else
        {
            _vehicleMotor->_rpm -= _vehicleMotor->_rpm*timeSinceLastFrame;
        }


        ///Steering

        /*if (botInput)
        {
            for(NxU32 i = 0; i < _wheels.size(); i++)
            {
                Wheel* wheel = _wheels[i];

                if(wheel->getWheelFlag(NX_WF_STEERABLE_INPUT))
                {
                    if (abs(steering) > 1)
                        wheel->setAngle(NxMath::atan(_steeringMaxAngleRad));
                    else
                        wheel->setAngle(NxMath::atan(steering * _steeringMaxAngleRad));
                } 
                else if(wheel->getWheelFlag(NX_WF_STEERABLE_AUTO))
                {
                    NxVec3 localVelocity = mVehicleActor->getLocalPointVelocity(wheel->getWheelPos());
                    NxQuat local2Global = mVehicleActor->getGlobalOrientationQuat();
                    local2Global.inverseRotate(localVelocity);
                    localVelocity.y = 0;
                    if(localVelocity.magnitudeSquared() < 0.01f)
                    {
                        wheel->setAngle(0.0f);
                    } else {
                        localVelocity.normalize();
                        if(localVelocity.x < 0)
                            localVelocity = -localVelocity;
                        NxReal angle = NxMath::clamp((NxReal)atan(localVelocity.z / localVelocity.x), 0.3f, -0.3f);
                        wheel->setAngle(angle);
                    }
                }
            }
        }
        else*/

        if(joyInput)
        {
            for(NxU32 i = 0; i < _wheels.size(); i++)
            {
                Wheel* wheel = _wheels[i];

                if(wheel->getWheelFlag(NX_WF_STEERABLE_INPUT))
                {
                    wheel->setAngle(NxMath::atan(steering * _steeringMaxAngleRad));
                } 
                else if(wheel->getWheelFlag(NX_WF_STEERABLE_AUTO))
                {
                    NxVec3 localVelocity = mVehicleActor->getLocalPointVelocity(wheel->getWheelPos());
                    NxQuat local2Global = mVehicleActor->getGlobalOrientationQuat();
                    local2Global.inverseRotate(localVelocity);
                    localVelocity.y = 0;
                    if(localVelocity.magnitudeSquared() < 0.01f)
                    {
                        wheel->setAngle(0.0f);
                    } else {
                        localVelocity.normalize();
                        if(localVelocity.x < 0)
                            localVelocity = -localVelocity;
                        NxReal angle = NxMath::clamp((NxReal)atan(localVelocity.z / localVelocity.x), 0.3f, -0.3f);
                        wheel->setAngle(angle);
                    }
                }
            }
        }
        else
        {
            if (NxMath::abs(steering) > 0.0001f) {
                _steeringWheelState += NxMath::sign(steering) * _digitalSteeringDelta;
            } else if (NxMath::abs(_steeringWheelState) > 0.0001f) {
                _steeringWheelState -= NxMath::sign(_steeringWheelState) * _digitalSteeringDelta;
            }
            _steeringWheelState = NxMath::clamp(_steeringWheelState, 1.f, -1.f);


            NxReal distanceSteeringAxisCarTurnAxis = _steeringSteerPoint.x  - _steeringTurnPoint.x;
            NX_ASSERT(_steeringSteerPoint.z == _steeringTurnPoint.z);
            NxReal distance2 = 0;
            if (NxMath::abs(_steeringWheelState) > 0.01f)
                distance2 = distanceSteeringAxisCarTurnAxis / NxMath::tan(_steeringWheelState * _steeringMaxAngleRad);

            for(NxU32 i = 0; i < _wheels.size(); i++)
            {
                Wheel* wheel = _wheels[i];

                if(wheel->getWheelFlag(NX_WF_STEERABLE_INPUT))
                {
                    if(distance2 != 0)
                    {
                        NxReal xPos = wheel->getWheelPos().x;
                        NxReal zPos = wheel->getWheelPos().z;
                        NxReal dz = -zPos + distance2;
                        NxReal dx = xPos - _steeringTurnPoint.x;
                        wheel->setAngle(NxMath::atan(dx/dz));
                    } else {
                        wheel->setAngle(0.f);
                    }

                } else if(wheel->getWheelFlag(NX_WF_STEERABLE_AUTO))
                {
                    NxVec3 localVelocity = mVehicleActor->getLocalPointVelocity(wheel->getWheelPos());
                    NxQuat local2Global = mVehicleActor->getGlobalOrientationQuat();
                    local2Global.inverseRotate(localVelocity);
                    localVelocity.y = 0;
                    if(localVelocity.magnitudeSquared() < 0.01f)
                    {
                        wheel->setAngle(0.0f);
                    } else {
                        localVelocity.normalize();
                        if(localVelocity.x < 0)
                            localVelocity = -localVelocity;
                        NxReal angle = NxMath::clamp((NxReal)atan(localVelocity.z / localVelocity.x), 0.3f, -0.3f);
                        wheel->setAngle(angle);
                    }
                }
            }
        }


        _wheels[1]->tick(false, torque, btorque , timeSinceLastFrame);
        _wheels[0]->tick(false, torque, btorque , timeSinceLastFrame);
    }

    void Vehicle::updateGraphicVehicle(float timeSinceLastFrame)
    {
        NxVec3 position = mVehicleActor->getShapes()[0]->getGlobalPosition();
        NxQuat orientation;
        mVehicleActor->getShapes()[0]->getGlobalOrientation().toQuat(orientation);
        pMainNode->setPosition(toOgre(position));
        pMainNode->setOrientation(toOgre(orientation));

       /* (_wheels[0]->getWheelPose()e.M.toQuat(orientation);
        _GpahicWheels[0]->setPosition(toOgre(_wheels[0]->getWheelPose().t));
        _GpahicWheels[0]->setOrientation(toOgre(orientation));
        LOG("", toOgre(orientation).w);*/

        for (uint8 i = 0; i < 4; ++i)
        {
            //NxMat34 pose = ((ShapeUserData*)(_wheels[i]->userData))->wheelShapePose;

            
            //pose.M.toQuat(orientation);

            NxMat34 pose;
            pose = _wheels[i]->getWheelPose();
            NxReal steerAngle = _wheels[i]->getWheelShape()->getSteerAngle();

            NxReal rollAngle = _wheels[i]->getWheelShapeRollAngle();
            rollAngle += _wheels[i]->getWheelShape()->getAxleSpeed() * timeSinceLastFrame;
            while (rollAngle > NxTwoPi)	//normally just 1x
                rollAngle -= NxTwoPi;
            while (rollAngle < -NxTwoPi)	//normally just 1x
                rollAngle += NxTwoPi;

            _wheels[i]->setWheelShapeRollAngle(rollAngle);

            NxMat33 rot, axisRot;
            rot.rotY(steerAngle);
            axisRot.rotY(0);

            NxMat33 rollRot;
            rollRot.rotX(rollAngle);
            pose.M = rot * pose.M * axisRot * rollRot;


            pose.M.toQuat(orientation);
            position = pose.t;//_wheels[i]->getWheelPosition();

            if (i==1 || i==3)
            {
                _GpahicWheels[i]->setPosition(toOgre(position));
                _GpahicWheels[i]->setOrientation(toOgre(orientation*toNx(Ogre::Vector3::UNIT_Y)));
            }
            else
            {
                _GpahicWheels[i]->setPosition(toOgre(position));
                _GpahicWheels[i]->setOrientation(toOgre(orientation));
            }

           /* _wheels[i]->getWheelPose().M.toQuat(orientation);
            _GpahicWheels[i]->setPosition(toOgre(_wheels[i]->getWheelPose().t));
            _GpahicWheels[i]->setOrientation(toOgre(orientation));*/
        }
    }

    uint8 Vehicle::getClosestWheelToPoint(Ogre::Vector3 vPoint)
    {
        if (_GpahicWheels[0]->getPosition().squaredDistance(vPoint) > _GpahicWheels[1]->getPosition().squaredDistance(vPoint))
            return 0;
        else
            return 1;
    }

    void Vehicle::mouseMove(const OIS::MouseEvent &e)
    {

    }

    void Vehicle::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void Vehicle::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void Vehicle::keyPressed(const OIS::KeyEvent &e)
    {

    }

    void Vehicle::keyReleased(const OIS::KeyEvent &e)
    {

    }

    void Vehicle::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {

    }

    void Vehicle::createCubeMap(Ogre::SceneManager *mSceneMgr)
    {
        // create the camera used to render to our cubemap
        pCubeCamera = mSceneMgr->createCamera(this->getMainNode()->getName());
        pCubeCamera->setFOVy(Degree(90));
        pCubeCamera->setAspectRatio(1);
        pCubeCamera->setFixedYawAxis(false);
        pCubeCamera->setNearClipDistance(5);

        // create our dynamic cube map texture
        TexturePtr tex = TextureManager::getSingleton().createManual("dyncubemap",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_CUBE_MAP, 256, 256, 0, PF_R8G8B8, TU_RENDERTARGET);

        TexturePtr texRearview = TextureManager::getSingleton().createManual("rearview",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 256, 256, 0, PF_R8G8B8, TU_RENDERTARGET);

        mRearView = texRearview->getBuffer()->getRenderTarget();
        mRearView->addViewport(pCubeCamera)->setOverlaysEnabled(false);
        mRearView->addListener(this);
        // assign our camera to all 6 render targets of the texture (1 for each direction)
        for (unsigned int i = 0; i < 6; i++)
        {
            mTargets[i] = tex->getBuffer(i)->getRenderTarget();
            mTargets[i]->addViewport(pCubeCamera)->setOverlaysEnabled(false);
            mTargets[i]->addListener(this);
        }
    }

    void Vehicle::preRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        pBodyModel->setVisible(false);
        pCubeCamera->setPosition(pBodyModel->getParentSceneNode()->getPosition() + Vector3(0,3,0));
        // point the camera in the right direction based on which face of the cubemap this is
        pCubeCamera->setOrientation(Quaternion::IDENTITY);
        if (evt.source == mTargets[0]) pCubeCamera->yaw(Degree(-90));
        else if (evt.source == mTargets[1]) pCubeCamera->yaw(Degree(90));
        else if (evt.source == mTargets[2]) pCubeCamera->pitch(Degree(90));
        else if (evt.source == mTargets[3]) pCubeCamera->pitch(Degree(-90));
        else if (evt.source == mTargets[5]) pCubeCamera->yaw(Degree(180));

        //mRearView->writeContentsToFile("start.png");
        if (evt.source == mRearView) pCubeCamera->setOrientation(pBodyModel->getParentSceneNode()->getOrientation());
    }

    void Vehicle::postRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        pBodyModel->setVisible(true);
    }
}



