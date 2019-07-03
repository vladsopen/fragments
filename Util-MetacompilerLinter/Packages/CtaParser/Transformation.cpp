// Transformation.cpp
// (see "CToken.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CTransformContext.h"
#include "CAlterMember.h"
#include "CCodeMarking.h"
#include "CCtapTransformer.h"
#include "CCtapContext.h"
#include "CXlang.h"
#include "CNameHookMap.h"

void CToken::Transform()
{
    xx_bCtapPhpMode = 
        GetTransformContext()->GetCtapContext()->xx_bInsidePhp;

    // Set the flag
    if (m_fMisc & F_Misc_Transformed)
        return;
    (int&) m_fMisc |= F_Misc_Transformed;

    OnTransform();

    // PHP: Apply PHP transforms
    if (xx_bCtapPhpMode)
    {
        CCtapTransformer* pCtapTransformer = new CCtapTransformer;
        pCtapTransformer->TransformPhpToken(this);
    }
}

void CToken::TransformArray(
    const array<CToken*>& aToken)
{
    repeat(iToken, aToken.GetCount())
        aToken[iToken]->Transform();
}

void CDeclaration::OnTransform()
{
    super::OnTransform();

    GetTransformContext()->HandleDeclarationTransform(this);

    x_pDeclModifierList->Transform();
    x_pDeclareTypeName->Transform();
    x_pBaseClassList->Transform();
    x_pClassBody->Transform();
    x_pDeclaratorList->Transform();
    x_pDeclareRelatedConstructors->Transform();
    x_pFunctionBody->Transform();
}

void CDeclModifierList::OnTransform()
{
    TransformArray((array<CToken*>&) x_aDeclModifier);
}

void CFunctionBody::OnTransform()
{
    // Save coverage state
    CCodeMarking* pCodeMarking = GetTransformContext()->x_pCodeMarking;
    bool bSaveEnableCoverageHooks = pCodeMarking->x_bEnableCoverageHooks;

    // Hook
    // Do not instrument pseudo-functions "__if_exists", "__if_not_exists"
    if (!IsIfVerifyingExistenceBody() &&
        !x_npFunctionClosingBrace.IsNull()) // otherwise we hook enums! :O
    {
        // assume for now everything else IS a function

        // get the name
        if not_null(
            CDeclaration* pDeclaration =
                FindFirstParentDeclaration())
        {
            if not_null(
                CQualifiedName* pFunctionName =
                    pDeclaration->LookupFirstDeclaratorName())
            {
                // Name not excluded?
                if (!coverage::GIsFunctionNameExcludedFromCoverage(
                        pFunctionName->GetRootUnqualifiedNameStr()))
                {
                    pCodeMarking->
                        HandleEnterCoverageHook(
                            "CTA_FN", // CTA_FN/CTA_BRACE
                            x_pStatementList,
                            CCodeMarking::E_InsertCoverage_AtStart);
                }
            }
        }
    }

    x_pStatementList->Transform();

    if not_null(
        CCharRightBrace* pRightBrace = x_npFunctionClosingBrace)
    {
        pRightBrace->Transform();
    }

    // Restore coverage mode
    pCodeMarking->x_bEnableCoverageHooks = bSaveEnableCoverageHooks;
}

void CCompound::OnTransform()
{
    // Save coverage state
    CCodeMarking* pCodeMarking = GetTransformContext()->x_pCodeMarking;
    bool bSaveEnableCoverageHooks = pCodeMarking->x_bEnableCoverageHooks;

    // Hook
    bool bHook = false;
    if (x_bExecutableCompound)
    {
        bHook = true;

        pCodeMarking->
            HandleEnterCoverageHook(
                "CTA_BRACE", // CTA_FN/CTA_BRACE
                x_pStatementList,
                CCodeMarking::E_InsertCoverage_AtStart);
    }

    x_pStatementList->Transform();

    if not_null(
        CCharRightBrace* pRightBrace = x_npClosingBrace)
    {
        pRightBrace->Transform();
    }

    // Hook
    if (bHook &&
        !IsFollowedByElseOrWhile())
    {
        pCodeMarking->
            HandleEnterCoverageHook(
                "CTA_BRACE", // CTA_FN/CTA_BRACE
                x_pStatementList,
                CCodeMarking::E_InsertCoverage_AfterEnd);
    }

[...]