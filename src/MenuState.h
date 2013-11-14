#ifndef MenuStateH
#define MenuStateH

#include "State.h"
#include "GameplaySettings.h"
#include "Profile.h"

namespace Gibdd
{
    class GameplaySettings;
    class ProfileManager;
    class Profile;

    class MenuState: public State
    {
        public:
            MenuState();
            ~MenuState();

            void mouseMove(const OIS::MouseEvent &e);
            void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

            void keyPressed(const OIS::KeyEvent &e);
            void keyReleased(const OIS::KeyEvent &e);

            void povMoved(const OIS::JoyStickEvent &e, int pov);
            void axisMoved(const OIS::JoyStickEvent &e, int axis);
            void sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
            void buttonPressed(const OIS::JoyStickEvent &e, int button);
            void buttonReleased(const OIS::JoyStickEvent &e, int button);

            bool frameStarted(const Ogre::FrameEvent &evt);
            bool frameEnded(const Ogre::FrameEvent &evt);
            void preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
            void postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);

            void Hide();
            void Enter();
            void Restore();

        private:

            void loadGui();
            void changeCurrPlayerInfo(GameplayActions action, OIS::Type inputDevice, int deviceID,OIS::KeyCode key, OIS::ComponentType compType, int val, int direction);

            void notifyExitButtonClick(MyGUI::WidgetPtr _sender);
            void notifyStartButtonClick(MyGUI::WidgetPtr _sender);
            void notifyDebugButtonClick(MyGUI::WidgetPtr _sender);
            void notifySingleButtonClick(MyGUI::WidgetPtr _sender);
            void notifySplitButtonClick(MyGUI::WidgetPtr _sender);
            void notifyPedButtonClick(MyGUI::WidgetPtr _sender);
            void notifyDrivButtonClick(MyGUI::WidgetPtr _sender);
            void notifyAccelerateButtonClick(MyGUI::WidgetPtr _sender);
            void notifyBrakeButtonClick(MyGUI::WidgetPtr _sender);
            void notifySteerLeftButtonClick(MyGUI::WidgetPtr _sender);
            void notifySteerRightButtonClick(MyGUI::WidgetPtr _sender);
            void notifyGearUpButtonClick(MyGUI::WidgetPtr _sender);
            void notifyGearDownButtonClick(MyGUI::WidgetPtr _sender);
            void notifyTurnLeftLButtonClick(MyGUI::WidgetPtr _sender);
            void notifyTurnRightLButtonClick(MyGUI::WidgetPtr _sender);
            void notifyAddPlayerButtonClick(MyGUI::WidgetPtr _sender);
            void notifyRemovePlayerButtonClick(MyGUI::WidgetPtr _sender);
            void notifyChangePlayerButtonClick(MyGUI::WidgetPtr _sender);
            void notifyChangeOkButtonClick(MyGUI::WidgetPtr _sender);

            void prepareKeyMap();
            void processAxisMoved();
            void fillPlayersList();

        private:

            MyGUI::Gui *pGUI;
            MyGUI::WidgetPtr pWidget;
            MyGUI::EditPtr   pEditPed;
            MyGUI::EditPtr   pEditDriv;
            MyGUI::EditPtr   pEditPlayerName;
            MyGUI::ButtonPtr pBuAccel;
            MyGUI::ButtonPtr pBuBrake;
            MyGUI::ButtonPtr pBuSteerL;
            MyGUI::ButtonPtr pBuSteerR;
            MyGUI::ButtonPtr pBuGearUp;
            MyGUI::ButtonPtr pBuGearDown;
            MyGUI::ButtonPtr pBuTurnLightL;
            MyGUI::ButtonPtr pBuTurnLightR;
            MyGUI::ListPtr   pListAvalPlayers;
            MyGUI::WindowPtr pWinCtrl;
            GameplaySettings *gpSettings;

            bool bEditPlayer;
            bool bNewPlayer;
            std::string sCurrPlayer;
            bool bEditAccelerate;
            bool bEditBrake;
            bool bEditSteerL;
            bool bEditSteerR;
            bool bEditGearUp;
            bool bEditGearDown;
            bool bEditTurnLL;
            bool bEditTurnRL;

            typedef stdext::hash_map<OIS::KeyCode, std::string> KeyMap;
            KeyMap mKeyMap;
            KeyMap mJoyMap;
            PlayerInfo mCurrPlayerInfo;
            Profile *pCurrProfile;
            ProfileManager *pProfileMgr;
    };
}

#endif