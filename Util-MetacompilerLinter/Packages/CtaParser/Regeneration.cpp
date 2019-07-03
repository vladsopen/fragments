// Regeneration.cpp
// (see "CToken.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "DComplexityStats.h"
#include "CTransformContext.h"
#include "CAlterTranslate.h"
#include "CMagicStrGenerator.h"
#include "CCodeMarking.h"
#include "CCtapTransformer.h"
#include "CXlang.h"

void CToken::Regenerate(CWriter* pWriter)
{
    if (CTokenizer::GetStage() > CTokenizer::E_Stage_Transforming)
    {
        if (!(m_fMisc & (F_Misc_Transformed | F_Misc_Inserted)))
            GetTokenizer()->ReportInternalError(
                errn(0xFF4A),
                "token transform skipped",
                this);
    }

    if (IsRegenerationEnabled())
    {
        // code inserted before
        if (m_sBeforeToken != "")
        {
            pWriter->
                WriteTokenMultiLineStr(
                    this,
                    m_sBeforeToken,
                    E_LineNumbering_SyncWithToken);
        }

        pWriter->HandleTokenStart(this);

        OnRegenerate(pWriter);

        pWriter->HandleTokenEnd(this);

        // code inserted after
        if (m_sAfterToken != "")
        {
            pWriter->
                WriteTokenMultiLineStr(
                    this,
                    m_sAfterToken,
                    E_LineNumbering_SyncWithToken);
        }
    }
}

void CToken::OnRegenerate(CWriter* pWriter)
{
    pWriter->xx_pdataComplexityStats->xx_nTotalTokens++;

    if (m_nFragmentLength > 0)
    {
        // Rewrite parsed token as it was read
        pWriter->
            WriteTokenBuffer(
                this,
                m_szFragmentStart,
                m_nFragmentLength,
                E_LineNumbering_SyncWithToken);
    }
    else
    if (IsDefaultTextRepresentationDefined())
    {
        // Write generated token according to default representation
        pWriter->
            WriteTokenZeroTerminatedString(
                this,
                GetDefaultTextRepresentation(),
                E_LineNumbering_SyncWithToken);
    }

    // BEFORE cannot go AFTER!
    /*
    // code inserted after
    if (m_sBeforeToken != "")
    {
        pWriter->
            WriteTokenStr(
                this,
                m_sBeforeToken,
                E_LineNumbering_SyncWithToken);
    }
    */
}

const char* CToken::GetDefaultTextRepresentation()
{
    const char* szText = OnGetDefaultTextRepresentation();

    if (szText[0] == 0)
    {
        FAIL("CToken::OnGetDefaultTextRepresentation() must be overriden for tokens "
                "which require default text regeneration");
        return "UNKNOWN_TOKEN_TEXT";
    }

    return szText;
}

str CToken::WriteSimpleSource(
        CWriterOptions* pWriterOptions,
        CWriter** out_unsafe_pReturnWriter)
{
    // Create writer
    CWriter* pWriter = NULL;
    if (out_unsafe_pReturnWriter)
        pWriter = *out_unsafe_pReturnWriter;
    if (!pWriter)
        pWriter = new CWriteRegeneratedText();

    if (out_unsafe_pReturnWriter)
        *out_unsafe_pReturnWriter = pWriter;

    // Configure
    pWriterOptions->xm_bAlwaysSpaceSeparate = true;
    pWriterOptions->xm_bEnablePreservedDirectives = false;
    pWriterOptions->xm_bEnableLineDirectives = false;
    pWriter->SetOptions(pWriterOptions);

    //debug verify for FINALIZE_OBJECT
    bool bPrev = GetTokenizer()->IsMessagesEnabled();

    // Go!
    try
    {
        // Disable any error messages
        GetTokenizer()->EnableMessages(false);

        // Regenerate
        Regenerate(pWriter);
    }
    catch (CWriterOptions::exception_HitMaxOutputChars)
    {
        // ignore - if the client requested this, it will take care
    }

    GetTokenizer()->EnableMessages(bPrev);

    // Result in spaces
    str sWriterResult = pWriter->GetRegenerationResult();
    str sResult;
    sResult.ReserveAlloc(sWriterResult.Len() + 2);
    sResult += " ";
    sResult += sWriterResult;
    sResult += " ";

    return sResult;
}
[...]