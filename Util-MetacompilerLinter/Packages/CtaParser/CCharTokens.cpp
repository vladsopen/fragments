// CCharTokens.cpp
// (see "CCharTokens.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "VirtOs_unicode.h"
#include "CWriter.h"

//#include "CAlterAssert.h"

CCharToken::CCharToken()
{
}

#define IMPLEMENT_CHAR_TOKEN(CCharXxxx) \
    CToken* CCharXxxx::OnTryExtend() \
    { \
        return CCharToken::OnTryExtend(); \
    } \

IMPLEMENT_CHAR_TOKEN(CCharNumber)

//IMPLEMENT_CHAR_TOKEN(CCharWord)

CCharAssignmentToken::CCharAssignmentToken()
{
}

CCharToken* CCharAssignmentToken::GetOperationToken()
{
    return OnGetOperationToken();
}

CToken* CCharWord::OnTryExtend()
{
    //sync:_bad_string_deny_

    /* OK but not needed now
    str sWord = GetSourceFragmentStr();
        // it will still be stred when checking for a keyword
    if (sWord.FindPos_fast("Inseted") != -1)
    {
        GetTokenizer()->ReportTransformClientError(
            errn(0xBA57),
            str() + 
                "spelling error in \"" + 
                    sWord + "\"",
            this);
    }
    */

    // Could be a name or a keyword

    // Check for a keyword
    nullable<CToken*> npKeywordToken = GetTokenizer()->FindKeywordKind(this);
    if (!npKeywordToken.IsNull())
    {
        if not_null(CToken* pNewKeywordToken =
            derive(CToken, npKeywordToken.GetValidPointer()->NewOfTheKind()))
        {
            return pNewKeywordToken;
        }
        else
        {
            FAIL("failed to create a keyword token");
        }
    }

    // Parse as a name,
    //   initiate possible qualifier specification parsing
    CUnqualifiedName* pUnqualifiedName = new CUnqualifiedName;
    //pUnqualifiedName->SetCharWord(this);

    return pUnqualifiedName;
}
// continued in "ParseQualifiedName.cpp"
//          and "ParseDeclKeywords.cpp"

IMPLEMENT_CHAR_TOKEN(CCharUninitialized)
IMPLEMENT_CHAR_TOKEN(CWhiteSpace)
IMPLEMENT_CHAR_TOKEN(CCharExtendedAscii)
IMPLEMENT_CHAR_TOKEN(CEof)

IMPLEMENT_CHAR_TOKEN(CCharExclamation)

//IMPLEMENT_CHAR_TOKEN(CCharSharp)

//IMPLEMENT_CHAR_TOKEN(CCharDollar)
IMPLEMENT_CHAR_TOKEN(CCharPercent)
IMPLEMENT_CHAR_TOKEN(CCharAnd)
IMPLEMENT_CHAR_TOKEN(CCharLeftParen)
IMPLEMENT_CHAR_TOKEN(CCharRightParen)
[...]