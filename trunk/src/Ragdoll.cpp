#include "Precompiled.h"
#include "Ragdoll.h"
#include "PhysicManager.h"

namespace Gibdd
{
    PHYSX_BONE::PHYSX_BONE(NxActor* Actor, Ogre::Bone* Bone)
    {
        SetPhysxBone(Actor);
        SetOgreBone(Bone);
    }

    void PHYSX_BONE::SetOgreBone(Ogre::Bone* pBone)
    {
        pOgreBone = pBone;
        OgreGlobalBindOrientation = pBone->_getDerivedOrientation();
    }

    void PHYSX_BONE::SetPhysxBone(NxActor* Actor)
    {
        pPhysxBone = Actor;
        PhysxBindOrientationInverse = toOgre(Actor->getGlobalOrientationQuat()).Inverse();
    }

    Ragdoll::Ragdoll(/*Ogre::SceneManager *ScnMgr,*/ PhysicManager *PhysMgr):
    /*pScnMgr(ScnMgr),*/
    pPhysMgr(PhysMgr),
    bPhysControlled(false),
    m_bRagDoll(false),
    fRagDollScale(0.3)
    {
        NxQuat qRotLeft, qRotRight, qRotAround;
        qRotLeft.fromAngleAxis(90, NxVec3(0,0,1));
        qRotRight.fromAngleAxis(-90, NxVec3(0,0,1));
        qRotAround.fromAngleAxis(180, NxVec3(0,0,1));

        NxMat33 mRotLeft, mRotRight, mRotAround;
        mRotLeft.fromQuat(qRotLeft);
        mRotRight.fromQuat(qRotRight);
        mRotAround.fromQuat(qRotAround);

        // Create body parts
        head = pPhysMgr->CreateSphere(NxVec3(0,8.8,0) * fRagDollScale, 0.5 * fRagDollScale, 10);
        torso = pPhysMgr->CreateSphere(NxVec3(0,7,0)* fRagDollScale, 0.95* fRagDollScale, 10);
        pelvis = pPhysMgr->CreateSphere(NxVec3(0,5.8,0)* fRagDollScale, 0.7* fRagDollScale, 10);

        leftUpperArm = pPhysMgr->CreateCapsule(NxVec3(0.5,8.5,0)* fRagDollScale, 1* fRagDollScale, 0.4* fRagDollScale, 10);
        leftUpperArm->setGlobalOrientationQuat(qRotRight);
        leftForeArm = pPhysMgr->CreateCapsule(NxVec3(2,8.5,0)* fRagDollScale, 1* fRagDollScale, 0.3* fRagDollScale, 10);
        leftForeArm->setGlobalOrientationQuat(qRotRight);
        leftHand = pPhysMgr->CreateBox(NxVec3(3.5,8.5,0)* fRagDollScale, NxVec3(0.3,0.3,0.1)* fRagDollScale, 10);
        leftHand->setGlobalOrientationQuat(qRotRight);

        rightUpperArm = pPhysMgr->CreateCapsule(NxVec3(-0.5, 8.5, 0)* fRagDollScale, 1* fRagDollScale, 0.4* fRagDollScale, 10);
        rightUpperArm->setGlobalOrientationQuat(qRotLeft);
        rightForeArm = pPhysMgr->CreateCapsule(NxVec3(-2, 8.5, 0)* fRagDollScale, 1* fRagDollScale, 0.3* fRagDollScale, 10);
        rightForeArm->setGlobalOrientationQuat(qRotLeft);
        rightHand = pPhysMgr->CreateBox(NxVec3(-3.5, 8.5, 0)* fRagDollScale, NxVec3(0.3,0.3,0.1)* fRagDollScale, 10);
        rightHand->setGlobalOrientationQuat(qRotLeft);

        leftThigh = pPhysMgr->CreateCapsule(NxVec3(0.6,6,0)* fRagDollScale, 1.5* fRagDollScale, 0.5* fRagDollScale, 10);
        leftThigh->setGlobalOrientationQuat(qRotAround);
        leftCalf = pPhysMgr->CreateCapsule(NxVec3(0.6,3.5,0)* fRagDollScale, 1.5* fRagDollScale, 0.35* fRagDollScale, 10);
        leftCalf->setGlobalOrientationQuat(qRotAround);
        leftFoot = pPhysMgr->CreateBox(NxVec3(0.6,1.5,0.2)* fRagDollScale, NxVec3(0.4,0.2,0.75)* fRagDollScale, 10);
        leftFoot->setGlobalOrientationQuat(qRotAround);

        rightThigh = pPhysMgr->CreateCapsule(NxVec3(-0.6,6,0)* fRagDollScale, 1.5* fRagDollScale, 0.5* fRagDollScale, 10);
        rightThigh->setGlobalOrientationQuat(qRotAround);
        rightCalf = pPhysMgr->CreateCapsule(NxVec3(-0.6,3.5,0)* fRagDollScale, 1.5* fRagDollScale, 0.35* fRagDollScale, 10);
        rightCalf->setGlobalOrientationQuat(qRotAround);
        rightFoot = pPhysMgr->CreateBox(NxVec3(-0.6,1.5,0.2)* fRagDollScale, NxVec3(0.4,0.2,0.75)* fRagDollScale, 10);
        rightFoot->setGlobalOrientationQuat(qRotAround);

        // Joint body parts together
        neck = pPhysMgr->CreateBodySphericalJoint(head,torso,NxVec3(0,8,0)* fRagDollScale,NxVec3(0,1,0));
        leftShoulder = pPhysMgr->CreateBodySphericalJoint(leftUpperArm,torso,NxVec3(0.5,8.5,0)* fRagDollScale,NxVec3(1,0,0));
        rightShoulder = pPhysMgr->CreateBodySphericalJoint(rightUpperArm,torso,NxVec3(-0.5,8.5,0)* fRagDollScale,NxVec3(-1,0,0));
        spine = pPhysMgr->CreateBodySphericalJoint(torso,pelvis,NxVec3(0,6,0)* fRagDollScale,NxVec3(0,-1,0));
        leftHip = pPhysMgr->CreateBodySphericalJoint(leftThigh,pelvis,NxVec3(0.6,6,0)* fRagDollScale,NxVec3(0,-1,0));
        rightHip = pPhysMgr->CreateBodySphericalJoint(rightThigh,pelvis,NxVec3(-0.6,6,0)* fRagDollScale,NxVec3(0,-1,0));

        leftElbow = pPhysMgr->CreateRevoluteJoint(leftForeArm,leftUpperArm,NxVec3(2,8.5,0)* fRagDollScale,NxVec3(0,0,-1));
        rightElbow = pPhysMgr->CreateRevoluteJoint(rightForeArm,rightUpperArm,NxVec3(-2,8.5,0)* fRagDollScale,NxVec3(0,0,-1));

        leftWrist = pPhysMgr->CreateRevoluteJoint2(leftHand,leftForeArm,NxVec3(0,-0.15,0)* fRagDollScale,NxVec3(0,1.3,0)* fRagDollScale,NxVec3(0,1,0),NxVec3(0,1,0));
        rightWrist = pPhysMgr->CreateRevoluteJoint2(rightHand,rightForeArm,NxVec3(0,-0.15,0)* fRagDollScale,NxVec3(0,1.3,0)* fRagDollScale,NxVec3(0,1,0),NxVec3(0,1,0));

        leftKnee = pPhysMgr->CreateRevoluteJoint(leftCalf,leftThigh,NxVec3(0.6,3.5,0)* fRagDollScale,NxVec3(1,0,0));
        rightKnee = pPhysMgr->CreateRevoluteJoint(rightCalf,rightThigh,NxVec3(-0.6,3.5,0)* fRagDollScale,NxVec3(-1,0,0));

        leftAnkle = pPhysMgr->CreateRevoluteJoint(leftFoot,leftCalf,NxVec3(0.6,1.3,0)* fRagDollScale,NxVec3(1,0,0));
        rightAnkle = pPhysMgr->CreateRevoluteJoint(rightFoot,rightCalf,NxVec3(-0.6,1.3,0)* fRagDollScale,NxVec3(-1,0,0));


        //////////old 
        /*
        // Create body parts
        head = pPhysMgr->CreateSphere(NxVec3(0,8.8,0), 0.5, 10);
        torso = pPhysMgr->CreateSphere(NxVec3(0,7,0), 0.95, 10);
        pelvis = pPhysMgr->CreateSphere(NxVec3(0,5.8,0), 0.7, 10);

        leftUpperArm = pPhysMgr->CreateCapsule(NxVec3(0.5,8.5,0), 1, 0.4, 10);
        leftUpperArm->setGlobalOrientationQuat(qRotRight);
        leftForeArm = pPhysMgr->CreateCapsule(NxVec3(2,8.5,0), 1, 0.3, 10);
        leftForeArm->setGlobalOrientationQuat(qRotRight);
        leftHand = pPhysMgr->CreateBox(NxVec3(3.5,8.5,0), NxVec3(0.3,0.3,0.1), 10);
        leftHand->setGlobalOrientationQuat(qRotRight);

        rightUpperArm = pPhysMgr->CreateCapsule(NxVec3(-0.5,8.5,0), 1, 0.4, 10);
        rightUpperArm->setGlobalOrientationQuat(qRotLeft);
        rightForeArm = pPhysMgr->CreateCapsule(NxVec3(-2,8.5,0), 1, 0.3, 10);
        rightForeArm->setGlobalOrientationQuat(qRotLeft);
        rightHand = pPhysMgr->CreateBox(NxVec3(-3.5,8.5,0), NxVec3(0.3,0.3,0.1), 10);
        rightHand->setGlobalOrientationQuat(qRotLeft);

        leftThigh = pPhysMgr->CreateCapsule(NxVec3(0.6,6,0), 1.5, 0.5, 10);
        leftThigh->setGlobalOrientationQuat(qRotAround);
        leftCalf = pPhysMgr->CreateCapsule(NxVec3(0.6,3.5,0), 1.5, 0.35, 10);
        leftCalf->setGlobalOrientationQuat(qRotAround);
        leftFoot = pPhysMgr->CreateBox(NxVec3(0.6,1.5,0.2), NxVec3(0.4,0.2,0.75), 10);
        leftFoot->setGlobalOrientationQuat(qRotAround);

        rightThigh = pPhysMgr->CreateCapsule(NxVec3(-0.6,6,0), 1.5, 0.5, 10);
        rightThigh->setGlobalOrientationQuat(qRotAround);
        rightCalf = pPhysMgr->CreateCapsule(NxVec3(-0.6,3.5,0), 1.5, 0.35, 10);
        rightCalf->setGlobalOrientationQuat(qRotAround);
        rightFoot = pPhysMgr->CreateBox(NxVec3(-0.6,1.5,0.2), NxVec3(0.4,0.2,0.75), 10);
        rightFoot->setGlobalOrientationQuat(qRotAround);

        // Joint body parts together
        neck = pPhysMgr->CreateBodySphericalJoint(head,torso,NxVec3(0,8.8,0),NxVec3(0,1,0));
        leftShoulder = pPhysMgr->CreateBodySphericalJoint(leftUpperArm,torso,NxVec3(0.5,8.5,0),NxVec3(1,0,0));
        rightShoulder = pPhysMgr->CreateBodySphericalJoint(rightUpperArm,torso,NxVec3(-0.5,8.5,0),NxVec3(-1,0,0));
        spine = pPhysMgr->CreateBodySphericalJoint(torso,pelvis,NxVec3(0,7,0),NxVec3(0,-1,0));
        leftHip = pPhysMgr->CreateBodySphericalJoint(leftThigh,pelvis,NxVec3(0.6,6,0),NxVec3(0,-1,0));
        rightHip = pPhysMgr->CreateBodySphericalJoint(rightThigh,pelvis,NxVec3(-0.6,6,0),NxVec3(0,-1,0));

        leftElbow = pPhysMgr->CreateRevoluteJoint(leftForeArm,leftUpperArm,NxVec3(2,8.5,0),NxVec3(0,0,-1));
        rightElbow = pPhysMgr->CreateRevoluteJoint(rightForeArm,rightUpperArm,NxVec3(-2,8.5,0),NxVec3(0,0,-1));

        leftWrist = pPhysMgr->CreateRevoluteJoint2(leftHand,leftForeArm,NxVec3(0,-0.15,0),NxVec3(0,1.3,0),NxVec3(0,1,0),NxVec3(0,1,0));
        rightWrist = pPhysMgr->CreateRevoluteJoint2(rightHand,rightForeArm,NxVec3(0,-0.15,0),NxVec3(0,1.3,0),NxVec3(0,1,0),NxVec3(0,1,0));

        leftKnee = pPhysMgr->CreateRevoluteJoint(leftCalf,leftThigh,NxVec3(0.6,3.5,0),NxVec3(1,0,0));
        rightKnee = pPhysMgr->CreateRevoluteJoint(rightCalf,rightThigh,NxVec3(-0.6,3.5,0),NxVec3(-1,0,0));

        leftAnkle = pPhysMgr->CreateRevoluteJoint(leftFoot,leftCalf,NxVec3(0.6,1.3,0),NxVec3(1,0,0));
        rightAnkle = pPhysMgr->CreateRevoluteJoint(rightFoot,rightCalf,NxVec3(-0.6,1.3,0),NxVec3(-1,0,0));
        */
    }
    Ragdoll::~Ragdoll()
    {

    }

