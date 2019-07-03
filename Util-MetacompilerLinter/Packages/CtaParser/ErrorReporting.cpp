// ErrorReporting.cpp
// (see "CToken.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "CSourceLines.h"
#include "VirtOs_heapbracing.h"
#include "CTransformContext.h"
#include "CErrorSuppressionMatch.h"
#include "VirtOs_file.h"

/*
CErrorNullPointer::CErrorNullPointer(str sErrorId, sloc slocError)
: CError(sErrorId, slocError, "Cta")
{
}
*/

bool CTokenizer::ExpectCharToken(
        errn errnCode,
        CToken* pKindOfToken,
        const char* szStatementDescription,
        CToken* pParentToken)
{
    if (!ExpectCharTokenGetToken(
            errnCode,
            pKindOfToken,
            szStatementDescription,
            pParentToken).IsNull())
    {
        return true;
    }
    else
    {
        return false;
    }
}

nullable<CToken*> CTokenizer::ExpectCharTokenGetToken(
        errn errnCode,
        CToken* pKindOfToken,
        const char* szStatementDescription,
        CToken* pParentToken)
{
    CToken* pNextToken = PeekNextToken(pParentToken);
    if (iskindofobject(pKindOfToken, pNextToken))
    {
        // Skip expected object
        return ReadNextToken(pParentToken);
    }
    else
    {
        ReportErrorExpecting(
            errnCode,
            pKindOfToken,
            szStatementDescription,
            pParentToken);

        return nullable<CToken*>();
    }
}

void CTokenizer::ReportErrorUnexpectedToken(
        errn errnCode,
        CToken* pToken,
        str sStatementDescription,
        nullable<CToken*> npContextToken)
{
    ReportError(
        errnCode,
        "unexpected " + pToken->GetContentPreviewFriendlyLine() + " " +
        sStatementDescription,
        npContextToken);
}

void CTokenizer::ReportErrorExpecting(
        errn errnCode,
        CToken* pKindOfToken,
        str sStatementDescription,
        nullable<CToken*> npContextToken)
{
    ReportError(
        errnCode,
        "missing " + pKindOfToken->GetContentPreviewFriendlyLine() + " " +
        sStatementDescription,
        npContextToken);
}

void CTokenizer::ReportErrorExpectingClosing(
        errn errnCode,
        CToken* pKindOfToken,
        str sStatementDescription,
        nullable<CToken*> npContextToken)
{
    ReportErrorExpecting(
        errnCode,
        pKindOfToken,
        "closing " + sStatementDescription,
        npContextToken);
}

void CTokenizer::ReportInternalError(
        errn errnCode,
        str sErrorReport,
        nullable<CToken*> npContextToken)
{
    ReportError(
        errnCode,
        " [INTERNAL ERROR] " + sErrorReport,
        npContextToken);
}

void CTokenizer::ReportInternalTransformError(
        errn errnCode,
        str sErrorReport,
        nullable<CToken*> npContextToken)
{
    ReportError(
        errnCode,
        " [internal transformer error] " + sErrorReport,
        npContextToken);
}

void CTokenizer::ReportTransformClientError(
        errn errnCode,
        str sErrorReport,
        nullable<CToken*> npContextToken)
{
    ReportError(
        errnCode,
        sErrorReport + " [transform client error]",
        npContextToken);
}

void CTokenizer::FailOnInternalCheck(
        bool bCheckOk)
{
    if (bCheckOk)
    {
        ReportMessage(
            errn(0xAD48),
            "INTERNAL CHECK FAILED",
            nullable<CToken*>());
    }
}

void CTokenizer::ReportWarning(
        errn errnCode,
        str sErrorReport,
        nullable<CToken*> npContextToken)
{
    str sCompleteReport =
        "warning: " + sErrorReport;

    /*
    ReportMessage(
        errnCode,
        sCompleteReport,
        npContextToken);
        */

    // report as error
    if (ReportMessage(
            errnCode,
            sCompleteReport,
            npContextToken))
    {
        ReturnErrorCode();
    }
}

