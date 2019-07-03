// CXlangCpp.h

#include "CXlang.h"

//
// CXlangCpp - C++ generator
//

class CXlangCpp : public CXlang
{
public:
    GENERIC(CXlangCpp, CXlang);
    CXlangCpp();

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
};

