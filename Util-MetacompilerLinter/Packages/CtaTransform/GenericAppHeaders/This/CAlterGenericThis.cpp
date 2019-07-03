// CAlterGenericThis.cpp
// (see "CAlterGenericThis.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "CSourceLines.h"
#include "CTransformContext.h"
#include "CAlterGenericThis.h"
#include "CAlterGenericClass.h"
#include "CAlterMember.h"

CAlterGenericThis::CAlterGenericThis()
{
}

void CAlterGenericThis::OnTransform()
{
    super::OnTransform();
}

void CAlterGenericThis::OnRegenerate(CWriter* pWriter)
{
    // Detect Generic method
    nullable<CQualifiedName*> npMethodQualifiedName;
    str sRootClassName;
    nullable<CAlterGenericClass*> npAlterGenericClass;
    nullable<CAlterMember*> npAlterMember;
    GetTransformContext()->
        IsGenericMethodImplHeader(
            FindParentFunctionBodyDeclaration(),
            out npMethodQualifiedName,
            out sRootClassName,
            out npAlterGenericClass,
            out npAlterMember);

    // If it looks like it is declared in a Generic class
    //   it is enough to apply the transform
    bool bTransform = false;
    if not_null(CAlterGenericClass* pAlterGenericClass = npAlterGenericClass)
    {
        // Check a stop-flag
        if (pAlterGenericClass->IsNonRefThisPointer())
        {
            // skip it
            bTransform = false;
        }
        else
        {
            bTransform = true;

            // but look for 'this ->' expressions, don't need ref<> for them
            if not_null(
                CExpression* pParentExpression =
                    derive(CExpression, GetParentToken()))
            {
                int iThisIndex = pParentExpression->x_aToken.Find(this);
                if ok(iThisIndex >= 0)
                {
                    nullable<CToken*> npPrevToken = 
                        pParentExpression->x_aToken.GetAtOrNull(iThisIndex - 1);
                    nullable<CToken*> npNextToken = 
                        pParentExpression->x_aToken.GetAtOrNull(iThisIndex + 1);

                    // "this ->"
                    if (iskindof(CCharArrow, npNextToken))
                    {
                        bTransform = false;
                    }

                    // "this ->*"
                    if (iskindof(CCharArrowStar, npNextToken))
                    {
                        bTransform = false;
                    }

                    // "* this"
                    if (iskindof(CCharStar, npPrevToken))
                    {
                        bTransform = false;
                    }

                    // "! this"
                    if (iskindof(CCharExclamation, npPrevToken))
                    {
                        bTransform = false;
                    }
                }
            }
        }
    }

    if (bTransform)
    {
        // Write generated token according to default representation
        pWriter->WriteTokenZeroTerminatedString(
            this,
            "CTA_GENERIC_THIS", // ThisRef CTA_THIS_REF
            E_LineNumbering_SyncWithToken);
    }
    else
    {
        super::OnRegenerate(pWriter);
    }
}