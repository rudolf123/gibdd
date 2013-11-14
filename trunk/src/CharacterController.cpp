#include "Precompiled.h"
#include "CharacterController.h"

namespace Gibdd
{
    #define COLLIDABLE_MASK    (1<<GROUP_COLLIDABLE_NON_PUSHABLE)

    CharacterController::CharacterController(NxControllerManager* manager, NxScene *scene, NxVec3 pos, NxReal radius, NxReal height)
    {
        pManager = manager;

        fCharacterHeight = height;

        NxCapsuleControllerDesc desc;
        desc.position.x = pos.x;
        desc.position.y = pos.y;
        desc.position.z = pos.z;
        desc.height = height;
        desc.radius = radius;
        desc.skinWidth = 0.01f;
        desc.slopeLimit = cosf(NxMath::degToRad(45.0f));
        desc.stepOffset = 2.0f;
        desc.upDirection = NX_Y;
        desc.climbingMode = CLIMB_EASY;
        desc.callback = &mReport;

        pController = pManager->createController(scene, desc);
        pController->getActor()->setGroup(GROUP_COLLIDABLE_NON_PUSHABLE);
        pController->setCollision(true);

        //pController->getActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
        //pController->getActor()->clearBodyFlag(NX_BF_KINEMATIC);
    }

    CharacterController::~CharacterController()
    {
        if(pManager)
        {
            pManager->releaseController(*pController);
        }
    }

    void CharacterController::Move(const NxVec3 &disp, NxU32 &flag)
    {
        pController->move(disp, COLLIDABLE_MASK, 0.000001f, flag);
    }

    NxActor* CharacterController::GetCharacterActor()
    {
        return pController->getActor();
    }

    bool CharacterController::SetCharacterPos(NxVec3 pos)
    {
        return pController->setPosition(NxExtendedVec3(pos.x, pos.y, pos.z));
    }

    NxVec3 CharacterController::GetCharacterPos()
    {
        NxExtendedVec3 vec = pController->getPosition();

        return NxVec3(vec.x, vec.y, vec.z);
    }
}