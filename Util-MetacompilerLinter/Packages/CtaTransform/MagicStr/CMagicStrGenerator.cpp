// CMagicStrGenerator.cpp
// (see "CMagicStrGenerator.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "CSourceLines.h"
#include "CTransformContext.h"
#include "CMagicStrGenerator.h"
#include "CMagicStrContext.h"
#include "CAlterCtaMidZero.h"
#include "CAlterFunctionMacro.h"
#include "CAlterCtaSz.h"
#include "CAlterCtaStrPrecalc.h"
#include "VirtOs_unicode.h"

// shared C51 firmware code
#undef LICENSE_C51 // compiler selection
#define LICENSE_FIRMWARE_HEADER ;
#include "ProtectedResponse.h"
#include "ProtectedTable.h"

CMagicStrGenerator::CMagicStrGenerator()
{
    m_bInsideMagicStrSequence = false;
    m_bMagicStrModeStartedInFile = false;
    m_apMagicStrToken;
    m_bPlusSuppressed = false;
    m_bMidZeroExpected = false;
    m_bStrPrecalcRequested = false;
    xx_bKillSemicolonAtEnd = false;
}

void CMagicStrGenerator::StartAtRootExpression(
        CExpression* pExpression,
        CWriter* pWriter)
{
    m_bMagicStrModeStartedInFile = pExpression->x_bMagicStrModeEnabled;

    if (pWriter->GetOptions()->xm_bDisableMagicStr)
        m_bMagicStrModeStartedInFile = false;

    m_apMagicStrToken.RemoveAll();
    m_bPlusSuppressed = false;

    // Start recursive tree scan
    DiveIntoSubExpression(
        pExpression,
        pWriter);

    if (m_bInsideMagicStrSequence)
    {
        EmitMagicStrAtEndOfExpression(
            pExpression, 
            pWriter, 
            m_bPlusSuppressed);
    }
}

void CMagicStrGenerator::DiveIntoSubExpression(
        CExpression* pExpression,
        CWriter* pWriter)
{
    bool bAlterCtaSzMode = false;
    bool bStringFoundAlterCtaSz = false;
    repeat(iToken, pExpression->x_aToken.GetCount())
    {
        // Next token
        CToken* pToken = pExpression->x_aToken[iToken];

        // Flatten subexpression where supported
        if (pToken->
                SupportMagicStrGeneratorScanner(
                    this,
                    pWriter))
        {
            continue;
        }

        // "+"-operator must be removed between string constants,
        //   but kept alive at the end of sequence
        bool bPlusWasSuppressed = m_bPlusSuppressed;
        m_bPlusSuppressed = false;
        bool bSkipRegenerateThisToken = false;

        // CTA_SZ flags
        bool bExpectingStringFollowingAlterCtaSz = bAlterCtaSzMode;
        if (iskindof(CAlterCtaSz, pToken))
        {
            bAlterCtaSzMode = true;
            bStringFoundAlterCtaSz = false;
        }
        else
        {
            bAlterCtaSzMode = false;
        }

        // Support for CTA_STR_PRECALC
        if (iskindof(CAlterCtaStrPrecalc, pToken))
        {
            m_bStrPrecalcRequested = true;
        }

        // sizeof() may disable MagicStr in this expression
        bool bDisableMagicStr = pExpression->IsDisableMagicStr();

        if (!m_bMagicStrModeStartedInFile)
        {
            // nop
        }
        else

        // Propagate disabled mode for nested sizeof() expressions
        if (bDisableMagicStr)
        {
            pToken->SetDisableMagicStr(true);
        }
        else

[...]