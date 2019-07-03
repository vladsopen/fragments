// CHookWriteIface.cpp

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CHookWriteIface.h"

CHookWriteIface::CHookWriteIface()
{
    _m_bInsideHook = false;
}

void CHookWriteIface::HookBeforeHandleTokenStart(
        CWriter* pWriter,
        CToken* pToken)
{
    if (_m_bInsideHook)
    {
        return;
    }

    cleaner<bool, false> cleanerbInsideHook(&_m_bInsideHook);
    _m_bInsideHook = true;

    return
        OnHookBeforeHandleTokenStart(
            pWriter,
            pToken);
}

void CHookWriteIface::HookBeforeWriteToken(
        CWriter* pWriter,
        CToken* pToken,
        const char* pcContent,
        int nLength,
        ELineNumbering eLineNumbering)
{
    if (_m_bInsideHook)
    {
        return;
    }

    cleaner<bool, false> cleanerbInsideHook(&_m_bInsideHook);
    _m_bInsideHook = true;

    return
        OnHookBeforeWriteToken(
            pWriter,
            pToken,
            pcContent,
            nLength,
            eLineNumbering);
}

