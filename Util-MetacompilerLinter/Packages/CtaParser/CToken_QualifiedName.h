// CToken_QualifiedName.h
// (see "ParseQualifiedName.cpp")
// (see "CToken.h")

//////////////////////////////////////////////////////////////////////////////////////
//
//   a < ... > :: b < ... > :: c < ... >
//
//  a      - CharWord "a"  \  UnqualifiedName  \
//  <...>  - TemplateNameArgs  /  'a' '<...>'   >  Qualifier 'a<...>' '::'
//  ::                                         /
//  b      - CharWord "b"  \  UnqualifiedName  \
//  <...>  - TemplateNameArgs  /  'b' '<...>'   >  Qualifier 'b<...>' '::'
//  ::                                         /
//  c      - CharWord "a"  \  UnqualifiedName  \
//  <...>  - TemplateNameArgs  /  'a' '<...>'   >  Qualifier 'a<...>' '::'
//                                             /

class CUnqualifiedName : public CToken
{
public:
    GENERIC(CUnqualifiedName, CToken);
    CUnqualifiedName();

// Constants

    #define C_szCtaHidePrefix "__cta_hide"

// Attribures

    // Name string
    void SetNameStr(str sName);
    str GetNameStr();

    // Explicitly prefixed template name
    void SetExplicitTemplatePrefix(bool bYes);
    bool IsExplicitTemplatePrefix();

    nullable<CTemplateArgList*> x_npTemplateArgList;

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual str OnGetSourceFragmentStr();

private:
    bool m_bExplicitTemplatePrefix;
    str m_sName;

private:
    // try to hide (use GetNameStr()!):
    str GetSourceFragmentStr(); // no implementation
    const char* GetUnterminatedSourceFragmentPointer(); // no implementation
};

class CQualifier : public CToken
{
public:
    GENERIC(CQualifier, CToken);
    CQualifier();

    void SetUnqualifiedName(CUnqualifiedName* pUnqualifiedName);

    CUnqualifiedName* GetUnqualifiedName();

protected:
    //virtual CToken* OnTryExtend(); -- parsed by enclosing CQualifiedName
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
    CUnqualifiedName* m_pUnqualifiedName;
};

//////////////////////////////////////////////////////////////////////////////////////
// CQualifiedName - fully qualified name spec

class CQualifiedName : public CToken
{
public:
    GENERIC(CQualifiedName, CToken);
    CQualifiedName();

// Content

    // Root name
    void SetRootUnqualifiedName(CUnqualifiedName* pRootUnqualifiedName);
    CUnqualifiedName* GetRootUnqualifiedName();

    // Global spec
    void SetGlobalOrigin(bool bGlobalNamespace);
    bool IsGlobalOrigin();

    // Qulifier list
private: // (special Add())
    array<CQualifier*> x_aQualifier;
public:
    void AddQualifier(CQualifier* pQualifier);
    CQualifier* GetQualifier(int iQualifier);
    int GetQualifierCount();

    // Constructor/Destructor cases
    bool IsConstructorName();
    bool IsDestructorName();
    void SetDestructorName();

    // Pointer to member declaration
    void SetPointerToMember(bool bYes);
    bool IsPointerToMember();

    // Report content as string
    str GetFullQualifiedNameAsStr();
    str GetQualifierChainAsStr();
    str GetRootUnqualifiedNameStr();

// Parsing

    // Temporary mode to recognize next template name
    bool QueryAndClearPendingExplicitTemplatePrefixedName();

// Scope

    bool IsDeclarableName(); // (not an 'operator')

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
    CUnqualifiedName* m_pRootUnqualifiedName;
    bool m_bGlobalOrigin;
    //bool m_bDestructorName;
    bool m_bPointerToMember;
    bool m_bPendingExplicitTemplatePrefixedName;

    void WriteQualifierChain(out str& out_appendResult);
};

class CRegisteredName : public CToken
{
public:
    GENERIC(CRegisteredName, CToken);
    CRegisteredName();

// Content

    array<CQualifiedName*> x_aQualifiedNameDeclaration;
    void AddDeclaration(CQualifiedName* pQualifiedName);
    int GetDeclarationCount();

protected:
    //virtual void OnTransform() {}

private:
};
