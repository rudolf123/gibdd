#ifndef PhysicManagerH
#define PhysicManagerH

#include "DynamicLines.h"
#include "CharacterController.h"
#include "UserAllocator.h"

namespace Gibdd
{
    using namespace Ogre;

    inline Ogre::Vector3 toOgre(const NxVec3 &v) {
   
        return Ogre::Vector3(v.x, v.y, v.z);
    }
    inline NxVec3 toNx(const Vector3 &v) {
    
        return NxVec3(v.x, v.y, v.z);
    }
    inline Ogre::Quaternion toOgre(const NxQuat &q) {
    
        return Ogre::Quaternion(q.w, q.x, q.y, q.z);
    }
    inline NxQuat toNx(Quaternion& q, bool _normalise = true) {
    
        if (_normalise)    q.normalise();
        NxQuat a;    a.x = q.x;    a.y = q.y;    a.z = q.z;    a.w = q.w;
    
        return a;
    }

    class PhysicManager
    {
        public: 
            PhysicManager(Ogre::SceneManager *sceneMgr);
            ~PhysicManager();

            void initializePhysX();
            void createPhysicScene();
            NxActor* CreateCapsule(NxVec3 pos, float height, float radius, float density);
            NxActor* CreateCapsule(float height, float radius);
            NxActor* CreateBox(NxVec3 dim, NxVec3 pos, NxQuat orient);
            NxActor* CreateBox(NxVec3 pos, NxVec3 dim, float density);
            NxActor* CreateSphere(const NxVec3& pos, const NxReal radius, const NxReal density);
            NxSphericalJoint* CreateBodySphericalJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis);
            NxRevoluteJoint* CreateRevoluteJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis);
            NxRevoluteJoint* CreateRevoluteJoint2(NxActor* a0, NxActor* a1, const NxVec3& localAnchor0,const NxVec3& localAnchor1, const NxVec3& localAxis0, const NxVec3& localAxis1);


            void simulate(float time);

            NxScene* getPxScene(){ return PxScene; }
   
            void showDebugLines(bool show);
            bool isShowingDebugLines(){ return bDebugEnable;};

            NxActor* createTriangleMesh(Entity *e);
            CharacterController* CreateCharacterController(Vector3 position,  float height, float radius);
            void SetActorCollisionGroup(NxActor *actor, NxCollisionGroup group);


        private:
            void DebugRender(const NxDebugRenderable&  data);

        private:    
            NxPhysicsSDK*    PxSDK;
            NxScene*         PxScene;
            SceneManager*    mSceneMgr;
            NxRevoluteJoint* revJoint;
            NxActor*         SelectedActor;
            NxVec3           gForceVec;

            NxControllerManager*    pControllerManager; 
            UserAllocator*          allocator;

            //debug render
            DynamicLines *lines;
            SceneNode *linesNode;
            bool bDebugEnable;
    };

}
#endif