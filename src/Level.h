#ifndef LevelH
#define LevelH

namespace Gibdd
{
    class Level
    {
        public:
            Level(const std::string& level_name);
            ~Level();

            void DestroyLevel();

            void BuildLevel();
            void CreateObject(const std::string& name, const std::string& mesh, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& rotation, bool shadows);

        private:
            std::string pName;
            Ogre::SceneManager* pMgr;

            Ogre::StaticGeometry* pStaticGeom;
    };
}

#endif