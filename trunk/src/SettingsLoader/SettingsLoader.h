#ifndef SettingLoaderH
#define SettingLoaderH

#include "Settings.h"

namespace Gibdd
{
    class SettingsLoader
    {
        public:
            SettingsLoader();
            ~SettingsLoader();

            void Initialize(Ogre::Root* root);

            void SaveGameSettings();
            void LoadGameSettings();

        private:
            HRESULT LoadSettings(const PCHAR pOutBuffer, const DWORD& dwDataSize);
            HRESULT SaveSettings(const PCHAR pInBuffer, const DWORD& dwDataSize);

        private:
            std::string pSettingsFileName;

            std::ifstream m_ifs;
            std::ofstream m_ofs;

            std::vector<Settings*> m_SettingHolder;

            VideoSettings* pVideo;
    };
}

#define sSettingsLoader Gibdd::Singleton<Gibdd::SettingsLoader>::Instance()

#endif