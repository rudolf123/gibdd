#include "Precompiled.h"
#include "Root.h"

#include "State.h"
#include "MenuState.h"
#include "PlayState.h"
#include "DebugState.h"

#include "SettingsLoader/SettingsLoader.h"
#include "GameplaySettings.h"
#include "ProfileManager.h"

namespace Gibdd
{
    Root* Root::pInstance = 0;

    Root::Root()
    {
        pInstance = this;

        pRoot = 0;
        pWindow = 0;
        pCamera = 0;
        pVp = 0;
        pJoystick1 = 0;
        pJoystick2 = 0;
        random_seed();

        pCurrentState = 0;
        pProfileMgr = 0;
    }

    Root::~Root()
    {
        StateMap::const_iterator itr = mStateMap.begin();
        for(; itr != mStateMap.end(); ++itr)
        {
            delete itr->second;
        }

        mStateMap.clear();

        Ogre::WindowEventUtilities::removeWindowEventListener(pWindow, this);

        if(pInputManager)
        {
            if(pMouse)
            {
                pInputManager->destroyInputObject(pMouse);
                pMouse = 0;
            }
            if(pKeyboard)
            {
                pInputManager->destroyInputObject(pKeyboard);
                pKeyboard = 0;
            }
            if(pJoystick1)
            {
                pInputManager->destroyInputObject(pJoystick1);
                pJoystick1 = 0;
            }
            if(pJoystick2)
            {
                pInputManager->destroyInputObject(pJoystick2);
                pJoystick2 = 0;
            }
            OIS::InputManager::destroyInputSystem(pInputManager);
            pInputManager = 0;
        }
        windowClosed(pWindow);

        if (pGUI)
        {
            pGUI->shutdown();
            delete pGUI;
            pGUI = 0;
        }

        if (pGUIPlatform)
        {
            pGUIPlatform->shutdown();
            delete pGUIPlatform;
            pGUIPlatform = 0;
        }

        if (pGameplaySettings)
        {
            delete pGameplaySettings;
            pGameplaySettings = 0;
        }
        if (pProfileMgr)
            delete pProfileMgr;
        if(pRoot)
            delete pRoot;
    }

    bool Root::Initialize()
    {
        pRoot = new Ogre::Root("", "", "Gibdd.log");
        
        LoadPlugins();
        LoadSettings();

        pWindow = pRoot->initialise(true, "Gibdd");
        if(!pWindow)
            return false;

        LoadResource();

        pSceneMgr = pRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);

        InitCamera();
        //InitLights();
        InitOIS();
        InitGUI();

        Ogre::WindowEventUtilities::addWindowEventListener(pWindow, this);
        pRoot->addFrameListener(this);

        ShowState(State::STATE_MENU);

