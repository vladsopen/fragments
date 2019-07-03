// CToken.h
// (see "AllSpecificTokenClasses.h" for additional token headers)
// (see "AllParserIncludes.h")
// (see "CToken.cpp")
// (see "ParseQualifiedName.cpp", "ParseDeclKeywords.cpp", "ParseDeclaration.cpp",
//      "ParseFlowControl.cpp", "ParseExpressions.cpp", "ParseUnusualSyntax.cpp",
//      "Autodetectors.cpp")
// (see "NameResolution.cpp")
// (see "Regeneration.cpp")

class CAddToToken;
class CMagicStrGenerator;
class CCompound;

//////////////////////////////////////////////////////////////////////////////////////
// CToken - abstract terminal or complex token.

class CToken : public CCommon
{
public:
    GENERIC_ABSTRACT(CToken, CCommon);
    CToken();

// Const
    
    enum { C_nUndefinedLineNumber = -1 };
    CASSERT(C_nUndefinedLineNumber < 0);

// Token Hierarchy Support

    void LinkToParent(CToken* pParent);
    nullable<CToken*> GetParentToken();
    nullable<CToken*> FindFirstParentOfKind(CToken* pKindOfToken);
    nullable<CToken*> FindSiblingTokenInParentExpression(
            int nStep);

// Query Context

    bool IsInFunctionBody();
    bool IsInFunctionHeader();
    bool IsInConstructorHeader();

// Overridable Behaviour

    bool IsEndOfSequence(
            CToken* pToken);
    bool IsEndOfStatement(CToken* pToken);
    bool IsSemicolonEnabledAtExpressionEnd();
    bool IsAlwaysLastInExpressionList();
    //bool IsBuiltInMacroName(); // built-in C/C++ macros don't get

// Parsing

    CToken* TryExtend(CToken* pParentToken);
    void TryParse(CToken* pParentToken);

    // Callback added via AddCallbackOnCompleteDeclaration()
    void HandleCompleteDeclaration(
            CDeclaration* pDeclaration);

// Source Text Fragment

    void SetSourceFragment(
            const char* szFragmentStart,
            int nFragmentLength,
            int iLineInfoNumber,
            int nLineIndent);
    void SetSourceFragmentToGenerated();
    str GetSourceFragmentStr();
    const char* GetUnterminatedSourceFragmentPointer();
    int GetSourceFragmentLength();
    void SetLineInfoNumber(int iLineInfoNumber);
    void ClearLineInfoNumber();
    int GetLineInfoNumber();
    bool IsLineInfoDefined();
    bool GetValidOriginalSourceLineInfo(
            out path& out_pathFile,
            out int& out_iLine,
            str sAddInfoOnError);
    str GetValidOriginalSourceFile(
            str sAddInfoOnError);
        // see also GetInitialSourceFile()
    bool IsSourceEqual(CToken* pOtherToken);
    void SetSourceFragmentIfNeededFrom(CToken* pChildToken);
    int GetOriginalLineIndent();

    // Auto-detect the project the source belongs to 
    //   or return "" if not detected
    str FindProjectNameForOriginalSourceFile(
            str sAddInfoOnError);

    // Support for generated source fragmants
    str GetGeneratedOrOriginalSource();

    // Suppress #line for short inserted tokes
    void SetNoLineDirective(bool bYes);
    bool IsNoLineDirective();

    // Special Mode
    void SetNoSpaceBefore(bool bYes);
    bool IsNoSpaceBefore();

    // MagicStr control
    void SetDisableMagicStr(bool bYes);
    bool IsDisableMagicStr();

    // Simple code insertion
    nullable<CAddToToken*> x_npAddToToken;

// Error reporting

    // Compile token user-friendly report
        enum EPreview
        {
            E_Preview_Compact,
            E_Preview_Extended,
        };
    str GetContentPreviewFriendlyLine(EPreview ePreview = E_Preview_Compact);

