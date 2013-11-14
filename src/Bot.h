#ifndef BotH
#define BotH
#include "CharacterController.h"
#include "PhysicManager.h"
#include "WPManager.h"
#include "ActionManager.h"
#include "SimaforeSystem.h"
#include "Ragdoll.h"

namespace Gibdd
{
    class PhysicManager;
    class SimaforeSystem;
    class Ragdoll;

    class Bot: public Ragdoll
    {
        public:
            Bot(Ogre::SceneManager* mgr, PhysicManager* PhysMgr, WPManager *WpMgr, SimaforeSystem *simafore);
            ~Bot();

            void create(const std::string& name, const std::string mesh_name, Ogre::Vector3 pos);
            void loadWaypoints();

            void simulate(float diff);

            void setPosition(Ogre::Vector3 vec) {pController->SetCharacterPos(toNx(vec));};
            Vector3 getPosition() { return toOgre(pController->GetCharacterPos());};

            void moveToNextWaypoint(float diff);
            void buildPathToNextPoint(Ogre::Vector3& destination);

            void updateDirection(Ogre::Vector3& mDirection, float diff);

            void useOpenSteer(bool use) { bUseOpenSteer = use;};
            bool isUsingOpenSteer() { return bUseOpenSteer;};

        private:
            Vector3 getNextPoint();
            bool useRayCasting();

        private:

            Ogre::Entity* pEntity;
            Ogre::SceneNode* pMainNode;
            Ogre::SceneManager* pSceneMgr;
            PhysicManager* pPhysicMgr;
            WPManager *pWaypointMgr;
            Graph* mPathGraph;
            ActionManager *mActionMgr;
            SimaforeSystem *pSimaforeMgr;


            CharacterController *pController;

            float speedX, speedY, speedZ;
            float mYaw;
            bool bMovementComplete;
            Vector3 mFinishOffset;
            bool bUseOpenSteer;
            //Vector3 mNextPoint;


            uint8 currentPoint;
            uint8 currentPath;
            bool debugWP;
            bool bWait;
            bool bWasStoppedbyLight;

            Actions mCurAnimation;

            typedef stdext::hash_map<int, Vector3> Waypoints;
            typedef stdext::hash_map<int, Waypoints> PathMap;
            PathMap PathsMap;
            stdext::hash_map<std::string, Vector3> usedWaypoints;

    };
}
#endif