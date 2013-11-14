#include "Precompiled.h"
#include "MenuState.h"
#include "Root.h"
#include "ProfileManager.h"

#include <sstream>

namespace Gibdd
{
    MenuState::MenuState()
    {

    }

    MenuState::~MenuState()
    {

    }

    void MenuState::mouseMove(const OIS::MouseEvent &e)
    {

    }

    void MenuState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void MenuState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void MenuState::keyPressed(const OIS::KeyEvent &e)
    {
        if (e.key == OIS::KC_ESCAPE)
            Root::GetInstance().RequestShutdown();
        if (e.key == OIS::KC_SPACE)
        {
            
        }

       // if (e.key == OIS::KC_D)
         //   Root::GetInstance().ShowState(State::STATE_DEBUG);

        if (bEditPlayer)
        {
            if (bEditAccelerate)
            {
                changeCurrPlayerInfo(ACCELERATE, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditAccelerate = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    //char c = static_cast<char>(e.text);
                    pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditBrake)
            {
                changeCurrPlayerInfo(BRAKE, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditBrake = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buBrake")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buBrake")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditSteerL)
            {
                changeCurrPlayerInfo(STEERLEFT, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditSteerL = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buSteerL")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buSteerL")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditSteerR)
            {
                changeCurrPlayerInfo(STEERRIGHT, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditSteerR = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buSteerR")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buSteerR")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditTurnLL)
            {
                changeCurrPlayerInfo(LEFTLIGHT, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditTurnLL = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buTurnLeftL")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buTurnLeftL")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditTurnRL)
            {
                changeCurrPlayerInfo(RIGHTLIGHT, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditTurnRL = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buTurnRightL")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buTurnRightL")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditGearUp)
            {
                changeCurrPlayerInfo(GEARUP, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditGearUp = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buGearUp")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buGearUp")->setCaption(mKeyMap[e.key]);

                return;
            }
            if (bEditGearDown)
            {
                changeCurrPlayerInfo(GEARDOWN, OIS::OISKeyboard, e.device->getID(), e.key, OIS::OIS_Unknown, 0, 0);
                bEditGearDown = false;
                KeyMap::iterator itr = mKeyMap.find(e.key);
                if(itr == mKeyMap.end())
                {
                    std::string s(1, static_cast<char>(e.text));
                    pGUI->findWidget<MyGUI::Button>("buGearDown")->setCaption(s);
                }
                else
                    pGUI->findWidget<MyGUI::Button>("buGearDown")->setCaption(mKeyMap[e.key]);

                return;
                return;
            }

        }
    }

    void MenuState::keyReleased(const OIS::KeyEvent &e)
    {

    }

    void MenuState::povMoved(const OIS::JoyStickEvent &e, int pov)
    {
        if (bEditPlayer)
        {
            if (bEditAccelerate)
            {
                changeCurrPlayerInfo(ACCELERATE, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_POV, pov, e.state.mPOV[pov].direction);
                bEditAccelerate = false;
                pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditBrake)
            {
                changeCurrPlayerInfo(BRAKE, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_POV, pov, e.state.mPOV[pov].direction);
                bEditBrake = false;
                pGUI->findWidget<MyGUI::Button>("buBrake")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditSteerL)
            {
                changeCurrPlayerInfo(STEERLEFT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_POV, pov, e.state.mPOV[pov].direction);
                bEditSteerL = false;
                pGUI->findWidget<MyGUI::Button>("buSteerL")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditSteerR)
            {
                changeCurrPlayerInfo(STEERRIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_POV, pov, e.state.mPOV[pov].direction);
                bEditSteerR = false;
                pGUI->findWidget<MyGUI::Button>("buSteerR")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditTurnLL)
            {
                changeCurrPlayerInfo(LEFTLIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_POV, pov, e.state.mPOV[pov].direction);
                bEditTurnLL = false;
                pGUI->findWidget<MyGUI::Button>("buTurnLeftL")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditTurnRL)
            {
                changeCurrPlayerInfo(RIGHTLIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_POV, pov, e.state.mPOV[pov].direction);
                bEditTurnRL = false;
                pGUI->findWidget<MyGUI::Button>("buTurnRightL")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditGearUp)
            {
                changeCurrPlayerInfo(GEARUP, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, pov, e.state.mPOV[pov].direction);
                bEditGearUp = false;
                pGUI->findWidget<MyGUI::Button>("buGearUp")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
            if (bEditGearDown)
            {
                changeCurrPlayerInfo(GEARDOWN, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, pov, e.state.mPOV[pov].direction);
                bEditGearDown = false;
                pGUI->findWidget<MyGUI::Button>("buGearDown")->setCaption("POV " + TO_STRING(e.state.mPOV[pov].direction));

                return;
            }
        }
    }
    void MenuState::axisMoved(const OIS::JoyStickEvent &e, int axis)
    {
        if (bEditPlayer)
        {
            if (bEditAccelerate)
            {
                changeCurrPlayerInfo(ACCELERATE, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Axis, axis, e.state.mAxes[axis].abs);
                bEditAccelerate = false;
                pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption("Axis " + TO_STRING(axis) + TO_STRING(e.state.mAxes[axis].abs));

                return;
            }
            if (bEditBrake)
            {
                changeCurrPlayerInfo(BRAKE, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Axis, axis, e.state.mAxes[axis].abs);
                bEditBrake = false;
                pGUI->findWidget<MyGUI::Button>("buBrake")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
            if (bEditSteerL)
            {
                changeCurrPlayerInfo(STEERLEFT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Axis, axis, e.state.mAxes[axis].abs);
                bEditSteerL = false;
                pGUI->findWidget<MyGUI::Button>("buSteerL")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
            if (bEditSteerR)
            {
                changeCurrPlayerInfo(STEERRIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Axis, axis, e.state.mAxes[axis].abs);
                bEditSteerR = false;
                pGUI->findWidget<MyGUI::Button>("buSteerR")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
            if (bEditTurnLL)
            {
                changeCurrPlayerInfo(LEFTLIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Axis, axis, e.state.mAxes[axis].abs);
                bEditTurnLL = false;
                pGUI->findWidget<MyGUI::Button>("buTurnLeftL")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
            if (bEditTurnRL)
            {
                changeCurrPlayerInfo(RIGHTLIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Axis, axis, e.state.mAxes[axis].abs);
                bEditTurnRL = false;
                pGUI->findWidget<MyGUI::Button>("buTurnRightL")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
            if (bEditGearUp)
            {
                changeCurrPlayerInfo(GEARUP, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, axis, e.state.mAxes[axis].abs);
                bEditGearUp = false;
                pGUI->findWidget<MyGUI::Button>("buGearUp")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
            if (bEditGearDown)
            {
                changeCurrPlayerInfo(GEARDOWN, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, axis, e.state.mAxes[axis].abs);
                bEditGearDown = false;
                pGUI->findWidget<MyGUI::Button>("buGearDown")->setCaption("Axis " + TO_STRING(axis));

                return;
            }
        }
    }

    void MenuState::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
    {

    }
    void MenuState::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {
        if (bEditPlayer)
        {
            if (bEditAccelerate)
            {
                changeCurrPlayerInfo(ACCELERATE, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditAccelerate = false;
                pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditBrake)
            {
                changeCurrPlayerInfo(BRAKE, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditBrake = false;
                pGUI->findWidget<MyGUI::Button>("buBrake")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditSteerL)
            {
                changeCurrPlayerInfo(STEERLEFT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditSteerL = false;
                pGUI->findWidget<MyGUI::Button>("buSteerL")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditSteerR)
            {
                changeCurrPlayerInfo(STEERRIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditSteerR = false;
                pGUI->findWidget<MyGUI::Button>("buSteerR")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditGearUp)
            {
                changeCurrPlayerInfo(GEARUP, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditGearUp = false;
                pGUI->findWidget<MyGUI::Button>("buGearUp")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditGearDown)
            {
                changeCurrPlayerInfo(GEARDOWN, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditGearDown = false;
                pGUI->findWidget<MyGUI::Button>("buGearDown")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditTurnLL)
            {
                changeCurrPlayerInfo(LEFTLIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditTurnLL = false;
                pGUI->findWidget<MyGUI::Button>("buTurnLeftL")->setCaption("Button " + TO_STRING(button));

                return;
            }
            if (bEditTurnRL)
            {
                changeCurrPlayerInfo(RIGHTLIGHT, OIS::OISJoyStick, e.device->getID(), OIS::KeyCode(0), OIS::OIS_Button, button, 0);
                bEditTurnRL = false;
                pGUI->findWidget<MyGUI::Button>("buTurnRightL")->setCaption("Button " + TO_STRING(button));

                return;
            }
        }
    }
    void MenuState::buttonReleased(const OIS::JoyStickEvent &e, int button)
    {

    }

    bool MenuState::frameStarted(const Ogre::FrameEvent &evt)
    {
        processAxisMoved();
        return true;
    }

    bool MenuState::frameEnded(const Ogre::FrameEvent &evt)
    {
        return true;
    }

    void MenuState::Hide()
    {
        pWidget->setVisible(false);
    }

    void MenuState::Enter()
    {
        pGUI = Root::GetInstance().GetGui();
        gpSettings = Root::GetInstance().GetGameplaySettings();
        pProfileMgr = Root::GetInstance().GetProfileManager();
        loadGui();
        prepareKeyMap();
        bEditPlayer = false;
        bEditAccelerate = false;
        bEditBrake = false;
        bEditGearUp = false;
        bEditGearDown = false;
        bEditSteerL = false;
        bEditSteerR = false;
        bEditTurnLL = false;
        bEditTurnRL = false;
        pCurrProfile = 0;
    }

    void MenuState::Restore()
    {
    }

    void MenuState::loadGui()
    {
        pWidget = pGUI->findWidget<MyGUI::Widget>("MainMenu.layout");

        MyGUI::ButtonPtr button = pGUI->findWidget<MyGUI::Button>("buExit");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyExitButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("buStart");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyStartButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("buDebug");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyDebugButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("singleCheck");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifySingleButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("splitscreenCheck");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifySplitButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("pedbotCheck");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyPedButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("drivbotCheck");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyDrivButtonClick);

        pBuAccel = pGUI->findWidget<MyGUI::Button>("buAccel");
        pBuAccel->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyAccelerateButtonClick);

        pBuBrake = pGUI->findWidget<MyGUI::Button>("buBrake");
        pBuBrake->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyBrakeButtonClick);

        pBuSteerL = pGUI->findWidget<MyGUI::Button>("buSteerL");
        pBuSteerL->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifySteerLeftButtonClick);

        pBuSteerR = pGUI->findWidget<MyGUI::Button>("buSteerR");
        pBuSteerR->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifySteerRightButtonClick);

        pBuGearUp = pGUI->findWidget<MyGUI::Button>("buGearUp");
        pBuGearUp->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyGearUpButtonClick);

        pBuGearDown = pGUI->findWidget<MyGUI::Button>("buGearDown");
        pBuGearDown->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyGearDownButtonClick);

        pBuTurnLightL = pGUI->findWidget<MyGUI::Button>("buTurnLeftL");
        pBuTurnLightL->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyTurnLeftLButtonClick);

        pBuTurnLightR = pGUI->findWidget<MyGUI::Button>("buTurnRightL");
        pBuTurnLightR->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyTurnRightLButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("buAddPlayer");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyAddPlayerButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("buRemovePlayer");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyRemovePlayerButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("buChangePlayer");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyChangePlayerButtonClick);

        button = pGUI->findWidget<MyGUI::Button>("buChangeOk");
        button->eventMouseButtonClick = MyGUI::newDelegate(this, &MenuState::notifyChangeOkButtonClick);

        pWinCtrl = pGUI->findWidget<MyGUI::Window>("winControl");

        pEditPed = pGUI->findWidget<MyGUI::Edit>("pedbotnumEdit");
        pEditDriv = pGUI->findWidget<MyGUI::Edit>("drivbotnumEdit");
        pEditPlayerName = pGUI->findWidget<MyGUI::Edit>("ePlayerName");
        pListAvalPlayers = pGUI->findWidget<MyGUI::List>("listPlayers");

        fillPlayersList();

        pWidget->setVisible(true);
        pGUI->setVisiblePointer(true);
    }

    void MenuState::notifyStartButtonClick(MyGUI::WidgetPtr _sender)
    {
        Root::GetInstance().ShowState(State::STATE_PLAY);
        if (pEditDriv->getOnlyText()!="")
            gpSettings->setDrivsNum(Ogre::StringConverter::parseInt(pEditDriv->getOnlyText()));
        if (pEditPed->getOnlyText()!="")
            gpSettings->setPedsNum(Ogre::StringConverter::parseInt(pEditPed->getOnlyText()));
    }

    void MenuState::notifyExitButtonClick(MyGUI::WidgetPtr _sender)
    {
        Root::GetInstance().RequestShutdown();
    }
    void MenuState::notifyDebugButtonClick(MyGUI::WidgetPtr _sender)
    {
        Root::GetInstance().ShowState(State::STATE_DEBUG);
    }

    void MenuState::notifySingleButtonClick(MyGUI::WidgetPtr _sender)
    {
        MyGUI::ButtonPtr button = pGUI->findWidget<MyGUI::Button>("singleCheck");
        button->setButtonPressed(true);
        button = pGUI->findWidget<MyGUI::Button>("splitscreenCheck");
        button->setButtonPressed(false);
    }

    void MenuState::notifySplitButtonClick(MyGUI::WidgetPtr _sender)
    {
        MyGUI::ButtonPtr button = pGUI->findWidget<MyGUI::Button>("splitscreenCheck");
        button->setButtonPressed(true);
        button = pGUI->findWidget<MyGUI::Button>("singleCheck");
        button->setButtonPressed(false);
    }

    void MenuState::notifyPedButtonClick(MyGUI::WidgetPtr _sender)
    {
        MyGUI::ButtonPtr button = pGUI->findWidget<MyGUI::Button>("pedbotCheck");
        button->setButtonPressed(!button->getButtonPressed());

        if (pEditPed->getOnlyText().empty())
            pEditPed->setOnlyText("0");
    }

    void MenuState::notifyDrivButtonClick(MyGUI::WidgetPtr _sender)
    {
        MyGUI::ButtonPtr button = pGUI->findWidget<MyGUI::Button>("drivbotCheck");
        button->setButtonPressed(!button->getButtonPressed());

        if (pEditDriv->getOnlyText().empty())
            pEditDriv->setOnlyText("0");
    }

    void MenuState::notifyAccelerateButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditAccelerate = true;
    }
    void MenuState::notifyBrakeButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditBrake = true;
    }
    void MenuState::notifySteerLeftButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditSteerL = true;
    }
    void MenuState::notifyGearUpButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditGearUp = true;
    }
    void MenuState::notifyGearDownButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditGearDown = true;
    }
    void MenuState::notifySteerRightButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditSteerR = true;
    }
    void MenuState::notifyTurnLeftLButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditTurnLL = true;
    }
    void MenuState::notifyTurnRightLButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bEditPlayer)
            bEditTurnRL = true;
    }

    void MenuState::notifyAddPlayerButtonClick(MyGUI::WidgetPtr _sender)
    {
        pBuSteerL->setCaption("...");
        pBuSteerR->setCaption("...");
        pBuGearUp->setCaption("...");
        pBuGearDown->setCaption("...");
        pBuTurnLightL->setCaption("...");
        pBuTurnLightR->setCaption("...");
        pEditPlayerName->setCaption("");
        bNewPlayer = true;
        bEditPlayer = true;
        pWinCtrl->setVisible(true);

        pCurrProfile = 0;
        pCurrProfile = pProfileMgr->CreateProfile();
    }

    void MenuState::notifyRemovePlayerButtonClick(MyGUI::WidgetPtr _sender)
    {
        pProfileMgr->removeProfile(pListAvalPlayers->getItemNameAt(pListAvalPlayers->getIndexSelected()));
        pProfileMgr->saveProfiles("Profiles.xml");
        pProfileMgr->loadProfiles("Profiles.xml");
        fillPlayersList();
    }

    void MenuState::notifyChangePlayerButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (!pProfileMgr->isEmpty())
        {
            bEditPlayer = true;
            pCurrProfile = 0;
            pCurrProfile = pProfileMgr->getProfile(pListAvalPlayers->getItemNameAt(pListAvalPlayers->getIndexSelected()));
            pEditPlayerName->setCaption(pCurrProfile->GetName());

            pWinCtrl->setVisible(true);
        }
    }

    void MenuState::notifyChangeOkButtonClick(MyGUI::WidgetPtr _sender)
    {
        if (bNewPlayer)
        {
            if (pEditPlayerName->getCaption() != "")
            {
                pCurrProfile->SetName(pEditPlayerName->getCaption());
                if (!pProfileMgr->HasProfile(pCurrProfile->GetName()))
                    pProfileMgr->addProfile(pCurrProfile);
                else
                    pProfileMgr->DestroyProfile(pCurrProfile);
            }
        }
        else 
            if (pEditPlayerName->getCaption() != "")
                if (!pProfileMgr->HasProfile(pEditPlayerName->getCaption()))
                {
                    pCurrProfile->SetName(pEditPlayerName->getCaption());
                    pCurrProfile = 0;
                }

        pWinCtrl->setVisible(false);
        bEditPlayer = false;
        bNewPlayer = false;

        pProfileMgr->saveProfiles("Profiles.xml");
        pProfileMgr->loadProfiles("Profiles.xml");
        fillPlayersList();
    }

    void MenuState::preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {

    }
    void MenuState::postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {

    }

    void MenuState::changeCurrPlayerInfo(GameplayActions action, OIS::Type inputDevice, int deviceID,OIS::KeyCode key, OIS::ComponentType compType, int val, int direction)
    {
        if (inputDevice == OIS::OISKeyboard)
            pCurrProfile->bindAction(action, key);
        if (inputDevice == OIS::OISJoyStick)
            pCurrProfile->bindAction(action, deviceID, compType, val, direction);
    }

    void MenuState::processAxisMoved()
    {
        OIS::JoyStick* pJoy = Root::GetInstance().GetJoystick(0);
		if (!pJoy)
			return;
        for(uint16 i = 0; i < pJoy->getNumberOfComponents(OIS::OIS_Axis); ++i)
            if (pJoy->getJoyStickState().mAxes[i].abs == pJoy->MAX_AXIS)
                pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption("+ " + TO_STRING(pJoy->getJoyStickState().mAxes[i].abs));
            else if (pJoy->getJoyStickState().mAxes[i].abs == pJoy->MIN_AXIS)
                pGUI->findWidget<MyGUI::Button>("buAccel")->setCaption("- " + TO_STRING(pJoy->getJoyStickState().mAxes[i].abs));
    }

    void MenuState::fillPlayersList()
    {
        pListAvalPlayers->removeAllItems();
        Profile *pProfile = 0;
        pProfile = pProfileMgr->GetFirstProfile();
        if (!pProfile)
            return;

        while (pProfile)
        {
            pListAvalPlayers->addItem(pProfile->GetName());
            pProfile = pProfileMgr->GetNextProfile();
        }
    }

    void MenuState::prepareKeyMap()
    {
        mKeyMap[OIS::KC_UP] = "UP";
        mKeyMap[OIS::KC_DOWN] = "DOWN";
        mKeyMap[OIS::KC_LEFT] = "LEFT";
        mKeyMap[OIS::KC_RIGHT] = "RIGHT";
        mKeyMap[OIS::KC_HOME] = "HOME";
        mKeyMap[OIS::KC_END] = "END";
        mKeyMap[OIS::KC_DELETE] = "DELETE";
        mKeyMap[OIS::KC_INSERT] = "INSERT";
        mKeyMap[OIS::KC_PGUP] = "PAGEUP";
        mKeyMap[OIS::KC_PGDOWN] = "PAGEDOWN";
        mKeyMap[OIS::KC_RCONTROL] = "RCONTROL";
        mKeyMap[OIS::KC_LCONTROL] = "LCONTROL";
        mKeyMap[OIS::KC_RSHIFT] = "RSHIFT";
        mKeyMap[OIS::KC_LSHIFT] = "LSHIT";
        mKeyMap[OIS::KC_SPACE] = "SPACE";
        mKeyMap[OIS::KC_RMENU] = "RALT";
        mKeyMap[OIS::KC_LMENU] = "LALT";
        mKeyMap[OIS::KC_NUMPAD1] = "NUM1";
        mKeyMap[OIS::KC_NUMPAD2] = "NUM2";
        mKeyMap[OIS::KC_NUMPAD3] = "NUM3";
        mKeyMap[OIS::KC_NUMPAD4] = "NUM4";
        mKeyMap[OIS::KC_NUMPAD5] = "NUM5";
        mKeyMap[OIS::KC_NUMPAD6] = "NUM6";
        mKeyMap[OIS::KC_NUMPAD7] = "NUM7";
        mKeyMap[OIS::KC_NUMPAD8] = "NUM8";
        mKeyMap[OIS::KC_NUMPAD9] = "NUM9";
        mKeyMap[OIS::KC_NUMPAD0] = "NUM0";
        mKeyMap[OIS::KC_TAB] = "TAB";

        //mJoyMap[OIS::]
    }
}