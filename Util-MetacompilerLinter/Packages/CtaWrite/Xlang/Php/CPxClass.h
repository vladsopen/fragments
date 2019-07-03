// CPxClass.h
#pragma once

//
// CPxClass - PHP class generator
//

class CPxClass : public CCommon
{
public:
    GENERIC(CPxClass, CCommon);
    CPxClass();

// Const

// Attributes

// Operations

    // A hook to insert a class header before first method definition
    void HandleBeforeNextPhpFunctionBody(
            CDeclaration* pDeclaration,
            str sClass,
            CWriter* pWriter);

    // End class
    void WritePhpClassEndIfStarted(
            CToken* pTokenSync,
            CWriter* pWriter);

protected:
private:

    void WritePhpClassStart(
            str sClass,
            CDeclaration* pDeclaration, 
            CWriter* pWriter);

};

