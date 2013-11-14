#ifndef configAnimsH
#define configAnimsH

#include "ActionManager.h"

namespace Gibdd
{
    using namespace Ogre;
    enum BlendingTransition
    {
        BlendSwitch,         // ������� ������������
        BlendWhileAnimating,   // ������������ ����������� ��� �������� �������� �� ������ � ������ ��� ������� � � ������ �������������
        BlendThenAnimate      // �������� �������� ������������ � ��������� ���� ����� ����������� ����� � ����������� ����
    };
    enum ActionTermination
    {
        Limit,    //������������ ����������, ���������� � ������������ ���������� ���������, ���� ������� �� ����������,
        //���������� ���������� �������� � ������� ����� ������� mBreak=true
        Normal,    //������� ����������, ����� �������� � ����� ������
        Final    //������������� ����������, ����������� ����� ����������� ��������
    };
    enum Actions
    {
        WAIT,
        STARTRUN,
        STOPRUN,
        WALKF,
        WALKB,
        WALKL,
        WALKR,
        RUNF,
        RUNB,
        RUNL,
        RUNR,
        SITDOWN,
        SIT1,
        SIT2,
        SITDRINK,
        PHONE,
        TALK,
        GETUP,
        WAVE,
        TAKEOBJ,

        UNKNOWN,

        EXTING
    };
    struct configAnims
    {
        Actions mActions;    //��� ��������
        String mName;        //�������� ��������
        Real mSpeed;        //�������� ��������
        BlendingTransition mTransition;            //��� ����������
        ActionTermination mActionTermination;    //��� ���������� ��������
        bool mBreak;        //��������� ����� �������� ���� true
        Real mDuration;        //����������������� ���������� � ��������
        bool mLooped;        //�������� ����������� ��� ���
        bool mSoundEnabled;    //�������������� ������ ��� ���
    };
}

#endif