void CTokenizer::ReportError(
        errn errnCode,
        str sErrorReport,
        nullable<CToken*> npContextToken)
{
    str sCompleteReport =
        "Cta error: " + sErrorReport;
    if (ReportMessage(
            errnCode,
            sCompleteReport,
            npContextToken))
    {
        ReturnErrorCode();
    }
}

void CTokenizer::ReportAppendix(
        errn errnCode,
        str sErrorReport,
        nullable<CToken*> npContextToken)
{
    str sCompleteReport =
        "note: " + sErrorReport;
    ReportMessage(
        errnCode,
        sCompleteReport,
        npContextToken);
}

str CTokenizer::GetSourceFileLineReference(
        nullable<CToken*> npReferenceToken,
        nullable<CToken*> npContextToken,
        out nullable<CToken*>& out_npFoundToken,
        out str& out_sSourceLocationReport)
{
    str sReferenceText;
    out_sSourceLocationReport = "";

    // Get default source reference for the token
    int iTokenLineInfo = CToken::C_nUndefinedLineNumber;

    // Find a closest token with a reference
    nullable<CToken*> npFindToken1;
    nullable<CToken*> npFindToken2;

    // Enable alternate reference?
    if (GetStage() == E_Stage_Parsing)
    {
        // Don't use non-context token while parsing if possible
        npFindToken1 = npContextToken;
        npFindToken2 = npReferenceToken;
        if not_null(CToken* pToken = npFindToken1)
        {
            out_sSourceLocationReport +=
                "parsing stage using " +
                    pToken->GetContentPreviewFriendlyLine() +
                    " context token; ";
            npFindToken2.MakeNull();
        }
        else
        if not_null(CToken* pToken2 = npFindToken2)
        {
            out_sSourceLocationReport +=
                "parsing stage using " +
                    pToken2->GetContentPreviewFriendlyLine() +
                    " reference token; ";
        }
        else
        {
            out_sSourceLocationReport +=
                "parsing stage not using reference tokens; ";
        }
    }
    else
    {
        // Use all available tokens
        ASSERT(GetStage() == E_Stage_Transforming ||
                GetStage() == E_Stage_Writing);
        npFindToken1 = npReferenceToken;
        npFindToken2 = npContextToken;
    }

    repeat(iRepeat, 100) // sanity
    {
        // Can 1st token supply the info?
        if not_null(CToken* pToken = npFindToken1)
        {
            iTokenLineInfo = pToken->GetLineInfoNumber();
            if (iTokenLineInfo != CToken::C_nUndefinedLineNumber)
            {
                out_npFoundToken = pToken;
                out_sSourceLocationReport +=
                    Str(iTokenLineInfo + 1) +
                        " line from " + pToken->GetContentPreviewFriendlyLine() +
                        " - primary token; ";
                break;
            }
            npFindToken1 = pToken->GetParentToken();
        }

        // Can 2nd token supply the info?
        if not_null(CToken* pToken = npFindToken2)
        {
            iTokenLineInfo = pToken->GetLineInfoNumber();
            if (iTokenLineInfo != CToken::C_nUndefinedLineNumber)
            {
                out_npFoundToken = pToken;
                out_sSourceLocationReport +=
                    Str(iTokenLineInfo + 1) +
                        " line from " + pToken->GetContentPreviewFriendlyLine() +
                        " - secondary token; ";
                break;
            }
            npFindToken2 = pToken->GetParentToken();
        }

        // Scan parents?
        if (GetStage() == E_Stage_Parsing)
        {
            // Don't scan parents, prefer m_iCurrentLineInfo
            out_sSourceLocationReport +=
                "not scanning parent tokens; ";
            break;
        }
        else
        {
            // Scan parent hierarcy because m_iCurrentLineInfo
            //   probably points to the EOF anyway
            ASSERT(GetStage() == E_Stage_Transforming ||
                   GetStage() == E_Stage_Writing);
        }

        // Continue loop?
        if not_null(npFindToken1)
            continue;
        if not_null(npFindToken2)
            continue;

        out_sSourceLocationReport +=
            "no reference from tokens; ";
        break;
    }

    // Got anything?
    if (iTokenLineInfo == CToken::C_nUndefinedLineNumber)
    {
        iTokenLineInfo = m_iCurrentLineInfo;
        out_sSourceLocationReport +=
            "current line number used; ";
    }

    // We may not have a source reference
    if (iTokenLineInfo == CToken::C_nUndefinedLineNumber)
    {
        sReferenceText += "<<undefined source line>>";
    }
    else
    {
        sReferenceText =
            GetSourceLines()->GetSourceFileLineReference(iTokenLineInfo);
    }

    return sReferenceText;
}

