#ifndef SimaforeSystemH
#define SimaforeSystemH

namespace Gibdd
{

    using namespace Ogre;

    enum SimaforeColors
    {
        RED,
        YELLOW,
        GREEN,
        BLINK,
        DEFAULT
    };

    class GlowMaterialListener : public Ogre::MaterialManager::Listener
    {
    protected:
        Ogre::MaterialPtr mBlackMat;
    public:
        GlowMaterialListener()
        {
            mBlackMat = Ogre::MaterialManager::getSingleton().create("mGlowBlack", "Internal");
            mBlackMat->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,0);
            mBlackMat->getTechnique(0)->getPass(0)->setSpecular(0,0,0,0);
            mBlackMat->getTechnique(0)->getPass(0)->setAmbient(0,0,0);
            mBlackMat->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,0);
        }

        Ogre::Technique *handleSchemeNotFound(unsigned short, const Ogre::String& schemeName, Ogre::Material*mat, unsigned short, const Ogre::Renderable*)
        {
            if (schemeName == "glow")
            {
                //LogManager::getSingleton().logMessage(">> adding glow to material: "+mat->getName());
                return mBlackMat->getTechnique(0);
            }
            return NULL;
        }
    };

    class SimaforeSystem
    {
    public:
        SimaforeSystem();
        ~SimaforeSystem();

        void simulate(float delta);

        void loadAllSemaforesInScene(SceneManager *pSceneMgr);

        void addAutoSemafore(Entity* ent);
        void addPedestrianSemafore(Entity* ent);

        float GetSimaforeTime() {return mTimer->getMilliseconds()/1000; };
        void setTimeRangeForColor(float time, SimaforeColors COLOR);

        SimaforeColors GetCurrPedColor() { return curColorPedestrian; };


    private:

        void removeSemafore();
        void setSimaforeAutoColor(SimaforeColors color);
        void setSimaforePedestrianColor(SimaforeColors color);

    private:

        float fTimeToRed;
        float fTimeToYellow;
        float fTimeToGreen;
        float fTimeToBlink;
        float fBlinkRange;

        bool isBlinking;

        SimaforeColors curColorAuto;
        SimaforeColors curColorPedestrian;

        typedef std::vector<Entity*> AutoSimaforeVector;
        AutoSimaforeVector::iterator SimaforesAutoIter;
        AutoSimaforeVector vSimaforesAuto;

        typedef std::vector<Entity*> PedestrianSimaforeVector;
        PedestrianSimaforeVector::iterator SimaforesPedIter;
        PedestrianSimaforeVector vSimaforesPedestrian;

        Ogre::Timer *mTimer;
        Ogre::Timer *mBlinkingTimer;
        GlowMaterialListener *gml;

    };

}

#endif