    bool QueryStatementSuppressionForIssuedError(
            str sErrorId,
            str sErrorText);

// Scope/Name Resolution

#if defined(SUPPORT_NAME_SCOPE)
    // Attach scope to a statement
    CScope* CreateScope();
    bool IsScope();
    nullable<CScope*> GetScope();
    nullable<CScope*> FindHomeScope();
#endif

    // Introduce a new name into current or explicitly qualified scope
    //   (see "CToken_ScopeAndNameResolution.cpp")
    // Note: also defines template names
    void DeclareName(
            CQualifiedName* pQualifiedName);

// Recursive Transforming

    void Transform();
    static void TransformArray(
            const array<CToken*>& aToken);

    // CMagicStrGenerator needs to scan our subexpressions
    bool SupportMagicStrGeneratorScanner(
            CMagicStrGenerator* pMagicStrGenerator,
            CWriter* pWriter);

// Regenerating

    // Generates source code for the token
    void Regenerate(CWriter* pWriter);
    static void RegenerateArray(
            const array<CToken*>& aToken,
            CWriter* pWriter);

    const char* GetDefaultTextRepresentation();
    bool IsDefaultTextRepresentationDefined();

    // Messaging support
    str WriteSimpleSource(
            CWriterOptions* pWriterOptions,
            CWriter** out_unsafe_pReturnWriter = NULL);

    // Regeneration suppression
    void EnableRegeneration(bool bYes);
    bool IsRegenerationEnabled();

    // Insert code while regenerating
    //
    //             <--------------
    //                -> AddBefore BeforeToken
    //   BeforeToken  
    //                -> AddAfter BeforeToken
    //             <--------------
    //
    //   the token
    //
    //             <--------------
    //                -> AddBefore AfterToken
    //   AfterToken  
    //                -> AddAfter AfterToken
    //             <--------------
    //
    //
    void AddBeforeBeforeToken(
            str sInsertCode);
    void AddAfterBeforeToken(
            str sInsertCode);
    void AddBeforeAfterToken(
            str sInsertCode);
    void AddAfterAfterToken(
            str sInsertCode);

protected:
    virtual CToken* OnTryExtend();
    virtual void OnExtendingFrom(CToken* pBaseToken);
    virtual void OnHandleCompleteDeclaration(
            CDeclaration* pDeclaration);
    virtual void OnTransform();
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation();
#if defined(SUPPORT_NAME_SCOPE)
    virtual nullable<CScope*> OnFindHomeScope();
#endif //defined(SUPPORT_NAME_SCOPE)
    virtual bool OnIsEndOfSequence(
            CToken* pToken);
    virtual bool OnIsEndOfStatement(CToken* pToken);
    virtual bool OnIsSemicolonEnabledAtExpressionEnd();
    virtual bool OnIsAlwaysLastInExpressionList();
    virtual str OnGetSourceFragmentStr();
    virtual bool OnSupportMagicStrGeneratorScanner(
            CMagicStrGenerator* pMagicStrGenerator,
            CWriter* pWriter);
    virtual str OnGetGeneratedOrOriginalSource();

private:

    // Hierarchy
    nullable<CToken*> m_npParent;
#if defined(SUPPORT_NAME_SCOPE)
    nullable<CScope*> m_npScope;
#endif

    // Source reference
    const char* m_szFragmentStart;
    int m_nFragmentLength;
    str m_sCacheSourceFragment;
    int m_iLineInfoNumber;
    str m_sBeforeToken;
    str m_sAfterToken;
    int m_nLineIndent;

    enum FMisc
    {
        F_Misc_EnableRegeneration = 0x0001,
        F_Misc_Inserted           = 0x0002,
        F_Misc_Transformed        = 0x0004,
        F_Misc_NoSpaceBefore      = 0x0008,
        F_Misc_NoLineDirective    = 0x0010,
        F_Misc_DisableMagicStr    = 0x0020,
    };
    FMisc m_fMisc;

    void HandleChildAttached(CToken* pChildToken);
};
