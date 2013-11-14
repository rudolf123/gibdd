#include "Precompiled.h"
#include "DebugState.h"

#include "Root.h"
#include "LevelLoader.h"
#include "CameraEx.h"
#include "PhysicManager.h"
#include "Vehicle.h"
#include "VehicleDesc.h"
#include "GearDesc.h"
#include "MotorDesc.h"
#include "SimaforeSystem.h"
#include "AIManager.h"
#include "WPManager.h"
#include "Ragdoll.h"
#include "Character.h"

namespace Gibdd
{
    using namespace Ogre;
    DebugState::DebugState():
    pCameraEx1(NULL),
    pCamera1(NULL),
    pCamera(NULL),
    pCameraEx(0),
    car(NULL),
    testRagdoll(0),
    graphNum(0),
    bHideAllModels(false),
    chara(0)
    {
    }

    DebugState::~DebugState()
    {
        if (pWaypointMgr)
            delete pWaypointMgr;
        if (pAIManager)
            delete pAIManager;
        if (pPhysicMgr)
            delete pPhysicMgr;

        if(pEngineSolver)
            delete pEngineSolver;
        if(pCameraEx)
            delete pCameraEx;
        if(pCameraEx1)
            delete pCameraEx1;
        if (testRagdoll)
            delete testRagdoll;
        if (chara)
            delete chara;
    }

    void DebugState::mouseMove(const OIS::MouseEvent &e)
    {
        if (pCameraEx)
            pCameraEx->mouseMoved(e);
        if(pCameraEx1)
            pCameraEx1->mouseMoved(e);

        if (chara)
            chara->mouseMove(e);

        if( mEditMode == EDIT_ADD_WAYPOINT)
        {
            Ogre::Vector3 pos;
            Ogre::Ray mouseRay = pCamera->getCameraToViewportRay(e.state.X.abs/float(e.state.width), e.state.Y.abs/float(e.state.height));
            if (RaycastToPolygon(mouseRay, pos))
                pPointPosText->setCaption("x: " + TO_STRING(pos.x) + "y: " + TO_STRING(pos.y) + "z: " + TO_STRING(pos.z));

            currWaypointNode->setPosition(pos);
        }
    }

