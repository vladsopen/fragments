// CParseQxTokenNameFrom.cpp

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
#include "CParseQxTokenNameFrom.h"
#include "CParseQxLevel.h"
#include "CParseQxTokenBase.h"
#include "CMapQxTypes.h"
#include "CParseQxCode.h"
#include "VirtOs_heapbracing.h"

CParseQxTokenNameFrom::CParseQxTokenNameFrom()
{
}

void CParseQxTokenNameFrom::ParseQxFrom(
        CParseQxTokenName* pName,
        CQualifiedName* pMainName,
        CParseQxLevel* pLevel,
        CParseQxTokenBase* pBase,
        out int& out_iToken,
        out str& out_sDefiniteQxClass)
{
    out_sDefiniteQxClass;
    pName;

    CGlobalDbContext* pGlobalDbContext = 
        GetTransformContext()->GetGlobalDbContext();
    CMapQxTypes* pMapQxTypes = 
        pGlobalDbContext->GetMapQxTypes();

    if (pMainName->GetQualifierCount() != 0)
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC0a5),
            "ill-formed SQL 'from' (table) name: " + 
                pMainName->GetSourceFragmentStr(),
            pBase->x_pIterToken);
    }

    // Column spec: fromXxxx->x_xxxProp
    if not_null(
        derive(CCharArrow, pBase->x_pNextToken))
    {
        out_iToken += 2; // '->' 'Name'

        if not_null(
            CQualifiedName* pColumnName =
                derive(CQualifiedName, pBase->x_pNextNextToken))
        {
            str sFullColumnName = 
                pColumnName->GetRootUnqualifiedNameStr();
            idparser::DNameParts dataColumnNameParts;
            idparser::GExtractNamePrefixes(
                sFullColumnName.c(),
                out dataColumnNameParts);

            str sColumnProperty = 
                pColumnName->GetRootUnqualifiedNameStr();
            str sColumnName = 
                dataColumnNameParts.m_sTypePrefix + 
                    dataColumnNameParts.m_sVerboseName;
            str sColumnType = dataColumnNameParts.m_sTypePrefix;

            // substitute some built-in VirtOs function<-->column mapping

            static map<str, str> s_mapSpecialQxFunctionToRawSql;
            if (s_mapSpecialQxFunctionToRawSql.GetCount() == 0)
            {
                globalheaps::disable hd;

                s_mapSpecialQxFunctionToRawSql["GetKey"] = 
                    C_szDbObjectPrimaryKey; // "keyPrimary"
                s_mapSpecialQxFunctionToRawSql["GetDbContainerKey"] = 
                    C_szDbObjectContainerKeyColumn; // "keyContainer"
                s_mapSpecialQxFunctionToRawSql["GetMmTraitKey"] = 
                    C_szDbObjectMmTrait; // "keyMmTrait"
                s_mapSpecialQxFunctionToRawSql["GetMmTieKey"] = 
                    C_szDbObjectMmTie; // "keyMmTie"
                // sScalar/keyNonscalar accesses as is
            }

            str sSubstituteSpecialFunction;
            if (s_mapSpecialQxFunctionToRawSql.
                    LookupExisting(
                        sFullColumnName,
                        out sSubstituteSpecialFunction))

            //str sSpecialFunction = "GetKey";
            //if (sFullColumnName == sSpecialFunction)
            {
                if (derive(CCharLeftParen, pBase->x_pNextNext3Token).IsNull()
                    ||
                    derive(CCharRightParen, pBase->x_pNextNext4Token).IsNull())
                {
                    GetTokenizer()->ReportTransformClientError(
                        errn(0xC02f),
                        "special SQL member function '" +
                            sFullColumnName + 
                            "' must be called without parameters. ",
                        pBase->x_pIterToken);
                }
                else
                {
                    //     Fn (   )
                    out_iToken += 1 + 1;

                    sColumnProperty += "()";
                    sColumnName = sSubstituteSpecialFunction; //"keyPrimary";
                }
            }
            else
            if (dataColumnNameParts.m_cMemberVariableAccessType != 'x'
                ||
                pColumnName->GetQualifierCount() != 0)
            {
                GetTokenizer()->ReportTransformClientError(
                    errn(0xC0a4),
                    "ill-formed property (column) name "
                        "in SQL column 'from' specification: " + 
                        pColumnName->GetSourceFragmentStr(),
                    pBase->x_pIterToken);
            }
            else
            {
                sColumnProperty += "__itl";

                // scalar class detection
                if (sColumnType == "b")
                {
                    pLevel->x_bBoolExpressionFound = true;
                }
                else
                {
                    str sQxClass =
                        pMapQxTypes->
                            LookupQxClassForExpressionPrefix(
                                sColumnType);

                    if (sQxClass != "")
                    {
                        pLevel->x_sQxClassFromToken = sQxClass;
                    }
                    else
                    {
                        GetTokenizer()->ReportTransformClientError(
                            errn(0xC0CC),
                            "unexpected class prefix in "
                                "'" + sFullColumnName + "'",
                            pBase->x_pIterToken);
                    }
                }
            }

            // SQL for accessing a column for a named FROM table alias
            pBase->x_sAddSql += 
                " " + str() +
                    //perf: can get statically only from locally declared name
                    //pMainName->GetRootUnqualifiedNameStr() + 
                    "\" + " +
                    pMainName->GetRootUnqualifiedNameStr() + 
                    "._udb_GetFromIdentifier()"
                    " + \"" +
                    "." +
                    sColumnName +
                    " "
                    ;

            // C++ syntax check for column/property access
            pBase->x_sAddCee += 
                " " + 
                    pMainName->GetRootUnqualifiedNameStr() + 
                    ".pcheck()->" +
                    sColumnProperty
                    ;

            //sAddBind;
        }
        else
        {
            GetTokenizer()->ReportTransformClientError(
                errn(0xC0a3),
                "expected property (column) name "
                    "after '->' in SQL column 'from' specification " 
                    "(found " + pBase->x_pNextNextToken->GetSourceFragmentStr() + 
                    ")",
                pBase->x_pIterToken);
        }
    }
    else
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xC0a2),
            "expected '->' after SQL column 'from' specification " 
                "(found " + pBase->x_pNextToken->GetSourceFragmentStr() + 
                ")",
            pBase->x_pIterToken);
    }
}

