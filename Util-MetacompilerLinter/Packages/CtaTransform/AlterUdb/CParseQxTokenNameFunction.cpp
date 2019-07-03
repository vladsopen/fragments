// CParseQxTokenNameFunction.cpp

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
#include "CParseQxTokenNameFunction.h"
#include "CParseQxLevel.h"
#include "CParseQxTokenBase.h"
#include "CMapQxTypes.h"
#include "CParseQxCode.h"
#include "CParseQxTokenName.h"

CParseQxTokenNameFunction::CParseQxTokenNameFunction()
{
}

void CParseQxTokenNameFunction::ParseQxFunction(
        CParseQxTokenName* pName,
        CQualifiedName* pMainName,
        CParseQxLevel* pLevel,
        CParseQxTokenBase* pBase,
        out int& out_iToken,
        out str& out_sDefiniteQxClass)
{
    out_sDefiniteQxClass;
    pMainName;

    //CGlobalDbContext* pGlobalDbContext = 
    //    GetTransformContext()->GetGlobalDbContext();
    //CMapQxTypes* pMapQxTypes = 
    //    pGlobalDbContext->GetMapQxTypes();

    // ORDER(ASC) ORDER(DESC) ORDER(eOrder)
    // MIN MAX SUM AVG COUNT

    if (pName->x_sRootName == "ORDER")
    {
        pLevel->x_nOrderByFound++;

        // skip '('
        out_iToken += 1;

        // only at top level to make expression list
        if (pLevel->x_iParenLevel != 0)
        {
            GetTokenizer()->ReportTransformClientError(
                errn(0xC1E0),
                "SQL ORDER cannot be specified in a subexpression",
                pBase->x_pIterToken);
        }

        str sSortError;
        if not_null(
            CQualifiedName* pSortArg = 
                derive(CQualifiedName, pBase->x_pNextNextToken))
        {
            str sDirection = 
                pSortArg->GetRootUnqualifiedNameStr();

            // skip FN arg
            out_iToken += 1;

            if not_null(
                derive(CCharRightParen, pBase->x_pNextNext3Token))
            {
                // skip ')'
                out_iToken += 1;

                if (sDirection == "E_Order_Ascending")
                {
                    pBase->x_sAddSql += " ASC ";
                }
                else
                if (sDirection == "E_Order_Descending")
                {
                    pBase->x_sAddSql += " DESC ";
                }
                else
                {
                    pBase->x_sAddSql += 
                        " \" + GSqlOrderVariable(" + 
                            sDirection + 
                            ") + \" ";
                }

                pBase->x_sAddCee;
                pBase->x_sAddBind;

                pLevel->x_nCommaExpectedAfterOrder = 1;
            }
            else
            {
                sSortError = 
                    "simple EOrder variable or constant "
                        "expected as parameter";
            }
        }
        else
        {
            sSortError = 
                "expected EOrder variable or constant "
                    "name as parameter";
        }

        if (sSortError != "")
        {
            GetTokenizer()->ReportTransformClientError(
                errn(0xC677),
                "SQL ORDER syntax error: " + 
                    sSortError,
                pBase->x_pNextNextToken);
        }
    }
    else

[...]