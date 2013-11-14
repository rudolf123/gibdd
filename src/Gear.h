#ifndef GearH
#define GearH

#include "GearDesc.h"

class Gear
{
    public:
        NxI32 curGear;
        Gear(): curGear(1)
        {
        }

        static Gear* createGear(GearDesc* gearDesc);
        
        float getCurrentRatio() const;
        float getDifferentialRatio() {return _differentialRatio;};
        float getRatio(NxI32 gear) const;
        NxI32 getMaxGear() const { return _nbForwardGears; }
        void gearUp() { curGear += 1; curGear = NxMath::min(curGear,(NxI32)_nbForwardGears); }
        void gearDown() { curGear -= 1; curGear = NxMath::max(curGear, -1); }

    private:
        NxU32 _nbForwardGears;
        //float _forwardGearRatios[MAX_NB_GEARS];
        std::vector<NxF32> _forwardGearRatios;
        float _backwardGearRatio;
        float _differentialRatio;
};

NX_INLINE Gear* Gear::createGear(GearDesc* gearDesc)
{
    if(!gearDesc->isValid())
        return NULL;

    Gear *gears = new Gear();
    //NxI32 nbForwardGears = gears->_nbForwardGears = gearDesc->nbForwardGears;
    gears->_nbForwardGears = 0;
    std::vector<NxF32>::iterator itr = gearDesc->forwardGearRatios.begin();
    for (; itr != gearDesc->forwardGearRatios.end(); ++itr)
    {
        gears->_forwardGearRatios.push_back((*itr));
        ++gears->_nbForwardGears;
    }
    //memcpy(gears->_forwardGearRatios, gearDesc->forwardGearRatios, sizeof(NxF32) * nbForwardGears);

    gears->curGear = 1;
    gears->_backwardGearRatio = gearDesc->backwardGearRatio;
    gears->_differentialRatio = gearDesc->differentialRatio;

    return gears;
}

NX_INLINE NxF32 Gear::getCurrentRatio() const
{
    return getRatio(curGear);
}

NX_INLINE NxF32 Gear::getRatio(NxI32 gear) const
{
    if(gear > 0)
        return _forwardGearRatios[gear-1];

    if(gear == -1)
        return _backwardGearRatio;

    return 0;
}

#endif
