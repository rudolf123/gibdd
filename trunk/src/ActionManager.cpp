#include "Precompiled.h"
#include "ActionManager.h"
#include <fstream>

#pragma warning (disable : 4996)

namespace Gibdd
{
    void ActionManager::init(const std::string& animationFile)
    {
        loadAnimationsFromFile(animationFile);
        AnimationStateSet *set = mEntity->getAllAnimationStates();
        AnimationStateIterator it = set->getAnimationStateIterator();
        while(it.hasMoreElements())
        {
            AnimationState *anim = it.getNext();
            anim->setEnabled(false);
            anim->setWeight(0);
            anim->setTimePosition(0);
        }
        std::vector<configAnims>::iterator iter = find_if(mConfig.begin(), mConfig.end(), find_byaction(WAIT));
        if (iter!=mConfig.end())
        {
            mSource = mEntity->getAnimationState( (*iter).mName );
            mSource->setEnabled(true);
            mSource->setWeight(1);
            mSourceLooped=(*iter).mLooped;
            mDuration = (*iter).mDuration;
            mTransition = (*iter).mTransition;
            mSourceSpeed = (*iter).mSpeed;
            mSourceAction = (*iter).mActions;
            mSourceActionTermination = (*iter).mActionTermination;
            mBreak=(*iter).mBreak;

        }
        else
        {
            mSource=0;
            mDuration = 1;
        }
        mTimeleft = 0;
        mTarget = 0;
        complete=false;
    } 

