// ParseFlowControl.cpp
// (see "CToken.h")
// (see "CToken_ExecutableExpressions.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"

//////////////////////////////////////////////////////////////////////////////////////
// CThrowOpOrModifier - executable statement or function modifier

CToken* CThrowOpOrModifier::OnTryExtend()
{
    if (IsInFunctionHeader())
        return new CThrowModifier;
    else
        return new CThrowOp;
}

//////////////////////////////////////////////////////////////////////////////////////
// CThrowOp - throw Exception(...);

CThrowOp::CThrowOp()
{
    x_pExpressionList = new CExpressionList;
}

CToken* CThrowOp::OnTryExtend()
{
    //x_pExpressionList->TryParse(this);
    // misses last ';'
    return this;
}
