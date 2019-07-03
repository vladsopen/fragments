// CCtapTransformer.cpp
// (see "CCtapTransformer.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
//#include "CWriter.h"
//#include "CWriteRegeneratedText.h"
//#include "CSourceLines.h"
#include "CTransformContext.h"
#include "CCtapTransformer.h"
#include "CWriter.h"
#include "CMagicStrGenerator.h"
#include "CAlterDollarName.h"
#include "CParsedArgument.h"
//#include "CAlterGenericClass.h"

CCtapTransformer::CCtapTransformer()
{
}

void CCtapTransformer::TransformPhpToken(
        CToken* pToken)
{
    if not_null(
        CExpression* pExpression =
            derive(CExpression, pToken))
    {
        TransformPhpExpression(
            pExpression);
    }
}

void CCtapTransformer::TransformPhpExpression(
        CExpression* pExpression)
{
    for (
        iter iToken; 
        iToken.Up(pExpression->x_aToken.GetCount());) // token count may grow
    {
        nullable<CToken*> npIterToken = pExpression->x_aToken.GetAtOrNull(iToken);

        // replace '.' ---> ' +str()+ '
        if not_null(
            //CCharDot* pParentExpression =
                derive(CCharDot, npIterToken))
        {
            CInsertCode* pInsert = new CInsertCode;
            pInsert->SetSourceFragmentIfNeededFrom(pExpression);

            pInsert->x_sTextFirst = " +str()+ ";

            pExpression->x_aToken.SetAt(iToken, pInsert);
        }

        // NOTE: #define null null()
    }
}

bool CCtapTransformer::IsCtapFunction(
        CFunctionBody* pFunctionBody)
{
    // PHP: "void" function still need to return a null cv()

    if (!pFunctionBody->xx_bCtapPhpMode)
    {
        return false;
    }

    // get function header
    if not_null(
        CDeclaration* pHeader = 
            pFunctionBody->
                FindParentFunctionHeaderDeclaration())
    {
        // get return type
        if not_null(
            CQualifiedName* pReturnTypeName = 
                pHeader->x_pDeclareTypeName->x_npQualifiedName)
        {
            // is this a PHP function?
            if (pReturnTypeName->GetRootUnqualifiedNameStr() == "cv")
            {
                return true;
            }
        }
    }

    return false;
}

void CCtapTransformer::GenerateCtapFunctionProlog(
        CFunctionBody* pFunctionBody,
        CWriter* pWriter)
{
    pWriter->WriteCosmeticNewLine();

    pWriter->WriteTokenZeroTerminatedString(
        pFunctionBody,
        "  /* PHP: */ cv cvL; ",
        E_LineNumbering_SuppressSync);

    pWriter->WriteCosmeticNewLine();

    // get formal paramenters from parent function declaration
    if not_null(
        CDeclaration* pFunctionDeclaration = 
            derive(CDeclaration, pFunctionBody->GetParentToken()))
    {
        if not_null(
            CDeclareFunctionArgs* pDeclareFunctionArgs = 
                pFunctionDeclaration->LookupFunctionArgs())
        {
            array<CParsedArgument*> apParsedArgument;
            pDeclareFunctionArgs->
                ParseFunctionArgs(
                    out apParsedArgument);

            repeat(iArg, apParsedArgument.GetCount())
            {
                CParsedArgument* pParsedArgument = 
                    apParsedArgument.GetAt(iArg);

                str sDeclaredName = 
                    pParsedArgument->xx_sDeclaredName;
                str sPhpClassName = 
                    pParsedArgument->xx_sDeclaredTypename;

                //
                // Write Binding
                //

                str sNameMagic;
                {
                    CMagicStrGenerator* pMagicStrGenerator = new CMagicStrGenerator;
                    sNameMagic = 
                        pMagicStrGenerator->
                            GenerateMagicStrFromString(
                                sDeclaredName,
                                nullable<CExpression*>(),
                                nullable<CExpression*>(),
                                pDeclareFunctionArgs); // npTokenForErrorContext
                }

                str sTypenameMagic;
                {
                    CMagicStrGenerator* pMagicStrGenerator = new CMagicStrGenerator;
                    sTypenameMagic = 
                        pMagicStrGenerator->
                            GenerateMagicStrFromString(
                                sPhpClassName,
                                nullable<CExpression*>(),
                                nullable<CExpression*>(),
                                pDeclareFunctionArgs); // npTokenForErrorContext
                }

                pWriter->WriteTokenStr(
                    pFunctionBody,
                    "  PHP_DECLARE_ARG(" + 
                        sDeclaredName +
                        ", " +
                        sNameMagic + 
                        ", " +
                        sTypenameMagic + 
                        "); ",
                    E_LineNumbering_SuppressSync);

                pWriter->WriteCosmeticNewLine();

[...]