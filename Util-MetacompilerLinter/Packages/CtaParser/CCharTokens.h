// CCharTokens.h
// (see "CToken.h")
// (see "CCharTokens.cpp")

//////////////////////////////////////////////////////////////////////////////////////
// Low-level terminal token classes

class CCharToken : public CToken
{
public:
    GENERIC_ABSTRACT(CCharToken, CToken);
    CCharToken();
};

    // = += *= >>= ...
    class CCharAssignmentToken : public CCharToken
    {
    public:
        GENERIC_ABSTRACT(CCharAssignmentToken, CCharToken);
        CCharAssignmentToken();

        // Returns simple opeation token implemented by this assignment
        CCharToken* GetOperationToken();

    protected:
        virtual CCharToken* OnGetOperationToken() = 0;
    };

#define DECLARE_CHAR_TOKEN_PLUS(CCharXxxx, text, CParent, more) \
    class CCharXxxx : public CParent \
    { \
    public: \
        GENERIC(CCharXxxx, CParent); \
    \
        more \
    \
    protected: \
        virtual CToken* OnTryExtend(); \
        /* virtual void OnTransform() {} */ \
        virtual const char* OnGetDefaultTextRepresentation() \
            { return text; } \
    }; \

#define DECLARE_COMPLEX_ASSIGN(CCharXxxx, text, COp) \
    class CCharXxxx : public CCharAssignmentToken \
    { \
    public: \
        GENERIC(CCharXxxx, CCharAssignmentToken); \
    \
    protected: \
        virtual CToken* OnTryExtend(); \
        virtual CCharToken* OnGetOperationToken() \
            { return new COp; } \
        /* virtual void OnTransform() {} */ \
        virtual const char* OnGetDefaultTextRepresentation() \
            { return text; } \
    }; \

#define DECLARE_CHAR_TOKEN(CCharXxxx, text) \
    DECLARE_CHAR_TOKEN_PLUS(CCharXxxx, text, CCharToken, ;)

#define NOTFIXED super::OnGetDefaultTextRepresentation()

// Multichar tokens
DECLARE_CHAR_TOKEN(CCharNumber,         NOTFIXED)  // 0123456789, 12.4e+038, 0xabcd, 2i64
DECLARE_CHAR_TOKEN(CCharWord,           NOTFIXED)  // 'AZaz_09$'
DECLARE_CHAR_TOKEN(CCharUninitialized,  NOTFIXED)  // initial map filling
DECLARE_CHAR_TOKEN(CWhiteSpace,         NOTFIXED)  // ' ', control, comments
DECLARE_CHAR_TOKEN(CCharExtendedAscii,  NOTFIXED)  // > 128
DECLARE_CHAR_TOKEN(CEof,                NOTFIXED)  // after the source end

// '\n' token as a kind of WhiteSpace (used for __asm statement parsing)
class CCharNewLine : public CWhiteSpace
{
public:
    GENERIC(CCharNewLine, CWhiteSpace);
};

// Punctuation
DECLARE_CHAR_TOKEN(CCharExclamation,    "!")
//DECLARE_CHAR_TOKEN(CCharDoubleQuote,  "\"" (see CCharDoubleQuotedString)
//DECLARE_CHAR_TOKEN(CCharSharp,        "#" (preprocessor)
//DECLARE_CHAR_TOKEN(CCharDollar,         "$")
DECLARE_CHAR_TOKEN(CCharPercent,        "%")
DECLARE_CHAR_TOKEN(CCharAnd,            "&")
DECLARE_CHAR_TOKEN(CCharLeftParen,      "(")
DECLARE_CHAR_TOKEN(CCharRightParen,     ")")
DECLARE_CHAR_TOKEN(CCharStar,           "*")
DECLARE_CHAR_TOKEN(CCharPlus,           "+")
DECLARE_CHAR_TOKEN(CCharComma,          ",")
DECLARE_CHAR_TOKEN(CCharMinus,          "-")
DECLARE_CHAR_TOKEN(CCharDot,            ".")
DECLARE_CHAR_TOKEN(CCharDivide,         "/")
DECLARE_CHAR_TOKEN(CCharColon,          ":")
DECLARE_CHAR_TOKEN(CCharSemicolon,      ";")
DECLARE_CHAR_TOKEN(CCharLess,           "<")
DECLARE_COMPLEX_ASSIGN(CCharAssign,     "=", CCharAssign)
DECLARE_CHAR_TOKEN(CCharGreater,        ">")
DECLARE_CHAR_TOKEN(CCharQuestion,       "?")
DECLARE_CHAR_TOKEN(CCharAt,             "@")
DECLARE_CHAR_TOKEN(CCharLeftBracket,    "[")
DECLARE_CHAR_TOKEN(CCharBackslash,      "\\")
DECLARE_CHAR_TOKEN(CCharRightBracket,   "]")
DECLARE_CHAR_TOKEN(CCharXor,            "^")
//DECLARE_CHAR_TOKEN(CCharUnderline,      "_")
DECLARE_CHAR_TOKEN(CCharBackQuote,      "`")
DECLARE_CHAR_TOKEN(CCharLeftBrace,      "{")
DECLARE_CHAR_TOKEN(CCharOr,             "|")
DECLARE_CHAR_TOKEN(CCharRightBrace,     "}")
DECLARE_CHAR_TOKEN(CCharTilde,          "~")

