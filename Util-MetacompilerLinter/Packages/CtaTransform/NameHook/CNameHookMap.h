// CNameHookMap.h
#pragma once

class CAlterCtaMacro;
class CNameHookDirective;

//
// CNameHookMap - 
//
//   Limit global ID use with SEMIPUBLIC(Id, "path", "path")
//

class CNameHookMap : public CCommon
{
public:
    GENERIC(CNameHookMap, CCommon);
    CNameHookMap();

// Registration

    // called from cta_macro
    void RegisterNameHook(
            str sName,
            CAlterCtaMacro* pAlterCtaMacro);

// Check

    // issue errors on incoming identifier
    void TriggerNameHook(
            str sName,
            CToken* pTokenLocation);

    // issue errors on _internalxxxx_ prefixes
    void TriggerInternalPrefixHook(
            str sName,
            CToken* pTokenLocation);

private:
    map<str, CNameHookDirective*> m_mapIdToDirective;
};

