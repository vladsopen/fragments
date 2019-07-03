// CXlang.h
#pragma once

class CWriter;
class CDeclaration;
class CExpression;

//
// CXlang - abstract base class for all types of regenerated languages
//

class CXlang : public CCommon
{
public:
    CXlang();

// Const

// Attributes

// Operations

    //
    // Init
    //

    // Append something to .ctatemp
    path GetRootOutputFileForXlang(
            path pathCtatemp);

    // Init CTransformContext for this language
    void HandleInitMultiXlang();

    // Add transformers
    void InitTransformersForXlang();

    //
    // Transform
    //

    // After default expression token transform
    void PostTransformExpression(
            CExpression* pExpression);

    // Declaration
    bool OverrideDeclarationRegenerate(  
            CDeclaration* pDeclaration,
            CWriter* pWriter);

    //
    // Special Rewrite Hooks
    //

    // WriteText hook
    bool IsFileRegenerationEnabled(
            str pathSourceFile);
    void AppendWriterOutput(
            out str& out_sAppendOutput,
            const char* pcAppend,
            int nAppendLength,
            int iPreviousLineInfoNumber,
            int iLastOriginalLineNumberWritten,
            int iActualCurrentOutputLine,
            int iMaxLineInfoForWhichPragmasWritten,
            str pathPreviousSourceFile);

protected:
    virtual path OnGetRootOutputFileForXlang(
            path pathCtatemp)
            = 0;
    virtual void OnHandleInitMultiXlang()
            = 0;
    virtual void OnInitTransformersForXlang()
            = 0;
    virtual void OnPostTransformExpression(
            CExpression* pExpression)
            = 0;
    virtual bool OnOverrideDeclarationRegenerate(  
            CDeclaration* pDeclaration,
            CWriter* pWriter)
            = 0;
private:
};

