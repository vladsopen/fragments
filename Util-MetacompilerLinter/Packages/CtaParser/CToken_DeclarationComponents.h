// CToken_DeclarationComponents.h
// (see "CToken.h")
// (see "ParseDeclKeywords.cpp")
// (see "ParseQualifiedName.cpp")

class CIfVerifyingExistence;

//////////////////////////////////////////////////////
// CDeclaration - general declaration:
//      class, object, method, typedef, etc.

class CDeclaration : public CStatement
{
public:
    GENERIC(CDeclaration, CStatement);
    CDeclaration();

// Content

    CDeclModifierList* x_pDeclModifierList;
    CDeclareTypeName* x_pDeclareTypeName;
    CBaseClassList* x_pBaseClassList;
    CClassBody* x_pClassBody;
    CDeclaratorList* x_pDeclaratorList;
    CDeclareRelatedConstructors* x_pDeclareRelatedConstructors; // < : CBase(), Member()>
    CFunctionBody* x_pFunctionBody;

// Classification

    bool x_bDeclarationForFunction;
    bool x_bDeclarationForConstructor;
    bool x_bDeclarationForLabel;

// Declaration Types

    bool IsDeclarationForFunction();
    bool IsDeclarationForClass();
    bool IsDeclarationForAggregate();
    bool IsClassBodyCanBePresent();
    bool IsDeclarationForConstructor();
    bool IsDeclarationForDestructor();
    bool IsDeclarationForTemplate();
    bool IsClassBodyInScope();
    bool IsDeclarationForLabel();
    bool IsVoidReturn();

    // Query parent declaration for 'enum' modifier
    bool IsDeclarationForEnumConstant();

    // For functions, namespaces, extern {} statements terminal ';' is not required,
    //  hence in such cases nothing can follow '{ }'-compound
    bool IsBracedBodyTerminatesDeclaraton();

// Query Declaration Properties

    //nullable<CBuiltinType*> LookupBuiltinTypeAmongModifiers();
    nullable<CConstKeyword*> LookupConstKeywordAmongModifiers();
    nullable<CIfVerifyingExistence*> LookupIfVerifyingExistenceAmongModifiers();
    nullable<CDeclModifier*> LookupDeclModifierKind(
            CDeclModifier* pDeclModifierKind);

// Shortcuts

    str GetRootTypeNameOr(str sReturnOnError);
    str WriteTypeSpec(CWriterOptions* pWriterOptions);

    nullable<CDeclareFunctionArgs*> LookupFunctionArgs();
    nullable<CQualifiedName*> LookupFirstDeclaratorName();

// Callbacks

    // Any token can be added, even not related to the declaration
    void AddCallbackOnCompleteDeclaration(
            CToken* pHandlerToken);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual bool OnIsSemicolonAtEnd();

private:
    array<CToken*> m_apCallbackOnCompleteDeclaration;
};

/*
////////////////////////////////////////////////////////////////////////
// CDeclarationElement - declaration consists of a sequence of such elements.

//???????
class CDeclarationElement : public CStatement
{
public:
    GENERIC(CDeclarationElement, CStatement);
protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
private:
};
*/

/////////////////////////////////////////////////////////////////////////////////////
// CDeclModifierList - chained CDeclModifier's

class CDeclModifierList : public CToken
{
public:
    GENERIC(CDeclModifierList, CToken);
    CDeclModifierList();

// Content

private: // (special Add())
    array<CDeclModifier*> x_aDeclModifier;
public:
    void AddDeclModifier(CDeclModifier* pDeclModifier);
    int GetDeclModifierCount();
    CDeclModifier* GetDeclModifier(int iModifier);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
};

//////////////////////////////////////////////////////////////////////////////////
// CDeclareTypeName - <class **CClass**> or <**int** nValue>

class CDeclareTypeName : public CToken
{
public:
    GENERIC(CDeclareTypeName, CToken);
    CDeclareTypeName();

    nullable<CQualifiedName*> x_npQualifiedName;

    str GetUnqialifiedTypeNameStr();
    str GetFullQualifiedTypeNameAsStr();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
};

//////////////////////////////////////////////////////////////////////////////////////
// CBaseClass - <'modifiers' CBase>

class CBaseClass : public CToken
{
public:
    GENERIC(CBaseClass, CToken);
    CBaseClass();

// Content

    array<CToken*> x_aToken;
    void AddToken(CToken* pToken);
    int GetTokenCount();
    CToken* GetToken(int iToken);

    nullable<CQualifiedName*> GetQualifiedName();

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
};

