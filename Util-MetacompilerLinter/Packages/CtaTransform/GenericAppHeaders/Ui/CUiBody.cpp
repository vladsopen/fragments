// CUiBody.cpp
// (see "..CUiBody.h")

#include "Precompiled.h"
#include "VirtOs_array_Advanced.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CTransformContext.h"
#include "CAlterDeclaration.h"
#include "CUiBody.h"
#include "CUiBranch.h"
#include "CAlterGenericClass.h"

CUiBody::CUiBody()
{
    m_bFirstBranch = true;
}

/*
    {                    PanelUI("UiXxx") {
        code1;               code1;
        ref<UiXxx> rUi;      UiXxx* rUi = cta_InitUI(UiXxxx);
        code2;               cta_Ievent("OnUpdate") { code2; }
        { }                  { }
        |                    |
        rUi->OnMmm = [=]     &UiXxx::OnMmm; cta_Ievent("Mmm")
            { ... }              { ...; return; }
    }                    }

*/

void CUiBody::ParseUiMethodStatementList(
        CStatementList* pOriginalStatementList,
        out int& out_nUiFound)
{
    //
    // Enum all top level statements in the list
    //  looking for magic ref<Ui...> rUi ...; declarations
    //

    repeat(iStatement, pOriginalStatementList->x_aStatement.GetCount())
    {
        CStatement* pStatement = pOriginalStatementList->x_aStatement[iStatement];

        // Transform only once
        if (!QueryTransformPermission(pStatement))
            continue;

        // 'rUi'-Declaration?
        if not_null(
            CDeclaration* pDeclaration = derive(CDeclaration, pStatement))
        {
            // Declaration for 'rUi'?
            if not_null(CDeclarator* pFirstDeclarator =
                pDeclaration->x_pDeclaratorList->x_aDeclarator.GetAtOrNull(0))
            {
                // 'rUi'?
                if not_null(CQualifiedName* pQualifiedName =
                    derive(CQualifiedName,
                        pFirstDeclarator->x_pDeclareObject->x_aToken.GetAtOrNull(0)))
                {
                    // 'rUi'?
                    if (pQualifiedName->GetRootUnqualifiedNameStr() == "rUi")
                    {
                        out_nUiFound++;

                        // Yes! Declaration marking new UI block
                        HandleUiBlockDeclaration(
                            pDeclaration);
                        break;
                    }
                }
            }
        }

        // Recursive calls for all nested Compounds
        if not_null(
            CExecutable* pExecutable = derive(CExecutable, pStatement))
        {
            repeat(iExpression,
                pExecutable->x_pExpressionList->x_aExpression.GetCount())
            {
                CExpression* pExpression =
                    pExecutable->x_pExpressionList->x_aExpression[iExpression];

                // Transform only once
                if (!QueryTransformPermission(pExpression))
                    continue;

                repeat(iToken, pExpression->x_aToken.GetCount())
                {
                    CToken* pToken = pExpression->x_aToken[iToken];

                    // Transform only once
                    if (!QueryTransformPermission(pToken))
                        continue;

                    if not_null(CCompound* pCompound =
                        derive(CCompound, pToken))
                    {
                        //int nUiFound = 0;
                        ParseUiMethodStatementList(
                            pCompound->x_pStatementList,
                            out out_nUiFound);
                    }
                }
            }
        }
    }
}

void CUiBody::HandleUiBlockDeclaration(
        CDeclaration* pDeclaration)
{
    CUiBranch* pUiBranch = new CUiBranch;

    // Extract and cache class name
    if (!GetUiClassName(pDeclaration, pUiBranch).Len())
        return;

    // Has initializer?
    TransformInitializer(pDeclaration, pUiBranch);

    if not_null(
        CStatementList* pParentStatementList =
            derive(CStatementList, pDeclaration->GetParentToken()))
    {
        ASSERT(pUiBranch->x_bUiUpdateBlockInserted == false);

        // Insert PanelUI() prefix
        InsertInitialCaption(pDeclaration, pParentStatementList, pUiBranch);

        if (!pUiBranch->x_bSubUi)
        {
            // Insert "#define rUi _rUi_" / "#undef rUi"
            {
                int iDeclarationInList =
                    pParentStatementList->x_aStatement.Find(pDeclaration);
                InsertUndefUnlockingUi(
                    pParentStatementList,
                    iDeclarationInList,
                    "// enable 'rUi' in declaration",
                    iDeclarationInList + 2,
                    "// disable 'rUi' after declaration",
                    "", // sFinalizerCode,
                    pUiBranch);
            }

            // Process event handlers
            TransformAllUiEventHandlers(pDeclaration, pParentStatementList, pUiBranch);

            // Update block may not be created yet
            EnsureUiUpdateBlockBracing(
                pDeclaration,
                pParentStatementList->x_aStatement.GetCount(),
                pUiBranch);
        }
        else
        {
            ASSERT(pUiBranch->x_bSubUi);

            // Insert "#define rUi _rUi_" / "#undef rUi"
            {
                int iDeclarationInList =
                    pParentStatementList->x_aStatement.Find(pDeclaration);
                InsertUndefUnlockingUi(
                    pParentStatementList,
                    iDeclarationInList,
                    "// enable 'rUi' in SubUI declaration",
                    pParentStatementList->x_aStatement.GetCount() + 1,
                    "// disable 'rUi' after SubUI block",
                    "rUi->_internal_FinalizeChildContainer(CTA_GENERIC_THIS); ",
                    pUiBranch);
            }

            // Don't transform anything else for SubUI
        }
    }
    else
    {
        GetTokenizer()->ReportInternalTransformError(
            errn(0xAA70),
            "can't get 'rUi'-declaration parent StatementList",
            pDeclaration);
    }
}

void CUiBody::TransformRefToPtr(
        CDeclaration* pDeclaration,
        CUiBranch* pUiBranch)
{
    pUiBranch;

    str sError;
    if not_null(
        CQualifiedName* pTypeQualifiedName =
            pDeclaration->x_pDeclareTypeName->x_npQualifiedName)
    {
        CUnqualifiedName* pTypeUnqualifiedName =
            pTypeQualifiedName->GetRootUnqualifiedName();

        str sUiClassName = GetUiClassName(pDeclaration, pUiBranch);

        // Append '*' and set name to the root typename token
        pTypeUnqualifiedName->SetNameStr(sUiClassName + "*");

        // Kill template arguments
        pTypeUnqualifiedName->x_npTemplateArgList =
            nullable<CTemplateArgList*>();
    }
}


[...]