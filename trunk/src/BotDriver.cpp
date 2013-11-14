#include "Precompiled.h"
#include "BotDriver.h"
#include "Vehicle.h"
#include "WPManager.h"
#include "PhysicManager.h"
#include "Graph.h"

namespace Gibdd
{
    BotDriver::BotDriver(Ogre::SceneManager* mgr, PhysicManager *PhysMgr, WPManager *WpMgr):
    pSceneMgr(mgr),
    pWaypointMgr(WpMgr),
    pPhysicMgr(PhysMgr),
    pCar(0),
    mPathGraph(0),
    bMovementComplete(true),
    steer(0),
    accelerate(0),
    brake(0)
    {

    }

    BotDriver::~BotDriver()
    {

    }

    void BotDriver::create(const std::string& name, const std::string mesh_name, const NxVec3& pos, const std::string& filename)
    {
        bool frontWheelDrive = true;
        bool backWheelDrive = true;
        Ogre::Entity *pBodyModel;
        pBodyModel = pSceneMgr->createEntity(name + "car", mesh_name);
        pSceneMgr->getRootSceneNode()->createChildSceneNode(name + "car")->attachObject(pBodyModel);
        Ogre::AxisAlignedBox aab = pBodyModel->getBoundingBox(); 
        Ogre::Vector3 min = aab.getMinimum();
        Ogre::Vector3 max = aab.getMaximum();
        Ogre::Vector3 center = aab.getCenter();
        Ogre::Vector3 size(fabs(max.x-min.x)/2,fabs(max.y-min.y)/2,fabs(max.z-min.z)/2);

        VehicleDesc vehicleDesc;
        NxBoxShapeDesc boxShape;

        boxShape.dimensions.set(size.x, size.y/2, size.z);
        vehicleDesc.carShapes.pushBack(&boxShape);
        vehicleDesc.pBodyModel = pBodyModel;

        vehicleDesc.joyInput = false;
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

        wheelRadius = 0.4f;

        NxWheelDesc wheelDesc[4];
        for(NxU32 i=0;i<4;i++)
        {
            wheelDesc[i].wheelApproximation = 10;
            wheelDesc[i].wheelRadius = wheelRadius;
            wheelDesc[i].wheelWidth = 0.3f;
            wheelDesc[i].wheelSuspension = 0.4f;
            wheelDesc[i].springRestitution = 7000;
            wheelDesc[i].springDamping = 0;//800;
            wheelDesc[i].springBias = 0.0f;
            wheelDesc[i].maxBrakeForce = 1.f;
            wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
            vehicleDesc.carWheels.pushBack(&wheelDesc[i]);
        }
        NxReal widthPos = 0.9f;
        NxReal heightPos = -0.5f;
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

        pCar = Vehicle::createVehicle(pSceneMgr, pPhysicMgr->getPxScene(), &vehicleDesc);
        NxQuat q;
        q.fromAngleAxis(270.0f, NxVec3(0.0f, 1.0f, 0.0f));
        pCar->getActor()->setGlobalOrientationQuat(q);

        mPathGraph = pWaypointMgr->loadGraphFromFile("TrafficGraph_0.txt", name);
        //mPathGraph->findPath(random_int(0,mPathGraph->getNodesCount() - 1), random_int(0,mPathGraph->getNodesCount() - 1), PFM_ASTAR);
        int num1,num2;
        num1 = random_int(0,mPathGraph->getNodesCount() - 1);
        num2 = random_int(0,mPathGraph->getNodesCount() - 1);
        LOG("PointStart ", num1);
        LOG("Pointsfinish ", num2);
        mPathGraph->findPath(num1, num2, PFM_ASTAR);
        Vector3 startpos = mPathGraph->getNextPointOfPath();
        startpos.y +=5; 
        pCar->getActor()->setGlobalPosition(toNx(startpos));


        pCar->setJoyInput(true);
        sqrLenPrev = 0;

       // pController->SetCharacterPos(toNx(startpos));
        //pCar->gearUp();

    }

