#include "Precompiled.h"
#include "PlayState.h"
#include "Root.h"
#include "LevelLoader.h"
#include "PhysicManager.h"
#include "SimaforeSystem.h"
#include "AIManager.h"
#include "PlayerManager.h"
#include "GameplaySettings.h"

namespace Gibdd
{
    using namespace Ogre;
    PlayState::PlayState():
    pSceneMgr(0),
    pGUI(0),
    pCamera(0),
    pPhysicMgr(0),
    pAIManager(0),
    pPlayersMgr(0),
    gpSettings(0)
    {
    }

    PlayState::~PlayState()
    {
        if (pPlayersMgr)
            delete pPlayersMgr;
        if (pAIManager)
            delete pAIManager;
        if (pPhysicMgr)
            delete pPhysicMgr;
    }

    void PlayState::mouseMove(const OIS::MouseEvent &e)
    {
        pPlayersMgr->mouseMove(e);
    }

    void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {

    }

    void PlayState::keyPressed(const OIS::KeyEvent &e)
    {
        pPlayersMgr->keyPressed(e);
        if (e.key == OIS::KC_ESCAPE)
            Root::GetInstance().RequestShutdown();
    }

    void PlayState::keyReleased(const OIS::KeyEvent &e)
    {

    }

    void PlayState::povMoved(const OIS::JoyStickEvent &e, int pov)
    {

    }
    void PlayState::axisMoved(const OIS::JoyStickEvent &e, int axis)
    {

    }
    void PlayState::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
    {

    }
    void PlayState::buttonPressed(const OIS::JoyStickEvent &e, int button)
    {
        pPlayersMgr->buttonPressed(e, button);
    }
    void PlayState::buttonReleased(const OIS::JoyStickEvent &e, int button)
    {

    }

    bool PlayState::frameStarted(const Ogre::FrameEvent &evt)
    {
        pPlayersMgr->Update(evt.timeSinceLastFrame);
        
        pAIManager->simulate(evt.timeSinceLastFrame);
        pPhysicMgr->simulate(evt.timeSinceLastFrame);

        return true;
    }

    bool PlayState::frameEnded(const Ogre::FrameEvent &evt)
    {

        return true;
    }

    void PlayState::Hide()
    {
        pGUI->setVisiblePointer(true);
        pWidget->setVisible(false);
    }

    void PlayState::Enter()
    {
        pCamera = Root::GetInstance().GetCamera();
        pSceneMgr = Root::GetInstance().GetSceneManager();
        pGUI = Root::GetInstance().GetGui();
        gpSettings = Root::GetInstance().GetGameplaySettings();
        loadGui();
        loadLevel();
        loadPhysic();
        loadAI();
        loadPlayers();

        //pSceneMgr->getEntity("static10_house02")->setMaterialName("RearViewMaterial");
        
    }

    void PlayState::Restore()
    {

    }

    void PlayState::loadLevel()
    {
        sLevelLoader.LoadLevelFromdotScene("levels/level_0.scene");
        pSceneMgr->setSkyBox(true, "Sky/EveningSky", 500);
        pSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
        pSceneMgr->setShadowFarDistance(1000);

        Ogre::Light* pointLight = pSceneMgr->createLight("pointLight");
        pointLight->setCastShadows(true);
        pointLight->setType(Ogre::Light::LT_POINT);
        pointLight->setPosition(227, 1100, 1100);
        pointLight->setDiffuseColour(1, 0.63, 0.41);
        pointLight->setSpecularColour(1.0, 1.0, 1.0);
    }

    void PlayState::loadGui()
    {
        pWidget = pGUI->findWidget<MyGUI::Widget>("play.layout");

        pGearText = pGUI->findWidget<MyGUI::StaticText>("Gear");
        pWidget->setVisible(false);
        pGUI->setVisiblePointer(false);
    }

    void PlayState::loadPlayers()
    {
        Root::GetInstance().GetRenderWindow()->removeViewport(0);
        if (!pPlayersMgr)
            pPlayersMgr = new PlayerManager(pSceneMgr, pPhysicMgr);

        ProfileManager *pProfMgr = Root::GetInstance().GetProfileManager();
        Profile *pProfile = 0;
        pProfile = pProfMgr->GetFirstProfile();
        if (!pProfile)
            return;

        while (pProfile)
        {
            pPlayersMgr->addPlayer(pProfile);
            pProfile = pProfMgr->GetNextProfile();
        }
    }

    void PlayState::loadPhysic()
    {
        if (!pPhysicMgr)
            pPhysicMgr = new PhysicManager(pSceneMgr);

        if (pSceneMgr->hasEntity("terrain"))
            pPhysicMgr->createTriangleMesh(pSceneMgr->getEntity("terrain"));
    }

    void PlayState::loadAI()
    {
        if (!pAIManager)
            pAIManager = new AIManager(pSceneMgr, pPhysicMgr);

        for (uint8 i = 0; i < gpSettings->getNumPeds(); ++i)
            pAIManager->add(BOT_PEDESTRIAN);

        for (uint8 i = 0; i < gpSettings->getNumDrivs(); ++i)
            pAIManager->add(BOT_DRIVER);
    }

    void PlayState::preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {

    }
    void PlayState::postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
    {

    }
}