    /*void Ragdoll::loadMesh(std::string meshName)
    {
        pMesh = pScnMgr->createEntity("Ragdoll_" + meshName, meshName);
        pNode = pScnMgr->getRootSceneNode()->createChildSceneNode("Ragdoll_" + meshName);
        pNode->attachObject(pMesh);
        
        pNode->setScale(0.01,0.01,0.01);

        Ogre::SkeletonInstance* pSkeleton = pMesh->getSkeleton();
        Ogre::Skeleton::BoneIterator BoneIter = pSkeleton->getBoneIterator();
        Ogre::Bone* pBone;
        while (BoneIter.hasMoreElements())
        {
            pBone = BoneIter.getNext();
            pBone->setManuallyControlled(true);
        }

        // pNode->setPosition(pScnMgr->getEntity("start_point")->getParentSceneNode()->getPosition());
    }*/

    void Ragdoll::init()
    {
        Ogre::SkeletonInstance* pSkeleton = pMesh->getSkeleton();

        m_RagDollBones.push_back(PHYSX_BONE(head, pSkeleton->getBone("Bip01 Head")));
        m_RagDollBones.push_back(PHYSX_BONE(head, pSkeleton->getBone("Bip01 Neck")));
        m_RagDollBones.push_back(PHYSX_BONE(torso, pSkeleton->getBone("Bip01 Spine")));
        m_RagDollBones.push_back(PHYSX_BONE(pelvis, pSkeleton->getBone("Bip01")));
        m_RagDollBones.push_back(PHYSX_BONE(pelvis, pSkeleton->getBone("Bip01 Pelvis")));
        m_RagDollBones.push_back(PHYSX_BONE(pelvis, pSkeleton->getBone("Bip01 Spine")));

        m_RagDollBones.push_back(PHYSX_BONE(leftUpperArm, pSkeleton->getBone("Bip01 L UpperArm")));
        m_RagDollBones.push_back(PHYSX_BONE(leftUpperArm, pSkeleton->getBone("Bip01 L Clavicle")));
        m_RagDollBones.push_back(PHYSX_BONE(leftForeArm, pSkeleton->getBone("Bip01 L Forearm")));
        m_RagDollBones.push_back(PHYSX_BONE(leftHand, pSkeleton->getBone("Bip01 L Hand")));

        m_RagDollBones.push_back(PHYSX_BONE(rightUpperArm, pSkeleton->getBone("Bip01 R UpperArm")));
        m_RagDollBones.push_back(PHYSX_BONE(rightUpperArm, pSkeleton->getBone("Bip01 R Clavicle")));
        m_RagDollBones.push_back(PHYSX_BONE(rightForeArm, pSkeleton->getBone("Bip01 R Forearm")));
        m_RagDollBones.push_back(PHYSX_BONE(rightHand, pSkeleton->getBone("Bip01 R Hand")));

        m_RagDollBones.push_back(PHYSX_BONE(leftThigh, pSkeleton->getBone("Bip01 L Thigh")));
        m_RagDollBones.push_back(PHYSX_BONE(leftCalf, pSkeleton->getBone("Bip01 L Calf")));
        m_RagDollBones.push_back(PHYSX_BONE(leftFoot, pSkeleton->getBone("Bip01 L Foot")));

        m_RagDollBones.push_back(PHYSX_BONE(rightThigh, pSkeleton->getBone("Bip01 R Thigh")));
        m_RagDollBones.push_back(PHYSX_BONE(rightCalf, pSkeleton->getBone("Bip01 R Calf")));
        m_RagDollBones.push_back(PHYSX_BONE(rightFoot, pSkeleton->getBone("Bip01 R Foot")));

        SetRagDoll(false);

        FitRagDollToMesh();
     /*   Real Density = 1;

        Ogre::SkeletonInstance* pSkeleton = pMesh->getSkeleton();

        // BONE POSITIONS
        Vector3 RootPos = pSkeleton->getBone("Bip01 Pelvis")->getPosition();
        Vector3 VertPos = pSkeleton->getBone("Bip01 Spine")->getPosition();
        Vector3 NeckPos = pSkeleton->getBone("Bip01 Neck")->getPosition();

        Vector3 LeftLegPos = pSkeleton->getBone("Bip01 L Thigh")->getPosition();
        Vector3 RightLegPos = pSkeleton->getBone("Bip01 R Thigh")->getPosition();
        Vector3 LeftKneePos = pSkeleton->getBone("Bip01 L Calf")->getPosition();
        Vector3 RightKneePos = pSkeleton->getBone("Bip01 R Calf")->getPosition();
        Vector3 LeftAnklePos = pSkeleton->getBone("Bip01 L Foot")->getPosition();
        Vector3 RightAnklePos = pSkeleton->getBone("Bip01 R Foot")->getPosition();

        Vector3 LeftArmPos = pSkeleton->getBone("Bip01 L Forearm")->getPosition();
        Vector3 RightArmPos = pSkeleton->getBone("Bip01 R Forearm")->getPosition();
        Vector3 LeftElbowPos = pSkeleton->getBone("Bip01 L UpperArm")->getPosition();
        Vector3 RightElbowPos = pSkeleton->getBone("Bip01 R UpperArm")->getPosition();
        Vector3 LeftWristPos = pSkeleton->getBone("Bip01 L Hand")->getPosition();
        Vector3 RightWristPos = pSkeleton->getBone("Bip01 R Hand")->getPosition();

        // CREATE ACTOR SHAPES
        NxActor* Torso = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RootPos));
        NxActor* UpperTorso = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(VertPos));
        NxActor* Head = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(NeckPos));

        NxActor* LeftLeg = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(LeftLegPos));
        NxActor* RightLeg = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RightLegPos));
        NxActor* LeftLowerLeg = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(LeftKneePos));
        NxActor* RightLowerLeg = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RightKneePos));
        NxActor* LeftFoot = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(LeftAnklePos));
        NxActor* RightFoot = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RightAnklePos));

        NxActor* LeftArm = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(LeftArmPos));
        NxActor* RightArm = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RightArmPos));
        NxActor* LeftFrontArm = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(LeftElbowPos));
        NxActor* RightFrontArm = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RightElbowPos));
        NxActor* LeftHand = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(LeftWristPos));
        NxActor* RightHand = pPhysMgr->CreateBox(NxVec3(1,1,1), Density, toNx(RightWristPos));
        
        // ADD ACTORS
        m_RagDollBones.push_back(PHYSX_BONE(Torso, pSkeleton->getBone("Bip01 Pelvis")));
        m_RagDollBones.push_back(PHYSX_BONE(UpperTorso, pSkeleton->getBone("Bip01 Spine")));
        m_RagDollBones.push_back(PHYSX_BONE(Head, pSkeleton->getBone("Bip01 Neck")));

        m_RagDollBones.push_back(PHYSX_BONE(LeftLeg, pSkeleton->getBone("Bip01 L Thigh")));
        m_RagDollBones.push_back(PHYSX_BONE(RightLeg, pSkeleton->getBone("Bip01 R Thigh")));
        m_RagDollBones.push_back(PHYSX_BONE(LeftLowerLeg, pSkeleton->getBone("Bip01 L Calf")));
        m_RagDollBones.push_back(PHYSX_BONE(RightLowerLeg, pSkeleton->getBone("Bip01 R Calf")));
        m_RagDollBones.push_back(PHYSX_BONE(LeftFoot, pSkeleton->getBone("Bip01 L Foot")));
        m_RagDollBones.push_back(PHYSX_BONE(RightFoot, pSkeleton->getBone("Bip01 R Foot")));

        m_RagDollBones.push_back(PHYSX_BONE(LeftArm, pSkeleton->getBone("Bip01 L Forearm")));
        m_RagDollBones.push_back(PHYSX_BONE(RightArm, pSkeleton->getBone("Bip01 R Forearm")));
        m_RagDollBones.push_back(PHYSX_BONE(LeftFrontArm, pSkeleton->getBone("Bip01 L UpperArm")));
        m_RagDollBones.push_back(PHYSX_BONE(RightFrontArm, pSkeleton->getBone("Bip01 R UpperArm")));
        m_RagDollBones.push_back(PHYSX_BONE(LeftHand, pSkeleton->getBone("Bip01 L Hand")));
        m_RagDollBones.push_back(PHYSX_BONE(RightHand, pSkeleton->getBone("Bip01 R Hand")));

        // CREATE JOINTS
       /* m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(Torso, UpperTorso, toNx(VertPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(UpperTorso, Head, toNx(NeckPos), toNx(Vector3::UNIT_Y)));

        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(Torso, LeftLeg, toNx(LeftLegPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(Torso, RightLeg, toNx(RightLegPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(LeftLeg, LeftLowerLeg, toNx(LeftKneePos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(RightLeg, RightLowerLeg, toNx(RightKneePos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(LeftLowerLeg, LeftFoot, toNx(LeftKneePos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(RightLowerLeg, RightFoot, toNx(RightKneePos), toNx(Vector3::UNIT_Y)));

        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(UpperTorso, LeftArm, toNx(LeftArmPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(UpperTorso, RightArm, toNx(RightArmPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(LeftArm, LeftFrontArm, toNx(LeftElbowPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateRevoluteJoint(RightArm, RightFrontArm, toNx(RightElbowPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(LeftFrontArm, LeftHand, toNx(LeftWristPos), toNx(Vector3::UNIT_Y)));
        m_RagDollJoints.push_back(pPhysMgr->CreateBodySphericalJoint(RightFrontArm, RightHand, toNx(RightWristPos),toNx(Vector3::UNIT_Y)));
    */}

