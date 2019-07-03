// CToken_UnusualSyntaxOperators.h
// (see "CToken.h")
// (see "ParseUnusualSyntax.cpp")

// CUsingOp
class CUsingOp : public CStatement
{
public:
    GENERIC(CUsingOp, CStatement);
    CUsingOp();

// Content

    array<CToken*> x_aToken;
    void AddToken(CToken* pToken);
    int GetTokenCount();
    CToken* GetToken(int iToken);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation()
        { return "using"; }
    virtual bool OnIsSemicolonAtEnd();

private:
};

// COperatorSpec
class COperatorSpec : public CUnqualifiedName
{
public:
    GENERIC(COperatorSpec, CUnqualifiedName);
    COperatorSpec();

// Content

    array<CToken*> x_aToken;
    void AddToken(CToken* pToken);
    int GetTokenCount();
    CToken* GetToken(int iToken);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation()
        { return "operator"; }

private:
};

// CAsmStandard
class CAsmStandard : public CToken
{
public:
    GENERIC(CAsmStandard, CToken);
    //CAsmStandard();


[...]