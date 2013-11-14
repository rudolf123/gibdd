#ifndef RootH
#define RootH

namespace Gibdd
{
    class GameplaySettings;
    class ProfileManager;

    class Root: public Ogre::FrameListener,
                public Ogre::RenderTargetListener,
                public Ogre::WindowEventListener,
                public OIS::MouseListener,
                public OIS::KeyListener,
                public OIS::JoyStickListener
    {
        public:
            Root();
            ~Root();

            static Root& GetInstance() { return *pInstance; }

            bool Initialize();

            void StartEngine();
            
            void ShowState(uint8 idx);

            Ogre::SceneManager* GetSceneManager() { return pSceneMgr; }
            OIS::Keyboard* GetKeyboard() { return pKeyboard; }
            OIS::Mouse* GetMouse() { return pMouse; }
            OIS::JoyStick* GetJoystick(uint8 num) { if (num == 0) return pJoystick1;else  return pJoystick2; }
            MyGUI::Gui* GetGui() { return pGUI; }
            Ogre::Camera* GetCamera()  { return pCamera; }
            Ogre::RenderWindow* GetRenderWindow()  { return pWindow; }
            GameplaySettings* GetGameplaySettings()  { return pGameplaySettings; }
            ProfileManager* GetProfileManager()  { return pProfileMgr; }

            void RequestShutdown();

            void LoadLayout(const std::string& _layout, bool visible = false);

            void preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
            void postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);

        private:
            static Root* pInstance;

            Ogre::Root* pRoot;
            Ogre::RenderWindow* pWindow;
            Ogre::Camera* pCamera;
            Ogre::Viewport* pVp;

            Ogre::Light* pLight;
            Ogre::Light* pLightDir;

            Ogre::SceneManager* pSceneMgr;

            typedef stdext::hash_map<uint8, Gibdd::State*> StateMap;
            StateMap mStateMap;

            Gibdd::State* pCurrentState;

            OIS::InputManager* pInputManager;

            OIS::Mouse* pMouse;
            OIS::Keyboard* pKeyboard;
            OIS::JoyStick* pJoystick1;
            OIS::JoyStick* pJoystick2;

            MyGUI::Gui* pGUI;
            MyGUI::OgrePlatform* pGUIPlatform;

            GameplaySettings *pGameplaySettings;
            ProfileManager *pProfileMgr;

        private:
            void LoadPlugins();
            void LoadResource();
            void LoadSettings();

            bool keyPressed(const OIS::KeyEvent &e);
            bool keyReleased(const OIS::KeyEvent &e);

            bool mouseMoved(const OIS::MouseEvent &e);
            bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

            bool povMoved(const OIS::JoyStickEvent &e, int pov);
            bool axisMoved(const OIS::JoyStickEvent &e, int axis);
            bool sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
            bool buttonPressed(const OIS::JoyStickEvent &e, int button);
            bool buttonReleased(const OIS::JoyStickEvent &e, int button);

            bool frameStarted(const Ogre::FrameEvent& evt);
            bool frameEnded(const Ogre::FrameEvent& evt);

            ///
            
            void InitGUI();
            void InitCamera();
            void InitLights();
            void InitOIS();
    };
}

#endif