    void Ragdoll::FitMeshToRagDoll()
    {
        std::vector<PHYSX_BONE>::iterator itBone;
        Ogre::Quaternion PhysxRotation, OgreGlobalQuat, NodeRotationInverse = pMesh->getParentSceneNode()->getOrientation().Inverse();
        for (itBone = m_RagDollBones.begin(); itBone != m_RagDollBones.end(); ++itBone)
        {
            PhysxRotation = toOgre(itBone->pPhysxBone->getGlobalOrientationQuat())*itBone->PhysxBindOrientationInverse;
            Quaternion ParentInverse = NodeRotationInverse;
            if (itBone->pOgreBone->getParent())
                ParentInverse = itBone->pOgreBone->getParent()->_getDerivedOrientation().Inverse() * NodeRotationInverse;
            else
                pNode->setPosition(toOgre(itBone->pPhysxBone->getGlobalPosition()) - pNode->getOrientation()*itBone->pOgreBone->getPosition()* pNode->getScale() );
            OgreGlobalQuat = PhysxRotation * itBone->OgreGlobalBindOrientation;
            itBone->pOgreBone->setOrientation(ParentInverse * OgreGlobalQuat);
        }
    }

    void Ragdoll::FitRagDollToMesh()
    {
        std::vector<PHYSX_BONE>::iterator itBone;
        Quaternion PhysxRotation, OgreGlobalQuat;
        Ogre::Vector3 OgrePosition;
        for (itBone = m_RagDollBones.begin(); itBone != m_RagDollBones.end(); ++itBone)
        {
            OgrePosition = (pMesh->getParentSceneNode()->getOrientation() * itBone->pOgreBone->_getDerivedPosition() * pNode->getScale()) + pMesh->getParentSceneNode()->getPosition();
            OgreGlobalQuat = itBone->pOgreBone->_getDerivedOrientation() * itBone->OgreGlobalBindOrientation.Inverse();
            PhysxRotation = OgreGlobalQuat * itBone->PhysxBindOrientationInverse.Inverse();
            itBone->pPhysxBone->setGlobalPosition(toNx(OgrePosition));
            itBone->pPhysxBone->setGlobalOrientationQuat(toNx(pMesh->getParentSceneNode()->getOrientation() * PhysxRotation));
        }
    }

