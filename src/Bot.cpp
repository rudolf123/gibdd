#include "Precompiled.h"
#include "Bot.h"

namespace Gibdd
{
    class CharacterController;
    class WPManager;

    Bot::Bot(Ogre::SceneManager* mgr, PhysicManager* PhysMgr, WPManager *WpMgr, SimaforeSystem *simafore):
    Ragdoll(PhysMgr),
    pEntity(NULL),
    pMainNode(NULL),
    pController(NULL),
    speedX(NULL), 
    speedY(NULL), 
    speedZ(NULL),
    mYaw(NULL),
    bMovementComplete(true),
    pSceneMgr(mgr),
    pPhysicMgr(PhysMgr),
    pWaypointMgr(WpMgr),
    bWait(true),
    pSimaforeMgr(simafore)
    {
    }

    Bot::~Bot()
    {
        pSceneMgr->destroyMovableObject(pEntity);
        pSceneMgr->destroySceneNode(pMainNode);
        
        if (pController)
            delete pController;

        if(mActionMgr)
            delete mActionMgr;
    }

    void Bot::create(const std::string& name, const std::string mesh_name, Ogre::Vector3 pos)
    {
        pEntity = pSceneMgr->createEntity(name, mesh_name);

        //pEntity->setCastShadows(true);
        pEntity->setQueryFlags(Ogre::SceneManager::STATICGEOMETRY_TYPE_MASK);

        pMainNode = pSceneMgr->getRootSceneNode()->createChildSceneNode(name + "_Node");
        pMainNode->attachObject(pEntity);
        pMainNode->setVisible(true);

        pMainNode->setPosition(pos);
        pMainNode->setScale(0.006,0.006,0.006); /////////////fix later
        
        LOGS("setMesh(pEntity);");
        setMesh(pEntity);
        LOGS("setNode(pMainNode);");
        setNode(pMainNode);
        LOGS("init();");
        init();
        LOGS("init();");
        //setRagdollScale(0.2);

        pController = pPhysicMgr->CreateCharacterController(pos, 0.5, 0.5);
        mActionMgr = new ActionManager(pEntity);
        mActionMgr->init("AnimsConfig.txt");
        mCurAnimation = RUNF;
        mActionMgr->DoAction(mCurAnimation);

        mPathGraph = pWaypointMgr->loadGraphFromFile("graph.txt", name);
        //int point1 = 45;
        //int point2 = 75;
        //LOG("Point1 ", point1);
        //LOG("Point2 ", point2);
        while (true)
        {
            int point1 = random_int(0,mPathGraph->getNodesCount() - 1);
            int point2 = random_int(0,mPathGraph->getNodesCount() - 1);
           // LOG("Point1 ", point1);
           // LOG("Point2 ", point2);
            if (point1 == 45 || point2 == 45)
                continue;
            if (mPathGraph->findPath(point1, point2, PFM_ASTAR))
                break;

        }
        Vector3 startpos = getNextPoint();
        startpos.y += pController->GetCharacterHeight()+1; 
        pController->SetCharacterPos(toNx(startpos));

        //shit
        bWasStoppedbyLight = false;
    }

    void Bot::simulate(float diff)
    {
        if (!m_bRagDoll)
        {
            if (bWait)
            {
                if (pSimaforeMgr->GetCurrPedColor() == GREEN)
                {
                    bWait = false;
                    mActionMgr->DoAction(RUNF);
                    return;
                }

                mActionMgr->addTime(diff);
                return;
            }

            if(!bMovementComplete)
            {
                bWait = false;

                moveToNextWaypoint(diff);
                useRayCasting();
                mActionMgr->addTime(diff);
                return;
            }

            mActionMgr->addTime(diff);
            buildPathToNextPoint(getNextPoint());    

            return;
        }

        updateRagdoll(diff);
    }

    void Bot::updateDirection(Ogre::Vector3& mDirection, float diff)
    {
        Vector3 orient = pMainNode->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
        Vector3 dir = mDirection;
        dir.y = 0;

        Ogre::Quaternion quat = orient.getRotationTo(dir);
        pMainNode->yaw(quat.getYaw());
    }

    void Bot::buildPathToNextPoint(Ogre::Vector3& destination)
    {
        bMovementComplete = false;

        if (mFinishOffset == destination)
        {
            while (true)
            {
                int point1 = mPathGraph->getFinishNode();
                int point2 = random_int(0,mPathGraph->getNodesCount() - 1);
                if (point1 == 45 || point2 == 45)
                    continue;
                if (mPathGraph->findPath(point1, point2, PFM_ASTAR))
                    break;
            }

            return;
        }

        mFinishOffset = destination;

        float x;
        float z;
        if ((destination.x > pMainNode->getPosition().x) || (destination.z > pMainNode->getPosition().z))
        {
            x = (destination.x - pMainNode->getPosition().x);
            z = (destination.z - pMainNode->getPosition().z);

            if(x > z)
            {
                speedZ = z / x;
                speedX = 1.0f;
            }
            else
            {
                speedX = x / z;
                speedZ = 1.0f;
            }

        }
    }

    void Bot::moveToNextWaypoint(float diff)
    {
        if(bMovementComplete)
        {
            return;
        }

        int X_abs = abs(pMainNode->getPosition().x - mFinishOffset.x);
        int Z_abs = abs(pMainNode->getPosition().z - mFinishOffset.z);

        if(X_abs < 3)
            speedX = 0;

        if(Z_abs < 3)
            speedZ = 0;

        if(X_abs < 3 && Z_abs < 3)
        {
            speedX = 0.0f;
            speedZ = 0.0f;
            bMovementComplete = true;
            return;
        }

        updateDirection(mFinishOffset - pMainNode->getPosition(), diff);

        NxVec3 ControllerDirection = toNx(mFinishOffset - pMainNode->getPosition());

        //ControllerDirection.y = -98;
        ControllerDirection.normalize();

        NxU32 u32;
        pController->Move(ControllerDirection*diff, u32);
        pMainNode->setPosition(toOgre(NxVec3(pController->GetCharacterPos().x, pController->GetCharacterPos().y+0.5, pController->GetCharacterPos().z)));
    }

    Vector3 Bot::getNextPoint()
    {
        return  mPathGraph->getNextPointOfPath();
    }

    void Bot::loadWaypoints()
    {
        
    }

    bool Bot::useRayCasting()
    {
        Ogre::RaySceneQuery* query = pSceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
        Ogre::Vector3 ourpos = pMainNode->getPosition();
        Ogre::Ray ray(Ogre::Vector3(ourpos.x, ourpos.y, ourpos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);

        query->clearResults();
        query->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
        query->setRay(ray);
        query->setSortByDistance(true);
        Ogre::RaySceneQueryResult &query_result = query->execute();
        Ogre::RaySceneQueryResult::iterator itr = query_result.begin();

        for (; itr != query_result.end();  ++itr )
        {
            if ((itr->movable) && (itr->distance<10))
            {
                Entity *pentity = static_cast<Ogre::Entity*>(itr->movable);

                std::string name = pentity->getName();
                size_t found = name.find("simaforezone");

                if (found != std::string::npos)
                {
                    if (pSimaforeMgr->GetCurrPedColor() == RED && bWasStoppedbyLight)
                        bWasStoppedbyLight = false;
                    else
                        if (pSimaforeMgr->GetCurrPedColor() == RED && !bWasStoppedbyLight)
                        {
                            bWait = true;
                            bWasStoppedbyLight = true;
                            mActionMgr->DoAction(WAIT);
                        }
                }
            }
        }
        pSceneMgr->destroyQuery(query);
        return true;
    }
}