#ifndef DebugStateH
#define DebugStateH

#include "State.h"
#include "EngineSolver.h"

namespace Gibdd
{
    class CameraEx;
    class PhysicManager;
    class Vehicle;
    class SimaforeSystem;
    class AIManager;
    class WPManager;
    class Graph;
    class Ragdoll;
    class Character;

    class DebugState: public State
    {
        enum EditModes
        {
            EDIT_ADD_WAYPOINT,
            EDIT_LINK_WAYPOINT,
            EDIT_ENTITY,

            EDIT_NONE
        };

    public:
        DebugState();
        ~DebugState();

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
        Vehicle* createCar(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive);
        void setEditMode(EditModes mode);
        uint8 getEditMode(){ return mEditMode; };

        bool RaycastToPolygon(Ogre::Ray &ray, Ogre::Vector3 &result);
        void GetMeshInformation(const Ogre::MeshPtr mesh,
            size_t &vertex_count,
            Ogre::Vector3* &vertices,
            size_t &index_count,
            unsigned long* &indices,
            const Ogre::Vector3 &position,
            const Ogre::Quaternion &orient,
            const Ogre::Vector3 &scale);

    private:
        Ogre::SceneManager *pSceneMgr;

        MyGUI::Gui *pGUI;
        MyGUI::WidgetPtr pWidget;
        MyGUI::StaticTextPtr pFpsText;
        MyGUI::StaticTextPtr pEntityText;
        MyGUI::StaticTextPtr pPointPosText;


        Ogre::Camera *pCamera;
        Ogre::Camera *pCamera1;
        CameraEx *pCameraEx;
        CameraEx *pCameraEx1;
        PhysicManager *pPhysicMgr;
        Vehicle *car; 

        AIManager *pAIManager;

        SimaforeSystem *simafore;

        WPManager *pWaypointMgr;
        Graph *WPGraph;

        Ogre::Entity *selectedEntity;
        Ogre::MaterialPtr selectedEntityMaterial; 

        bool bHideAllModels;

        uint8 mEditMode;

        Ogre::SceneNode *currWaypointNode;

        EngineSolver *pEngineSolver;

        //shit
        uint8 graphNum;
        Ogre::Entity *ragdoll;
        Ogre::SceneNode *ragdollNode;
        Ragdoll *testRagdoll;
        Ogre::Entity *pAngleTestEnt;
        Ogre::SceneNode *pAngleTestNode;

        NxActor* actor1;
        NxActor* actor2;

        Character *chara;
    };
}

#endif