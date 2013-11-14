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
            // ������� ��������
            float Torque[10000]; //�������� ������
            float torq;
            float torqBacktorq;
            float additionRPM;
            float Power[10000]; //�������� ���������
            float Inertia, Mass, BackTorque; //���������� ��������� ���������, �����, �������� �������� ������
            char Direction;
            float Throttle;
            float IdleRPM; // ������� ��������� ����

            // ������������ ��������
            float RPM; //������� ���������

            float TopGear;
            float Gear[7]; 

            float WheelInertia;
            float EngineInertia;
            int CurrentGear;
            float FastestWheelRotation;

            float Wh0Rotation;
            float Wh1Rotation;
            float toWheelTorq;


            // ������� ����������
    };
}
#endif