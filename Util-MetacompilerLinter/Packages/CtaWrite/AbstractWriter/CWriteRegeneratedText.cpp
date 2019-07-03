// CWriteRegeneratedText.cpp
// (see "CWriteRegeneratedText.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "CSourceLines.h"
#include "CToken_QualifiedName.h"
#include "CTransformContext.h"
#include "CXlang.h"

CWriteRegeneratedText::CWriteRegeneratedText()
{
    m_bInitCalled = false;
    m_bInitCalled = true; // don't need it?

    m_sAppendOutput;
    //m_pSourceLines = new CSourceLines;
    m_iPreviousLineInfoNumber = CToken::C_nUndefinedLineNumber;
    m_iLastOriginalLineNumberWritten = CToken::C_nUndefinedLineNumber;
    CASSERT(CToken::C_nUndefinedLineNumber == -1);
    m_iActualCurrentOutputLine = 0;
    m_pathPreviousSourceFile = "";
    m_iMaxLineInfoForWhichPragmasWritten = -1;
    m_iLastOriginalLineIndent = 0;
    //m_szLastWritten = "";
}

void CWriteRegeneratedText::HandleLineNumbering(
        CToken* pToken,
        ELineNumbering eLineNumbering)
{
    //ASSERT(nLength >= 0);

    if (eLineNumbering == E_LineNumbering_SyncWithToken)
    {
        HandleTokenLineSync(pToken);
    }
    else
    {
        ASSERT(eLineNumbering == E_LineNumbering_SuppressSync);
    }
}

/*
 !"#$%&'()*+,-./
0123456789:;<=>?
@ABCDEFGHIJKLMNO
PQRSTUVWXYZ[]^_`
abcdefghijklmnop
qrstuvwxyz{|}~
*/

static charmap C_charmapIdOrNumber("[A-Z][a-z][0-9]_");
static charmap C_charmapNeverSpaceBefore("),.;]");
static charmap C_charmapNeverSpaceAfter("!([");

void CWriteRegeneratedText::HandleSpaceSeparation(
        CToken* pToken,
        const char* pcNextFragment)
{
    if (!GetOptions()->xm_bAlwaysSpaceSeparate)
    {
        if (IsDebugDumpMode())
            return;

        if (pToken->IsNoSpaceBefore())
            return;
    }

    unsigned char cLast = m_sAppendOutput.GetAtOr(m_sAppendOutput.Len() - 1, 0);
    unsigned char cNext = pcNextFragment[0];

    // Was space?
    if (cLast <= (unsigned char) '\x20')
        return;

    // Closing punctuation
    if (C_charmapNeverSpaceBefore[cNext])
        return;

    // Opening punctuation
    if (C_charmapNeverSpaceAfter[cLast])
        return;

    // After an ID
    if (C_charmapIdOrNumber[cLast])
    {
        // Function(...
        if (cNext == '(')
            return;
    }

    EmitZeroTerminated(" ");
}

void CWriteRegeneratedText::OnWriteTokenBuffer(
        CToken* pToken,
        const char* pcNextFragment,
        int nLength,
        ELineNumbering eLineNumbering)
{
    //if (ShouldCancelWrite(pToken, pcNextFragment))
    //    return;

    if (nLength == 0)
        return;

    HandleLineNumbering(pToken, eLineNumbering);
    HandleSpaceSeparation(pToken, pcNextFragment);
    EmitSingleLineBuffer(pcNextFragment, nLength);
}

void CWriteRegeneratedText::EmitAnyBuffer(const char* pcNextFragment, int nLength)
{
    ASSERT(m_bInitCalled);

    DoAppendBuffer(pcNextFragment, nLength);

    //m_sAppendOutput += 
    //    sbuf(sbuf::ECopyFromUnsafeBufferSpecifiedCount(), 
    //        pcNextFragment, 
    //        nLength);

    int nMaxOutputChars = GetOptions()->xm_nMaxOutputChars;
    if (nMaxOutputChars > 0)
    {
        if (m_sAppendOutput.Len() > nMaxOutputChars)
        {
            m_sAppendOutput += " ....";
            throw CWriterOptions::exception_HitMaxOutputChars();
        }
    }
}

[...]