//////////////////////////////////////////////////////////////////////////////////////
// CBaseClassList - <':' 'public' CBase ',' ...>

class CBaseClassList : public CToken
{
public:
    GENERIC(CBaseClassList, CToken);
    CBaseClassList();

// Content

    array<CBaseClass*> x_aBaseClass;
    void AddBaseClass(CBaseClass* pBaseClass);
    int GetBaseClassCount();
    CBaseClass* GetBaseClass(int iBaseClass);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
};

//////////////////////////////////////////////////////////////////////////////////////
// CClassBody - <'{' StatementList '}'>

class CClassBody : public CToken
{
public:
    GENERIC(CClassBody, CToken);
    CClassBody();

// Content

    CStatementList* x_pStatementList;
    bool x_bPresent;

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
};

////////////////////////////////////////////////////////////////////////////////////
// CDeclaratorList - <(decl), (decl),...>

class CDeclaratorList : public CToken
{
public:
    GENERIC(CDeclaratorList, CToken);
    CDeclaratorList();

// Content

    array<CDeclarator*> x_aDeclarator;
    void AddDeclarator(CDeclarator* pDeclarator);
    int GetDeclaratorCount();
    CDeclarator* GetDeclarator(int iDeclarator);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);

private:
};

    /////////////////////////////////////////////////////////////////////////////////
    // CDeclarator - (decl) = init

    class CDeclarator : public CToken
    {
    public:
        GENERIC(CDeclarator, CToken);
        CDeclarator();

    // Content

        CDeclareObject* x_pDeclareObject;
        //CDeclaratorInitByConstructor* x_pDeclaratorInitByConstructor;
        CDeclareFunctionArgs* x_pDeclareFunctionArgs;
        CDeclareFunctionModifiers* x_pDeclareFunctionModifiers;
        CDeclaratorInitAsAssign* x_pDeclaratorInitAsAssign;

        str GetRootUnqualifiedNameStrOr(
                str sOrReturn);

    protected:
        virtual CToken* OnTryExtend();
        virtual void OnTransform();
        virtual void OnRegenerate(CWriter* pWriter);

    private:
    };

        ////////////////////////////////////////////////////////////////////
        // CDeclareObject - * (*(Id))

        class CDeclareObject : public CToken
        {
        public:
            GENERIC(CDeclareObject, CToken);
            CDeclareObject();

        // Content

            array<CToken*> x_aToken;
            void AddToken(CToken* pToken);
            nullable<CQualifiedName*> GetQualifiedName();
            int FindReferenceDeclPos();
            int FindPointerOrReferenceDeclPos();

        // Regeneration

            str WriteExtraQualifiedSource(
                    str sClassQualification,
                    CWriterOptions* pWriterOptions);

        protected:
            virtual CToken* OnTryExtend();
            virtual void OnTransform();
            virtual void OnRegenerate(CWriter* pWriter);

        private:
        };

        /*
        ///////////////////////////////////////////////////////////////////////////////
        // CDeclaratorInitByConstructor - (1, 2, 3)

        class CDeclaratorInitByConstructor : public CToken
        {
        public:
            GENERIC(CDeclaratorInitByConstructor, CToken);
            CDeclaratorInitByConstructor();

        protected:
            virtual CToken* OnTryExtend();
            virtual void OnTransform();
            virtual void OnRegenerate(CWriter* pWriter);

        private:
            CStatementList* x_pStatementList;
            bool m_bPresent;
        };
        */

        //////////////////////////////////////////////////////////////////////////////
        // CDeclareFunctionArgs - (int x)

        class CDeclareFunctionArgs : public CToken
        {
        public:
            GENERIC(CDeclareFunctionArgs, CToken);
            CDeclareFunctionArgs();

            int GetObligatoryFunctionArgCount();
            str WriteActualParameterNameList(
                    str sSubstituteForEmptyNames,
                    int iOnlyWriteSingleParameter = off);

            void ParseFunctionArgs(
                    out array<CParsedArgument*>& out_apParsedArgument);

            CStatementList* x_pStatementList;

            bool IsPresent();

        protected:
            virtual CToken* OnTryExtend();
            virtual void OnTransform();
            virtual void OnRegenerate(CWriter* pWriter);

        private:
            bool m_bPresent;
        };

        //////////////////////////////////////////////////////////////////////////////
        // CDeclareFunctionModifiers - const throw() try

        class CDeclareFunctionModifiers : public CToken
        {
        public:
            GENERIC(CDeclareFunctionModifiers, CToken);
            CDeclareFunctionModifiers();

[...]