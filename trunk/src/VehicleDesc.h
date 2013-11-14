#ifndef VEHICLE_DESC
#define VEHICLE_DESC

#include "NxWheelDesc.h"
#include "MotorDesc.h"
#include "GearDesc.h"
#include <PhysX/NxArray.h>
#include <PhysX/NxShapeDesc.h>

class VehicleDesc
{
public:
    NxArray<NxShapeDesc*>	carShapes;
    NxArray<NxWheelDesc*>	carWheels;

    MotorDesc*		motorDesc;
    GearDesc*		gearDesc;

    Ogre::Entity *pBodyModel;
    std::string sWheelmeshName;

    NxVec3					position;
    NxReal					mass;
    NxReal					motorForce;
    NxReal					transmissionEfficiency;
    NxReal					differentialRatio;

    NxVec3					steeringTurnPoint;
    NxVec3					steeringSteerPoint;
    NxReal					steeringMaxAngle;

    NxVec3					centerOfMass;

    NxReal					digitalSteeringDelta;

    NxReal					maxVelocity;
    NxReal					maxBrakeForce;
    NxReal					cameraDistance;
    bool    joyInput;
    //NxReal					digitalSteeringDeltaVelocityModifier;

    void*					userData;

    NX_INLINE VehicleDesc();
    NX_INLINE void setToDefault();
    NX_INLINE bool isValid() const;
};

NX_INLINE VehicleDesc::VehicleDesc()	//constructor sets to default
{
    setToDefault();
}

NX_INLINE void VehicleDesc::setToDefault()
{
    userData = NULL;
    motorDesc = NULL;
    gearDesc = NULL;
    transmissionEfficiency = 1.0f;
    differentialRatio = 1.0f;
    maxVelocity = 80;
    cameraDistance = 5.f;
    carWheels.clear();
}

NX_INLINE bool VehicleDesc::isValid() const
{
    for (NxU32 i = 0; i < carWheels.size(); i++) {
        if (!carWheels[i]->isValid())
            return false;
    }

    if (mass < 0)
        return false;

    return true;
}


#endif
