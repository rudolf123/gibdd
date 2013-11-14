#ifndef SettingH
#define SettingH

namespace Gibdd
{
    class Settings
    {
        public:
	        virtual const PVOID GetSettings() = 0;

	        virtual DWORD GetSettingsSize() = 0;

	        virtual void SetSettings(PVOID	pNewSettings) = 0;

	        virtual void ResetToDefault() = 0;
    };
}

#endif