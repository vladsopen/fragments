// CToken.cpp
// (see "CToken.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CErrorSuppressor.h"
#include "CWriter.h"
#include "CSourceLines.h"
#include "CAddToToken.h"
#include "CTransformContext.h"
#include "CCtapContext.h"
#include "VirtOs_dir.h"
#include "VirtOs_CFileList.h"
#include "CMapProjects.h"

//////////////////////////////////////////////////////////////////////////////////////
// CToken

CToken::CToken()
{
    m_npParent;
#if defined(SUPPORT_NAME_SCOPE)
    m_npScope;
#endif

    m_szFragmentStart = "";
    m_sCacheSourceFragment;
    m_nFragmentLength = 0;
    m_iLineInfoNumber = C_nUndefinedLineNumber;
    m_nLineIndent = -1;

    m_fMisc = F_Misc_EnableRegeneration;

    if (CTokenizer::GetStage() > CTokenizer::E_Stage_Parsing)
        (int&) m_fMisc |= F_Misc_Inserted;
}

void CToken::SetSourceFragment(
        const char* szFragmentStart,
        int nFragmentLength,
        int iLineInfoNumber,
        int nLineIndent)
{
    ASSERT(szFragmentStart != NULL);
    ASSERT(nFragmentLength > 0 || iskindof(CEof, this));
    ASSERT((int) strlen(szFragmentStart) >= nFragmentLength);

    m_nLineIndent = nLineIndent;

    m_szFragmentStart = szFragmentStart;
    m_sCacheSourceFragment.EmptyAndPreserveMemory();
    m_nFragmentLength = nFragmentLength;

    SetLineInfoNumber(iLineInfoNumber);
}

void CToken::SetSourceFragmentToGenerated()
{
    m_szFragmentStart = "";
    // cannot rFAIL: "EmptyAndPreserveMemory cannot reuse shared memory "
    //               "when you have multiple references in str"
    m_sCacheSourceFragment = ""; //.EmptyAndPreserveMemory();
    //m_sCacheSourceFragment.EmptyAndPreserveMemory();
    m_nFragmentLength = 0;
    m_iLineInfoNumber = C_nUndefinedLineNumber;
}

bool CToken::GetValidOriginalSourceLineInfo(
        out path& out_pathFile,
        out int& out_iLine,
        str sAddInfoOnError)
{
    // Get header original source file 
    int iLineInfo = GetLineInfoNumber();
    if (iLineInfo == CToken::C_nUndefinedLineNumber)
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xBB87),
            "cannot get source line info " + sAddInfoOnError,
            this);
    }
    else
    {
        if not_null(
            CSourceLines::CLineInfo* pLineInfo = 
                GetTokenizer()->
                    GetSourceLines()->
                        GetLineInfo(iLineInfo))
        {
            out_pathFile = pLineInfo->GetOriginalSourceFile();
            out_iLine = pLineInfo->GetOriginalLineNumber();
            return true;
        }
        else
        {
            GetTokenizer()->ReportTransformClientError(
                errn(0xBB88),
                "invalid source line info " + Str(iLineInfo) + " " +
                    sAddInfoOnError,
                this);
        }
    }

    return false;
}

nullable<CDeclaration*> CToken::FindParentFunctionBodyDeclaration()
{
    // Scan up all parents
    nullable<CToken*> npParent = GetParentToken();
    while (!npParent.IsNull())
    {
        // Return the parent if it is of the kind
        if not_null(
            CFunctionBody* pFunctionBody = 
                derive(CFunctionBody, npParent))
        {
            return pFunctionBody->FindFirstParentDeclaration();
        }

        // Next parent
        npParent = npParent.GetValidPointer()->GetParentToken();
    }

    // the case for do/return extensions
    npParent = GetParentToken();
    bool bFunctionModifiersFound = false;
    while (!npParent.IsNull())
    {
        // the case for do/return extensions
        if not_null(
            CDeclaration* pDeclaration = 
                derive(CDeclaration, npParent))
        {
            if (pDeclaration->IsDeclarationForFunction() &&
                bFunctionModifiersFound)
            {
                return pDeclaration;
            }
        }

        if not_null(
            derive(CDeclareFunctionModifiers, npParent))
        {
            bFunctionModifiersFound = true;
        }

        // Next parent
        npParent = npParent.GetValidPointer()->GetParentToken();
    }

    // Return null pointer
    return nullable<CDeclaration*>();
}

[...]