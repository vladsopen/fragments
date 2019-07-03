// CPxExpression.h
#pragma once

//
// CPxExpression - PHP expression transformer
//

class CPxExpression : public CCommon
{
public:
    GENERIC(CPxExpression, CCommon);
    CPxExpression();

// Const

// Attributes

// Operations

    // Main
    void PostTransformPhpExpression(
            CExpression* pExpression);

protected:
private:

    void ConvertDotToFunctionCall(
            CExpression* pExpression);
    void ConvertPlusToPhpDot(
            CExpression* pExpression);
    bool ReplaceSingleDotToFunction(
            CExpression* pExpression);
    void DoReplaceDotToFunctionAt(
            CExpression* pExpression,
            int iDotToken);
    int SkipLeftExpression(
            CExpression* pExpression,
            int iDotToken);
    bool ConvertContructorToGlobalCall(
            CExpression* pExpression,
            CQualifiedName* pQualifiedName,
            int iFunctionNameToken);
    void ConvertMemberToPhp(
            CExpression* pExpression,
            CQualifiedName* pQualifiedName,
            CToken* pPrevToken,
            int iMemberNameToken);

    bool IsStringToken(
            CToken* pToken);
    bool IsNumericToken(
            CToken* pToken);
    void GetPrdPrefixAndName(
            CToken* pToken,
            out str& out_sPrefix,
            out str& out_sName);
};

