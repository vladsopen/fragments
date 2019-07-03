// CCodeCoverageNextHookWrite.cpp

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CHookWriteIface.h"
#include "CCodeCoverageNextHookWrite.h"
#include "CWriter.h"
#include "CHookWriteMap.h"
#include "CTransformContext.h"
#include "CCodeMarking.h"

void CCodeCoverageNextHookWrite::OnHookBeforeWriteToken(
        CWriter* pWriter,
        CToken* pToken,
        const char* pcContent,
        int nLength,
        ELineNumbering eLineNumbering)
{
    pcContent;
    eLineNumbering;

    // wait for REAL code
    if (nLength == 0)
    {
        return;
    }

    // skip ourselves!
    if (pToken == x_pCharRightBrace)
    {
        return;
    }

    // skip all inserted
    if (iskindof(CInsertCode, pToken))
    {
        return;
    }

    // end of block without any code?
    if (pToken != x_pCharRightBrace &&
        (
            iskindof(CCharRightBrace, pToken) ||
            iskindof(CCharLeftBrace, pToken) ||
            iskindof(CCompound, pToken) ||
            iskindof(CFunctionBody, pToken)
        ))
    {
        // no marker!
        GetTransformContext()->
            x_pCodeMarking->
                CancelLineCoverageAt(
                    x_iLineEntry,
                    x_sPrefixedCounterId);
    }
    else
    {
        // mark!
        pWriter->
            WriteTokenStr(
                pToken,
                x_sCtaBraceInsert,
                E_LineNumbering_SyncWithToken);
    }

    // do not duplicate it!
    pWriter->x_pHookWriteMap->RemoveHook(this);
}

