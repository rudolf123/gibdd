#ifndef ActionManagerH
#define ActionManagerH

#include "configAnims.h"

namespace Gibdd
{
    using namespace Ogre;

    class find_byaction
    {
        Actions mActions;
        public:
            find_byaction(Actions action)
            {
                mActions=action;
            }
            bool operator ()(const configAnims &ca)
            {
                if (ca.mActions==mActions)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
    };

    class ActionManager
    {
        public:
            ActionManager( Entity *entity)
            {
                mEntity = entity;
            }
            ~ActionManager()
            {
            }
            void init(const std::string& animationFile);
            void blend( const configAnims &ca );
            void addTime( Real );
            bool loadAnimationsFromFile(const std::string &filename);
            inline Real getProgress() { return mTimeleft/ mDuration; }
            inline AnimationState *getSource() { return mSource; }
            inline AnimationState *getTarget() { return mTarget; }

            Actions getCurrentAnimation() { return mCurrentAnim; }

            void DoAction(Actions action);
            inline bool Complete() { return complete;}

        private:
            std::vector<configAnims> mConfig;    //указатель на настройки действий
            Entity *mEntity;
            AnimationState *mSource;
            AnimationState *mTarget;
            Actions mSourceAction;    //исходное действие
            Actions mTargetAction;    //последующее действие
            Real mSourceSpeed;    //скорость исходной анимации
            Real mTargetSpeed;    //скорость последующей анимации
            ActionTermination mSourceActionTermination;
            ActionTermination mTargetActionTermination;
            bool mBreak;

            BlendingTransition mTransition;
            bool mSourceLooped;
            bool mTargetLooped;
            Real mTimeleft, mDuration;
            bool complete;

            Actions mCurrentAnim;
    };

}
#endif