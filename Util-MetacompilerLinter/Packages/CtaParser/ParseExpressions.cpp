// ParseExpressions.cpp
// (see "CToken.h")
// (see "CToken_ExecutableExpressions.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CTransformContext.h"
#include "CMagicStrContext.h"
#include "CCtapContext.h"

//////////////////////////////////////////////////////////////////////////////////////
// CExecutable

CExecutable::CExecutable()
{
    x_pExpressionList = new CExpressionList;
}

bool CExecutable::OnIsSemicolonAtEnd()
{
    // Initializer: int a[] = { x, y };
    //                              ^--- no ';' after 'y'.
    if not_null(FindFirstParentOfKind(new CDeclaratorInitAsAssign))
    {
        return false;
    }

    // Use last expression
    int nExpressionCount = x_pExpressionList->GetExpressionCount();
    if (nExpressionCount == 0)
        return true;

    CExpression* pLastExpression =
        x_pExpressionList->GetExpression(nExpressionCount - 1);

    return 
        pLastExpression->IsSemicolonAtEndExpected() &&
        !pLastExpression->IsSemicolonAtEndAlreadyEmited();
}

CToken* CExecutable::OnTryExtend()
{
    x_pExpressionList->TryParse(this);

    return this;
}

//////////////////////////////////////////////////////////////////////////////////////
// CExpressionList - comma separated expressions

CExpressionList::CExpressionList()
{
    x_aExpression;
}

void CExpressionList::AddExpression(CExpression* pExpression)
{
    x_aExpression.Add(pExpression);
    pExpression->LinkToParent(this);
}

int CExpressionList::GetExpressionCount()
{
    return x_aExpression.GetCount();
}

CExpression* CExpressionList::GetExpression(int iExpression)
{
    return x_aExpression[iExpression];
}

bool CExpressionList::OnIsEndOfSequence(
            CToken* pToken)
{
    return
        // Generic terminators
        iskindof(CCharSemicolon, pToken) ||
        iskindof(CCharRightParen, pToken) ||
        iskindof(CCharRightBracket, pToken) ||
        iskindof(CCharRightBrace, pToken);
}

CToken* CExpressionList::OnTryExtend()
{
    // Collect sequence
    bool bDontExpectSemicolon = false;
    while (true)
    {
        // Check the pending token for an expression sequence terminator
        CToken* pToken = GetTokenizer()->PeekNextToken(this);
        if (IsEndOfSequence(pToken))
            break;

        // Skip separating ','
        if (iskindof(CCharComma, pToken))
            VERIFY(pToken == GetTokenizer()->ReadNextToken(this));

        // Parse the new statement until ';' or ','
        CExpression* pExpression = new CExpression;
        pExpression->TryParse(this);
        if (GetTokenizer()->PeekNextToken(this) == pToken)
        {
            // Didn't go any further, break the loop
            FAIL("infinite loop in CExpressionList::OnTryExtend()");
            break;
        }

        AddExpression(pExpression);

        // '}' terminates expression list
        //LOGIC:BRACE.EXPR.TERM
        if (!pExpression->IsSemicolonAtEndExpected())
        {
            bDontExpectSemicolon = true;
            break;
        }
        else
        {
            bDontExpectSemicolon = false;
        }
    }

    CToken* pFollowingToken = GetTokenizer()->PeekNextToken(this);
    if ((iskindof(CCharSemicolon, pFollowingToken) && !bDontExpectSemicolon) ||
        iskindof(CCharComma, pFollowingToken))
    {
        CToken* pVerifyToken = GetTokenizer()->ReadNextToken(this);
        ASSERTM(pVerifyToken == pFollowingToken,
            "';' or ',' can't extend themselves at the end of expression");
        pVerifyToken;
    }

    return this;
}

//////////////////////////////////////////////////////////////////////////////////////
// CExpression

CExpression::CExpression()
{
    x_aToken;

    x_bMagicStrModeEnabled = false;
    x_bForceAnyCommaContinueExpression = false;

    m_bParseEndedAtSemicolon = false;
    m_bSemicolonEmited = false;

    _init_AssignGlobalContextExpressionModes();

    //x_bMidZeroExpected = false;
}

void CExpression::_init_AssignGlobalContextExpressionModes()
{
    x_bMagicStrModeEnabled = 
        GetTransformContext()->GetMagicStrContext()->xx_bStartMagicStr;
}

void CExpression::AddToken(CToken* pToken)
{
    x_aToken.Add(pToken);
    pToken->LinkToParent(this);
}

void CExpression::ReplaceToken(
        int iAt,
        CToken* pToken)
{
    x_aToken.SetAt(iAt, pToken);
    pToken->LinkToParent(this);
    pToken->SetSourceFragmentIfNeededFrom(this);
}

int CExpression::GetTokenCount()
{
    return x_aToken.GetCount();
}

CToken* CExpression::GetToken(int iToken)
{
    return x_aToken[iToken];
}

int CExpression::IsPresent()
{
    return x_aToken.GetCount() > 0;
}

bool CExpression::IsSemicolonAtEndExpected()
{
    //if (m_bParseEndedAtSemicolon)
    //    return true;
        // kills parser logic

    if (x_aToken.GetCount() == 0)
        return true;

    CToken* pLastToken = x_aToken.GetLast();

    // Comma at the end of expression means initializer = { { }, { } }
    if (iskindof(CCharComma, pLastToken))
        return false;

    // Other cases
    if (!pLastToken->IsSemicolonEnabledAtExpressionEnd())
        return false;

    /*
    // '{ }'
    if not_null(derive(CCompound, pLastToken))
        return false;

    // __asm '{ }'
    if not_null(CAsmUnn* pAsmUnn =
        derive(CAsmUnn, pLastToken))
    {
        return false; //pAsmUnn->IsBraced();
    }
    */

    return true;
}

bool CExpression::IsSemicolonAtEndAlreadyEmited()
{
    return m_bSemicolonEmited;
}

CToken* CExpression::OnTryExtend()
{
    _init_AssignGlobalContextExpressionModes();
        //x_bMagicStrModeEnabled = 
            //GetTransformContext()->GetMagicStrContext()->xx_bStartMagicStr;

    // Init matching status
    int nParenCount = 0;

    CToken* pPrevToken = new CWhiteSpace;
    while (true)
    {
        CToken* pNextToken = GetTokenizer()->PeekNextToken(this);

        // Count guarded sub-levels
        if (iskindof(CCharLeftParen, pNextToken))
        {
            nParenCount++;
        }
        else
        if (iskindof(CCharRightParen, pNextToken))
        {
            nParenCount--;
            if (nParenCount < 0)
                break;
        }

        // Compound
        else
        if (iskindof(CCharLeftBrace, pNextToken))
        {
            // Always break expression on compound statement
            //LOGIC:BRACE.EXPR.TERM

            // Skip '{'
            pNextToken = GetTokenizer()->ReadNextToken(this);

            // Create compound statement
            CCompound* pCompound = new CCompound;

            // Are we sure this is a real expression?
            // Not preceeded with '='
            if (!iskindof(CCharAssign, pPrevToken))
            {
                // not a part of parsed ' = { ... } '
                if (FindParentDeclaratorInitAsAssign().IsNull())
                {
                    pCompound->x_bExecutableCompound = true;
                }
            }

            // Grab the rest into the compound
            pCompound->TryParse(this);

            // '{...}' is the last part of the expression list
            AddToken(pCompound);

            //fix: Missed comma in:  int a[] = { { 1 }, { 2 } };

[...]