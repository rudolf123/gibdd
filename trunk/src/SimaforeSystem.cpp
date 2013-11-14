#include "Precompiled.h"
#include "SimaforeSystem.h"

namespace Gibdd
{
    using namespace Ogre;

    SimaforeSystem::SimaforeSystem()
    {
        fTimeToRed = 20000;
        fTimeToGreen = 20000;
        fTimeToYellow = 4000;
        fTimeToBlink = 5000;
        fBlinkRange = 500;
        isBlinking = false;
        mTimer = new Ogre::Timer;
        mBlinkingTimer = new Ogre::Timer;
        curColorAuto = RED;
        curColorPedestrian = RED;
        gml = new GlowMaterialListener();
        Ogre::MaterialManager::getSingleton().addListener(gml);
    }

    SimaforeSystem::~SimaforeSystem()
    {
        if (gml)
            delete gml;
    }

    void SimaforeSystem::simulate(float delta)
    {
        if (isBlinking)
        {
            if ((mBlinkingTimer->getMilliseconds()) >= (fTimeToBlink - 4*fBlinkRange))
                setSimaforeAutoColor(DEFAULT);
            if ((mBlinkingTimer->getMilliseconds()) >= (fTimeToBlink - 3*fBlinkRange))
                setSimaforeAutoColor(GREEN);
            if ((mBlinkingTimer->getMilliseconds()) >= (fTimeToBlink - 2*fBlinkRange))
                setSimaforeAutoColor(DEFAULT);
            if ((mBlinkingTimer->getMilliseconds()) >= (fTimeToBlink - fBlinkRange))
                setSimaforeAutoColor(GREEN);
            if ((mBlinkingTimer->getMilliseconds()) >= (fTimeToBlink))
                isBlinking = false;

            return;
        }

        if ((mTimer->getMilliseconds()) >= fTimeToRed)
        {
            if (curColorAuto != YELLOW)
            {
                setSimaforeAutoColor(YELLOW);
                setSimaforePedestrianColor(RED);
            }
        }
        if ((mTimer->getMilliseconds()) >= (fTimeToRed + fTimeToYellow))
        {
            if (curColorAuto != GREEN)
            {
                setSimaforeAutoColor(GREEN);
            }
        }

        if ((mTimer->getMilliseconds()) >= (fTimeToRed + fTimeToYellow + fTimeToGreen))
        {
            isBlinking = true;
            mBlinkingTimer->reset();
        }

        if ((mTimer->getMilliseconds()) >= (fTimeToRed + fTimeToYellow + fTimeToGreen + fTimeToBlink))
        {
            isBlinking = false;
            if (curColorAuto != YELLOW)
                setSimaforeAutoColor(YELLOW);
        }
        if ((mTimer->getMilliseconds()) >= (fTimeToRed + 2*fTimeToYellow + fTimeToGreen + fTimeToBlink))
        {
            if (curColorAuto != RED)           
            {
                setSimaforeAutoColor(RED);
                setSimaforePedestrianColor(GREEN);
                mTimer->reset();
            }
        }
    }

    void SimaforeSystem::addAutoSemafore(Entity* ent)
    {
        vSimaforesAuto.push_back(ent);
        setSimaforeAutoColor(RED);
    }

    void SimaforeSystem::addPedestrianSemafore(Entity* ent)
    {
        vSimaforesPedestrian.push_back(ent);
        setSimaforePedestrianColor(GREEN);
    }

    void SimaforeSystem::setSimaforeAutoColor(SimaforeColors color)
    {
        SimaforesAutoIter = vSimaforesAuto.begin();
        for(; SimaforesAutoIter != vSimaforesAuto.end(); ++SimaforesAutoIter)
        {
            switch (color)
            {
            case RED: 
                {
                    curColorAuto = RED;
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(1)->setMaterialName("simafore/red");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(2)->setMaterialName("simafore/default");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(3)->setMaterialName("simafore/default");
                } break;  
            case YELLOW: 
                {
                    curColorAuto = YELLOW;
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(2)->setMaterialName("simafore/yellow");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(1)->setMaterialName("simafore/default");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(3)->setMaterialName("simafore/default");
                } break;  
            case GREEN:
                {
                    curColorAuto = GREEN;
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(3)->setMaterialName("simafore/green");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(1)->setMaterialName("simafore/default");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(2)->setMaterialName("simafore/default");
                } break;
            case DEFAULT:
                {
                    curColorAuto = DEFAULT;
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(3)->setMaterialName("simafore/default");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(1)->setMaterialName("simafore/default");
                    ((Entity*)*SimaforesAutoIter)->getSubEntity(2)->setMaterialName("simafore/default");
                } break;
            }
        }
    }

    void SimaforeSystem::setSimaforePedestrianColor(SimaforeColors color)
    {
        SimaforesPedIter = vSimaforesPedestrian.begin();
        for(; SimaforesPedIter != vSimaforesPedestrian.end(); ++SimaforesPedIter)
        {
            switch (color)
            {
            case RED: 
                {
                    curColorPedestrian = RED;
                    ((Entity*)*SimaforesPedIter)->getSubEntity(1)->setMaterialName("simafore_ped/red");
                    ((Entity*)*SimaforesPedIter)->getSubEntity(2)->setMaterialName("simafore_ped/default");
                } break;  
            case GREEN:
                {
                    curColorPedestrian = GREEN;
                    ((Entity*)*SimaforesPedIter)->getSubEntity(2)->setMaterialName("simafore_ped/green");
                    ((Entity*)*SimaforesPedIter)->getSubEntity(1)->setMaterialName("simafore_ped/default");
                } break;
            case DEFAULT:
                {
                    curColorPedestrian = DEFAULT;
                    ((Entity*)*SimaforesPedIter)->getSubEntity(1)->setMaterialName("simafore_ped/default");
                    ((Entity*)*SimaforesPedIter)->getSubEntity(2)->setMaterialName("simafore_ped/default");
                } break;
            }
        }
    }

    void SimaforeSystem::setTimeRangeForColor(float time, SimaforeColors COLOR)
    {
        switch (COLOR)
        {
        case RED: 
            {
                fTimeToRed = time;
            } break;  
        case YELLOW: 
            {
                fTimeToYellow = time;
            } break;  
        case GREEN:
            {
                fTimeToGreen = time;
            } break;
        case BLINK:
            {
                fTimeToBlink = time;
                fBlinkRange = fTimeToBlink/5;
            } break;
        }
    }

    void SimaforeSystem::loadAllSemaforesInScene(SceneManager *pSceneMgr)
    {
        Ogre::SceneNode::ChildNodeIterator it = pSceneMgr->getRootSceneNode()->getChildIterator();
        SceneNode *n;
        while (it.hasMoreElements())
        {
            n = (SceneNode *)it.getNext();
            if (n->getName().substr(0,8)=="simafore" && n->getName().substr(0,12)!="simaforezone")
                if (n->getName().substr(9,3)=="ped")
                    this->addPedestrianSemafore(static_cast<Entity*>(n->getAttachedObject(0)));
                else
                    this->addAutoSemafore(static_cast<Entity*>(n->getAttachedObject(0)));
        }

    }
}