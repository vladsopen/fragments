// CErrorSuppressor.h
// (see "CErrorSuppressor.cpp")

//
// CErrorSuppressor - implements cta_ignore error suppression logic
//  for a client statement
//

#define C_szErrorIdTemplate "[ERRN]"

class CErrorSuppressor : public CCommon
{
public:
    GENERIC(CErrorSuppressor, CCommon);
    CErrorSuppressor();

// Control

    void SetErrorSuppression(
            str sErrorId,
            str sErrorQuote,
            CStatement* pStatement);

    bool QuerySuppressionForIssuedError(
            str sErrorId,
            str sErrorText,
            CStatement* pStatement);

    void VerifySuppressedErrorsReallyIssued(
            CStatement* pStatement);

private:
    map<str, str> m_mapPendingErrorIdToQuote;
    map<str, unused> m_mapIssuedErrorId;
};

