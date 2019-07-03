// CIniSettings.h

#define NAME_Cta_ini "Cta.ini"

class CIniSettings : public CCommon
{
public:
    GENERIC(CIniSettings, CCommon);
    CIniSettings();

    // Locate the .ini file
    bool Initialize();
        // szStartPath can be absolute or relative to the current directory,
        //   search proceeds up the tree.

    // Reads an Option value
    str ReadOption(str sKey, str sDefault) const;
    bool ReadOption(str sKey, bool bDefault) const;

private:
    path m_pathIni;
    char m_pcOccPath[MAX_PATH*3];
    char m_pcMetaclassLibPath[MAX_PATH*3];
};

