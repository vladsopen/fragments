// CAlterPropertyName.cpp
// (see "CAlterPropertyName.h")

#include "Precompiled.h"
#include "VirtOs_array_Advanced.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CWriteRegeneratedText.h"
#include "CTransformContext.h"
#include "CAlterDeclaration.h"
#include "CAlterGenericClass.h"
#include "CAlterMember.h"
#include "CAlterQualifiedName.h"
#include "CAlterPropertyName.h"
#include "CSession.h"

CAlterPropertyName::CAlterPropertyName()
{
}

void CAlterPropertyName::ApplyTransform()
{
    // this -> x_zProp    ==>  this -> x_zProp__itl
    // x_zProp = e ;      ==>  x_zProp_Set ( e )
    // x_zProp = e , a ;  ==>  x_zProp_Set ( e , a )
    // x_zProp . f        ==>  x_zProp_f
    // x_zProp . f        ==>  x_zProp_f
    // for simple [r]:
    // [r]x_zProp ++/?= e ==>  [r]x_zProp_Set ( [r]x_zProp_Get() ? e )
    // [r]x_zProp . Not() ==>  [r]x_zProp_Set ( ! [r]x_zProp_Get() )
    // x_zProp     /else/ ==>  x_zProp_Get ( )

    CQualifiedName* pQualifiedName = GetQualifiedName();
    CUnqualifiedName* pRootUnqualifiedName = pQualifiedName->GetRootUnqualifiedName();
    str sRootName = pRootUnqualifiedName->GetNameStr();

    // Get parent expression
    if not_null(
        CExpression* pParentExpression =
            derive(CExpression, pQualifiedName->GetParentToken()))
    {
        int iNameIndexInParent = pParentExpression->x_aToken.Find(pQualifiedName);
        if (iNameIndexInParent >= 0)
        {
            // Private access
            // this -> x_zProp  ==>  this -> x_zProp__itl
            if (iskindof(CCharArrow,
                    pParentExpression->x_aToken.GetAtOrNull(iNameIndexInParent - 1)) &&
                iskindof(CThisPointer,
                    pParentExpression->x_aToken.GetAtOrNull(iNameIndexInParent - 2)))
            {
                // Allow direct access only through explicit 'this' pointer
                pRootUnqualifiedName->SetNameStr(
                    sRootName + "__itl");

                GetTransformContext()->
                    x_mapMentionedPrivatePropertyNameTransforms[sRootName] = this;
                return;
            }

            // 'Set' assignment
            // x_zProp = e ;      ==>  x_zProp_Set ( e )
            // x_zProp = e , a ;  ==>  x_zProp_Set ( e , a )
            nullable<CToken*> npNextToken = 
                pParentExpression->x_aToken.GetAtOrNull(iNameIndexInParent + 1);
            if (iskindof(CCharAssign, npNextToken))
            {

[...]