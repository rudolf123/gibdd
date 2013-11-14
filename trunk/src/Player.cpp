#include "Precompiled.h"
#include "Player.h"
#include "Vehicle.h"
#include "VehicleDesc.h"
#include "GearDesc.h"
#include "MotorDesc.h"
#include "PhysicManager.h"
#include "CameraEx.h"
#include "Root.h"


namespace Gibdd
{
    Player::Player(const std::string& name, Ogre::SceneManager *scnMgr, PhysicManager *physMgr, uint16 ID, ProfileInputMap inp):
    sName(name),
    car(0),
    pCameraEx(0),
    pSceneMgr(scnMgr),
    pPhysicMgr(physMgr),
    mInput(inp),
    playerID(ID),
    isTurnLeftOn(false),
    isTurnRightOn(false)
    {
        Ogre::Camera *pCamera;
        pCamera = pSceneMgr->createCamera("Camera" + name);
        pCamera->setPosition(Ogre::Vector3(0, 0, 0));
        pCamera->lookAt(Ogre::Vector3(0, 0, 0));

        pCamera->setNearClipDistance(0.1f);
        pCamera->setFarClipDistance(1000);
        pCamera->setUseRenderingDistance(true);
        pCamera->setAutoAspectRatio(true);
        pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
        inputKey = Root::GetInstance().GetKeyboard();
        inputJoy = Root::GetInstance().GetJoystick(playerID);

        pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,ID,0,0,1,1);

       /* if (numplayers == 1)
            pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,1,0,0,1,1);
        if ((numplayers == 2) && (name == "Player1"))
        {
            pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,1,0,0,1,0.5);
          //  pVp->setBackgroundColour(Ogre::ColourValue::Black);
           // pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,4,0,0,0.5,0.5);
        }

        if ((numplayers == 2) && (name == "Player2"))
        {
            pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,2,0,0.5,1,0.5);
        }
        if ((numplayers == 3) && (name == "Player1"))
            pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,1,0,0,1,0.5);
        if ((numplayers == 3) && (name == "Player2"))
            pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,2,0,0.5,0.5,1);
        if ((numplayers == 3) && (name == "Player3"))
            pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera,3,0.5,0.5,1,1);*/

        pVp->setBackgroundColour(Ogre::ColourValue::Black);

        pCameraEx = new CameraEx(pCamera->getName(), pCamera);
        Vector3 pos = pSceneMgr->getEntity("start_point")->getParentSceneNode()->getPosition();
        if (sName == "Player2")
            pos.x += 20;
        
        car = CreateCar(toNx(pos), true, false, "VehicleConfig" + name + ".cfg");

        if (mInput[STEERLEFT].inputType == OIS::OISJoyStick && mInput[STEERLEFT].compType != OIS::OIS_Button)
            car->setJoyInput(true);

        if (sName == "Player2")
        {
            Ogre::MaterialPtr mat;
            mat = Ogre::MaterialManager::getSingleton().getByName("MaterialPlayer2");
            car->getMainModel()->getSubEntity(0)->setMaterial(mat);
        }

        pCameraEx->connect(car);
        mTahometrOverlay = OverlayManager::getSingleton().getByName("Tahometr" + name);
        mTahometrOverlay->show();
        mSpeedometrOverlay = OverlayManager::getSingleton().getByName("Speedometr" + name);
        mSpeedometrOverlay->show();
        mTurningLightsOverlay = OverlayManager::getSingleton().getByName("TurningLights" + name);
        mTurningLightsOverlay->show();
        OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr" + sName + "/Gear");
        element->setCaption("1");

        element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeft");
        element->show();
        element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeftLight");
        element->hide();
        element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRight");
        element->show();
        element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRightLight");
        element->hide();
        
        mBlinkingTimer = new Ogre::Timer;

        accelerate = 0;
        brake = 0;
        steering = 0;

