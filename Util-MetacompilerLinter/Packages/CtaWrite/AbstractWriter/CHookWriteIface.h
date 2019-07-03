// CHookWriteIface.h
#pragma once

class CWriter;

//
// CHookWriteIface - 
//
//   Pure iface for a Writer callback
//

class CHookWriteIface : public CCommon
{
public:
    GENERIC_ABSTRACT(CHookWriteIface, CCommon);
    CHookWriteIface();

// Constants

// Attributes

// Operations

    // Called at the beginning of CWriter::HandleTokenStart()
    void HookBeforeHandleTokenStart(
            CWriter* pWriter, 
            CToken* pToken);

    // Called from all WriteTokenXxxx() (WriteTokenStr() etc)
    void HookBeforeWriteToken(
            CWriter* pWriter, 
            CToken* pToken,
            const char* pcContent,
            int nLength,
            ELineNumbering eLineNumbering);

// UI

protected:

    virtual void OnHookBeforeHandleTokenStart(
            CWriter* pWriter, 
            CToken* pToken)
            = 0;

    virtual void OnHookBeforeWriteToken(
            CWriter* pWriter, 
            CToken* pToken,
            const char* pcContent,
            int nLength,
            ELineNumbering eLineNumbering)
            = 0;

private:

    // reentry prevetion
    bool _m_bInsideHook;

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
