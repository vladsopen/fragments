// CAlterCtaMacro.h
// (see "CAlterCtaMacro.cpp")
#pragma once

#include "VirtOs_text.h"

class CNameHookMap;

// CAlterCtaMacro - simple contextual code substitution/generation

class CAlterCtaMacro : public CToken
{
public:
    GENERIC_ABSTRACT(CAlterCtaMacro, CToken);
    CAlterCtaMacro();

// Operations

    // NameHook support called on recognized incoming identifier
    void ProcessNameHook(
            str sName,
            CToken* pTokenLocation);

protected:
    virtual CToken* OnTryExtend();
    //virtual void OnTransform() {}
    virtual void OnRegenerate(CWriter* pWriter);
    virtual const char* OnGetDefaultTextRepresentation() = 0;
    //virtual bool OnIsSemicolonEnabledAtExpressionEnd();
    //virtual bool OnIsAlwaysLastInExpressionList();

    virtual void OnHandleCompleteDeclaration(
            CDeclaration* pDeclaration);

private:

    // arguments '(' ... ')'
    str m_sWrite;
    str m_sFind;
    text m_textReplace;
    str m_sMustMatch;
    str m_sErrorIfMatch;
    str m_sNameHookRawArgument;
    str m_sNameHook;
    text m_textAllowPath;
    CNameHookMap* m_pUseNameHookMap;
    bool m_bIncludeOnlyFrom;
    bool m_bForceExecuteInRegenerate;

    nullable<CDeclaration*> m_npFunctionHeader;
    //nullable<CAlterGenericClass*> m_npAlterGenericClass;
    nullable<CDeclaration*> m_npParentClass;

    void ExecuteCtaMacro(
            out str& out_sWriteOnRegenerate,
            CToken* pTokenLocation);

    void HandleInternalSemipublicMacro(
            CCharDoubleQuotedString* pString,
            str sMacroArg,
            path pathSource,
            CNameHookMap* pNameHookMap);

    bool CheckMatchSource(
            path pathSource,
            CToken* pTokenLocation);

    void RegisterNameHook();
};

    // one-time ignore
    class CAlterCtaMacroXcode : public CAlterCtaMacro
    {
    public:
        GENERIC(CAlterCtaMacroXcode, CAlterCtaMacro);
        CAlterCtaMacroXcode();

    protected:
        virtual const char* OnGetDefaultTextRepresentation();

    private:
    };
