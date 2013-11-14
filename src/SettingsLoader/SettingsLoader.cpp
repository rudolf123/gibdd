#include "Precompiled.h"
#include "Settings.h"
#include "VideoSettings.h"
#include "SettingsLoader.h"

INSTANTIATE_SINGLETON_1(Gibdd::SettingsLoader);

namespace Gibdd
{
    SettingsLoader::SettingsLoader()
    {
    }

    SettingsLoader::~SettingsLoader()
    {
        if( m_ifs.is_open() )
            m_ifs.close();

        if( m_ofs.is_open() )
            m_ofs.close();

        std::vector<Settings*>::iterator itr = m_SettingHolder.begin();
        for(; itr != m_SettingHolder.end(); ++itr)
            delete *itr;

        m_SettingHolder.clear();
        pVideo = 0;
    }

    void SettingsLoader::Initialize(Ogre::Root* root)
    {
        if(!m_SettingHolder.empty())
            return;

        pSettingsFileName = "Settings.dat";

        pVideo = new VideoSettings();
        pVideo->SetRoot(root);
        pVideo->ResetToDefault();

        m_SettingHolder.push_back(pVideo);
    }

    void SettingsLoader::SaveGameSettings()
    {
        m_ofs.open(pSettingsFileName.c_str());

        PVOID pSomeSettings = NULL;
        DWORD dwSettingsSize = 0;
        HRESULT hResult = S_OK;

        std::vector<Settings*>::const_iterator itr = m_SettingHolder.begin();
        for (; itr != m_SettingHolder.end(); ++itr)
        {
            Settings* setting = *itr;

            pSomeSettings = setting->GetSettings();
            dwSettingsSize = setting->GetSettingsSize();

            hResult = SaveSettings((PCHAR) pSomeSettings, dwSettingsSize );
            if(hResult != S_OK)
            {
                // ОШИБКА НА!
                return;
            }
        }
    }

    void SettingsLoader::LoadGameSettings()
    {
        m_ifs.open(pSettingsFileName.c_str());
        if( !m_ifs.is_open() )
        {
            pVideo->ApplySettings();
            return;
        }

        DWORD dwSettingsSize = 0;
        PVOID pInSettings = NULL;
        HRESULT hResult = S_OK;

        std::vector<Settings*>::const_iterator itr = m_SettingHolder.begin();
        for (; itr != m_SettingHolder.end(); ++itr)
        {
            Settings* setting = *itr;
            dwSettingsSize = setting->GetSettingsSize();

            pInSettings = new UCHAR[dwSettingsSize];
            if ( !pInSettings )
            {
                // Память не выделилась
                return;
            }

            hResult = LoadSettings( (PCHAR) pInSettings, dwSettingsSize );
            if ( hResult != S_OK )
            {
                // Ошибка при чтении...
            }
            else
            {
                // Устанавливаем новые параметры
                setting->SetSettings( pInSettings );
            }

            delete[] pInSettings;
        }
    }

    HRESULT SettingsLoader::LoadSettings(const PCHAR pOutBuffer, const DWORD& dwDataSize)
    {
        DWORD dwBytesReaded = 0;

        m_ifs.read(pOutBuffer, dwDataSize);
    //     // Проверяем количество считанных байт
    //     if ( dwDataSize != dwBytesReaded )
    //     {
    //         return S_FALSE;
    //     }

        return S_OK;
    }

    HRESULT SettingsLoader::SaveSettings(const PCHAR pInBuffer, const DWORD& dwDataSize)
    {
        m_ofs.write(pInBuffer, dwDataSize);
        m_ofs.flush();

        return S_OK;
    }
}