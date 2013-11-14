#ifndef PrecompiledH
#define PrecompiledH

#include <Windows.h>
#include <hash_map>

#include "Defines.h"
#include "Classes.h"

#include "Policies/SingletonImp.h"

#include <OIS/OIS.h>

#include <Ogre/Ogre.h>
#include <Ogre/OgreVector3.h>
#include <Ogre/OgreVector4.h>

#include <mygui/MyGUI.h>
#include <mygui/MyGUI_OgrePlatform.h>

#include <PhysX/NxPhysics.h>
#include <PhysX/NxCooking.h>

#include "Util.h"

#define TO_STRING(str) Ogre::StringConverter::toString(str)
#define LOG(str, val) Ogre::LogManager::getSingleton().logMessage(str + TO_STRING(val));
#define LOGS(str) Ogre::LogManager::getSingleton().logMessage(str);

enum LightMasks
{
    LIGHT_MASK_1,
    LIGHT_MASK_2,
    LIGHT_MASK_3,

    LIGHT_MASK_0
};

#endif