// CToken_LabelsAndAccess.h
// (see "CToken.h")
// (see "ParseExpressions.cpp")

// Class member access mode statement
class CAccessStatement : public CStatement
{
public:
    GENERIC(CAccessStatement, CStatement);
    CAccessStatement();

// Content

    void SetAccessLabel(CAccessKeyword* pAccessLabel);
    CAccessKeyword* GetAccessLabel();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsSemicolonAtEnd();

private:
    CAccessKeyword* m_pAccessLabel;
};

// Class member access mode keyword
//  (used as an access label statement or as a declaration modifier)
class CAccessKeyword : public CToken
{
public:
    GENERIC(CAccessKeyword, CToken);

protected:
    virtual CToken* OnTryExtend();
    //virtual void OnTransform() {}
    //virtual void OnRegenerate(CWriter* pWriter);

private:
};

    class CPublicAccess : public CAccessKeyword
    {
    public:
        GENERIC(CPublicAccess, CAccessKeyword);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "public"; }
    };

    class CProtectedAccess : public CAccessKeyword
    {
    public:
        GENERIC(CProtectedAccess, CAccessKeyword);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "protected"; }
    };

    class CPrivateAccess : public CAccessKeyword
    {
    public:
        GENERIC(CPrivateAccess, CAccessKeyword);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "private"; }
    };

// Switch operator labels
class CSwitchBranchLabel : public CToken
{
public:
    GENERIC_ABSTRACT(CSwitchBranchLabel, CToken);

protected:
    //virtual CToken* OnTryExtend(CTokenizer*) { return this; }
    //virtual void OnTransform();

private:
};

    class CCaseLabel : public CSwitchBranchLabel
    {
    public:
        GENERIC(CCaseLabel, CSwitchBranchLabel);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "case"; }
    };

    class CDefaultLabel : public CSwitchBranchLabel
    {
    public:
        GENERIC(CDefaultLabel, CSwitchBranchLabel);

    protected:
        //virtual void OnTransform() {}
        virtual const char* OnGetDefaultTextRepresentation()
            { return "default"; }
    };

