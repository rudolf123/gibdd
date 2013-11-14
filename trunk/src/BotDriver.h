#ifndef BotDriverH
#define BotDriverH

namespace Gibdd
{
    class Vehicle;
    class WPManager;
    class PhysicManager;
    class Graph;

    class BotDriver
    {
        public:
            BotDriver(Ogre::SceneManager* mgr, PhysicManager *PhysMgr, WPManager *WpMgr);
            ~BotDriver();

            void create(const std::string& name, const std::string mesh_name, const NxVec3& pos, const std::string& filename);
            void loadWaypoints();

            void simulate(float diff);

        private:

            void updateDirection(Ogre::Vector3& mDirection);
            void buildPathToNextPoint(Ogre::Vector3& destination);
            void moveToNextWaypoint(float diff);



            Vehicle *pCar;
            Ogre::SceneManager* pSceneMgr;
            PhysicManager *pPhysicMgr;
            WPManager *pWaypointMgr;
            Graph* mPathGraph;

            bool bMovementComplete;
            Ogre::Vector3 mFinishOffset;

            float accelerate;
            float brake;
            float steer;

            float sqrLenPrev;
    };

}

#endif