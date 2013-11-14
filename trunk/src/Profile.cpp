#include "Precompiled.h"
#include "Profile.h"

namespace Gibdd
{
    Profile::Profile()
    {

    }
    Profile::~Profile()
    {

    }

    void Profile::bindAction(GameplayActions action, OIS::KeyCode key)
    {
        ProfileInput in;
        in.inputType = OIS::OISKeyboard;
        in.key = key;
        mInputMap[action] = in;
    }

    void Profile::bindAction(GameplayActions action, int joyID, OIS::ComponentType joyCompType, int val, int direction)
    {
        ProfileInput in;
        in.inputType = OIS::OISJoyStick;
        in.deviceID = joyID;
        in.compType = joyCompType;
        in.val = val;
        in.direction = direction;
        mInputMap[action] = in;
    }

}