        subentitynum = 0;

    }

    Player::~Player()
    {
        if(car)
            delete car;
        if(pCameraEx)
            delete pCameraEx;
    }

    Vehicle* Player::CreateCar(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, const std::string& filename)
    {
        Ogre::Entity *pBodyModel;
        pBodyModel = pSceneMgr->createEntity(sName + "car","vaz_01.mesh");
        pSceneMgr->getRootSceneNode()->createChildSceneNode(sName + "car")->attachObject(pBodyModel);
        Ogre::AxisAlignedBox aab = pBodyModel->getBoundingBox(); 
        Ogre::Vector3 min = aab.getMinimum();
        Ogre::Vector3 max = aab.getMaximum();
        Ogre::Vector3 center = aab.getCenter();
        Ogre::Vector3 size(fabs(max.x-min.x)/2,fabs(max.y-min.y)/2,fabs(max.z-min.z)/2);

        VehicleDesc vehicleDesc;
        NxBoxShapeDesc boxShape;

        boxShape.dimensions.set(size.x, size.y, size.z);
        vehicleDesc.carShapes.pushBack(&boxShape);
        vehicleDesc.pBodyModel = pBodyModel;

        vehicleDesc.position = pos;
        vehicleDesc.mass = 1200; 
        vehicleDesc.digitalSteeringDelta = 0.05f;
        vehicleDesc.steeringMaxAngle = 30.f;
        vehicleDesc.motorForce = 3500.f;
        vehicleDesc.maxBrakeForce = 300.f;
        vehicleDesc.cameraDistance = 8.0f;
        vehicleDesc.maxVelocity = 800.f;
        vehicleDesc.differentialRatio = 3.42f;
        vehicleDesc.centerOfMass.set(0,-1,0);

        MotorDesc motorDesc;
        GearDesc gearDesc;
        NxReal wheelRadius;

        motorDesc.loadSettingsFromFile(filename);
        vehicleDesc.motorDesc = &motorDesc;
        gearDesc.loadSettingsFromFile(filename);
        vehicleDesc.gearDesc = &gearDesc;

        vehicleDesc.sWheelmeshName = "wheel.mesh";

        vehicleDesc.joyInput = false;

        wheelRadius = 0.4f;

        NxWheelDesc wheelDesc[4];
        for(NxU32 i=0;i<4;i++)
        {
            wheelDesc[i].wheelApproximation = 10;
            wheelDesc[i].wheelRadius = wheelRadius;
            wheelDesc[i].wheelWidth = 0.3f;
            wheelDesc[i].wheelSuspension = 0.6f;
            wheelDesc[i].springRestitution = 7000;
            wheelDesc[i].springDamping = 800;
            wheelDesc[i].springBias = 0.0f;
            wheelDesc[i].maxBrakeForce = 1.f;
            wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
            vehicleDesc.carWheels.pushBack(&wheelDesc[i]);
        }
        NxReal widthPos = 0.9f;
        NxReal heightPos = -0.4f;
        wheelDesc[0].position.set( 1.9f, heightPos, widthPos);
        wheelDesc[1].position.set( 1.9f, heightPos,-widthPos);
        wheelDesc[2].position.set(-1.1f, heightPos, widthPos);
        wheelDesc[3].position.set(-1.1f, heightPos,-widthPos);
        NxU32 flags = NX_WF_BUILD_LOWER_HALF;
        wheelDesc[0].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
        wheelDesc[1].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
        wheelDesc[2].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;
        wheelDesc[3].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;

        vehicleDesc.steeringSteerPoint.set(1.8, 0, 0);
        vehicleDesc.steeringTurnPoint.set(-1.5, 0, 0);

        Vehicle *car;

        car = Vehicle::createVehicle(pSceneMgr, pPhysicMgr->getPxScene(), &vehicleDesc);
        NxQuat q;
        q.fromAngleAxis(270.0f, NxVec3(0.0f, 1.0f, 0.0f));
        car->getActor()->setGlobalOrientationQuat(q);

        return car;
    }

    void Player::processInputs()
    {
        if (mInput[ACCELERATE].inputType == OIS::OISKeyboard)
            if(inputKey->isKeyDown(mInput[ACCELERATE].key))
                accelerate = 1;
            else 
                if (mInput[BRAKE].inputType == OIS::OISKeyboard)
                    if(inputKey->isKeyDown(mInput[BRAKE].key))
                    brake = 1;
                    else
                    {
                        accelerate = 0;
                        brake = 0;
                    }

        if (mInput[STEERLEFT].inputType == OIS::OISKeyboard)
            if (mInput[STEERRIGHT].inputType == OIS::OISKeyboard)
                if(inputKey->isKeyDown(mInput[STEERLEFT].key) && inputKey->isKeyDown(mInput[STEERRIGHT].key))
                    steering = 0; 
                else if(inputKey->isKeyDown(mInput[STEERLEFT].key) && steering > -1)
                    steering -= 0.001;
                else if(inputKey->isKeyDown(mInput[STEERRIGHT].key) && steering < 1)
                    steering += 0.001;
                else
                    steering = 0;

        if (mInput[ACCELERATE].inputType == OIS::OISJoyStick)
        {
            OIS::JoyStick *pJoy = Root::GetInstance().GetJoystick(mInput[ACCELERATE].deviceID);

            if (!pJoy)
                return;

            if (mInput[ACCELERATE].compType == OIS::OIS_Axis)
            {
                if((float)pJoy->getJoyStickState().mAxes[mInput[ACCELERATE].val].abs/pJoy->MAX_AXIS < 0)
                    accelerate = abs((float)pJoy->getJoyStickState().mAxes[mInput[ACCELERATE].val].abs/pJoy->MAX_AXIS);
            }
            if (mInput[ACCELERATE].compType == OIS::OIS_Button)
                if(pJoy->getJoyStickState().mButtons[mInput[ACCELERATE].val])
                    accelerate = 1;
                else
                    accelerate = 0;

            if (mInput[ACCELERATE].compType == OIS::OIS_POV)
                if(pJoy->getJoyStickState().mPOV[mInput[ACCELERATE].val].direction == mInput[ACCELERATE].direction)
                    accelerate = 1;
                else
                    accelerate = 0;
        }
        if (mInput[BRAKE].inputType == OIS::OISJoyStick)
        {
            OIS::JoyStick *pJoy = Root::GetInstance().GetJoystick(mInput[BRAKE].deviceID);

            if (!pJoy)
                return;

            if (mInput[BRAKE].compType == OIS::OIS_Axis)
            {
                if((float)pJoy->getJoyStickState().mAxes[mInput[BRAKE].val].abs/pJoy->MAX_AXIS > 0)
                    brake = (float)pJoy->getJoyStickState().mAxes[mInput[BRAKE].val].abs/pJoy->MAX_AXIS;
            }
            if (mInput[BRAKE].compType == OIS::OIS_Button)
                if(pJoy->getJoyStickState().mButtons[mInput[BRAKE].val])
                    brake = 1;
                else
                    brake = 0;
            if (mInput[BRAKE].compType == OIS::OIS_POV)
                if(pJoy->getJoyStickState().mPOV[mInput[BRAKE].val].direction == mInput[BRAKE].direction)
                    brake = 1;
                else
                    brake = 0;
        }

        if (mInput[STEERLEFT].inputType == OIS::OISJoyStick)
        {
            OIS::JoyStick *pJoy = Root::GetInstance().GetJoystick(mInput[STEERRIGHT].deviceID);
            if (!pJoy)
                return;

            if (mInput[STEERLEFT].compType == OIS::OIS_Axis)
                steering = (float)pJoy->getJoyStickState().mAxes[mInput[STEERLEFT].val].abs/pJoy->MAX_AXIS;
        }

        if (mInput[STEERRIGHT].inputType == OIS::OISJoyStick)
        {
            OIS::JoyStick *pJoy = Root::GetInstance().GetJoystick(mInput[STEERRIGHT].deviceID);
            if (!pJoy)
                return;

            if (mInput[STEERRIGHT].compType == OIS::OIS_Axis)
                steering = (float)pJoy->getJoyStickState().mAxes[mInput[STEERRIGHT].val].abs/pJoy->MAX_AXIS;

        }
        if(mInput[STEERRIGHT].inputType == OIS::OISJoyStick && mInput[STEERLEFT].inputType == OIS::OISJoyStick)
            if (mInput[STEERRIGHT].compType == OIS::OIS_Button && mInput[STEERLEFT].compType == OIS::OIS_Button)
            {
                OIS::JoyStick *pJoy = Root::GetInstance().GetJoystick(mInput[STEERRIGHT].deviceID);
                if (!pJoy)
                    return;

                if(pJoy->getJoyStickState().mButtons[mInput[STEERLEFT].val] && steering > -1)
                    steering -= 0.001; 
                else if(pJoy->getJoyStickState().mButtons[mInput[STEERRIGHT].val] && steering < 1)
                    steering += 0.001;
                else
                    steering = 0;
            }
    }

    void Player::Update(float timeSinceLastFrame)
    {
        processTurningLight();
        processInputs();
        car->control(accelerate, brake, steering);
        car->updateVehicle(timeSinceLastFrame);
        pCameraEx->simulate(timeSinceLastFrame);

        OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr" + sName + "/Needle");
        TextureUnitState *mNeedleTexture=element->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        mNeedleTexture->setTextureRotate(Degree(-(car->getMotor()->getRps()/37)));
        element = OverlayManager::getSingleton().getOverlayElement("Speedometr" + sName+ "/Needle");
        mNeedleTexture = element->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        mNeedleTexture->setTextureRotate(Degree(-(toOgre(car->getActor()->getLinearVelocity()).length()*1000 / 60)*0.34));
    }

    void Player::mouseMove(const OIS::MouseEvent &e)
    {
        pCameraEx->mouseMoved(e);
    }
    void Player::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }
    void Player::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void Player::keyPressed(const OIS::KeyEvent &e)
    {
        pCameraEx->keyPressed(e);

        if (e.key == OIS::KC_SPACE)
            if (car->getVisible())
                car->setVisible(false);
            else
                car->setVisible(true);

        if (mInput[GEARUP].inputType == OIS::OISKeyboard)
            if(e.key == mInput[GEARUP].key)
            {
                car->gearUp();
                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr"  + sName + "/Gear");
                element->setCaption(TO_STRING(car->getGears()->curGear));
            }

        if (mInput[GEARDOWN].inputType == OIS::OISKeyboard)
            if(e.key == mInput[GEARDOWN].key)
            {
                car->gearDown();
                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr" + sName + "/Gear");
                element->setCaption(TO_STRING(car->getGears()->curGear));
            }
        if (mInput[LEFTLIGHT].inputType == OIS::OISKeyboard)
            if(e.key == mInput[LEFTLIGHT].key)
            {
                if (!isTurnLeftOn)
                {
                    isTurnLeftOn = true;
                    mBlinkingTimer->reset();
                }
                else
                {
                    isTurnLeftOn = false;
                    OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeft");
                    element->show();
                    element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeftLight");
                    element->hide();
                }
            }
        if (mInput[RIGHTLIGHT].inputType == OIS::OISKeyboard)
            if(e.key == mInput[RIGHTLIGHT].key)
            {
                if (!isTurnRightOn)
                {
                    isTurnRightOn = true;
                    mBlinkingTimer->reset();
                }
                else
                {
                    isTurnRightOn = false;   
                    OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRight");
                    element->show();
                    element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRightLight");
                    element->hide();
                }
            }
    }
    void Player::keyReleased(const OIS::KeyEvent &e)
    {

    }

    void Player::povMoved(const OIS::JoyStickEvent &e, int pov)
    {

    }
    void Player::axisMoved(const OIS::JoyStickEvent &e, int axis)
    {
        if (mInput[GEARUP].inputType == OIS::OISJoyStick)
            if(axis == mInput[GEARUP].val)
            {
                car->gearUp();
                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr"  + sName + "/Gear");
                element->setCaption(TO_STRING(car->getGears()->curGear));
            }

            if (mInput[GEARDOWN].inputType == OIS::OISJoyStick)
                if(axis == mInput[GEARDOWN].val)
                {
                    car->gearDown();
                    OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr" + sName + "/Gear");
                    element->setCaption(TO_STRING(car->getGears()->curGear));
                }
                if (mInput[LEFTLIGHT].inputType == OIS::OISJoyStick)
                    if(axis == mInput[LEFTLIGHT].val)
                    {
                        if (!isTurnLeftOn)
                        {
                            isTurnLeftOn = true;
                            mBlinkingTimer->reset();
                        }
                        else
                        {
                            isTurnLeftOn = false;
                            OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeft");
                            element->show();
                            element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeftLight");
                            element->hide();
                        }
                    }
                    if (mInput[RIGHTLIGHT].inputType == OIS::OISJoyStick)
                        if(axis == mInput[RIGHTLIGHT].val)
                        {
                            if (!isTurnRightOn)
                            {
                                isTurnRightOn = true;
                                mBlinkingTimer->reset();
                            }
                            else
                            {
                                isTurnRightOn = false;   
                                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRight");
                                element->show();
                                element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRightLight");
                                element->hide();
                            }
                        }
    }
    void Player::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
    {

    }
    void Player::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {

        if (mInput[GEARUP].inputType == OIS::OISJoyStick)
            if(button == mInput[GEARUP].val)
            {
                car->gearUp();
                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr"  + sName + "/Gear");
                element->setCaption(TO_STRING(car->getGears()->curGear));
            }

            if (mInput[GEARDOWN].inputType == OIS::OISJoyStick)
                if(button == mInput[GEARDOWN].val)
                {
                    car->gearDown();
                    OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr" + sName + "/Gear");
                    element->setCaption(TO_STRING(car->getGears()->curGear));
                }
                if (mInput[LEFTLIGHT].inputType == OIS::OISJoyStick)
                    if(button == mInput[LEFTLIGHT].val)
                    {
                        if (!isTurnLeftOn)
                        {
                            isTurnLeftOn = true;
                            mBlinkingTimer->reset();
                        }
                        else
                        {
                            isTurnLeftOn = false;
                            OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeft");
                            element->show();
                            element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeftLight");
                            element->hide();
                        }
                    }
                    if (mInput[RIGHTLIGHT].inputType == OIS::OISJoyStick)
                        if(button == mInput[RIGHTLIGHT].val)
                        {
                            if (!isTurnRightOn)
                            {
                                isTurnRightOn = true;
                                mBlinkingTimer->reset();
                            }
                            else
                            {
                                isTurnRightOn = false;   
                                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRight");
                                element->show();
                                element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRightLight");
                                element->hide();
                            }
                        }
    }

    void Player::processTurningLight()
    {
        if (isTurnLeftOn)
        {
            if ((mBlinkingTimer->getMilliseconds() < 500))
            {
                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeft");
                element->show();
                element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeftLight");
                element->hide();
            }
            else
            {
                if ((mBlinkingTimer->getMilliseconds() > 1000))
                    mBlinkingTimer->reset();
                else
                {
                    OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeftLight");
                    element->show();
                    element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnLeft");
                    element->hide();
                }
            }

            return;
        }

        if (isTurnRightOn)
        {
            if ((mBlinkingTimer->getMilliseconds() < 500))
            {
                OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRight");
                element->hide();
                element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRightLight");
                element->show();
            }
            else
            {
                if ((mBlinkingTimer->getMilliseconds() > 1000))
                    mBlinkingTimer->reset();
                else
                {
                    OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRightLight");
                    element->hide();
                    element = OverlayManager::getSingleton().getOverlayElement("TurningLights" + sName + "/turnRight");
                    element->show();
                }

            }

            return;
        }
    }

    void Player::buttonReleased(const OIS::JoyStickEvent &e, int button)
    {

    }

    void Player::preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {

    }
    void Player::postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {
        //mTahometrOverlay->hide();
       // mSpeedometrOverlay->hide();
    }
}