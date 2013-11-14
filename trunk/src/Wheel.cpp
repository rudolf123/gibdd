#include "Precompiled.h"
#include "Wheel.h"
//#include "Vehicle.h"

Wheel::Wheel(NxActor* a, NxWheelDesc* wheelDesc) : actor(a)
{
    NxScene *scene = &actor->getScene();

    static NxMaterial * wsm = 0;
    if(!wsm)
    {
        NxMaterialDesc m;
        m.flags |= NX_MF_DISABLE_FRICTION;
        wsm = scene->createMaterial(m);
    }

    NxWheelShapeDesc wheelShapeDesc;

    wheelShapeRollAngle = 0;

    wheelShapeDesc.localPose.t = wheelDesc->position;
    NxQuat q;
    q.fromAngleAxis(90.0f, NxVec3(0,1,0));
    wheelShapeDesc.localPose.M.fromQuat(q);
    wheelShapeDesc.materialIndex = wsm->getMaterialIndex();
    wheelFlags = wheelDesc->wheelFlags;

    NxReal heightModifier = (wheelDesc->wheelSuspension + wheelDesc->wheelRadius) / wheelDesc->wheelSuspension;

    wheelShapeDesc.suspension.spring = wheelDesc->springRestitution*heightModifier;
    wheelShapeDesc.suspension.damper = 0;
    wheelShapeDesc.suspension.targetValue = wheelDesc->springBias*heightModifier;

    wheelShapeDesc.radius = wheelDesc->wheelRadius;
    wheelShapeDesc.suspensionTravel = wheelDesc->wheelSuspension; 
    wheelShapeDesc.inverseWheelMass = 0.1f;

    wheelShapeDesc.lateralTireForceFunction.stiffnessFactor *= wheelDesc->frictionToSide;	
    wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor *= wheelDesc->frictionToFront;	

    wheelShape = static_cast<NxWheelShape *>(actor->createShape(wheelShapeDesc));
}

Wheel::~Wheel()
{
}

void Wheel::tick(bool handBrake, NxReal motorTorque, NxReal brakeTorque, NxReal dt)
{
    brakeTorque *= 10.0f;
    if(handBrake && getWheelFlag(NX_WF_AFFECTED_BY_HANDBRAKE))
        brakeTorque = 1000.0f;

    if(getWheelFlag(NX_WF_ACCELERATED)) 
        wheelShape->setMotorTorque(motorTorque);

    wheelShape->setBrakeTorque(brakeTorque);
}

NxActor* Wheel::getTouchedActor() const
{
    NxWheelContactData wcd;
    NxShape * s = wheelShape->getContact(wcd);	
    return s ? &s->getActor() : 0;
}

NxVec3 Wheel::getWheelPos() const
{
    return wheelShape->getLocalPosition();
}

NxVec3 Wheel::getWheelPosition()
{
    return wheelShape->getGlobalPosition();
}

NxQuat Wheel::getWheelOrientation()
{
    NxQuat orient;
    wheelShape->getGlobalOrientation().toQuat(orient);
    return orient;
}

NxMat34 Wheel::getWheelPose()
{
    return wheelShape->getGlobalPose();
}

void Wheel::setAngle(NxReal angle)
{
    wheelShape->setSteerAngle(-angle);
}

void Wheel::drawWheel(NxReal approx, bool debug) const
{

}

NxReal Wheel::getRpm() const
{
    return NxMath::abs(wheelShape->getAxleSpeed())/NxTwoPi * 60.0f;
}
