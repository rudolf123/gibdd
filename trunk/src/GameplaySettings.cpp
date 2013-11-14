#include "Precompiled.h"
#include "GameplaySettings.h"

namespace Gibdd
{
    GameplaySettings::GameplaySettings()
    {
        //if (!loadSettingsFromFile("PlayersConfig.txt"))
            setDefaultSettings();
    }

    GameplaySettings::~GameplaySettings()
    {

    }

    void GameplaySettings::setDefaultSettings()
    {
        numPeds = 10;
        numDrivs = 0;
        numPlayers = 0;

        /*PlayerInfo info;

        info.sName = "Player1";
        PlayerInput in;
        InputMap mPlayerInputMap;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_UP;
        mPlayerInputMap[ACCELERATE] = in;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_DOWN;
        mPlayerInputMap[BRAKE] = in;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_LEFT;
        mPlayerInputMap[STEERLEFT] = in;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_RIGHT;
        mPlayerInputMap[STEERRIGHT] = in;
        info.mInputMap = mPlayerInputMap;

        addPlayerInfo(info.sName, info);

        /*currPlayerID = 0;

        PlayerInput in;
        InputMap mPlayerInputMap;
        in.inputType = OIS::OISJoyStick;
        in.deviceID = 0;
        in.compType = OIS::OIS_Axis;
        in.val = 1;
        mPlayerInputMap[ACCELERATE] = in;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_DOWN;
        mPlayerInputMap[BRAKE] = in;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_LEFT;
        mPlayerInputMap[STEERLEFT] = in;
        in.inputType = OIS::OISKeyboard;
        in.key = OIS::KC_RIGHT;
        mPlayerInputMap[STEERRIGHT] = in;
        mPlayers[currPlayerID].sName = "Player1";
        mPlayers[currPlayerID].mInputMap = mPlayerInputMap;*/
    }

    void GameplaySettings::changePlayerInputSettings(std::string name, GameplayActions action, OIS::Type inputDevice, int deviceID,OIS::KeyCode key, OIS::ComponentType compType, int val)
    {
        //++currPlayerID;
       /* PlayerInput in;
        InputMap mPlayerInputMap;
        in.inputType = inputDevice;
        if (inputDevice == OIS::OISKeyboard)
        {
            mPlayerInputMap = mPlayers[name].mInputMap;
            in.key = key;
            mPlayerInputMap[action] = in;
            mPlayers[name].mInputMap = mPlayerInputMap;
        }
        if (inputDevice == OIS::OISJoyStick)
        {
            mPlayerInputMap = mPlayers[name].mInputMap;
            in.deviceID = deviceID;
            in.compType = compType;
            in.val = val;
            mPlayerInputMap[action] = in;
            mPlayers[name].mInputMap = mPlayerInputMap;
        }   */
    }

    void GameplaySettings::addPlayerInfo(std::string name, PlayerInfo info)
    {
        ++numPlayers;
       // typedef std::pair <std::string, PlayerInfo> Pair;
        
        //mPlayers.insert(Pair(name, info));
        mPlayers[name] = info;
        saveSettingsToFile("gameplaySettings.txt");
    }
    void GameplaySettings::changePlayerInfo(std::string name, PlayerInfo info)
    {
        mPlayers[name] = info;
    }