    void BotDriver::loadWaypoints()
    {

    }
    void BotDriver::simulate(float diff)
    {

        if(!bMovementComplete)
        {

            moveToNextWaypoint(diff);
           // LOG("steer = ", steer);
            pCar->control(1,0,steer);
            pCar->updateVehicle(diff);
            
            return;
        }
        //Ogre::Vector3 destination = mPathGraph->getNextPointOfPath(); 
       //     LOGS(TO_STRING(destination.x) +";" +TO_STRING(destination.y)+ ";" +TO_STRING(destination.z) );

        buildPathToNextPoint(mPathGraph->getNextPointOfPath());    
    }

    void BotDriver::moveToNextWaypoint(float diff)
    {
        if(bMovementComplete)
        {
            return;
        }

        int X_abs = abs(pCar->getMainNode()->getPosition().x - mFinishOffset.x);
        int Z_abs = abs(pCar->getMainNode()->getPosition().z - mFinishOffset.z);

        if(X_abs < 3 && Z_abs < 3)
        {
            bMovementComplete = true;
            return;
        }

        updateDirection(mFinishOffset - pCar->getMainNode()->getPosition());

        /*NxVec3 ControllerDirection = toNx(mFinishOffset - pMainNode->getPosition());

        ControllerDirection.normalize();

        NxU32 u32;
        pController->Move(ControllerDirection*diff, u32);
        pMainNode->setPosition(toOgre(pController->GetCharacterPos()));*/
    }

    void BotDriver::updateDirection(Ogre::Vector3& mDirection)
    {

        Vector3 orient = toOgre(pCar->getActor()->getGlobalOrientationQuat()) * Vector3::UNIT_X;
        Vector3 dir = mDirection;
      //  float len = (dir - orient).squaredLength();
       
       /* dir.y = 0;
       // orient.y = 0;

        //LOGS("orient.x = " + TO_STRING(orient.x) + "orient.z = " + TO_STRING(orient.z));
        //LOGS("dir.x = " + TO_STRING(dir.x) + "dir.z = " + TO_STRING(dir.z));

        Ogre::Quaternion quat = orient.getRotationTo(dir);*/

        steer= 0;
      //  LOG("rzn  ", );
        //steer = quat.getYaw().valueRadians();
      //  if (abs(len - sqrLenPrev) > 1)

       // LOG("Sin = ", Ogre::Math::Sin(orient.angleBetween(mFinishOffset)));
      //  LOG("COS = ", Ogre::Math::Cos(orient.angleBetween(mFinishOffset)));
        if (Ogre::Math::Abs(Ogre::Math::Sin(dir.angleBetween(mFinishOffset))) != 1.0f)
        {
           // LOGS("123");
            if (pCar->getClosestWheelToPoint(mFinishOffset)==0)
            {
                steer= -1 * Ogre::Math::Sin(dir.angleBetween(mFinishOffset));
/*
                if (steer<-1)
                    steer = -1;*/
            } 
            else          
            {
                steer= 1 * Ogre::Math::Sin(dir.angleBetween(mFinishOffset));
/*
                if (steer>1)
                    steer = 1;*/
            }
        }
      //  LOG("curr  ", len);
       // LOG("prev  ", sqrLenPrev);
        //LOG("prev  ", abs(len - sqrLenPrev));
       // sqrLenPrev = len;
    }

    void BotDriver::buildPathToNextPoint(Ogre::Vector3& destination)
    {
        bMovementComplete = false;

        if (mFinishOffset == destination)
        {
            int num1,num2;
            num1 = mPathGraph->getFinishNode();
            num2 = random_int(0,mPathGraph->getNodesCount() - 1);
            LOG("PointStart ", num1);
            LOG("Pointsfinish ", num2);
            mPathGraph->findPath(num1, num2, PFM_ASTAR);

            return;
        }

        mFinishOffset = destination;
    }
}