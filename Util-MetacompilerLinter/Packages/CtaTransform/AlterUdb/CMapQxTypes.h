// CMapQxTypes.h
#pragma once

//
// CMapQxTypes - 
//
//   Udb scalar types allowed/recognized/transformed and property prefixes.
//     The Singleton is global in CGlobalDbContext.
//

class CMapQxTypes : public CCommon
{
public:
    GENERIC(CMapQxTypes, CCommon);
    CMapQxTypes();

// Constants

// Attributes

// Operations

    // For CheckDbProperty()
    bool IsDbPropertyPrefixAllowed(
            str sTypePrefix);

    // Qx parser type detection
    str LookupQxClassForExpressionPrefix(
            str sTypePrefix);

    // For Cee code verify
    str LookupCeeTypeForQxClass(
            str sTypePrefix);

    // Catches SQL incompatible operators, mainly C++ string concatenations
    void CheckOperatorApplicableToQxClass(
            str sIterSource,
            str sQxClassFromPrevToken,
            CToken* pReportErrorForToken);

// UI

protected:
private:
    map<str, str> m_mapPrefixToQx;
    map<str, str> m_mapQxClassToCeeCheckType;
    map<str, unused> m_mapNoNumericQxClassToOperators;
};
