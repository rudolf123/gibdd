#ifndef VideoSettingsH
#define VideoSettingsH

namespace Gibdd
{
    class VideoSettings: public Settings
    {
        public:
	        const PVOID GetSettings();

	        DWORD GetSettingsSize();

	        void SetSettings(PVOID pNewSettings);

	        void ResetToDefault();

            void ApplySettings();

            void SetRoot(Ogre::Root* root) { pRoot = root; }

            const Ogre::StringVector& getPossibleSettings(const std::string& name);

            std::string GetAllowNVPerfHUD() { return m_VideoSettings.m_AllowNVPerfHUD; }

            std::string GetFloatingPointMode() { return m_VideoSettings.m_FloatingPointMode; }

            std::string GetFSAA() { return m_VideoSettings.m_FSAA; }

            std::string GetFullScreen() { return m_VideoSettings.m_FullScreen; }

            std::string GetRenderingDevice() { return m_VideoSettings.m_RenderingDevice; }

            std::string GetResourceCreationPolicy() { return m_VideoSettings.m_ResourceCreationPolicy; }

            std::string GetVSync() { return m_VideoSettings.m_VSync; }

            std::string GetVSyncInterval() { return m_VideoSettings.m_VSyncInterval; }

            std::string GetVideoMode() { return m_VideoSettings.m_VideoMode; }

            std::string GetGammaConversion() { return m_VideoSettings.m_sRGBGammaConversion; }
        
            void SetAllowNVPerfHUD(const std::string& data) { memcpy(m_VideoSettings.m_AllowNVPerfHUD, data.c_str(), data.size() + 1); }

            void SetFloatingPointMode(const std::string& data) { memcpy(m_VideoSettings.m_FloatingPointMode, data.c_str(), data.size() + 1); }

            void SetFSAA(const std::string& data) { memcpy(m_VideoSettings.m_FSAA, data.c_str(), data.size() + 1); }

            void SetFullScreen(const std::string& data) { memcpy(m_VideoSettings.m_FullScreen, data.c_str(), data.size() + 1); }

            void SetRenderingDevice(const std::string& data) { memcpy(m_VideoSettings.m_RenderingDevice, data.c_str(), data.size() + 1); }

            void SetResourceCreationPolicy(const std::string& data) { memcpy(m_VideoSettings.m_ResourceCreationPolicy, data.c_str(), data.size() + 1); }

            void SetVSync(const std::string& data) { memcpy(m_VideoSettings.m_VSync, data.c_str(), data.size() + 1); }

            void SetVSyncInterval(const std::string& data) { memcpy(m_VideoSettings.m_VSyncInterval, data.c_str(), data.size() + 1);  }

            void SetVideoMode(const std::string& data) { memcpy(m_VideoSettings.m_VideoMode, data.c_str(), data.size() + 1); }

            void SetGammaConversion(const std::string& data) { memcpy(m_VideoSettings.m_sRGBGammaConversion, data.c_str(), data.size() + 1); }

            bool LoadSettingsFromFile(const std::string& filename);

            static const char AllowPerfHud[];
            static const char FloatingPoint[];
            static const char FSAA[];
            static const char FullScreen[];
            static const char RenderingDevice[];
            static const char ResourceCreationPolicy[];
            static const char VSync[];
            static const char VSyncInterval[];
            static const char VideoMode[];
            static const char GammaConversion[];

        //private:
	        VideoSettings();
	        ~VideoSettings();

        private:
        #pragma pack(1)
	        struct
	        {
                char m_AllowNVPerfHUD[5];
                char m_FloatingPointMode[11];
                char m_FSAA[20];
                char m_FullScreen[5];
                char m_RenderingDevice[100];
                char m_ResourceCreationPolicy[30];
                char m_VSync[5];
                char m_VSyncInterval[2];
                char m_VideoMode[30];
                char m_sRGBGammaConversion[5];
	        } m_VideoSettings;
        #pragma pack()

            Ogre::Root* pRoot;
    };
}

#endif