DECLARE_CHAR_TOKEN(CCharSingleQuotedString, NOTFIXED);

// see also derived CInsertDoubleQuotedStringCode
//DECLARE_CHAR_TOKEN(CCharDoubleQuotedString, NOTFIXED);
DECLARE_CHAR_TOKEN_PLUS(CCharDoubleQuotedString, NOTFIXED, CCharToken, 
    str _xx_bConvertWin1251ToUtf8;
    virtual void OnRegenerate(CWriter* pWriter);
    )

// Composite Punctuation
DECLARE_CHAR_TOKEN(CCharEllipsis,                    "...")
DECLARE_CHAR_TOKEN(CCharColonColonQualifier,         "::")
DECLARE_CHAR_TOKEN(CCharArrow,                       "->")
DECLARE_CHAR_TOKEN(CCharDotStar,                     ".*")
DECLARE_CHAR_TOKEN(CCharArrowStar,                   "->*")
DECLARE_CHAR_TOKEN(CCharPlusPlus,                    "++")
DECLARE_CHAR_TOKEN(CCharMinusMinus,                  "--")
DECLARE_CHAR_TOKEN(CCharShiftLeft,                   "<<")
DECLARE_CHAR_TOKEN(CCharShiftRight,                  ">>")
DECLARE_CHAR_TOKEN(CCharLessOrEqual,                 "<=")
DECLARE_CHAR_TOKEN(CCharGreaterOrEqual,              ">=")
DECLARE_CHAR_TOKEN(CCharEqualEqual,                  "==")
DECLARE_CHAR_TOKEN(CCharNotEqual,                    "!=")
DECLARE_CHAR_TOKEN(CCharAndAnd,                      "&&")
DECLARE_CHAR_TOKEN(CCharOrOr,                        "||")
DECLARE_COMPLEX_ASSIGN(CCharStarAssign,              "*=",  CCharStar)
DECLARE_COMPLEX_ASSIGN(CCharDivideAssign,            "/=",  CCharDivide)
DECLARE_COMPLEX_ASSIGN(CCharPercentAssign,           "%=",  CCharPercent)
DECLARE_COMPLEX_ASSIGN(CCharPlusAssign,              "+=",  CCharPlus)
DECLARE_COMPLEX_ASSIGN(CCharMinusAssign,             "-=",  CCharMinus)
DECLARE_COMPLEX_ASSIGN(CCharShiftLeftAssign,         "<<=", CCharShiftLeft)
DECLARE_COMPLEX_ASSIGN(CCharShiftRightAssign,        ">>=", CCharShiftRight)
DECLARE_COMPLEX_ASSIGN(CCharAndAssign,               "&=",  CCharAnd)
DECLARE_COMPLEX_ASSIGN(CCharXorAssign,               "^=",  CCharXor)
DECLARE_COMPLEX_ASSIGN(CCharOrAssign,                "|=",  CCharOrAssign)

//DECLARE_CHAR_TOKEN(CCharPhpStart,                    "<?php")
    class CCharPhpStart : public CCharToken
    {
    public:
        GENERIC(CCharPhpStart, CCharToken);
   
    protected:
        virtual CToken* OnTryExtend();
        /* virtual void OnTransform() {} */
        virtual const char* OnGetDefaultTextRepresentation()
            { return "<?php"; }
        virtual void OnRegenerate(CWriter* pWriter);
    };

//DECLARE_CHAR_TOKEN(CCharPhpEnd,                      "?>")
    class CCharPhpEnd : public CCharToken
    {
    public:
        GENERIC(CCharPhpEnd, CCharToken);
   
    protected:
        virtual CToken* OnTryExtend();
        /* virtual void OnTransform() {} */
        virtual const char* OnGetDefaultTextRepresentation()
            { return "?>"; }
        virtual void OnRegenerate(CWriter* pWriter);
    };

#undef NOTFIXED
