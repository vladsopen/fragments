// CTokenizer.cpp
// (see "CTokenizer.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
//#include "CTokenizer.h"
#include "CSourceInput.h"
#include "CSourceLines.h"
#include "CKeywordMapping.h"
#include "CWriter.h"
#include "CTransformContext.h"
#include "CTokenizerMacroMap.h"
#include "CNameHookMap.h"
//#include "CWriteRegeneratedText.h"

CTokenizer::EStage CTokenizer::s_eStage = E_Stage_Parsing;

const charmap CTokenizer::C_charmapWhiteSpaceWithoutNewLine(
    CHARMAP("[\1-\x09][\x0b-\x20]"));
    CASSERT('\n' == 0x0a);

const charmap CTokenizer::C_charmapStartOfWord(
    CHARMAP("[a-z][A-Z]_$"));

const charmap CTokenizer::C_charmapMiddleOfWord(
    CHARMAP("[a-z][A-Z]_$[0-9]"));

const charmap CTokenizer::C_charmapDigit(
    CHARMAP("[0-9]"));

const charmap CTokenizer::C_charmapInsideNumber(
    CHARMAP("[0-9][a-z][A-Z].+-"));

const charmap CTokenizer::C_charmapAllowedAscii(
    CHARMAP("[\x01-~]"));

const charmap CTokenizer::C_charmapAnyEnglishLetter(
    CHARMAP("[A-Z][a-z]"));

CTokenizer::CTokenizer(CProcessing* pProcessing)
{
    m_pProcessing = pProcessing;
    s_eStage = E_Stage_Parsing;

    m_pSourceInput = new CSourceInput;
    m_pSourceLines = new CSourceLines;

    m_pKeywordMapping = new CKeywordMapping;
    m_pNameHookMap = new CNameHookMap;
    m_pInternalPrefixHookMap = new CNameHookMap;

    m_npNextToken;
    m_bNextTokenExtended = false;
    m_iCurrentLineInfo = 0;
    m_bPreprocessorMode = false;

    m_mapTemplateNames;

    m_bTokenLookupInitialized = false;

    m_pTransformContext = new CTransformContext;

    m_nReturnErrorCode = 0;
    m_bErrorsAsWarnings = false;
    m_bEnableMessages = true;

    m_nCurrentLineIndent = 0;
    m_bAtStartOfLineIndent = true;

    m_pTokenizerMacroMap = new CTokenizerMacroMap;
}

void CTokenizer::EnsureTokenLookupInitialized()
{
    // Initialize only once - after tokens registered
    if (m_bTokenLookupInitialized)
        return;
    m_bTokenLookupInitialized = true;

    // First clear table for verification
    repeat(iChar, C_nTokenLookupTableSize)
        m_unsafe_aTokenLookup[iChar] = OnCharUninitialized;

    // Define other chars
    DefineSingleCharLookup('\0',  OnEof);
    //CASSERT(C_nEof == 0);
    DefineCharRangeLookup('\1',
                          '\x20', OnCharWhiteSpace);
    ReDefineSingleCharLookup(
                          '\n',   OnCharNewLine);
    DefineSingleCharLookup('!',   OnCharExclamation);
    DefineSingleCharLookup('"',   OnCharDoubleQuote);
    DefineSingleCharLookup('#',   OnCharSharp);
    DefineSingleCharLookup('$',   OnCharLetter); //OnCharDollar);
    DefineSingleCharLookup('%',   OnCharPercent);
    DefineSingleCharLookup('&',   OnCharAnd);
    DefineSingleCharLookup('\'',  OnCharSingleQuote);
    DefineSingleCharLookup('(',   OnCharLeftParen);
    DefineSingleCharLookup(')',   OnCharRightParen);
    DefineSingleCharLookup('*',   OnCharStar);
    DefineSingleCharLookup('+',   OnCharPlus);
    DefineSingleCharLookup(',',   OnCharComma);
    DefineSingleCharLookup('-',   OnCharMinus);
    DefineSingleCharLookup('.',   OnCharDot);
    DefineSingleCharLookup('/',   OnCharDivide);
    DefineCharRangeLookup( '0',
                           '9',   OnCharDigit);
    DefineSingleCharLookup(':',   OnCharColon);
    DefineSingleCharLookup(';',   OnCharSemicolon);
    DefineSingleCharLookup('<',   OnCharLess);
    DefineSingleCharLookup('=',   OnCharAssign);
    DefineSingleCharLookup('>',   OnCharGreater);
    DefineSingleCharLookup('?',   OnCharQuestion);
    DefineSingleCharLookup('@',   OnCharAt);
    DefineCharRangeLookup( 'A',
                           'Z',   OnCharLetter);
    DefineSingleCharLookup('[',   OnCharLeftBracket);
    DefineSingleCharLookup('\\',  OnCharBackslash);
    DefineSingleCharLookup(']',   OnCharRightBracket);
    DefineSingleCharLookup('^',   OnCharXor);
    DefineSingleCharLookup('~',   OnCharTilde);
    DefineCharRangeLookup((unsigned char) '\x7F',
                          (unsigned char) '\xFF', OnCharExtendedAscii);
    //DefineSingleCharLookup(C_nEof, OnEof);

    // Verify all table entries
    repeat(iChar, C_nTokenLookupTableSize)
    {
        ASSERT(m_unsafe_aTokenLookup[iChar] != OnCharUninitialized);

        // Verify character classes defintition synchronization
        //chardefsync;

        ASSERT(C_charmapWhiteSpaceWithoutNewLine[iChar]
            == (
                m_unsafe_aTokenLookup[iChar] == OnCharWhiteSpace)
            ||
                iChar == 0);

        ASSERT(C_charmapStartOfWord[iChar]
            == (
                m_unsafe_aTokenLookup[iChar] == OnCharLetter));
             //|| m_unsafe_aTokenLookup[iChar] == OnCharUnderline
             //|| m_unsafe_aTokenLookup[iChar] == OnCharDollar)

        ASSERT(C_charmapMiddleOfWord[iChar]
            == (
                m_unsafe_aTokenLookup[iChar] == OnCharLetter
             || m_unsafe_aTokenLookup[iChar] == OnCharDigit)
             //|| m_unsafe_aTokenLookup[iChar] == OnCharUnderline
             //|| m_unsafe_aTokenLookup[iChar] == OnCharDollar)
             );

        ASSERT(C_charmapDigit[iChar]
            == (
                m_unsafe_aTokenLookup[iChar] == OnCharDigit));

        ASSERT(C_charmapInsideNumber[iChar]
            == (
                m_unsafe_aTokenLookup[iChar] == OnCharLetter ||
                m_unsafe_aTokenLookup[iChar] == OnCharDigit ||
                m_unsafe_aTokenLookup[iChar] == OnCharDot ||
                m_unsafe_aTokenLookup[iChar] == OnCharPlus ||
                m_unsafe_aTokenLookup[iChar] == OnCharMinus)
            || iChar == '$'
            || iChar == '_');
    }
}

