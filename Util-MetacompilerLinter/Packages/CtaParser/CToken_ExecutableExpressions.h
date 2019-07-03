// CToken_ExecutableExpressions.h
// (see "CToken.h")
// (see "ParseExpressions.cpp")

//////////////////////////////////////////////////////////////////////////////////////
// CExecutable - a list of executable expressions in a function body

class CExecutable : public CStatement
{
public:
    GENERIC(CExecutable, CStatement);
    CExecutable();

// Content

    CExpressionList* x_pExpressionList;

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsSemicolonAtEnd();

private:
};

//////////////////////////////////////////////////////////////////////////////////////
// CExpression - a part of
//   CExecutable or another expression or initializer, etc

class CExpression : public CToken
{
public:
    GENERIC(CExpression, CToken);
    CExpression();

// Content

    // by default a ',' out of subexpression ends CExpression
    bool x_bForceAnyCommaContinueExpression;

    array<CToken*> x_aToken;
    void AddToken(CToken* pToken);
    int GetTokenCount();
    CToken* GetToken(int iToken);

    // Attach a new token properly
    void ReplaceToken(
            int iAt,
            CToken* pToken);

    bool x_bMagicStrModeEnabled;
    //bool x_bCtapPhpMode; moved to basic CToken
    //bool x_bMidZeroExpected;

// Parser Logic

    int IsPresent();
    bool IsSemicolonAtEndExpected();
    bool IsSemicolonAtEndAlreadyEmited();

    void SetEmitSemicolonFlag(
            bool b);
    void EmitOriginalSemicolon(CWriter* pWriter);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnSupportMagicStrGeneratorScanner(
            CMagicStrGenerator* pMagicStrGenerator,
            CWriter* pWriter);

private:
    bool m_bParseEndedAtSemicolon;
    bool m_bSemicolonEmited;

    void CheckValidArrow(
            CToken* pToken,
            out bool& out_bPrevIsArrow);

    void MaybeCheckForValidStatementStart();
    void DoCheckForValidStatementStart();

    void _init_AssignGlobalContextExpressionModes();
};

//////////////////////////////////////////////////////////////////////////////////////
// CExpressionList - comma separated expressions

class CExpressionList : public CToken
{
public:
    GENERIC(CExpressionList, CToken);
    CExpressionList();

// Content

    array<CExpression*> x_aExpression;
    void AddExpression(CExpression* pExpression);
    int GetExpressionCount();
    CExpression* GetExpression(int iExpression);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsEndOfSequence(
            CToken* pToken);

private:
};

//////////////////////////////////////////////////////////////////////////////////////
// CTemplateArgList - template statement sequence

class CTemplateArgList : public CExpressionList
{
public:
    GENERIC(CTemplateArgList, CExpressionList);
    CTemplateArgList();

protected:
    virtual CToken* OnTryExtend();
    virtual bool OnIsEndOfSequence(
            CToken* pToken);
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
};

//////////////////////////////////////////////////////////////////////////////////////
// CCompound - statement list in braces in function body

class CCompound : public CToken
{
public:
    GENERIC(CCompound, CToken);
    CCompound();

    bool x_bExecutableCompound;

    CStatementList* x_pStatementList;
    nullable<CCharRightBrace*> x_npClosingBrace;

    bool IsFollowedByElseOrWhile();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsSemicolonEnabledAtExpressionEnd();

private:
};

