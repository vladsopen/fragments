// CAlterDollarName.cpp

#include "Precompiled.h"
//#include "VirtOs_array_Advanced.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
//#include "CWriter.h"
//#include "CWriteRegeneratedText.h"
//#include "CTransformContext.h"
//#include "CAlterDeclaration.h"
#include "CAlterDollarName.h"
#include "CMagicStrGenerator.h"
//#include "CSession.h"

CAlterDollarName::CAlterDollarName()
{
}

void CAlterDollarName::OnTransform()
{
    str sName = GetQualifiedName()->GetRootUnqualifiedNameStr();
    if (CAlterDollarName::IsDollarName(sName))
        ApplyTransform();
}

void CAlterDollarName::ApplyTransform()
{
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
            // maybe formal argument declaration?
            if not_null(
                pQualifiedName->
                    FindFirstParentOfKind(
                        new CDeclareFunctionArgs))
            {
                // declate function parameters
                TransformDollarParameters(
                    pParentExpression,
                    iNameIndexInParent);
            }
            else
            {
                // executable expression
                TransformDollarExecutable(
                    pParentExpression,
                    iNameIndexInParent);
            }
        }
        else
        {
            GetTokenizer()->ReportInternalTransformError(
                errn(0xCA50),
                "can't find $dollar name index in its expression ('" +
                    pQualifiedName->GetContentPreviewFriendlyLine() + "')",
                pParentExpression);
        }
    }
    else
    {
        // it's OK, skip x-propery declaratons
        /*
        GetTokenizer()->ReportTransformClientError(
            errn(0xCA51),
            "cannot transform x-Dollar in this context, "
                "which is not an expression",
            pQualifiedName);
            */
    }
}

void CAlterDollarName::TransformDollarExecutable(
        CExpression* pParentExpression,
        int iNameIndexInParent)
{
    CQualifiedName* pQualifiedName = GetQualifiedName();
    CUnqualifiedName* pRootUnqualifiedName = pQualifiedName->GetRootUnqualifiedName();
    str sRootName = pRootUnqualifiedName->GetNameStr();

    // $name           ==> cvL.Get(GCv("$name"))
    // $name op= e     ==> cvL.SetOp(GCv("$name"), e)
    // $name++         ==> cvL.Increment("$name")
    //
    // $name[e]        ==> cvL.XetMap("$name", e)
    // $name->e        ==> cvL.XetMap("$name", e)
    //
    // $name [e] ->foo [f] ->Fn(....
    //                   ==> Fn(GetMapArrowMap("$name", e, "foo", f), ....

    // $name [e] ->foo   ==> cvL.Get(GCv("name", (e), "foo"))
    // $name ->foo += e  ==> cvL.SetPlus(GCv("name", "foo"), (e))

    //                  [e]  ->foo    x= ...        ->Fn(....        <else>
    // transform into:  ,e   ,"foo"   
    //        prepend:                cvL.Set(GCv(  Fn(cvL.Get(GCv(  cvL.Get(GCv(
    // append at stop:                              ))  |  )),       ))
    //  append at end:                ))       

    // $name           ==> (*this)["$name"]
    // $name op= e     ==> (*this)["$name"] op= e
    // $name++         ==> (*this)["$name"]++
    //
    // $name[e]        ==> (*this)["$name"][e]
    // $name->e        ==> (*this)["$name"][e]

    // $name->foo(     ==> (*this)["$name"]->foo( 
    // $name [e] ->foo [f] ->Fn(....
    //                 ==> (*this)["$name"][e]["foo"]->Fn(....

    //                  [e]  ->foo / except: foo(...
    // transform into:  [e]  [foo]

    //
    // transform $name into CV["$name"]
    //
    {
        // create ["name"] statement
        CToken* pInsertArrayed = 
            CreateArrayExpressionWithMagicStr(
                "CV", // sBefore
                sRootName,
                ""); // sAfter

        pParentExpression->x_aToken.
            SetAt(iNameIndexInParent, pInsertArrayed);
    }

    //
    // iterate all following tokens in parent expression
    //

    for (
        int iInParent = iNameIndexInParent + 1;
        iInParent < pParentExpression->GetTokenCount();
        iInParent++)
    {
        // next token
        nullable<CToken*> npIter =
            pParentExpression->x_aToken.GetAtOrNull(iInParent + 0);
        nullable<CToken*> npIter1 =
            pParentExpression->x_aToken.GetAtOrNull(iInParent + 1);
        nullable<CToken*> npIter2 =
            pParentExpression->x_aToken.GetAtOrNull(iInParent + 2);

        // $name[index]?
        if not_null(
            //CAttributes* pArray = 
                derive(CAttributes, npIter))
        {
            // leave as it is, C++ wrapper overloads operator []
        }
        else
        if not_null(
            //CCharArrow* pArrow = 
                derive(CCharArrow, npIter))
        {
            // ID must follow
            if not_null(
                CQualifiedName* pMember = derive(CQualifiedName, npIter1))
            {
                // but not a method call?
                if not_null(
                    //CCharLeftParen* pLeftParen = 
                        derive(CCharLeftParen, npIter2))
                {
                    // leave method call as it is: C++ translates it into wrapper method call
                    break;
                }
                else
                {
                    // transform  -> name  into  ["name"]
                    str sMember = pMember->GetFullQualifiedNameAsStr();

                    // create ["name"] statement
                    CToken* pInsertArrayed = 
                        CreateArrayExpressionWithMagicStr(
                            "", // sBefore
                            sMember,
                            ""); // sAfter

                    // erase '->'
                    pParentExpression->x_aToken.
                        SetAt(iInParent + 0, new CInsertCode);

                    // transform ->name to ["name"]
                    pParentExpression->x_aToken.
                        SetAt(iInParent + 1, pInsertArrayed);

                    // we've eaten two tokens: -> name
                    iInParent++;
                }
            }
        }
        else
        {
            // anything else ends transformation 
            break;
        }
    }
}

void CAlterDollarName::TransformDollarParameters(
        CExpression* pParentExpression,
        int iNameIndexInParent)
{
    CQualifiedName* pQualifiedName = GetQualifiedName();
    CUnqualifiedName* pRootUnqualifiedName = pQualifiedName->GetRootUnqualifiedName();
    str sRootName = pRootUnqualifiedName->GetNameStr();

    sRootName = sRootName.GetTruncateLeft(1);

    // transform $name parameter into cv name
    CInsertCode* pInsert = new CInsertCode;
    pInsert->SetSourceFragmentIfNeededFrom(GetQualifiedName());
    pInsert->x_sTextFirst = "cv " + sRootName;

    // save original name for binding transformer
    pInsert->x_sSaveOriginalName = 
        sRootName;

    // replace in argument expression
    pParentExpression->x_aToken.
        SetAt(iNameIndexInParent, pInsert);

    // bind parameters to local variable map
    // --- this is done in "cv cvL;" insertion
}

[...]