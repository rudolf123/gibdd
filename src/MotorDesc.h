#ifndef MOTOR_DESC_H
#define MOTOR_DESC_H

#include "NxLinearInterpolationValues.h"
#pragma warning(disable:4996)

class MotorDesc {
public:
    NxLinearInterpolationValues torqueCurve;
    float maxRpmToGearUp;
    float minRpmToGearDown;
    float maxRpm;
    float minRpm;
    float acceleration;

    NX_INLINE void setToDefault();
    void NxLinearInterpolationValues2() { setToDefault(); }
    bool loadSettingsFromFile(const std::string& filename);
    NX_INLINE bool isValid() const;
};

NX_INLINE void MotorDesc::setToDefault()
{
    torqueCurve.insert(500, 25.f);
    torqueCurve.insert(1000, 35);
    torqueCurve.insert(2000, 185);
    torqueCurve.insert(3000, 205);
    torqueCurve.insert(4000, 215);
    torqueCurve.insert(5000, 210);
    torqueCurve.insert(7000, 60);
    torqueCurve.insert(8000, 20);
    torqueCurve.insert(9000, 0);
    minRpmToGearDown = 41.67f;
    maxRpmToGearUp = 83.33f;
    minRpm = 1200;
    maxRpm = 9000;
    acceleration = 0.05;
}

NX_INLINE bool MotorDesc::loadSettingsFromFile(const std::string& filename)
{
    std::string line;

	std::ifstream ifs(filename.c_str());
    if(!ifs.is_open())
        return false;

    unsigned short val[2];
    bool parseRPMTorque = false;

    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
            continue;

        size_t found = line.find("#Gears");
        if (found != std::string::npos)
        {
            parseRPMTorque = false;
            break;
        }

        if (parseRPMTorque)
        {
            unsigned char itr = 0;

            char* pch = strtok((char*)line.c_str(), ";");
            while(pch)
            {
                if(itr > 1)
                    break;

                val[itr] = atoi(pch);

                pch = strtok(NULL, ";");
                ++itr;
            }
            torqueCurve.insert(val[0], val[1]);
            continue;
        }

        found = line.find("minRpm=");
        if (found != std::string::npos)
        {
            found = line.find("=");
            minRpm = atof(line.substr(found + 1).c_str());
            continue;
        }

        found = line.find("maxRpm=");
        if (found != std::string::npos)
        {
            found = line.find("=");
            maxRpm = atof(line.substr(found + 1).c_str());
            continue;
        }
        found = line.find("minRpmToGearDown");
        if (found != std::string::npos)
        {
            found = line.find("=");
            minRpmToGearDown = atof(line.substr(found + 1).c_str());
            continue;
        }
        found = line.find("maxRpmToGearUp");
        if (found != std::string::npos)
        {
            found = line.find("=");
            maxRpmToGearUp = atof(line.substr(found + 1).c_str());
            continue;
        }

        found = line.find("#RMP;Torque");
        if (found != std::string::npos)
        {
            parseRPMTorque = true;
            continue;
        }
    }

    ifs.close();

    return true;
}


NX_INLINE bool MotorDesc::isValid() const {
    if(torqueCurve.getSize() == 0) {
        //fprintf(stderr, "MotorDesc::isValid(): Empty TorqueCurve\n");
        return false;
    }
    if(maxRpmToGearUp < minRpmToGearDown) {
        //fprintf(stderr, "MotorDesc::isValid(): maxRpsToGearUp(%2.3f) is smaller than minRpsToGearDown(%2.3f)\n",maxRpsToGearUp, minRpsToGearDown);
        return false;
    }
    return true;
}

#endif
