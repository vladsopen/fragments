// ParseUnusualSyntax.cpp
// (see "CToken.h")
// (see "CCharTokens.h")
// (see "CToken_DeclarationComponents.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"

//////////////////////////////////////////////////////////////////////////////////////
// COperatorSpec

COperatorSpec::COperatorSpec()
{
    x_aToken;
}

void COperatorSpec::AddToken(CToken* pToken)
{
    x_aToken.Add(pToken);
    pToken->LinkToParent(this);
}

int COperatorSpec::GetTokenCount()
{
    return x_aToken.GetCount();
}

CToken* COperatorSpec::GetToken(int iToken)
{
    return x_aToken[iToken];
}

CToken* COperatorSpec::OnTryExtend()
{
    // Init matching status
    int nParenCount = 0;

    // Match tokens until '(' which starts argument list
    while (true)
    {
        CToken* pNextToken = GetTokenizer()->PeekNextToken(this);

        // Count guarded sub-levels
        if (iskindof(CCharLeftParen, pNextToken))
        {
            if (nParenCount <= 0 &&
                GetTokenCount() > 0)
            {
                // This paren opens arg list
                break;
            }

            nParenCount++;
        }
        else
        if (iskindof(CCharRightParen, pNextToken))
        {
            nParenCount--;
            if (nParenCount < 0)
                break;
        }

        // EOF stops everything, '}' closes outer statement list
        if (iskindof(CEof, pNextToken) ||
            iskindof(CCharSemicolon, pNextToken) ||
            iskindof(CCharLeftBrace, pNextToken) ||
            iskindof(CCharRightBrace, pNextToken))
        {
            GetTokenizer()->ReportErrorUnexpectedToken(
                errn(0xA23E),
                pNextToken,
                "'operator' specificaion",
                this);
            break;
        }

        // Eat token
        pNextToken = GetTokenizer()->ReadNextToken(this);
        AddToken(pNextToken);
    }

    // Continue extending as CUnqualifiedName onto CQualifiedName
    return super::OnTryExtend();
}

//////////////////////////////////////////////////////////////////////////////////////
// CUsingOp

CUsingOp::CUsingOp()
{
    x_aToken;
}

void CUsingOp::AddToken(CToken* pToken)
{
    x_aToken.Add(pToken);
    pToken->LinkToParent(this);
}

int CUsingOp::GetTokenCount()
{
    return x_aToken.GetCount();
}

CToken* CUsingOp::GetToken(int iToken)
{
    return x_aToken[iToken];
}

bool CUsingOp::OnIsSemicolonAtEnd()
{
    return true;
}

bool CAsmUnn::IsBraced()
{
    if (x_aToken.GetCount() > 0)
    {
        if not_null(derive(CCharRightBrace, x_aToken.GetLast()))
            return true;
    }

    return false;
}

bool CAsmUnn::OnIsSemicolonEnabledAtExpressionEnd()
{
    return false;
}

CToken* CAsmUnn::OnTryExtend()
{
    // Special low level tokenining is employed to parse assembler code
    //  __asm dec dword ptr fs:[0x10]
    //  __asm { nop }
    //  __asm nop
    //  __asm nop ; comment

    bool bAsmComment = false;
    bool bBracedBody = false;

    while (true)
    {
        CToken* pNextToken = GetTokenizer()->ReadNextTokenInLowLevelMode(this);

        // End of statement?
        if (iskindof(CEof, pNextToken))
        {
            GetTokenizer()->ReportErrorUnexpectedToken(
                errn(0xA24E),
                pNextToken,
                "in __asm statement",
                this);
            break;
        }

        if (!bAsmComment)
        {
            //__asm { nop }

            // Braced mode?
            if (iskindof(CCharLeftBrace, pNextToken))
                bBracedBody = true;

            // End of Braced mode?
            else
            if (iskindof(CCharRightBrace, pNextToken))
            {
                if (!bBracedBody)
                {
                    // '}' must be a part of enclosing compound statement
                    GetTokenizer()->RollbackToToken(pNextToken);
                    break;
                }
                else
                {
                    // __asm { }    -or-
                    // __asm { } ;

                    // add '}'
                    x_aToken.Add(pNextToken);

                    CToken* pAfterNextToken =
                        GetTokenizer()->ReadNextTokenInLowLevelMode(this);
                    if (iskindof(CCharSemicolon, pAfterNextToken))
                    {
                        // add ';'
                        x_aToken.Add(pAfterNextToken);

[...]