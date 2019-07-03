// CPxFunction.h
#pragma once

class CPxClass;
class CXlangPhp;

//
// CPxFunction - PHP function generator
//

class CPxFunction : public CCommon
{
public:
    GENERIC(CPxFunction, CCommon);
    CPxFunction();

// Const

// Attributes

// Operations

    // Main handler called by CXlangPhp
    //   when a function declaration is detected
    void HandlePhpFunctionRegeneration(
            CXlangPhp* pXlangPhp,
            CDeclaration* pDeclaration,
            CWriter* pWriter,
            CPxClass* pPxClass);

protected:
private:

    void WritePhpFunction(
            str sFunctionName,
            CDeclaration* pDeclaration, 
            CWriter* pWriter);
};

