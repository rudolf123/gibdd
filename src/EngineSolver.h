#ifndef EngineSolverH
#define EngineSolverH

namespace Gibdd
{
    class EngineSolver
    {
        public:
            EngineSolver();
            ~EngineSolver();

            void Progress(float Clutch, float diff);
            bool loadRPMTorqueFromFile();
            // базовые величины
            float Torque[10000]; //крутящий момент
            float torq;
            float torqBacktorq;
            float additionRPM;
            float Power[10000]; //мощность двигателя
            float Inertia, Mass, BackTorque; //инертность вращающих элементов, масса, обратный крутящий момент
            char Direction;
            float Throttle;
            float IdleRPM; // обороты холостого хода

            // динамические величины
            float RPM; //обороты двигателя

            float TopGear;
            float Gear[7]; 

            float WheelInertia;
            float EngineInertia;
            int CurrentGear;
            float FastestWheelRotation;

            float Wh0Rotation;
            float Wh1Rotation;
            float toWheelTorq;


            // функция обновления
    };
}
#endif