void CTokenizer::ReturnErrorCode(int nExitCode)
{
    m_nReturnErrorCode = nExitCode;
}

int CTokenizer::GetReturnedErrorCode()
{
    return m_nReturnErrorCode;
}

void CTokenizer::SetErrorsAsWarnings(bool bYes)
{
    m_bErrorsAsWarnings = bYes;
}

bool CTokenizer::IsErrorsAsWarnings()
{
    return m_bErrorsAsWarnings;
}

CTransformContext* CTokenizer::GetLinkedTransformContext()
{
    return m_pTransformContext;
}

void CTokenizer::SetStage(EStage eStage)
{
    if (eStage == E_Stage_Parsing)
    {
        FAIL("E_Stage_Parsing cannot be set explicitly");
    }
    else
    if (eStage == E_Stage_Transforming)
    {
        ASSERT(s_eStage == E_Stage_Parsing);
        //ASSERT(iskindof(CEof, GetContextToken()));
        s_eStage = eStage;
    }
    else
    if (eStage == E_Stage_Writing)
    {
        ASSERT(s_eStage == E_Stage_Transforming);
        s_eStage = eStage;
    }
    else
        FAIL("bad stage");
}

CTokenizer::EStage CTokenizer::GetStage()
{
    return s_eStage;
}

void CTokenizer::SetSourceInfo(
        str sSourceText,
        path pathInitialSourceFile)
{
    ASSERT(GetStage() == E_Stage_Parsing);

    m_pathInitialSourceFile = pathInitialSourceFile;

    // Init keywords
    m_pKeywordMapping->InitCppKeywords();
    InitTransformers();

    // Init low level macros
    m_pTokenizerMacroMap->
        InitTokenizerMacroMap();

    // Only used once
    m_pSourceInput->SetSourceText(sSourceText);

    // Init lines
    m_pSourceLines->PrepareToSourceText(
        sSourceText.Len(),
        pathInitialSourceFile);

    // Init TransformContext
    GetLinkedTransformContext()->HandleBeforeStartProcessingSource();
}

path CTokenizer::GetInitialSourceFile()
{
    ASSERT(m_pathInitialSourceFile.Len());
    return m_pathInitialSourceFile;
}

CToken* CTokenizer::PeekNextToken(CToken* pParentToken)
{
    ASSERT(GetStage() == E_Stage_Parsing);

    // Already have an extended lookahead token?
    if (m_bNextTokenExtended)
        if ok(!m_npNextToken.IsNull())
            return m_npNextToken.GetValidPointer();

    CToken* pNextToken;
    if (m_npNextToken.IsNull())
    {
        // Extract a raw token which can start an extended sequence
        pNextToken = ExtractNextToken(pParentToken);
    }
    else
    {
        // Use cached token extracted by PeekNextToken()
        pNextToken = m_npNextToken.GetValidPointer();
    }

    // Force reading chars ahead
    m_npNextToken.MakeNull();
    m_bNextTokenExtended = false;

    // Extend the token (this may start recursion!)
    pNextToken = pNextToken->TryExtend(pParentToken);

    // A lookahead token left dangling?
    if (!m_npNextToken.IsNull())
    {
        // Reject lookahead token
        RollbackToToken(m_npNextToken.GetValidPointer());
    }

    // Keep the next token pointer at the 'peeking' position
    //  (during the recursive calls it could shift forward and point to the last token
    //   in the extended sequence; leaving it there is wrong from the original caller
    //   standpoint)
    m_npNextToken = pNextToken;
    m_bNextTokenExtended = true;

    // Skip generated white space (from auto-removed tokens)
    if not_null(derive(CWhiteSpace, pNextToken))
    {
        // Skip this one
        ReadNextToken(pParentToken);

        // return the next
        return PeekNextToken(pParentToken);
    }

    // Return extended
    return pNextToken;
}

void CTokenizer::RollbackToToken(CToken* pAlreadyReadToken)
{
    str sRollbackError;
    ImplCurrentTokenChange(
        pAlreadyReadToken,
        out sRollbackError);

    if (sRollbackError.Len() > 0)
    {
        ReportInternalTransformError(
            errn(0xFFC2),
            sRollbackError,
            pAlreadyReadToken);

[...]