static str ReportContextInfoForToken(nullable<CToken*> npToken)
{
    if not_null(CToken* pToken = npToken)
    {
        return pToken->GetContentPreviewFriendlyLine(CToken::E_Preview_Extended);
    }
    else
    {
        return "<Undefined>";
    }
}

void CTokenizer::EnableMessages(bool bEnable)
{
    m_bEnableMessages = bEnable;
}

bool CTokenizer::IsMessagesEnabled()
{
    return m_bEnableMessages;
}

bool CTokenizer::ReportMessage(
        errn errnCode,
        str sReport,
        nullable<CToken*> npReferenceToken)
{
    ASSERT(errnCode.x_nError > 0x0000 &&
           errnCode.x_nError < 0xFFFF);

    if (!IsMessagesEnabled())
        return false;

    //debug:
    //FAIL(sReport);

    // New line
    str sCompleteReport = "\n";

    // file(line)
    nullable<CToken*> npFoundReferenceToken;
    str sSourceLocationReport;
    str sFileLineReference =
        GetSourceFileLineReference(
            npReferenceToken,
            GetContextToken(),
            out npFoundReferenceToken,
            out sSourceLocationReport);
    sCompleteReport += sFileLineReference;

    // -Cta-
    str sErrorCode = "[" + Hex(errnCode.x_nError).GetUpper() + "]";
    sCompleteReport += " : -Cta- " + sErrorCode + " ";

    // Main text
    sCompleteReport += sReport;
    //sCompleteReport += " Err#" + sErrorCode;
    sCompleteReport += "\n";

    // Filter similar messages
    str sErrorTextForCompare = sCompleteReport;

    // cta_ignore() filter
    bool bErrorSuppressed = false;

    // Error globally hidden
    bErrorSuppressed =
        bErrorSuppressed ||
            GetTransformContext()->
                IsMessageMatchingAllSkipError(
                    sErrorTextForCompare);

    // Error hidden in the file?
    CErrorSuppressionMatch* pErrorSuppressionMatch = new CErrorSuppressionMatch;
    pErrorSuppressionMatch->xx_sErrorId = sErrorCode;
    pErrorSuppressionMatch->xx_sErrorQuote = sErrorTextForCompare;

    // Source dump
    if not_null(CToken* pContextToken = npFoundReferenceToken)
    {
        // Source quote
        sCompleteReport += "\n" + GetSourceContextQuote(pContextToken);

        bErrorSuppressed = bErrorSuppressed ||
            pContextToken->QueryStatementSuppressionForIssuedError(
                sErrorCode,
                sErrorTextForCompare);

        // support cta_ignore_in_file()
        if (pContextToken->GetLineInfoNumber() != CToken::C_nUndefinedLineNumber)
        {
            pErrorSuppressionMatch->xx_sFilePathSubstring = 
                pContextToken->GetValidOriginalSourceFile(
                    str() + "for error suppresion by FoundReferenceToken");
    

[...]