    void Ragdoll::addForce()
    {
        head->addForce(NxVec3(0,1000,100));
    }

    void Ragdoll::SetRagDoll(bool bRagDoll)
    {
        m_bRagDoll = bRagDoll;

        if (bRagDoll)
        {
            FitRagDollToMesh();
        }
        Ogre::SkeletonInstance* pSkeleton = pMesh->getSkeleton();
        Ogre::Skeleton::BoneIterator BoneIter = pSkeleton->getBoneIterator();
        Ogre::Bone* pBone;
        while (BoneIter.hasMoreElements())
        {
            pBone = BoneIter.getNext();
            pBone->setManuallyControlled(bRagDoll);
            if (bRagDoll)
                pBone->reset();
        }
        if (bRagDoll)
        {
            AnimationStateSet *set = pMesh->getAllAnimationStates();
            AnimationStateIterator it = set->getAnimationStateIterator();
            Ogre::AnimationState *anim;
            while(it.hasMoreElements())
            {
                anim = it.getNext();
                anim->setEnabled(false);
                anim->setWeight(0);
            }
        }

        std::vector<PHYSX_BONE>::iterator itBone;
        for (itBone = m_RagDollBones.begin(); itBone != m_RagDollBones.end(); ++itBone)
        {
            if (bRagDoll)
            {
                itBone->pPhysxBone->clearActorFlag(NX_AF_DISABLE_COLLISION);
                itBone->pPhysxBone->clearBodyFlag(NX_BF_KINEMATIC);
                itBone->pPhysxBone->wakeUp();
            }
            else
            {
                itBone->pPhysxBone->raiseActorFlag(NX_AF_DISABLE_COLLISION);
                itBone->pPhysxBone->raiseBodyFlag(NX_BF_KINEMATIC);
                itBone->pPhysxBone->putToSleep();
            }
        }
    }

    void Ragdoll::updateRagdoll(float delta)
    {
        if (m_bRagDoll)
            FitMeshToRagDoll();
      //  else
          //  FitRagDollToMesh();
    }
}