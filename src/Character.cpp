#include "Precompiled.h"
#include "Character.h"

namespace Gibdd
{
    Character::Character(Ogre::Camera *cam, PhysicManager *physicMgr, Ogre::Vector3 pos):Ragdoll(physicMgr)
    {
        pPhysicMgr = physicMgr;
        LOGS("setupBody");
        setupBody(cam->getSceneManager(), pos);
        setupRagdoll();
        LOGS("setupCamera");
        setupCamera(cam);
        LOGS("setupAnimations");
        setupAnimations("123");

        isRunning = false;
        iRunSpeed = 1;
    }

    Character::~Character()
    {
       // if (pController)
        //    delete pController;
    }
    void Character::mouseMove(const OIS::MouseEvent &e)
    {
       // LOGS("Character::mouseMove");
        updateCameraGoal(-0.05f * e.state.X.rel, -0.05f * e.state.Y.rel, -0.0005f * e.state.Z.rel);
    }
    void Character::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }
    void Character::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }
    void Character::keyPressed(const OIS::KeyEvent &evt)
    {
      //  LOGS("Character::keyPressed");
        // keep track of the player's intended direction
        if (evt.key == OIS::KC_LSHIFT)
        {
            iRunSpeed = 5;
            isRunning = true;

            setBaseAnimation(ANIM_RUN_LOOP, true);
        }

        if (evt.key == OIS::KC_W) mKeyDirection.z = 1;
        else if (evt.key == OIS::KC_A) mKeyDirection.x = 1;
        else if (evt.key == OIS::KC_S) mKeyDirection.z = -1;
        else if (evt.key == OIS::KC_D) mKeyDirection.x = -1;


        if (!mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_WAIT)
        {
            // start running if not already moving and the player wants to move
            if (isRunning)
                setBaseAnimation(ANIM_RUN_LOOP, true);
            else
                setBaseAnimation(ANIM_WALK, true);            
        }

        

    }
    void Character::keyReleased(const OIS::KeyEvent &evt)
    {
        if (evt.key == OIS::KC_LSHIFT)
        {
            iRunSpeed = 1;
            isRunning = false;

            setBaseAnimation(ANIM_WALK, true); 
        }
        // keep track of the player's intended direction
        if (evt.key == OIS::KC_W && mKeyDirection.z == 1) mKeyDirection.z = 0;
        else if (evt.key == OIS::KC_A && mKeyDirection.x == 1) mKeyDirection.x = 0;
        else if (evt.key == OIS::KC_S && mKeyDirection.z == -1) mKeyDirection.z = 0;
        else if (evt.key == OIS::KC_D && mKeyDirection.x == -1) mKeyDirection.x = 0;

        if (mKeyDirection.isZeroLength() && (mBaseAnimID == ANIM_WALK || mBaseAnimID == ANIM_RUN_LOOP))
        {
            // stop running if already moving and the player doesn't want to move
            setBaseAnimation(ANIM_WAIT);
        }
    }
    void Character::setupRagdoll()
    {
        setMesh(mBodyEnt);
        setNode(mBodyNode);
        init();
    }
    void Character::setupCamera(Camera* cam)
    {
        // create a pivot at roughly the character's shoulder
        mCameraPivot = cam->getSceneManager()->getRootSceneNode()->createChildSceneNode();
        // this is where the camera should be soon, and it spins around the pivot
        mCameraGoal = mCameraPivot->createChildSceneNode(Vector3(0, 0, 15));
        // this is where the camera actually is
        mCameraNode = cam->getSceneManager()->getRootSceneNode()->createChildSceneNode();
        mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

        mCameraPivot->setFixedYawAxis(true);
        mCameraGoal->setFixedYawAxis(true);
        mCameraNode->setFixedYawAxis(true);

        // our model is quite small, so reduce the clipping planes
        cam->setNearClipDistance(0.1);
        cam->setFarClipDistance(100);
        mCameraNode->attachObject(cam);

        mPivotPitch = 0;
    }

    void Character::setupBody(SceneManager* sceneMgr, Ogre::Vector3 pos)
    {
        // create main model
        mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y);
        mBodyNode->setScale(0.006,0.006,0.006);
        mBodyEnt = sceneMgr->createEntity("CharacterModel", "Body01.mesh");
        mBodyNode->attachObject(mBodyEnt);

        mKeyDirection = Vector3::ZERO;
        mVerticalVelocity = 0;

        pController = pPhysicMgr->CreateCharacterController(pos, 0.5, 0.5);
        Vector3 startpos = pos;
        startpos.y += pController->GetCharacterHeight(); 
        pController->SetCharacterPos(toNx(startpos));
        mBodyNode->setPosition(toOgre(NxVec3(pController->GetCharacterPos().x, pController->GetCharacterPos().y+0.5, pController->GetCharacterPos().z)));


        LOGS("Bones of " + mBodyEnt->getName() + "/////////////////////////////////////");
        Ogre::Bone *pBone;
        Ogre::Skeleton::BoneIterator boneitr = mBodyEnt->getSkeleton()->getBoneIterator();
        while (boneitr.hasMoreElements())
        {
            pBone = (Bone *)boneitr.getNext();

            LOGS(pBone->getName());
        }

        LOGS("/////////////////////////////////////");
    }

    void Character::setupAnimations(std::string filename)
    {
        // this is very important due to the nature of the exported animations
        mBodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

        String animNames[] =
        {"Wait", "WalkForward", "RunStart", "RunLoop", "RunStop", "RunBack"};

        // populate our animation list
        for (int i = 0; i < NUM_ANIMS; i++)
        {
            mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
            mAnims[i]->setLoop(true);
            mFadingIn[i] = false;
            mFadingOut[i] = false;
        }

        // start off in the idle state (top and bottom together)
        setBaseAnimation(ANIM_WAIT);

        // relax the hands since we're not holding anything
       // mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
    }

    void Character::setBaseAnimation(AnimID id, bool reset)
    {
        if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
        {
            // if we have an old animation, fade it out
            mFadingIn[mBaseAnimID] = false;
            mFadingOut[mBaseAnimID] = true;
        }

        mBaseAnimID = id;

        if (id != ANIM_NONE)
        {
            // if we have a new animation, enable it and fade it in
            mAnims[id]->setEnabled(true);
            mAnims[id]->setWeight(0);
            mFadingOut[id] = false;
            mFadingIn[id] = true;
            if (reset) mAnims[id]->setTimePosition(0);
        }
    }

    void Character::updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom)
    {
      //  LOGS("Character::updateCameraGoal");
        mCameraPivot->yaw(Degree(deltaYaw), Node::TS_WORLD);

        // bound the pitch
        if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
            !(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
        {
            mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
            mPivotPitch += deltaPitch;
        }

        Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
        Real distChange = deltaZoom * dist;

        // bound the zoom
        if (!(dist + distChange < 8 && distChange < 0) &&
            !(dist + distChange > 25 && distChange > 0))
        {
            mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
        }
    }

    void Character::updateCamera(Real deltaTime)
    {
     //   LOGS("Character::updateCamera");
        // place the camera pivot roughly at the character's shoulder
        mCameraPivot->setPosition(mBodyNode->getPosition() + Vector3::UNIT_Y * CAM_HEIGHT);
        // move the camera smoothly to the goal
        Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
        mCameraNode->translate(goalOffset * deltaTime * 9.0f);
        // always look at the pivot
        mCameraNode->lookAt(mCameraPivot->_getDerivedPosition(), Node::TS_WORLD);
    }

    void Character::fadeAnimations(Real deltaTime)
    {
     //   LOGS("Character::fadeAnimations");
        for (int i = 0; i < NUM_ANIMS; i++)
        {
            if (mFadingIn[i])
            {
                // slowly fade this animation in until it has full weight
                Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
                mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
                if (newWeight >= 1) mFadingIn[i] = false;
            }
            else if (mFadingOut[i])
            {
                // slowly fade this animation out until it has no weight, and then disable it
                Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
                mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
                if (newWeight <= 0)
                {
                    mAnims[i]->setEnabled(false);
                    mFadingOut[i] = false;
                }
            }
        }
    }

    void Character::updateAnimations(Real deltaTime)
    {
      //  LOGS("Character::updateAnimations");
        Real baseAnimSpeed = 1;
        Real topAnimSpeed = 1;

        mTimer += deltaTime;

        /*if (mTopAnimID == ANIM_DRAW_SWORDS)
        {
            // flip the draw swords animation if we need to put it back
            topAnimSpeed = mSwordsDrawn ? -1 : 1;

            // half-way through the animation is when the hand grasps the handles...
            if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
                mTimer - deltaTime < mAnims[mTopAnimID]->getLength() / 2)
            {
                // so transfer the swords from the sheaths to the hands
                mBodyEnt->detachAllObjectsFromBone();
                mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mSword1);
                mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mSword2);
                // change the hand state to grab or let go
                mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
                mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);

                // toggle sword trails
                if (mSwordsDrawn)
                {
                    mSwordTrail->setVisible(false);
                    mSwordTrail->removeNode(mSword1->getParentNode());
                    mSwordTrail->removeNode(mSword2->getParentNode());
                }
                else
                {
                    mSwordTrail->setVisible(true);
                    mSwordTrail->addNode(mSword1->getParentNode());
                    mSwordTrail->addNode(mSword2->getParentNode());
                }
            }

            if (mTimer >= mAnims[mTopAnimID]->getLength())
            {
                // animation is finished, so return to what we were doing before
                if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
                else
                {
                    setTopAnimation(ANIM_RUN_TOP);
                    mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
                }
                mSwordsDrawn = !mSwordsDrawn;
            }
        }
        else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
        {
            if (mTimer >= mAnims[mTopAnimID]->getLength())
            {
                // animation is finished, so return to what we were doing before
                if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
                else
                {
                    setTopAnimation(ANIM_RUN_TOP);
                    mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
                }
            }

            // don't sway hips from side to side when slicing. that's just embarrasing.
            if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
        }
        else if (mBaseAnimID == ANIM_JUMP_START)
        {
            if (mTimer >= mAnims[mBaseAnimID]->getLength())
            {
                // takeoff animation finished, so time to leave the ground!
                setBaseAnimation(ANIM_JUMP_LOOP, true);
                // apply a jump acceleration to the character
                mVerticalVelocity = JUMP_ACCEL;
            }
        }
        else if (mBaseAnimID == ANIM_JUMP_END)
        {
            if (mTimer >= mAnims[mBaseAnimID]->getLength())
            {
                // safely landed, so go back to running or idling
                if (mKeyDirection == Vector3::ZERO)
                {
                    setBaseAnimation(ANIM_IDLE_BASE);
                    setTopAnimation(ANIM_IDLE_TOP);
                }
                else
                {
                    setBaseAnimation(ANIM_RUN_BASE, true);
                    setTopAnimation(ANIM_RUN_TOP, true);
                }
            }
        }*/

        // increment the current base and top animation times
        if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);

        // apply smooth transitioning between our animations
        fadeAnimations(deltaTime);
    }

    void Character::updateBody(Real deltaTime)
    {
      //  LOGS("Character::updateBody");
        mGoalDirection = Vector3::ZERO;   // we will calculate this

        if (mKeyDirection != Vector3::ZERO)
        {
            // calculate actually goal direction in world based on player's key directions
            mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();
            mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
            mGoalDirection.y = 0;
            mGoalDirection.normalise();

            Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

            // calculate how much the character has to turn to face goal direction
            Real yawToGoal = toGoal.getYaw().valueDegrees();
            // this is how much the character CAN turn this frame
            Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;

            // turn as much as we can, but not more than we need to
            if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
            else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);

            mBodyNode->yaw(Degree(yawToGoal));

            // move in current body direction (not the goal direction)
         //   mBodyNode->translate(0, 0, deltaTime * RUN_SPEED * mAnims[mBaseAnimID]->getWeight(),
          //      Node::TS_LOCAL);
            NxU32 u32;
            pController->Move(toNx(Ogre::Vector3(-mGoalDirection.x, -98, -mGoalDirection.z)) * deltaTime * iRunSpeed * mAnims[mBaseAnimID]->getWeight(), u32);
            mBodyNode->setPosition(toOgre(NxVec3(pController->GetCharacterPos().x, pController->GetCharacterPos().y+0.5, pController->GetCharacterPos().z)));
        }
    }

    void Character::executeRagdoll(bool exec)
    {
        SetRagDoll(exec);

        if (exec)
            pController->GetCharacterActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
        else
            pController->GetCharacterActor()->clearActorFlag(NX_AF_DISABLE_COLLISION);
    }

    void Character::addTime(Real deltaTime)
    {
       // LOGS("Character::addTime");
        if (!isRagdoll())
        {
            updateBody(deltaTime);
            updateAnimations(deltaTime);
        }
        else
            updateRagdoll(deltaTime);

        updateCamera(deltaTime);
    }
}