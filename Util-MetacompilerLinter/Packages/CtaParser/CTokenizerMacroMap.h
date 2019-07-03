// CTokenizerMacroMap.h
#pragma once

class CCharToken;
class CTokenizerMacro;

//
// CTokenizerMacroMap - 
//
//   Low level token substitution service
//
//ASSUME_IMPL_FOR_IFACE(CTokenizerMacroMapImpl, CTokenizerMacroMap)

class CTokenizerMacroMap : public object
{
public:
    CTokenizerMacroMap();
    //NEW_LOCAL(CTokenizerMacroMapImpl, CTokenizerMacroMap)
    //NEW_GP(Cta, CTokenizerMacroMapImpl, CTokenizerMacroMap)
    //virtual void OnExposeContent(
            //ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // Init
    void InitTokenizerMacroMap();

    // Lookup
    const char* TryTokenizerMacroMap(
            const char* szInput,
            int iCurrentLineInfo,
            out int& out_nSkipInput);

// UI

protected:
private:
    //bool _m_bOneTimeInitTokenizerMacroMapOk = false;

    array< CTokenizerMacro* > _m_apMacro;

    void _init_AttachToTokenizerMacroMap();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
