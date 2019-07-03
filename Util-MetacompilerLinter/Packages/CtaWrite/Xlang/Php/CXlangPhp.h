// CXlangPhp.h
#pragma once

#include "CXlang.h"

class CPxClass;

//
// CXlangPhp - PHP generator
//

class CXlangPhp : public CXlang
{
public:
    GENERIC(CXlangPhp, CXlang);
    CXlangPhp();

// Const

// Attributes

// Operations

protected:
    virtual path OnGetRootOutputFileForXlang(
            path pathCtatemp);
    virtual void OnHandleInitMultiXlang();
    virtual void OnInitTransformersForXlang();
    virtual void OnPostTransformExpression(
            CExpression* pExpression);
    virtual bool OnOverrideDeclarationRegenerate(  
            CDeclaration* pDeclaration,
            CWriter* pWriter);

private:

    bool m_bEnableRegeneration;
    CPxClass* m_pPxClass;

    void WriteModifiers(
            CDeclaration* pDeclaration,
            CWriter* pWriter);
    str GetInitializerForType(
            CDeclaration* pDeclaration,
            CWriter* pWriter);
    void WriteDeclarator(
            str sInitForType,
            CDeclaration* pDeclaration,
            CWriter* pWriter);
    void WriteDeclaredName(
            CDeclareObject* pDeclareObject,
            CWriter* pWriter);
};

