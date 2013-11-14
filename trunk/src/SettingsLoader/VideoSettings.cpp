#include "Precompiled.h"
#include "Settings.h"
#include "VideoSettings.h"
#include "../Root.h"
#include <fstream>

namespace Gibdd
{
    const char VideoSettings::AllowPerfHud[]           = "Allow NVPerfHUD";
    const char VideoSettings::FloatingPoint[]          = "Floating-point mode";
    const char VideoSettings::FSAA[]                   = "FSAA";
    const char VideoSettings::FullScreen[]             = "Full Screen";
    const char VideoSettings::RenderingDevice[]        = "Rendering Device";
    const char VideoSettings::ResourceCreationPolicy[] = "Resource Creation Policy";
    const char VideoSettings::VSync[]                  = "VSync";
    const char VideoSettings::VSyncInterval[]          = "VSync Interval";
    const char VideoSettings::VideoMode[]              = "Video Mode";
    const char VideoSettings::GammaConversion[]        = "sRGB Gamma Conversion";

    VideoSettings::VideoSettings()
    {
    }

    VideoSettings::~VideoSettings()
    {
    }

    const PVOID VideoSettings::GetSettings()
    {
	    return (&m_VideoSettings);
    }

    DWORD VideoSettings::GetSettingsSize()
    {
	    return sizeof(m_VideoSettings);
    }

    void VideoSettings::SetSettings(PVOID pNewSettings)
    {
	    memcpy(&m_VideoSettings, pNewSettings, sizeof(m_VideoSettings));

        ApplySettings();
    }

    void VideoSettings::ResetToDefault()
    {
        //добавить поиск второго графического адаптера
        Ogre::RenderSystemList::const_iterator render = pRoot->getAvailableRenderers().begin();
        Ogre::RenderSystem* rs = *render;

        const Ogre::ConfigOptionMap& opts = rs->getConfigOptions();
        Ogre::ConfigOptionMap::const_iterator option = opts.find(RenderingDevice);
        std::string name = option->second.currentValue;

        SetRenderingDevice(name);
        LoadSettingsFromFile("VideoSettings.cfg");
    }

    bool VideoSettings::LoadSettingsFromFile(const std::string& filename)
    {
        std::string line;

        std::ifstream ifs(filename);
        if(!ifs.is_open())
            return false;

        while(!ifs.eof())
        {
            std::getline(ifs, line);
            if(line.empty())
                continue;

            size_t found = line.find("AllowNVPerfHUD");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetAllowNVPerfHUD(line.substr(found + 1));
                continue;
            }
            found = line.find("FloatingPointMode");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetFloatingPointMode(line.substr(found + 1));
                continue;
            }
            found = line.find("FSAA");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetFSAA(line.substr(found + 1));
                continue;
            }
            found = line.find("FullScreen");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetFullScreen(line.substr(found + 1));
                continue;
            }
            found = line.find("ResourceCreationPolicy");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetResourceCreationPolicy(line.substr(found + 1));
                continue;
            }
            found = line.find("VSync");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetVSync(line.substr(found + 1));
                continue;
            }
            found = line.find("VSyncInterval");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetVSyncInterval(line.substr(found + 1));
                continue;
            }
            found = line.find("VideoMode");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetVideoMode(line.substr(found + 1));
                continue;
            }
            found = line.find("GammaConversion");
            if (found != std::string::npos)
            {
                found = line.find("=");
                SetGammaConversion(line.substr(found + 1));
                continue;
            }
        }
        ifs.close();

        return true;
    }

    void VideoSettings::ApplySettings()
    {
        //добавить поиск второго графического адаптера
        Ogre::RenderSystem* rs = *pRoot->getAvailableRenderers().begin();

        rs->setConfigOption(AllowPerfHud, m_VideoSettings.m_AllowNVPerfHUD);
        rs->setConfigOption(FSAA, m_VideoSettings.m_FSAA);
        rs->setConfigOption(FloatingPoint, m_VideoSettings.m_FloatingPointMode);
        rs->setConfigOption(FullScreen, m_VideoSettings.m_FullScreen);
        rs->setConfigOption(RenderingDevice, m_VideoSettings.m_RenderingDevice);
        rs->setConfigOption(ResourceCreationPolicy, m_VideoSettings.m_ResourceCreationPolicy);
        rs->setConfigOption(VSync, m_VideoSettings.m_VSync);
        rs->setConfigOption(VSyncInterval, m_VideoSettings.m_VSyncInterval);
        rs->setConfigOption(VideoMode, m_VideoSettings.m_VideoMode);
        rs->setConfigOption(GammaConversion, m_VideoSettings.m_sRGBGammaConversion);

        pRoot->setRenderSystem(rs);
    }

    const Ogre::StringVector& VideoSettings::getPossibleSettings(const std::string& name)
    {
        //добавить поиск второго графического адаптера
        Ogre::RenderSystemList::const_iterator render = pRoot->getAvailableRenderers().begin();
        Ogre::RenderSystem* rs = *render;

        const Ogre::ConfigOptionMap& opts = rs->getConfigOptions();
        Ogre::ConfigOptionMap::const_iterator option = opts.find(name);

        return option->second.possibleValues;
    }
}