// CAlterCtaIgnore.h
// (see "CAlterCtaIgnore.cpp")

// CAlterCtaIgnore - marks unusual code Cta should ignore

class CAlterCtaIgnore : public CToken
{
public:
    GENERIC_ABSTRACT(CAlterCtaIgnore, CToken);
    CAlterCtaIgnore();

// Operations

    // Global granted template file flush
    friend void GSaveCtaIgnoreReports();

protected:
    virtual CToken* OnTryExtend();
    //virtual void OnTransform() {}
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation() = 0;
    //virtual bool OnIsSemicolonEnabledAtExpressionEnd();
    //virtual bool OnIsAlwaysLastInExpressionList();
    virtual bool OnIsFileNameExpected() = 0;

private:

    // granted log support
    static void GLogCtaIgnoreSourceFile(
            path pathSource,
            CAlterCtaIgnore* pToken);

    // macro type
    bool IsFileNameExpected();
};

    // one-time ignore
    class CAlterCtaIgnoreOnce : public CAlterCtaIgnore
    {
    public:
        GENERIC(CAlterCtaIgnoreOnce, CAlterCtaIgnore);
        CAlterCtaIgnoreOnce();

    protected:
        virtual const char* OnGetDefaultTextRepresentation();
        virtual bool OnIsFileNameExpected();

    private:
    };

    // ignore in specific file
    class CAlterCtaIgnoreInFile : public CAlterCtaIgnore
    {
    public:
        GENERIC(CAlterCtaIgnoreInFile, CAlterCtaIgnore);
        CAlterCtaIgnoreInFile();

    protected:
        virtual const char* OnGetDefaultTextRepresentation();
        virtual bool OnIsFileNameExpected();

    private:
    };

