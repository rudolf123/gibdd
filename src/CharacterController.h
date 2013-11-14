#ifndef CharacterControllerH
#define CharacterControllerH
#include <PhysX/NxControllerManager.h>
#include <PhysX/NxBoxController.h>
#include <PhysX/NxCapsuleController.h>

namespace Gibdd
{
    enum CollisionGroup
    {
        GROUP_NON_COLLIDABLE,
        GROUP_COLLIDABLE_NON_PUSHABLE,
        GROUP_COLLIDABLE_PUSHABLE,
    };

    class ControllerHitReport : public NxUserControllerHitReport
    {
    public:

        virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit)
        {
            if(hit.shape)
            {
                NxCollisionGroup group = hit.shape->getGroup();
                if(group==GROUP_COLLIDABLE_PUSHABLE)
                {
                    NxActor& actor = hit.shape->getActor();
                    if(actor.isDynamic())
                    {
                        // We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
                        // useless stress on the solver. It would be possible to enable/disable vertical pushes on
                        // particular objects, if the gameplay requires it.
                        if(hit.dir.y==0.0f)
                        {
                            NxF32 coeff = actor.getMass() * hit.length * 10.0f;
                            actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
                        }
                    }
                }
            }

            return NX_ACTION_NONE;
        }

        virtual NxControllerAction onControllerHit(const NxControllersHit& hit)
        {
            return NX_ACTION_NONE;
        }

    };

    class CharacterController
    {
    public:
        CharacterController(NxControllerManager* manager, NxScene *scene, NxVec3 pos, NxReal radius, NxReal height);
        ~CharacterController();

        void Move(const NxVec3 &disp, NxU32 &flag);
        NxActor* GetCharacterActor();

        void UpdateCharacterExtents(NxScene *scene, NxVec3 offset);

        bool SetCharacterPos(NxVec3 pos);
        NxVec3 GetCharacterPos();
        float GetCharacterHeight(){return fCharacterHeight;};
        void SetCollision(bool collide){ pController->setCollision(collide);};

    private:
        NxControllerManager* pManager; // Singleton
        NxController* pController;
        ControllerHitReport mReport;

        float fCharacterHeight;
    };
}
#endif