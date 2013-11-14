#ifndef dotSceneParserH
#define dotSceneParserH

namespace Gibdd
{
    #define BAD_WORD_1 "VRay"
    #define BAD_WORD_2 "Camera"
    #define BAD_WORD_3 "Scene Root"
    #define BAD_WORD_4 "OgreMax"
    #define BAD_WORD_5 "Mig21"

    #define FIND_FIRST "node name"

    struct ObjectDescription
    {
        int mesh_size;
        std::string mesh_name;

        int entry_size;
        std::string entry_name;

        float pos_x;
        float pos_y;
        float pos_z;

        float scale_x;
        float scale_y;
        float scale_z;

        float rot_x;
        float rot_y;
        float rot_z;
        float rot_w;

        int shadows;
    };

    class dotSceneParser
    {
        public:
            dotSceneParser();
            ~dotSceneParser();

            bool ParseFile(const std::string& file);
            void LoadScene();

        private:
            bool ExtractPosition(std::string str, ObjectDescription& obj);
            bool ExtractScale(std::string str, ObjectDescription& obj);
            bool ExtractRotation(std::string str, ObjectDescription& obj);
            bool ExtractMaterial(std::string str, ObjectDescription& obj);
            bool CheckInputLine(const std::string& str);
            void CreateObject(const std::string& name, const std::string& mesh, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& rotation, bool shadows);


            std::vector<ObjectDescription> ObjectVector;

            Ogre::SceneManager *pSceneMgr;
    };
}
#endif