#ifndef MotorH
#define MotorH

#include "MotorDesc.h"
#include "Gear.h"

class Motor
{
    public:
        NxLinearInterpolationValues _torqueCurve;
        float _rpm;
        float _maxTorque;
        float _maxTorquePos;
        float _maxRpmToGearUp;
        float _minRpmToGearDown;
        float _maxRpm;
        float _minRpm;
        float _acceleration;

        Motor() : _rpm(0) {}
        static Motor* createMotor(MotorDesc* motorDesc);

        void setRps(NxReal rpm)
        {
            _rpm = rpm;
        }
        float getRps() const { return _rpm; }

        float getMinRpsToGearDown() const { return _minRpmToGearDown; }
        float getMaxRpsToGearUp() const { return _maxRpmToGearUp; }
        float getMinRps() const { return _minRpm; }
        float getMaxRps() const { return _maxRpm; }
        bool changeGears(Gear* gears, NxReal rpm);

        float getTorque() const { return _torqueCurve.getValue(_rpm); }
};

NX_INLINE Motor* Motor::createMotor(MotorDesc* motorDesc)
{
    if(!motorDesc->isValid())
        return NULL;

    Motor* motor = new Motor();
    motor->_torqueCurve = motorDesc->torqueCurve;
    NxReal maxTorque = 0;
    NxI32 maxTorquePos = -1;

    for(NxU32 i = 0; i < motor->_torqueCurve.getSize(); ++i)
    {
        float v = motor->_torqueCurve.getValueAtIndex(i);
        if(v > maxTorque)
        {
            maxTorque = v;
            maxTorquePos = i;
        }
    }

    motor->_acceleration = motorDesc->acceleration;
    motor->_maxTorque = maxTorque;
    motor->_maxTorquePos =(NxReal)maxTorquePos;
    motor->_maxRpmToGearUp = motorDesc->maxRpmToGearUp;
    motor->_minRpmToGearDown = motorDesc->minRpmToGearDown;
    motor->_maxRpm = motorDesc->maxRpm;
    motor->_minRpm = motorDesc->minRpm;
    return motor;
}

NX_INLINE bool Motor::changeGears(Gear* gears, NxReal rpm)
{
    NxI32 gear = gears->curGear;
    if(_rpm > _maxRpmToGearUp && gear < gears->getMaxGear())
    {
        gears->gearUp();
        _rpm = _minRpmToGearDown;
        return true;
    }
    else if(_rpm < _minRpmToGearDown && gear > 1)
    {
        gears->gearDown();
        _rpm = rpm;
        return true;
    }
    return false;
}

#endif
