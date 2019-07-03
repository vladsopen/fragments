// CCtapTransformer.h
// (see "CCtapTransformer.cpp")

//
// CCtapTransformer - 
//
//   PHP: Global PHP translator state
//

class CCtapTransformer : public CCommon
{
public:
    GENERIC(CCtapTransformer, CCommon);
    CCtapTransformer();

// Constants

// Attributes

// Operations

    // CExpression::OnTransform()
    void TransformPhpToken(
            CToken* pToken);

    // Function code generation
    bool IsCtapFunction(
            CFunctionBody* pFunctionBody);
    void GenerateCtapFunctionProlog(
            CFunctionBody* pFunctionBody,
            CWriter* pWriter);
    void GenerateCtapFunctionReturn(
            CFunctionBody* pFunctionBody,
            CWriter* pWriter);

private:

    void TransformPhpExpression(
            CExpression* pExpression);
};
