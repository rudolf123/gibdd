#ifndef GEAR_DESC
#define GEAR_DESC

#include "NxLinearInterpolationValues.h"

#define MAX_NB_GEARS 6

class GearDesc {
public:
    GearDesc() { setToDefault(); }
    void setToDefault();
    bool loadSettingsFromFile(const std::string& filename);
    bool isValid() const;
    NxU32 getMaxNumOfGears() const { return MAX_NB_GEARS; }


    NxU32 nbForwardGears;
    std::vector<NxF32> forwardGearRatios;
    //float forwardGearRatios[MAX_NB_GEARS];

    float backwardGearRatio;
    float differentialRatio;
};

NX_INLINE void GearDesc::setToDefault()
{
    /*forwardGearRatios[0] = 2.66f;
    forwardGearRatios[1] = 1.78f;
    forwardGearRatios[2] = 1.30f;
    forwardGearRatios[3] = 1;
    forwardGearRatios[4] = 0.74f;
    forwardGearRatios[5] = 0.50f;*/
    forwardGearRatios.push_back(2.66f);
    forwardGearRatios.push_back(1.78f);
    forwardGearRatios.push_back(1.30f);
    forwardGearRatios.push_back(1.00f);
    forwardGearRatios.push_back(0.74f);
    forwardGearRatios.push_back(0.50f);
    nbForwardGears = 6;
    backwardGearRatio = -2.90f;
    differentialRatio = 3.42f;
    forwardGearRatios.resize(nbForwardGears);
}

NX_INLINE bool GearDesc::loadSettingsFromFile(const std::string& filename)
{
    std::string line;

    std::ifstream ifs(filename.c_str());
    if(!ifs.is_open())
        return false;

    //unsigned short val[2];
    bool parseForwardGears = false;
    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
            continue;

        if (parseForwardGears)
        {
            forwardGearRatios.push_back(atof(line.c_str()));
            continue;
        }

        size_t found = line.find("backwardGearRatio");
        if (found != std::string::npos)
        {
            found = line.find("=");
            backwardGearRatio = atof(line.substr(found + 1).c_str());
            continue;
        }

        found = line.find("differentialRatio");
        if (found != std::string::npos)
        {
            found = line.find("=");
            differentialRatio = atof(line.substr(found + 1).c_str());
            continue;
        }

        found = line.find("#ForwardGearsRatios");
        if (found != std::string::npos)
        {
            parseForwardGears = true;
            continue;
        }
    }
    ifs.close();
    forwardGearRatios.resize(nbForwardGears);
   // nbForwardGears = 6;

    return true;
}

NX_INLINE bool GearDesc::isValid() const {
    if(nbForwardGears > getMaxNumOfGears()) {
        //fprintf(stderr, "GearDesc::isValid(): nbForwardGears(%d) is bigger than max(%d)\n",nbForwardGears, getMaxNumOfGears());
        return false;
    }
    if(nbForwardGears <= 0) {
        //fprintf(stderr, "GearDesc::isValid(): nbForwardGears(%d) smaller or equal 0\n", nbForwardGears);
        return false;
    }
    if(backwardGearRatio > 0) {
        //fprintf(stderr, "NxVehilceGearDesc::isValid(): backwardGearRatio(%2.3f) is bigger than 0, make it negative\n", backwardGearRatio);
        return false;
    }
    for(NxU32 i = 0; i < nbForwardGears; ++i) {
        if(forwardGearRatios[i] < 0) {
            //fprintf(stderr, "NxVehilceGearDesc::isValid(): forwardGearRatios[%d](%2.3f) has value smaller 0\n", i, forwardGearRatios[i]);
            return false;
        }
    }
    return true;
}

#endif
