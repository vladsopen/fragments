// CMapQxTypes.cpp

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "CSourceLines.h"
#include "CTransformContext.h"
#include "CAlterQx.h"
#include "CAlterMember.h"
#include "CCodeMarking.h"
#include "CGlobalDbContext.h"
#include "lib__idparser.h"
#include "CMapQxTypes.h"

CMapQxTypes::CMapQxTypes()
{
    //
    // Prefixes
    //

    m_mapPrefixToQx["b"] = "qxbool";
    m_mapQxClassToCeeCheckType["qxbool"] = "bool";

    m_mapPrefixToQx["n"] = "qxinteger";
    m_mapPrefixToQx["nn"] = "qxinteger";
    m_mapPrefixToQx["i"] = "qxinteger";
    m_mapPrefixToQx["ii"] = "qxinteger";
    m_mapPrefixToQx["tick"] = "qxinteger";
    m_mapPrefixToQx["utc"] = "qxinteger";
    m_mapPrefixToQx["rgb"] = "qxinteger";
    m_mapQxClassToCeeCheckType["qxinteger"] = "big";

    m_mapPrefixToQx["num"] = "qxfloating";
    m_mapQxClassToCeeCheckType["qxfloating"] = "num";

    m_mapPrefixToQx["s"] = "qxstring";
    m_mapPrefixToQx["rich"] = "qxstring";
    m_mapPrefixToQx["sloc"] = "qxstring";
    m_mapPrefixToQx["moment"] = "qxstring";
    m_mapPrefixToQx["period"] = "qxstring";
    m_mapPrefixToQx["path"] = "qxstring";
    m_mapPrefixToQx["type"] = "qxstring";
    m_mapQxClassToCeeCheckType["qxstring"] = "str";

    m_mapPrefixToQx["sbuf"] = "qxblob";
    //m_mapPrefixToQx["unidef"] = "qxblob";
    m_mapQxClassToCeeCheckType["qxblob"] = "sbuf";

    m_mapPrefixToQx["key"] = "qxkey";
    m_mapQxClassToCeeCheckType["qxkey"] = "key";

    //
    // SQL-compatible operators (non-numeric)
    //

    m_mapNoNumericQxClassToOperators << "qxbool ==";
    m_mapNoNumericQxClassToOperators << "qxbool !=";

    m_mapNoNumericQxClassToOperators << "qxstring ==";
    m_mapNoNumericQxClassToOperators << "qxstring !=";
    m_mapNoNumericQxClassToOperators << "qxstring >";
    m_mapNoNumericQxClassToOperators << "qxstring <";
    m_mapNoNumericQxClassToOperators << "qxstring >=";
    m_mapNoNumericQxClassToOperators << "qxstring <=";
    m_mapNoNumericQxClassToOperators << "qxstring LIKE";

    m_mapNoNumericQxClassToOperators << "qxblob ==";
    m_mapNoNumericQxClassToOperators << "qxblob !=";
    m_mapNoNumericQxClassToOperators << "qxblob >";
    m_mapNoNumericQxClassToOperators << "qxblob <";
    m_mapNoNumericQxClassToOperators << "qxblob >=";
    m_mapNoNumericQxClassToOperators << "qxblob <=";

    m_mapNoNumericQxClassToOperators << "qxkey ==";
    m_mapNoNumericQxClassToOperators << "qxkey !=";
    m_mapNoNumericQxClassToOperators << "qxkey LIKE";
}

bool CMapQxTypes::IsDbPropertyPrefixAllowed(
        str sTypePrefix)
{
    if (sTypePrefix.IsAbbrev("db"))
    {
        return true;
    }

    return m_mapPrefixToQx.IsIn(sTypePrefix);
}

str CMapQxTypes::LookupQxClassForExpressionPrefix(
        str sTypePrefix)
{
    str sQx;
    m_mapPrefixToQx.LookupExisting(sTypePrefix, out sQx);
    return sQx;
}

str CMapQxTypes::LookupCeeTypeForQxClass(
        str sQxClass)
{
    str sCee;
    m_mapQxClassToCeeCheckType.LookupExisting(sQxClass, out sCee);
    return sCee;
}

void CMapQxTypes::CheckOperatorApplicableToQxClass(
        str sIterSource,
        str sQxClassFromPrevToken,
        CToken* pReportErrorForToken)
{
    if (sQxClassFromPrevToken == "qxinteger"
        ||
        sQxClassFromPrevToken == "qxfloating")
    {
        // all ops OK
        return;
    }

    if (!m_mapNoNumericQxClassToOperators.
            IsIn(
                sQxClassFromPrevToken + " " + sIterSource))
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC00C),
            "operation " + 
                sIterSource + 
                " is not SQL compatible for " +
                sQxClassFromPrevToken,
            pReportErrorForToken);
    }
}