    bool ActionManager::loadAnimationsFromFile(const std::string &filename)
    {
        std::string line;

        configAnims mConfigAnims;

		std::ifstream ifs(filename.c_str());
        if(!ifs.is_open())
            return false;

        while(!ifs.eof())
        {
            std::getline(ifs, line);
            if(line.empty())
                continue;

            std::string val[9];
            uint8 itr = 0;

            char* pch = strtok((char*)line.c_str(), ";");
            while(pch)
            {
                if(itr > 8)
                    break;

                val[itr] = pch;

                pch = strtok(NULL, ";");
                ++itr;
            }

            mConfigAnims.mActions = (Actions)atoi(val[0].c_str());
            mConfigAnims.mSpeed = atof(val[1].c_str());
            mConfigAnims.mDuration = atof(val[2].c_str());
            if (val[3] == "true")
                mConfigAnims.mLooped = true;
            else
                mConfigAnims.mLooped = false;
            mConfigAnims.mName = val[4];
            if (val[5] == "true")
                mConfigAnims.mSoundEnabled = true;
            else
                mConfigAnims.mSoundEnabled = false;
            if (val[6] == "true")
                mConfigAnims.mBreak = true;
            else
                mConfigAnims.mBreak = false;
            mConfigAnims.mActionTermination = (ActionTermination)atoi(val[7].c_str());
            mConfigAnims.mTransition = (BlendingTransition)atoi(val[8].c_str());

            mConfig.push_back(mConfigAnims);
        }
        ifs.close();

        return true;
    }
    void ActionManager::blend( const configAnims &ca )
    {
        if ( mSource != 0 )
        {
            if (mSourceActionTermination != Final)
            {
                mBreak=ca.mBreak;
                if( ca.mTransition == BlendSwitch )
                {
                    if( mSource != 0 ) mSource->setEnabled(false);
                    mSource = mEntity->getAnimationState( ca.mName );
                    mSource->setEnabled(true);
                    mSource->setWeight(1);
                    mSource->setTimePosition(0);
                    mTimeleft = 0;
                } 
                else 
                { 
                    AnimationState *newTarget = mEntity->getAnimationState( ca.mName );
                    if( mTimeleft > 0 || (mSourceActionTermination==Limit && !mBreak))    // не закончено предыдущее смешивание или ограничено завершение действия
                    {
                        if (mSourceActionTermination==Limit && !mBreak)
                        {
                            if( ca.mActions == mTargetAction )
                            {
                                // ничего не делаем (игнорируем время смешивания)
                            }
                            else if( ca.mActions == mSourceAction )
                            {
                                // ничего не делаем (игнорируем время смешивания)
                            }
                            else
                            {
                                mTarget = newTarget;
                                mTargetSpeed=ca.mSpeed;
                                mTargetAction=ca.mActions;
                                mTargetActionTermination=ca.mActionTermination;
                                mTargetLooped=ca.mLooped;
                            }
                        }
                        else if (mTimeleft > 0)
                        {
                            if( ca.mActions == mTargetAction )
                            {
                                // ничего не делаем (игнорируем время смешивания)
                            }
                            else if( ca.mActions == mSourceAction )
                            {
                                /*
                                Помогаем быстрее завершить предыдущее смешивание и назначаем
                                новую анимацию для смешивания.
                                */
                                mSource = mTarget;
                                mSourceSpeed=mTargetSpeed;
                                mSourceAction=mTargetAction;
                                mSourceActionTermination=mTargetActionTermination;
                                mSourceLooped=mTargetLooped;

                                mTarget = newTarget;
                                mTargetSpeed=ca.mSpeed;
                                mTargetAction=ca.mActions;
                                mTargetActionTermination=ca.mActionTermination;
                                mTargetLooped=ca.mLooped;

                                mTimeleft = mDuration - mTimeleft; //игнорируем время смешивания
                            }
                            else
                            {
                                /*
                                Это действительно новая анимация, поэтому обновляем последующее
                                или исходное состояние, в зависимости от того, сколько длится смешивание
                                */
                                if( mTimeleft < mDuration * 0.5 )    //смешивание недавно началось
                                {
                                    // просто сбрасываем старое последующее состояние и назначим новое
                                    mTarget->setEnabled(false);
                                    mTarget->setWeight(0);
                                }
                                else
                                {
                                    // сбрасываем исходное состояние и присваиваем ему последующее
                                    mSource->setEnabled(false);
                                    mSource->setWeight(0);
                                    mSource = mTarget;
                                    mSourceSpeed=mTargetSpeed;
                                    mSourceAction=mTargetAction;
                                    mSourceActionTermination=mTargetActionTermination;
                                    mSourceLooped=mTargetLooped;
                                } 
                                mTarget = newTarget;
                                mTargetSpeed=ca.mSpeed;
                                mTargetAction=ca.mActions;
                                mTargetActionTermination=ca.mActionTermination;
                                mTargetLooped=ca.mLooped;
                                mTarget->setEnabled(true);
                                mTarget->setWeight( 1.0 - mTimeleft / mDuration );
                                mTarget->setTimePosition(0);
                            }
                        }
                    }
                    else
                    {
                        mTransition = ca.mTransition;
                        mTimeleft = mDuration = ca.mDuration;
                        mTarget = newTarget;
                        mTargetSpeed=ca.mSpeed;
                        mTargetAction=ca.mActions;
                        mTargetActionTermination=ca.mActionTermination;
                        mTargetLooped=ca.mLooped;
                        mTarget->setEnabled(true);
                        mTarget->setWeight(0);
                        mTarget->setTimePosition(0);
                    }
                }
            }
        }
    }
    void ActionManager::addTime( Real time )
    {
     if( mSource != 0 )
     {
        if( mTimeleft > 0 )
        {
           mTimeleft -= time * mSourceSpeed;
           if( mTimeleft <= 0 )
           {
              // смешивание завершено
              mSource->setEnabled(false);
              mSource->setWeight(0);
              mSource = mTarget;
              mSourceSpeed=mTargetSpeed;
              mSourceAction=mTargetAction;
              mSourceActionTermination=mTargetActionTermination;
              mSourceLooped=mTargetLooped;
              mSource->setEnabled(true);
              mSource->setWeight(1);
              mTarget = 0;
           }
           else
           {

              // плавное смешивание, пересчитываем вес анимаций
              mSource->setWeight(mTimeleft / mDuration);    //вес плавно уменьшается
              mTarget->setWeight(1.0 - mTimeleft / mDuration);    //вес плавно увеличивается
              if (mTransition == BlendWhileAnimating)
                 mTarget->addTime(time * mTargetSpeed);
           }
        }
        if (mSource->getTimePosition() >= mSource->getLength()/5*4)
        {
            if (mSourceActionTermination==Limit)
            {
                if (mTarget!=0)
                {

                    mTarget->setTimePosition(0);
                    // смешивание завершено
                    mSource->setEnabled(false);
                    mSource->setWeight(0);
                    mSource = mTarget;
                    mSourceSpeed=mTargetSpeed;
                    mSourceAction=mTargetAction;
                    mSourceActionTermination=mTargetActionTermination;
                    mSourceLooped=mTargetLooped;
                    mSource->setEnabled(true);
                    mSource->setWeight(1);
                    mTarget = 0;
                }
            }
            complete=true;
        }
        else
        {
          complete=false;
        }
        mSource->addTime(time * mSourceSpeed);
        mSource->setLoop(mSourceLooped);
     }
    }

    void ActionManager::DoAction(Actions action)
    {
        mCurrentAnim = action;

        std::vector<configAnims>::iterator it = find_if(mConfig.begin(), mConfig.end(), find_byaction(action));
        if (it!=mConfig.end())
        {
            blend( *it );
        }
    }

}