    void DebugState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        if (pCameraEx)
            pCameraEx->mouseReleased(e, id);
    }

    void DebugState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        if (id == OIS::MB_Left && mEditMode == EDIT_ADD_WAYPOINT)
        {
            Entity *e;
            currWaypointNode->getAttachedObject(0)->setQueryFlags(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
            WPGraph->addGraphNode(currWaypointNode->getPosition());
            currWaypointNode = pSceneMgr->getRootSceneNode()->createChildSceneNode("WPNode_" + TO_STRING(WPGraph->getNodesCount()));
            e = pSceneMgr->createEntity("WPNode_" + TO_STRING(WPGraph->getNodesCount()),"WP.mesh");
            e->setQueryFlags(WAYPOINT_MASK);
            currWaypointNode->attachObject(e); 
        }

        if(id == OIS::MB_Left && mEditMode == EDIT_NONE)
	    {
            Ogre::Vector3 pos;
		    Ogre::Ray mouseRay = pCamera->getCameraToViewportRay(e.state.X.abs/float(e.state.width), e.state.Y.abs/float(e.state.height));
            if (RaycastToPolygon(mouseRay, pos))
                pEntityText->setCaption(selectedEntity->getName() + "\n" +
                                       TO_STRING(selectedEntity->getParentSceneNode()->getPosition().x) + "\n" +
                                       TO_STRING(selectedEntity->getParentSceneNode()->getPosition().y) + "\n" +
                                       TO_STRING(selectedEntity->getParentSceneNode()->getPosition().z));
            if (selectedEntity->getName().substr(0,7) == "WPNode_")
                currWaypointNode = selectedEntity->getParentSceneNode();
        }

        if (id == OIS::MB_Left && mEditMode == EDIT_LINK_WAYPOINT)
        {
            SceneNode *Node1;
            Node1 = currWaypointNode;
            if (currWaypointNode->getName().substr(0,7) == "WPNode_")
            {
                Ogre::Vector3 pos;
                Ogre::Ray mouseRay = pCamera->getCameraToViewportRay(e.state.X.abs/float(e.state.width), e.state.Y.abs/float(e.state.height));
                if (RaycastToPolygon(mouseRay, pos))
                {
                    if (selectedEntity->getName().substr(0,7) == "WPNode_" && selectedEntity->getName()!=Node1->getName())
                    {
                        std::string num1 = selectedEntity->getName().substr(7, selectedEntity->getName().length());
                        std::string num2 = Node1->getName().substr(7, Node1->getName().length());
                       
                        WPGraph->linkNodes(atoi(num2.c_str()), atoi(num1.c_str()));
                      
                        pEntityText->setCaption(" " + selectedEntity->getName() + "\n" +
                                                " "  + Node1->getName());
                        
                        currWaypointNode = Node1;
                    }
                }
            }

        }

        if (pCameraEx)
            pCameraEx->mousePressed(e, id);
        if (chara)
            chara->mousePressed(e, id);

    }

    void DebugState::keyPressed(const OIS::KeyEvent &e)
    {
        if (e.key == OIS::KC_ESCAPE)
            Root::GetInstance().RequestShutdown();
        if (e.key == OIS::KC_F2)
        {
            if (pPhysicMgr->isShowingDebugLines())
                pPhysicMgr->showDebugLines(false);
            else
                pPhysicMgr->showDebugLines(true);
        }
        if (e.key == OIS::KC_F3)
        {
            if (pSceneMgr->getShowBoundingBoxes())
                pSceneMgr->showBoundingBoxes(false);
            else
                pSceneMgr->showBoundingBoxes(true);
        }
        if (e.key == OIS::KC_F4)
        {
            if (bHideAllModels)
            {
                Ogre::SceneNode::ChildNodeIterator it = pSceneMgr->getRootSceneNode()->getChildIterator();
                SceneNode *n;
                while (it.hasMoreElements())
                {
                    n = (SceneNode *)it.getNext();
                    n->setVisible(true);
                }

                bHideAllModels = false;
            }
            else   
            {
                Ogre::SceneNode::ChildNodeIterator it = pSceneMgr->getRootSceneNode()->getChildIterator();
                SceneNode *n;
                while (it.hasMoreElements())
                {
                    n = (SceneNode *)it.getNext();
                    n->setVisible(false);
                }

                bHideAllModels = true;
            }
        }
        if (e.key == OIS::KC_F9)
        {
            if(pCameraEx)
                pCameraEx->changeViewModeToFree();
        }
        if (e.key == OIS::KC_F10)
        {
            if (pCameraEx)
                pCameraEx->changeViewModeToDebug();
        }

        if (e.key == OIS::KC_F12)
        {
            pGUI->setVisiblePointer(!pGUI->isVisiblePointer());
        }

        if (e.key == OIS::KC_RSHIFT)
            setEditMode(EDIT_ADD_WAYPOINT);
        if (e.key == OIS::KC_LCONTROL)
            setEditMode(EDIT_LINK_WAYPOINT);
        if (e.key == OIS::KC_ADD)
        {
            //pAIManager->add(BOT_DRIVER);
            pAIManager->add(BOT_PEDESTRIAN);
            pEntityText->setCaption(TO_STRING(pAIManager->getBotCount())); 
        }
        if (e.key == OIS::KC_MINUS)
        {
            pAIManager->add(BOT_DRIVER);
            //pAIManager->add(BOT_PEDESTRIAN);
            pEntityText->setCaption(TO_STRING(pAIManager->getBotCount())); 
        }

        if (e.key == OIS::KC_SPACE)
        {
            WPGraph->writeGraphToFile(WPGraph->GetName() + TO_STRING(graphNum));
            ++graphNum;
        }
        if (e.key == OIS::KC_F5)
            if (pWidget->isVisible())
                pWidget->setVisible(false);
            else
                pWidget->setVisible(true);
        if (e.key == OIS::KC_F6)
        {
            Ogre::CompositorManager::getSingleton().addCompositor(pCamera->getViewport(), "Glow");
            Ogre::CompositorManager::getSingleton().addCompositor(pCamera->getViewport(), "Bloom");
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(pCamera->getViewport(), "Glow", true);
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(pCamera->getViewport(), "Bloom", true);
        }

        if (e.key == OIS::KC_NUMPAD7)
        {
/*
            Ogre::Quaternion q;
            q = pAngleTestNode->getOrientation();
            pAngleTestNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(10.0f));*/

            testRagdoll->addForce();
        }

        if (e.key == OIS::KC_NUMPAD9)
        {
            if (testRagdoll)
                testRagdoll->SetRagDoll(!testRagdoll->isRagdoll());

            if (chara)
                chara->executeRagdoll(!chara->isRagdoll());
        }

        if (e.key == OIS::KC_NUMPAD3)
        {
            pAIManager->killBots(true);
        }

        if (e.key == OIS::KC_NUMPAD2)
        {
            pAIManager->killBots(false);
        }

       /* if (e.key == OIS::KC_RETURN)
        {
            pCamera1 = pSceneMgr->createCamera("ExtendedCamera1");
            pCamera1->setPosition(Ogre::Vector3(0, 0, 0));
            pCamera1->lookAt(Ogre::Vector3(0, 0, 0));

            pCamera1->setNearClipDistance(0.1f);
            pCamera1->setFarClipDistance(0);
            pCamera1->setUseRenderingDistance(true);
            pCamera1->setAutoAspectRatio(true);
            pCamera1->setProjectionType(Ogre::PT_PERSPECTIVE);

            Ogre::Viewport *pVp = Root::GetInstance().GetRenderWindow()->addViewport(pCamera1,1,0,0.5,1,1);
            pVp->setBackgroundColour(Ogre::ColourValue::Black);
            pCamera->getViewport()->setDimensions(0,0,1,0.5);
            pCameraEx1 = new CameraEx("123",pCamera1);
        }*/
        if (pCameraEx)
            pCameraEx->keyPressed(e);
        if (chara)
            chara->keyPressed(e);
    }

    void DebugState::keyReleased(const OIS::KeyEvent &e)
    {
        if (e.key == OIS::KC_RSHIFT)
            setEditMode(EDIT_NONE);
        if (e.key == OIS::KC_LCONTROL)
            setEditMode(EDIT_NONE);

        if (pCameraEx)
            pCameraEx->keyReleased(e);
        if (chara)
            chara->keyReleased(e);
    }

    void DebugState::povMoved(const OIS::JoyStickEvent &e, int pov)
    {
                LOG("povMoved ",pov);
    }
    void DebugState::axisMoved(const OIS::JoyStickEvent &e, int axis)
    {
                LOG("axisMoved ",axis);
    }
    void DebugState::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
    {
        LOG("sliderMoved ",sliderID);
    }
    void DebugState::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {
        LOG("buttonPressed ",button);
    }
    void DebugState::buttonReleased(const OIS::JoyStickEvent &e, int button)
    {

    }

    bool DebugState::frameStarted(const Ogre::FrameEvent &evt)
    {
       // LOGS("frameStarted");
        pAIManager->simulate(evt.timeSinceLastFrame);
       // LOGS("before physic");
        pPhysicMgr->simulate(evt.timeSinceLastFrame);
        if (car)
            car->updateVehicle(evt.timeSinceLastFrame);
       // LOGS("before pCameraEx");
        if(pCameraEx)
            pCameraEx->simulate(evt.timeSinceLastFrame);
        if(pCameraEx1)
            pCameraEx1->simulate(evt.timeSinceLastFrame);

        if (testRagdoll)
            testRagdoll->updateRagdoll(evt.timeSinceLastFrame);

     //   LOGS("BEfore update chara");
        if (chara)
            chara->addTime(evt.timeSinceLastFrame);

      //  LOGS("After update chara");
     /*   if (Root::GetInstance().GetKeyboard()->isKeyDown(OIS::KC_UP))
        {
            if (pEngineSolver->Throttle < 1)
                pEngineSolver->Throttle += 0.001;
            pEngineSolver->Progress(0,evt.timeSinceLastFrame);
        }
        else
        {
            if (pEngineSolver->Throttle > 0.0001)
                pEngineSolver->Throttle -= 0.001;
            pEngineSolver->Progress(0,evt.timeSinceLastFrame);
        }*/



        return true;
    }

    bool DebugState::frameEnded(const Ogre::FrameEvent &evt)
    {
      /*  pEntityText->setCaption("Axis0: "  + TO_STRING((float)Root::GetInstance().GetJoystick(1)->getJoyStickState().mAxes[0].abs/Root::GetInstance().GetJoystick(1)->MAX_AXIS) + "\n" +
            "Axis1: "  + TO_STRING((float)Root::GetInstance().GetJoystick(1)->getJoyStickState().mAxes[1].abs/Root::GetInstance().GetJoystick(1)->MAX_AXIS) + "\n" +
            "Axis2: "  + TO_STRING((float)Root::GetInstance().GetJoystick(1)->getJoyStickState().mAxes[2].abs/Root::GetInstance().GetJoystick(1)->MAX_AXIS) + "\n" +
            "Axis3: "  + TO_STRING((float)Root::GetInstance().GetJoystick(1)->getJoyStickState().mAxes[3].abs/Root::GetInstance().GetJoystick(1)->MAX_AXIS) + "\n" +
            "Axis4: "  + TO_STRING((float)Root::GetInstance().GetJoystick(1)->getJoyStickState().mAxes[4].abs/Root::GetInstance().GetJoystick(1)->MAX_AXIS) + "\n" ); //+ "\n"
            "torque: "  + TO_STRING(car->getMotor()->getTorque()) + "\n" +
            "Currgear: "  + TO_STRING(car->getGears()->curGear) + "\n" +
            "WRPS: "  + TO_STRING(car->wrpm) + "\n" +
            "Speed: "  + TO_STRING(toOgre(car->getActor()->getLinearVelocity()).length()) + "\n" +
            "toWheelTorq: "  + TO_STRING(car->getMotor()->getTorque()));
        OverlayElement* element = OverlayManager::getSingleton().getOverlayElement("Tahometr/Needle");
        TextureUnitState *mNeedleTexture=element->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        mNeedleTexture->setTextureRotate(Degree(-(car->getMotor()->getRps()/37)));
        element = OverlayManager::getSingleton().getOverlayElement("Speedometr/Needle");
        mNeedleTexture = element->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        mNeedleTexture->setTextureRotate(Degree(-(toOgre(car->getActor()->getLinearVelocity()).length()*1000 / 60)*0.34));
        */
        try
        {
//             Ogre::Vector3 vec = pAngleTestNode->getPosition();
//             float x = Ogre::Math::Sin(vec.angleBetween(Ogre::Vector3(0,0,0)));
            const RenderTarget::FrameStats& stats = Root::GetInstance().GetRenderWindow()->getStatistics();
            pFpsText->setCaption("CurrFps: " + TO_STRING(stats.lastFPS) + "\n" +
                                   "TriangleCount: " + TO_STRING(stats.triangleCount) /*+ "\n" +
                                   "Sin = " + TO_STRING(x)*/);
        }
        catch(...) {}

        return true;
    }

    void DebugState::Hide()
    {
        pGUI->setVisiblePointer(true);
        pWidget->setVisible(false);
    }

    void DebugState::Enter()
    {
        mEditMode  = EDIT_NONE;
        pCamera = Root::GetInstance().GetCamera();
        //pCameraEx = new CameraEx("FreeCamera", pCamera);
        pSceneMgr = Root::GetInstance().GetSceneManager();
        pGUI = Root::GetInstance().GetGui();
        loadGui();
        pSceneMgr->setSkyBox(true, "Sky/EveningSky", 500);
        pSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
        pSceneMgr->setShadowFarDistance(10000);

        // set shadow properties
        pSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
        pSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
        pSceneMgr->setShadowTextureSize(1024);
        pSceneMgr->setShadowTextureCount(1);

        //pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
        Ogre::Light* pointLight = pSceneMgr->createLight("pointLight");


        sLevelLoader.LoadLevelFromdotScene("levels/level_0.scene");

        pointLight->setCastShadows(true);
        pointLight->setType(Ogre::Light::LT_POINT);
        pointLight->setPosition(227, 1100, 1100);
        pointLight->setDirection(pSceneMgr->getEntity("start_point")->getParentSceneNode()->getPosition());
        pointLight->setDiffuseColour(1, 0.63, 0.41);
        pointLight->setSpecularColour(1.0, 1.0, 1.0);
        //pointLight->setLightMask(LIGHT_MASK_1);

        /*pointLight->setCastShadows(true);
        pointLight->setType(Ogre::Light::LT_POINT);
        pointLight->setPosition(0, 0, 0);
        pointLight->setDiffuseColour(1, 0.63, 0.41);
        pointLight->setSpecularColour(0.0, 0.0, 0.0);*/



        pPhysicMgr = new PhysicManager(pSceneMgr);
       // car = createCar(toNx(pSceneMgr->getEntity("start_point")->getParentSceneNode()->getPosition()), true, false);
        
      //  pCameraEx->connect(car);


        pPhysicMgr->createTriangleMesh(pSceneMgr->getEntity("terrain"));
        pSceneMgr->getEntity("terrain")->setCastShadows(false);
        
      //  pSceneMgr->getEntity("terrain")->setLightMask(LIGHT_MASK_0);

        pAIManager = new AIManager(pSceneMgr, pPhysicMgr);

        pWaypointMgr = new WPManager("WPMgr");
        pWaypointMgr->setVisualDebug(true);
        WPGraph = pWaypointMgr->CreateGraph("EditWPGraph");

        //pWaypointMgr->loadGraphFromFile("TrafficGraph_0.txt","test");
        //Ogre::CompositorManager::getSingleton().addCompositor(pCamera->getViewport(), "Glow");
       // Ogre::CompositorManager::getSingleton().setCompositorEnabled(pCamera->getViewport(), "Glow", true);

        pEngineSolver = new EngineSolver();
    //    Overlay* mTahometrOverlay = OverlayManager::getSingleton().getByName("Tahometr");
     //   mTahometrOverlay->show();
    //    Overlay* mSpeedometrOverlay = OverlayManager::getSingleton().getByName("Speedometr");
    //    mSpeedometrOverlay->show();

        //ragdoll = pSceneMgr->createEntity("ragdoll", "Body01.mesh");
        //ragdollNode = pSceneMgr->getRootSceneNode()->createChildSceneNode("ragdollNode");
       // ragdollNode->attachObject(ragdoll);
       // testRagdoll = new Ragdoll(pPhysicMgr);
        //testRagdoll->loadMesh("Body01.mesh");
       // testRagdoll->init();
      //  testRagdoll->SetRagDoll(false);
       // testRagdoll->init();    

       // pCameraEx->connect();


        chara = new Character(pCamera, pPhysicMgr, pSceneMgr->getEntity("start_point")->getParentSceneNode()->getPosition());

        LOGS("CHARA CREATED!!");

//         pAngleTestEnt = pSceneMgr->createEntity("AngleTest","vaz_01.mesh");
//         pAngleTestNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
//         pAngleTestNode->attachObject(pAngleTestEnt);
//         pAngleTestNode->setPosition(0,0,10);

    }

    void DebugState::Restore()
    {
    }

    void DebugState::loadGui()
    {
        pWidget = pGUI->findWidget<MyGUI::Widget>("debug.layout");

        pFpsText = pGUI->findWidget<MyGUI::StaticText>("fps");
        pEntityText = pGUI->findWidget<MyGUI::StaticText>("Entity");
        pPointPosText = pGUI->findWidget<MyGUI::StaticText>("pointPos");

        pWidget->setVisible(true);
    }

    void DebugState::setEditMode(EditModes mode)
    {
        if (mode == EDIT_ADD_WAYPOINT)
        {
            mEditMode = mode;

            Entity *e;
            currWaypointNode = pSceneMgr->getRootSceneNode()->createChildSceneNode("WPNode_" + TO_STRING(WPGraph->getNodesCount()));
            e = pSceneMgr->createEntity("WPNode_" + TO_STRING(WPGraph->getNodesCount()),"WP.mesh");
            e->setQueryFlags(WAYPOINT_MASK);
            currWaypointNode->attachObject(e); 
        }

        if (mEditMode == EDIT_ADD_WAYPOINT && mode == EDIT_NONE)
        {
            mEditMode = mode;

            Entity *e;
            e = static_cast<Entity*>(currWaypointNode->getAttachedObject(0));
            pSceneMgr->destroyMovableObject(e);
            pSceneMgr->destroySceneNode(currWaypointNode);
        }

        if (mode == EDIT_LINK_WAYPOINT)
        {
            mEditMode = mode;
        }

        if (mode == EDIT_NONE)
        {
            mEditMode = mode;
        }
    }

    Vehicle* DebugState::createCar(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive)
    {
        Ogre::Entity *pBodyModel;
        pBodyModel = pSceneMgr->createEntity("carBody","vaz_01.mesh");
        pSceneMgr->getRootSceneNode()->createChildSceneNode("CarBody")->attachObject(pBodyModel);
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
        vehicleDesc.digitalSteeringDelta = 0.01f;
        vehicleDesc.steeringMaxAngle = 30.f;
        vehicleDesc.motorForce = 3500.f;
        vehicleDesc.maxBrakeForce = 300.f;
        vehicleDesc.cameraDistance = 8.0f;
        vehicleDesc.maxVelocity = 800.f;
        vehicleDesc.differentialRatio = 3.42f;
        vehicleDesc.centerOfMass.set(0,-1,0);

        vehicleDesc.sWheelmeshName = "wheel.mesh";

        MotorDesc motorDesc;
        GearDesc gearDesc;
        NxReal wheelRadius;

        motorDesc.loadSettingsFromFile("VehicleConfigPlayer1.cfg");
        vehicleDesc.motorDesc = &motorDesc;
        gearDesc.loadSettingsFromFile("VehicleConfigPlayer1.cfg");
        vehicleDesc.gearDesc = &gearDesc;

        wheelRadius = 0.5f;

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
        NxReal widthPos = 1.09f;
        NxReal heightPos = -0.4f;
        wheelDesc[0].position.set( 1.8f, heightPos, widthPos);
        wheelDesc[1].position.set( 1.8f, heightPos,-widthPos);
        wheelDesc[2].position.set(-1.5f, heightPos, widthPos);
        wheelDesc[3].position.set(-1.5f, heightPos,-widthPos);
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

    bool DebugState::RaycastToPolygon(Ogre::Ray &rayptr, Ogre::Vector3 &result)
    {
        Ogre::RaySceneQuery* mRayScnQuery;
        mRayScnQuery = pSceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
        mRayScnQuery->setSortByDistance(true);

        Ogre::Ray ray(rayptr);

        if (mRayScnQuery != NULL)
        {
            mRayScnQuery->setRay(ray);

            if (mRayScnQuery->execute().size() <= 0)
            {
                return (false);
            }
        }

        Ogre::Real closest_distance = -1.0f;
        Ogre::Vector3 closest_result;
        Ogre::Entity *closest_entity;
        Ogre::Entity *pentity;
        Ogre::RaySceneQueryResult &query_result = mRayScnQuery->getLastResults();
        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            if ((closest_distance >= 0.0f) &&
                (closest_distance < query_result[qr_idx].distance))
            {
                break;
            }

            if ((query_result[qr_idx].movable != NULL) &&
                (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
            {
                pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);           
                
                size_t vertex_count;
                size_t index_count;
                Ogre::Vector3 *vertices;
                unsigned long *indices;

                GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
                    pentity->getParentNode()->_getDerivedPosition(),
                    pentity->getParentNode()->_getDerivedOrientation(),
                    pentity->getParentNode()->_getDerivedScale());

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;
                for (int i = 0; i < static_cast<int>(index_count); i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
                        vertices[indices[i+1]], vertices[indices[i+2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first)
                    {
                        if ((closest_distance < 0.0f) ||
                            (hit.second < closest_distance))
                        {
                            // this is the closest so far, save it off
                            closest_distance = hit.second;
                            new_closest_found = true;
                        }
                    }
                }

                // free the verticies and indicies memory
                delete[] vertices;
                delete[] indices;

                // if we found a new closest raycast for this object, update the
                // closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    closest_entity = pentity;
                    closest_result = ray.getPoint(closest_distance);               
                }
            }       
        }

        // return the result
        if (closest_distance >= 0.0f)
        {
            selectedEntity = closest_entity;
            // raycast success
            result = closest_result;
            return (true);
        }
        else
        {
            // raycast failed
            return (false);
        } 
        pSceneMgr->destroyQuery(mRayScnQuery);
    }

    void DebugState::GetMeshInformation(const Ogre::MeshPtr mesh,
        size_t &vertex_count,
        Ogre::Vector3* &vertices,
        size_t &index_count,
        unsigned long* &indices,
        const Ogre::Vector3 &position,
        const Ogre::Quaternion &orient,
        const Ogre::Vector3 &scale)
    {
        bool added_shared = false;
        size_t current_offset = 0;
        size_t shared_offset = 0;
        size_t next_offset = 0;
        size_t index_offset = 0;

        vertex_count = index_count = 0;

        // Calculate how many vertices and indices we're going to need
        for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh( i );

            // We only need to add the shared vertices once
            if(submesh->useSharedVertices)
            {
                if( !added_shared )
                {
                    vertex_count += mesh->sharedVertexData->vertexCount;
                    added_shared = true;
                }
            }
            else
            {
                vertex_count += submesh->vertexData->vertexCount;
            }

            // Add the indices
            index_count += submesh->indexData->indexCount;
        }

        vertices = new Ogre::Vector3[vertex_count];
        indices = new unsigned long[index_count];

        added_shared = false;

        for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh(i);

            Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

            if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
            {
                if(submesh->useSharedVertices)
                {
                    added_shared = true;
                    shared_offset = current_offset;
                }

                const Ogre::VertexElement* posElem =
                    vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

                Ogre::HardwareVertexBufferSharedPtr vbuf =
                    vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

                unsigned char* vertex =
                    static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                float* pReal;

                for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);

                    Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                    vertices[current_offset + j] = (orient * (pt * scale)) + position;
                }

                vbuf->unlock();
                next_offset += vertex_data->vertexCount;
            }


            Ogre::IndexData* index_data = submesh->indexData;
            size_t numTris = index_data->indexCount / 3;
            Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

            bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

            unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


            size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
            size_t index_start = index_data->indexStart;
            size_t last_index = numTris*3 + index_start;

            if (use32bitindexes)
                for (size_t k = index_start; k < last_index; ++k)
                {
                    indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset );
                }

            else
                for (size_t k = index_start; k < last_index; ++k)
                {
                    indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) +
                        static_cast<unsigned long>( offset );
                }

                ibuf->unlock();
                current_offset = next_offset;
        }
    }

    void DebugState::preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {
    }
    void DebugState::postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {

    }
}