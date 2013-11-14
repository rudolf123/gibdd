#ifndef LevelLoaderH
#define LevelLoaderH

namespace Gibdd
{
    class LevelLoader
    {
        public:
            LevelLoader();
            ~LevelLoader();

            void SetSceneManager(Ogre::SceneManager* mgr);
            Level* LoadLevel(const std::string& filename);
            void LoadLevelFromdotScene(const std::string& filename);

        private:
            static uint32 iMapVersion;
    };
}

#define sLevelLoader Gibdd::Singleton<Gibdd::LevelLoader>::Instance()

#endif