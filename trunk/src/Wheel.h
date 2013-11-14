#include "NxWheelDesc.h"
#include <PhysX/NxPhysics.h>

//class Vehicle;

class Wheel
{
    public:
        Wheel(NxActor* actor, NxWheelDesc* wheelDesc);
        ~Wheel();

        void            tick(bool handbrake, NxReal motorTorque, NxReal brakeTorque, NxReal dt);
        NxActor *       getTouchedActor() const;
        NxActor *       getActor();
        NxMat34         getWheelPose();
        NxVec3          getWheelPos() const;
        NxVec3          getWheelPosition();
        NxQuat          getWheelOrientation();
        void            setAngle(NxReal angle);
        void            drawWheel(NxReal approx, bool debug = false) const;
        NxReal          getRpm() const;
        NxVec3          getGroundContactPos() const { return getWheelPos()+NxVec3(0, -wheelShape->getRadius(), 0); }
        float           getRadius() const { return wheelShape->getRadius(); }
        NxWheelShape*   getWheelShape() { return wheelShape; }
        float           getWheelShapeRollAngle() { return wheelShapeRollAngle; }
        void            setWheelShapeRollAngle(float RollAngle) {wheelShapeRollAngle = RollAngle; }

        NX_INLINE bool  hasGroundContact() const { return getTouchedActor() != NULL; }
        NX_INLINE bool  getWheelFlag(NxWheelFlags flag) const { return (wheelFlags & flag) != 0; }

        void*           userData;
    private:
        NxActor* actor;
        NxWheelShape * wheelShape;
    	NxU32 wheelFlags;
        float wheelShapeRollAngle;
};