    bool GameplaySettings::loadSettingsFromFile(std::string filename)
    {
   /*     std::string line;

        std::ifstream ifs(filename.c_str());
        if(!ifs.is_open())
            return false;

        while(!ifs.eof())
        {
            std::getline(ifs, line);
            if(line.empty())
                continue;

            size_t found = line.find("numPeds");
            if (found != std::string::npos)
            {
                found = line.find("=");
                numPeds = atoi(line.substr(found + 1).c_str());
            }
            found = line.find("numDrivs");
            if (found != std::string::npos)
            {
                found = line.find("=");
                numDrivs = atoi(line.substr(found + 1).c_str());
            }
            found = line.find("numPlayers");
            if (found != std::string::npos)
            {
                found = line.find("=");
                numPlayers = atoi(line.substr(found + 1).c_str());
            }
            found = line.find("#Player1Input");
            if (found != std::string::npos)
            {
                found = line.find("=");
                std::string InputType = line.substr(found + 1).c_str();
                if (InputType == "joystick")
                    Player1.inputType = JOYPAD;
                else
                    Player1.inputType = KEYBOARD_MOUSE;
            }
            found = line.find("#Player2Input");
            if (found != std::string::npos)
            {
                found = line.find("=");
                std::string InputType = line.substr(found + 1).c_str();
                if (InputType == "joystick")
                    Player2.inputType = JOYPAD;
                else
                    Player2.inputType = KEYBOARD_MOUSE;
            }
            found = line.find("#Player1.accelerate");
            if (found != std::string::npos)
            {
                found = line.find("=");
                std::string InputType = line.substr(found + 1).c_str();
                Player1.accelerate = (OIS::KeyCode)atoi(line.substr(found + 1).c_str());
            }
            found = line.find("#Player1.brake");
            if (found != std::string::npos)
            {
                found = line.find("=");
                std::string InputType = line.substr(found + 1).c_str();
                Player1.brake = (OIS::KeyCode)atoi(line.substr(found + 1).c_str());
            }
            found = line.find("#Player1.steerleft");
            if (found != std::string::npos)
            {
                found = line.find("=");
                std::string InputType = line.substr(found + 1).c_str();
                Player1.steerLeft = (OIS::KeyCode)atoi(line.substr(found + 1).c_str());
            }
            found = line.find("#Player1.steerright");
            if (found != std::string::npos)
            {
                found = line.find("=");
                std::string InputType = line.substr(found + 1).c_str();
                Player1.steerRight = (OIS::KeyCode)atoi(line.substr(found + 1).c_str());
            }
        }

        ifs.close();*/

        return true;
    }

    bool GameplaySettings::saveSettingsToFile(std::string filename)
    {
        std::ofstream outs(filename.c_str());
        if(!outs.is_open())
            return false;

        outs << "numPeds=" << numPeds << std::flush;
        outs << "\n" << std::flush;
        outs << "numDrivs=" << numDrivs << std::flush;
        outs << "\n" << std::flush;
        outs << "numPlayers=" << numPlayers << std::flush;
        outs << "\n" << std::flush;
        PlayersList::iterator itr;
        itr = mPlayers.begin();

        outs << "<Players>"<< "\n" << std::flush;
        std::string s;
        while (itr != mPlayers.end())
        {
            outs << "<Player name=" << itr->second.sName << ">"<< "\n" << std::flush;
            for (uint16 i = 0; i < 8; ++i)
            {
                if (i==0) 
                    s = "accelerate";
                if (i==1) 
                    s = "brake";
                if (i==2) 
                    s = "steerleft";
                if (i==3) 
                    s = "steerright";
                if (i==4) 
                    s = "gearup";
                if (i==5) 
                    s = "geardown";
                if (i==6) 
                    s = "leftlight";
                if (i==7) 
                    s = "rightlight";

                outs << "<"<< s << ">" << "\n"<< std::flush;
                outs << "<inputType=" << itr->second.mInputMap[(GameplayActions)i].inputType << ">" << "\n" << std::flush;
                outs << "<deviceID=" << itr->second.mInputMap[(GameplayActions)i].deviceID << ">"<< "\n" << std::flush;
                outs << "<compType=" << itr->second.mInputMap[(GameplayActions)i].compType << ">"<< "\n" << std::flush;
                outs << "<key=" << itr->second.mInputMap[(GameplayActions)i].key << ">"<< "\n" << std::flush;
                outs << "<val=" << itr->second.mInputMap[(GameplayActions)i].val << ">"<< "\n" << std::flush;
            }

            outs << "</Player>" << "\n" << std::flush;
            ++itr;
        }
        outs << "</Players>" << std::flush;

        /*if (Player1.inputType == JOYPAD) 
            outs << "#Player1Input=" << "joystick" << std::flush;
        else
            outs << "#Player1Input=" << "keyboard" << std::flush;
        outs << "\n" << std::flush;
        outs << "#Player1.accelerate=" << Player1.accelerate << std::flush;
        outs << "\n" << std::flush;
        outs << "#Player1.brake=" << Player1.brake << std::flush;
        outs << "\n" << std::flush;
        outs << "#Player1.steerleft=" << Player1.steerLeft << std::flush;
        outs << "\n" << std::flush;
        outs << "#Player1.steerright=" << Player1.steerRight << std::flush;
        outs << "\n" << std::flush;
*/

        outs.close();

        return true;

    }
}