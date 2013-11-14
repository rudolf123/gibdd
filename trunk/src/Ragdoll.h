#ifndef RagdollH
#define RagdollH

namespace Gibdd
{
    class PhysicManager;

    struct PHYSX_BONE
    {
    public:
        PHYSX_BONE(NxActor* Actor, Ogre::Bone* Bone);
        void SetOgreBone(Ogre::Bone* pBone);
        void SetPhysxBone(NxActor* Actor);

        NxActor* pPhysxBone;
        Ogre::Quaternion PhysxBindOrientationInverse;
        Ogre::Bone* pOgreBone;
        Ogre::Quaternion OgreGlobalBindOrientation;
    };

    class Ragdoll
    {
    public:
        Ragdoll(/*Ogre::SceneManager *ScnMgr,*/ PhysicManager *PhysMgr);
        ~Ragdoll();

        //void loadMesh(std::string meshName);
        void init();
        void updateRagdoll(float delta);
        void SetRagDoll(bool bRagDoll);
        bool isRagdoll() {return m_bRagDoll;};
        void addForce();
        void setMesh(Ogre::Entity *mesh){ pMesh = mesh;};
        void setNode(Ogre::SceneNode *node){ pNode = node;};
        void setRagdollScale(float scale){ fRagDollScale = scale;};
        float getRagdollScale(){ return fRagDollScale;};

    protected:

        void FitMeshToRagDoll();
        void FitRagDollToMesh();

    protected:

        Ogre::Entity *pMesh;
        Ogre::SceneNode *pNode;
        //Ogre::SceneManager *pScnMgr;
        PhysicManager *pPhysMgr;

        float fRagDollScale;

        bool bPhysControlled;
        bool m_bRagDoll;

        std::vector<PHYSX_BONE> m_RagDollBones;
        std::vector<NxJoint*> m_RagDollJoints;


        NxActor* head;
        NxActor* torso;
        NxActor* pelvis;
        NxActor* leftUpperArm;
        NxActor* rightUpperArm;
        NxActor* leftForeArm;
        NxActor* rightForeArm;
        NxActor* leftHand;
        NxActor* rightHand;
        NxActor* leftThigh;
        NxActor* rightThigh;
        NxActor* leftCalf;
        NxActor* rightCalf;
        NxActor* leftFoot;
        NxActor* rightFoot;

        NxSphericalJoint* neck;
        NxSphericalJoint* leftShoulder;
        NxSphericalJoint* rightShoulder;
        NxSphericalJoint* spine;
        NxSphericalJoint* leftHip;
        NxSphericalJoint* rightHip;

        NxRevoluteJoint* leftElbow;
        NxRevoluteJoint* rightElbow;
        NxRevoluteJoint* leftWrist;
        NxRevoluteJoint* rightWrist;
        NxRevoluteJoint* leftKnee;
        NxRevoluteJoint* rightKnee;
        NxRevoluteJoint* leftAnkle;
        NxRevoluteJoint* rightAnkle;

        std::vector<Ogre::AnimationState*> prevAnimSet;
        Ogre::AnimationState *prevAnim;
    };
}
#endif