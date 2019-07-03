// CErrorSuppressor.cpp
// (see "CErrorSuppressor.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
//#include "AllSpecificTokenClasses.h"
#include "CErrorSuppressor.h"

CErrorSuppressor::CErrorSuppressor()
{
    m_mapPendingErrorIdToQuote;
}

void CErrorSuppressor::SetErrorSuppression(
        str sErrorId,
        str sErrorQuote,
        CStatement* pStatement)
{
    ASSERT(sErrorId.Len() == sizeof(C_szErrorIdTemplate) - 1);
    ASSERT(sErrorQuote.Len() > 0);

    if (m_mapPendingErrorIdToQuote.IsIn(sErrorId))
    {
        GetTokenizer()->ReportError(
            errn(0xA70A),
            "duplicate suppression for error '" + sErrorId + "'",
            (CToken*) pStatement);
    }

    m_mapPendingErrorIdToQuote[sErrorId] = sErrorQuote;
}

bool CErrorSuppressor::QuerySuppressionForIssuedError(
        str sErrorId,
        str sErrorText,
        CStatement* pStatement)
{
    ASSERT(sErrorId.Len() == sizeof(C_szErrorIdTemplate) - 1);

    str sErrorQuote;
    if (!m_mapPendingErrorIdToQuote.LookupExisting(sErrorId, out sErrorQuote))
    {
        return false; // OK to report it to the user!
    }

    // Done with this error
    m_mapIssuedErrorId << sErrorId;

    // It IS suppresssed, check the quote
    sErrorQuote = sErrorQuote.GetAfterFindAndDelete("[optional]").GetStripBoth();
    if (sErrorText.FindPos_fast(sErrorQuote.c()) < 0)
    {
        GetTokenizer()->ReportError(
            errn(0xA71A),
            "invalid quote '" + sErrorQuote + "' for error ID '" + sErrorId + "' ",
            (CToken*) pStatement);

        return false;
    }

    return true;
}

void CErrorSuppressor::VerifySuppressedErrorsReallyIssued(
        CStatement* pStatement)
{
    if (POS pos = m_mapPendingErrorIdToQuote.GetHeadPos())
    {

[...]