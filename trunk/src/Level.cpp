#include "Precompiled.h"
#include "Level.h"
#include "Root.h"

namespace Gibdd
{
    Level::Level(const std::string& level_name)
    {
        pMgr = Root::GetInstance().GetSceneManager();

        pStaticGeom = pMgr->createStaticGeometry(level_name);
    }

    Level::~Level()
    {
        pMgr = 0;

        DestroyLevel();
    }

    void Level::DestroyLevel()
    {
        pMgr->destroyStaticGeometry(pName);
    }

    void Level::BuildLevel()
    {
        pStaticGeom->build();
    }

    void Level::CreateObject(const std::string& name, const std::string& mesh, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& rotation, bool shadows)
    {
        Ogre::Entity* entity = pMgr->createEntity(name, mesh);
        entity->setCastShadows(shadows);

        Ogre::SceneNode* node;

        node = pMgr->getRootSceneNode()->createChildSceneNode(name);
        node->setScale(scale);
        node->setOrientation(rotation);
        node->setPosition(position);
        node->attachObject(entity);

       // pStaticGeom->addSceneNode(node);
    }
}