#ifndef configAnimsH
#define configAnimsH

#include "ActionManager.h"

namespace Gibdd
{
    using namespace Ogre;
    enum BlendingTransition
    {
        BlendSwitch,         // простое переключение
        BlendWhileAnimating,   // одновременно проигрывает две анимации смешивая их причем у первой вес убывает а у второй увеличивается
        BlendThenAnimate      // исходная анимация доигрывается с убыванием веса затем запускается новая с увеличением веса
    };
    enum ActionTermination
    {
        Limit,    //ограниченное завершение, смешивание с последующими действиями запрещено, пока текущее не завершится,
        //исключение составляют действия у которых стоит признак mBreak=true
        Normal,    //обычное завершение, можно прервать в любой момент
        Final    //окончательное завершение, запрещается любые последующие действия
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
        Actions mActions;    //Вид действия
        String mName;        //Название анимации
        Real mSpeed;        //Скорость анимации
        BlendingTransition mTransition;            //тип смешивания
        ActionTermination mActionTermination;    //тип завершения действия
        bool mBreak;        //прерывает любое действие если true
        Real mDuration;        //Продолжительность смешивания в секундах
        bool mLooped;        //анимация циклическая или нет
        bool mSoundEnabled;    //сопровождается звуком или нет
    };
}

#endif