        return true;
    }

    void Root::StartEngine()
    {
        while(!pWindow->isClosed())
        {
            Ogre::WindowEventUtilities::messagePump();

            pRoot->renderOneFrame();
        }
    }

    //////////////////////////////////////////////////////////////////////////

    void Root::LoadSettings()
    {
        sSettingsLoader.Initialize(pRoot);
        sSettingsLoader.LoadGameSettings();

        pGameplaySettings = new GameplaySettings();
        pProfileMgr = new ProfileManager();
        pProfileMgr->loadProfiles("Profiles.xml");
        //pGameplaySettings->saveSettingsToFile("PlayersConfig.txt");
    }

    void Root::InitGUI()
    {
        ShowCursor(false);

        pGUIPlatform = new MyGUI::OgrePlatform();
        pGUIPlatform->initialise(pWindow, pSceneMgr);

        pGUI = new MyGUI::Gui();
        pGUI->initialise("core.xml");

        LoadLayout("MainMenu.layout");
        LoadLayout("debug.layout");
        LoadLayout("play.layout");
    }

    void Root::InitCamera()
    {
        pCamera = pSceneMgr->createCamera("ExtendedCamera");
        pCamera->setPosition(Ogre::Vector3(0, 0, 0));
        pCamera->lookAt(Ogre::Vector3(0, 0, 0));

        pCamera->setNearClipDistance(0.1f);
        if(pRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
        {
            pCamera->setFarClipDistance(0);
        }
        else
            pCamera->setFarClipDistance(1000);

        pCamera->setUseRenderingDistance(true);
        pCamera->setAutoAspectRatio(true);
        pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);

        pVp = pWindow->addViewport(pCamera);
        pVp->setBackgroundColour(Ogre::ColourValue::Black);
    }

    void Root::InitLights()
    {
        {
            pLight = pSceneMgr->createLight("Light0");
            pLight->setPosition(800, 1000, 800);
            //mLight->setDiffuseColour(Ogre::ColourValue(1, 1, 0,94));
            pLight->setDiffuseColour(Ogre::ColourValue::White);
        }
        {
            pLightDir = pSceneMgr->createLight("Light1");
            pLightDir->setPosition(-800.0f, -1000.0f, -800.0f);
            pLightDir->setDiffuseColour(Ogre::ColourValue::White);
            //mLightDir->setDiffuseColour(Ogre::ColourValue(0.5, 0.72, 0.83));
        }
    }

    void Root::InitOIS()
    {
        OIS::ParamList pl;
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;

        pWindow->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;

        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
        pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

        pInputManager = OIS::InputManager::createInputSystem( pl );

        pKeyboard = static_cast<OIS::Keyboard*>(pInputManager->createInputObject( OIS::OISKeyboard, true ));
        pKeyboard->setEventCallback(this);

        pMouse = static_cast<OIS::Mouse*>(pInputManager->createInputObject( OIS::OISMouse, true ));
        pMouse->setEventCallback(this);

        if (pInputManager->getNumberOfDevices(OIS::OISJoyStick) > 1)
        {
            pJoystick1 = static_cast<OIS::JoyStick*>(pInputManager->createInputObject( OIS::OISJoyStick, true));
            pJoystick2 = static_cast<OIS::JoyStick*>(pInputManager->createInputObject( OIS::OISJoyStick, true));
        }
        else
            if (pInputManager->getNumberOfDevices(OIS::OISJoyStick) > 0)
                pJoystick1 = static_cast<OIS::JoyStick*>(pInputManager->createInputObject( OIS::OISJoyStick, true));

        if (pJoystick1)
            pJoystick1->setEventCallback(this);

        if (pJoystick2)
            pJoystick2->setEventCallback(this);

        windowResized(pWindow);

        int w = pWindow->getWidth();
        int h = pWindow->getHeight();
        const OIS::MouseState &ms = pMouse->getMouseState();
        ms.width = w;
        ms.height = h;
    }

    void Root::LoadPlugins()
    {
        pRoot->loadPlugin("RenderSystem_Direct3D9");
        pRoot->loadPlugin("Plugin_CgProgramManager");     
		pRoot->loadPlugin("Plugin_ParticleFX");  
    }

    void Root::LoadResource()
    {
        Ogre::ResourceGroupManager& res = Ogre::ResourceGroupManager::getSingleton();
        res.addResourceLocation(".", "FileSystem");
        res.addResourceLocation("GUIRes\\fonts", "FileSystem");
        res.addResourceLocation("models", "FileSystem");
        res.addResourceLocation("models\\cars", "FileSystem");
        res.addResourceLocation("data", "FileSystem");
        res.addResourceLocation("GUIRes", "FileSystem");
        res.addResourceLocation("material", "FileSystem");
        res.addResourceLocation("texture", "FileSystem");

        res.initialiseAllResourceGroups();
    }

    void Root::ShowState(uint8 idx)
    {
        if(pCurrentState != NULL)
            pCurrentState->Hide();

        StateMap::const_iterator itr = mStateMap.find(idx);
        if(itr != mStateMap.end())
        {
            pCurrentState = itr->second;
            pCurrentState->Restore();
        }
        else
        {
            switch(idx)
            {
                case State::STATE_MENU:
                    pCurrentState = new MenuState(); break;
                case State::STATE_PLAY:
                    pCurrentState = new PlayState(); break;
                case State::STATE_DEBUG:
                    pCurrentState = new DebugState(); break;

                default:
                    return;
            }

            pCurrentState->Enter();
            mStateMap[idx] = pCurrentState;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    bool Root::keyPressed(const OIS::KeyEvent &e)
    {
        pGUI->injectKeyPress(MyGUI::KeyCode::Enum(e.key), e.text);
        pCurrentState->keyPressed(e);

        return true;
    }

    bool Root::keyReleased(const OIS::KeyEvent &e)
    {
        pGUI->injectKeyRelease(MyGUI::KeyCode::Enum(e.key));
        pCurrentState->keyReleased(e);

        return true;
    }

    bool Root::mouseMoved(const OIS::MouseEvent &e)
    {
        pGUI->injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
        pCurrentState->mouseMove(e);

        return true;
    }

    bool Root::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        pGUI->injectMousePress(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
        pCurrentState->mousePressed(e, id);

        return true;
    }

    bool Root::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        pGUI->injectMouseRelease(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
        pCurrentState->mouseReleased(e, id);

        return true;
    }

    bool Root::povMoved(const OIS::JoyStickEvent &e, int pov)
    {
        if (pJoystick1)
            pCurrentState->povMoved(e, pov);

        return true;
    }

    bool Root::axisMoved(const OIS::JoyStickEvent &e, int axis) 
    {
        if (pJoystick1)
            pCurrentState->axisMoved(e, axis);

        return true;
    }

    bool Root::sliderMoved(const OIS::JoyStickEvent &e, int sliderID) 
    {
        if (pJoystick1)
            pCurrentState->sliderMoved(e, sliderID);

        return true;
    }

    bool Root::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {
        if (pJoystick1)
            pCurrentState->buttonPressed(e, button);

        return true;
    }

    bool Root::buttonReleased(const OIS::JoyStickEvent &e, int button)
    {
        if (pJoystick1)
            pCurrentState->buttonReleased(e, button);

        return true;
    }

    bool Root::frameStarted(const Ogre::FrameEvent &evt)
    {/*
        if(evt.timeSinceLastFrame > 0.004f)
            const_cast<Ogre::FrameEvent&>(evt).timeSinceLastFrame = 0.004f;
            */
        pKeyboard->capture();
        pMouse->capture();
        if (pJoystick1)
            pJoystick1->capture();
        if (pJoystick2)
            pJoystick2->capture();
        //pGUI->injectFrameEntered(evt.timeSinceLastFrame);

        if(pWindow->isClosed())
            return false;

        return pCurrentState->frameStarted(evt);
    }

    bool Root::frameEnded(const Ogre::FrameEvent &evt)
    {
        return pCurrentState->frameEnded(evt);
    }

    void Root::RequestShutdown()
    {
        pWindow->destroy();
    }

    void Root::LoadLayout(const std::string& _layout, bool visible)
    {
        MyGUI::LayoutManager::getInstance().loadLayout(_layout);

        MyGUI::WidgetPtr obj = pGUI->findWidget<MyGUI::Widget>(_layout);
        obj->setVisible(visible);
    }

    void Root::preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {
        pCurrentState->preRenderTargetUpdate(evt);
      //  mTahometrOverlay->show();
      //  mSpeedometrOverlay->show();
    }
    void Root::postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {
        pCurrentState->postRenderTargetUpdate(evt);
      //  mTahometrOverlay->hide();
       // mSpeedometrOverlay->hide();
    }
}