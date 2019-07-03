// CParseQxTokenNameQxSub.cpp

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
#include "CParseQxTokenNameQxSub.h"
#include "CParseQxLevel.h"
#include "CParseQxTokenBase.h"
#include "CMapQxTypes.h"
#include "CParseQxCode.h"
#include "CParseQxTokenName.h"

CParseQxTokenNameQxSub::CParseQxTokenNameQxSub()
{
}

void CParseQxTokenNameQxSub::ParseQxSub(
        CParseQxTokenName* pName,
        CQualifiedName* pMainName,
        CParseQxLevel* pLevel,
        CParseQxTokenBase* pBase,
        out int& out_iToken,
        out str& out_sDefiniteQxClass)
{
    out_sDefiniteQxClass;
    out_iToken;
    pMainName;

    //CGlobalDbContext* pGlobalDbContext = 
    //    GetTransformContext()->GetGlobalDbContext();
    //CMapQxTypes* pMapQxTypes = 
    //    pGlobalDbContext->GetMapQxTypes();

    //
    // Check for parens around (qxExpression)
    //

    const str C_sSyntaxHelp = 
        " (A Qx local variable must be included in parens: (qxExample)"
            " or separated with ',' "
            " which ensures only *complete* expression can be merged)";

    bool bInParens = false;
    bool bInCommas = false;

    if not_null(
        derive(CCharComma, pBase->x_pPrevToken))
    {
        // ok if not braced at left
        bInCommas = true;
    }
    else
    if (pLevel->x_iLevelToken != 0)
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC0C1),
            "missing opening paren before 'qx' subexpression " 
                "(found " + pBase->x_pIterToken->GetSourceFragmentStr() + 
                ")" + 
                C_sSyntaxHelp,
            pBase->x_pIterToken);
    }
    else
    {
        bInParens = true;
    }

    if not_null(
        derive(CCharComma, pBase->x_pNextToken))
    {
        // ok if not braced at right
        bInCommas = true;
    }
    else
    if not_null(
        derive(CCharRightParen, pBase->x_pNextToken))
    {
        // ok
        bInParens = true;
    }
    else
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC0C2),
            "missing closing paren just after 'qx' subexpression " 
                "(found " + pBase->x_pNextToken->GetSourceFragmentStr() + 
                ")" + 
                C_sSyntaxHelp,
            pBase->x_pNextToken);
    }

    bInParens;
    bInCommas;

    str sQxVariable = 
        pName->x_sRootName;

    if (sQxVariable != pBase->x_pIterToken->GetSourceFragmentStr())
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC0C3),
            "'qx' subexpression must be a simple local variable in parens " 
                "(found " + pBase->x_pIterToken->GetSourceFragmentStr() + 
                ")" + 
                C_sSyntaxHelp,
            pBase->x_pIterToken);
    }

    pLevel->x_sQxClassFromToken = pName->x_sType;

    str sCeeTypeCheck = "nothing()";

    /*
    if (pName->x_sType == "qxdata")
    {
    }
    else
    */
    if (pName->x_sType == "qxbool")
    {
        pLevel->x_bBoolExpressionFound = true;
        sCeeTypeCheck = "true";
    }
    else
    if (pName->x_sType == "qxinteger")
    {
        sCeeTypeCheck = "123456";
    }
    else
    if (pName->x_sType == "qxfloating")
    {
        sCeeTypeCheck = "1.1";
    }
    else
    if (pName->x_sType == "qxstring")
    {
        sCeeTypeCheck = "str()";
    }
    else
    if (pName->x_sType == "qxblob")
    {
        sCeeTypeCheck = "sbuf()";
    }
    else
    if (pName->x_sType == "qxkey")
    {
        sCeeTypeCheck = "key()";
    }
    else
    if (pName->x_sType == "qxmany")
    {
    }
    else
    if (pName->x_sType == "qxstat")
    {
        pLevel->x_bAggregateFunctionsFound = true;
    }
    else
    if (pName->x_sType == "qxsort")
    {
        pLevel->x_nOrderByFound++;
    }
    else
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xCFC3),
            "unknoen Qx variable type prefix '" + 
                pName->x_sType + 
                "' ",
            pBase->x_pIterToken);
    }

    pBase->x_sAddSql += 
        " \" " // close string constant
            " + " +
            sQxVariable +
            "._udb_GetQxSqlOr(qxabstract::C_sEmptySubQxPlaceholder)"
            " + "
            " \" " // reopen string constant
            ;

    pBase->x_sAddCee += 
        " " + sCeeTypeCheck + " /* " + sQxVariable + " */ ";

    pBase->x_sAddBind += 
        sQxVariable;
}

