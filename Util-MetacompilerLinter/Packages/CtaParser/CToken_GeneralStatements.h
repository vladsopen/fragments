// CToken_GeneralStatements.h
// (see "CToken.h")

// Declaration / Definition hint
enum FStatementCouldBe
{
    F_StatementCouldBe_Declaration  = 0x0001,
    F_StatementCouldBe_Expression   = 0x0002,
    F_StatementCouldBe_anything     = -1,
};

// CStatement - the base for CDeclaration or CExecutable
class CStatement : public CToken
{
public:
    GENERIC(CStatement, CToken);
    CStatement();

    void SetAttributes(nullable<CAttributes*> npAttributes);
    bool IsSemicolonAtEnd();

    CErrorSuppressor* GetErrorSuppressorWhenNeeded();

protected:
    virtual CToken* OnTryExtend();
    virtual bool OnIsEndOfStatement(CToken* pToken);
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsSemicolonAtEnd();

private:
    nullable<CAttributes*> m_npAttributes;
    nullable<CErrorSuppressor*> m_npErrorSuppressor; // only allocated when needed

    void VerifySuppressedErrorsReallyIssued();
};

// CStatementCategorizer - must extend into CDeclaration or CExecutable
class CStatementCategorizer : public CToken
{
public:
    GENERIC(CStatementCategorizer, CToken);
    CStatementCategorizer();

    // Logic selection
    void StatementCouldBeDeclaration(bool bYes);
    void StatementCouldBeExpression(bool bYes);
    void SetStatementCouldBe(FStatementCouldBe fStatementCouldBe);
    bool IsStatementCouldBeDeclaration();
    bool IsStatementCouldBeExpression();

protected:
    virtual CToken* OnTryExtend();
    //virtual void OnTransform() {}

private:
    FStatementCouldBe m_fStatementCouldBe;

    // Decides statement kind by first few tokens
    bool IsReadingAheadLooksLikeDeclaration();
};

// CStatementList - generic statement sequence
class CStatementList : public CToken
{
public:
    GENERIC(CStatementList, CToken);
    CStatementList();

// Content

    array<CStatement*> x_aStatement;
    void AddStatement(CStatement* pStatement);
    void PrependStatement(CStatement* pStatement);
    void InsertStatementAt(int iAt, CStatement* pStatement);
    int GetStatementCount();
    CStatement* GetStatement(int iStatement);

    nullable<CTemplateArgList*> FindTemplateParametersInStatementAt(
            int iStatement,
            out str& out_sTemplateId);

    enum FStatementListModes
    {
        F_StatementList_DisableCoverageHook = 0x0001,
    };
    int x_fStatementListModes;

// Parser

    // Logic selection
    void StatementCouldBeDeclaration(bool bYes);
    void StatementCouldBeExpression(bool bYes);
    void SetStatementCouldBe(FStatementCouldBe fStatementCouldBe);
    bool IsStatementCouldBeDeclaration();
    bool IsStatementCouldBeExpression();

    // Separator
    void SetSeparatorTokenKind(CToken* pSeparatorToken);
    CToken* GetSeparatorTokenKind();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsEndOfSequence(
            CToken* pToken);

private:
    CToken* m_pSeparatorTokenKind; //perf: bool m_bCommaSeparated
    FStatementCouldBe m_fStatementCouldBe;

    void CheckForValidStatementStart(
            CToken* pFirst);
};

// CCompilationUnit - source file root scope
class CCompilationUnit : public CToken
{
public:
    GENERIC(CCompilationUnit, CToken);
    CCompilationUnit();

[...]