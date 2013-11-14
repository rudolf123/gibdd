#include "Precompiled.h"
#include "EngineSolver.h"
#include <fstream>

#pragma warning (disable : 4996)

namespace Gibdd
{
    EngineSolver::EngineSolver()
    {
        loadRPMTorqueFromFile();
        BackTorque = 1000;
        IdleRPM = 1000;
        Inertia = 0.1;
        RPM = 1101;
        Throttle = 0;
        torq = 0;
        additionRPM = 0;
        TopGear = 3.9f;
        toWheelTorq = 0;
        CurrentGear = 1;
        Gear[0] = -4.0f; // задн€€
        Gear[1] = 0.0f; // нейтраль
        Gear[2] = 3.64f; // перва€
        Gear[3] = 1.95f; // втора€
        Gear[4] = 1.36f; // треть€
        Gear[5] = 0.94f; // четвЄрта€
        Gear[6] = 0.78f; // п€та€

        Wh0Rotation = 0;
        Wh1Rotation = 0;
    }
    EngineSolver::~EngineSolver()
    {

    }

    void EngineSolver::Progress(float Clutch, float diff)
    {
        if (RPM < IdleRPM && Throttle < 0.3f)
            RPM = IdleRPM;

        torq = Torque[ (int)RPM ] * Throttle;

        toWheelTorq = torq * 0.9 / (TopGear * Gear[CurrentGear]);

        if(Throttle < 0.1)
            BackTorque = 5000;
        else
            BackTorque = 100;
        additionRPM = torq * 100 - BackTorque;
      //  torqBacktorq = BackTorque;
        RPM += (additionRPM) * (1.0f - Clutch) * diff;
    }

    bool EngineSolver::loadRPMTorqueFromFile()
    {
        std::string line;

        std::ifstream ifs("RPMTorque.txt");
        if(!ifs.is_open())
            return false;
        
        uint16 val1[2];
        uint16 val2[2];
        float step = 0;
        while(!ifs.eof())
        {
            std::getline(ifs, line);
            if(line.empty())
                continue;

            size_t found = line.find("#RMP;Torque");
            if (found != std::string::npos)
                continue;

            uint8 itr = 0;
                
            char* pch = strtok((char*)line.c_str(), ";");
            while(pch)
            {
                if(itr > 1)
                    break;

                val2[itr] = atoi(pch);

                pch = strtok(NULL, ";");
                ++itr;
            }

            if (val2[0] == 0 && val2[1] == 0)
            {
                val1[0] = val2[0];
                val1[1] = val2[1];
                continue;
            }
            for (uint16 i = val1[0]; i < val2[0]; ++i)
            {
                float y = (float)(i - val1[0])/(val2[0] - val1[0]) * (val2[1] - val1[1]) + val1[1];
                Torque[i] = y; 
                Power[i] = Torque[i] * i / 7000;
            }

            val1[0] = val2[0];
            val1[1] = val2[1];
        }
        ifs.close();

        std::ofstream outStream;
        outStream.open("test.txt", std::ios_base::trunc);

        for (uint16 i = 0; i < 10000; ++i)
        {
            outStream << Torque[i] << std::flush;
            outStream << "\n" << std::flush;
        }
        outStream.close();

        return true;
    }
}