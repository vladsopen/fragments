// Autodetectors.cpp
// (see "CToken.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CErrorSuppressor.h"

CToken* CStatementCategorizer::OnTryExtend()
{
    //
    // Declaration vs Expression Problem
    // ---------------------------------
    //
    // This function (together with IsReadingAheadLooksLikeDeclaration())
    // is so complex because it has to make an ultimate
    // differentiation between DECLARATION and EXPRESSION kinds of C++ statements.
    // It should be fast, and must not complicate metaprogramming logic.
    // This is done using some intricate pattern matching to satisfy screwed up C/C++
    // syntax as legal as possible.
    //
    // Basically everything is declaration that starts with one of declarative keywords,
    // modifiers or typenames.
    // With an exception: if we see tokens 'TYPEID' '(' it can be a constructor call
    //  -or-
    // a type conversion expression (direct constructor call).
    // This is the case where C++ standard requires potential infinite look ahead,
    // or a sophisticated compiler roll back.
    // This makes things unnecessary complicated, while practical benefits are questionable.
    //
    // Also note: Cta does not distinguish between IDENTIFIER and TYPENAME token at this
    // stage (to allow flexible late name resolution).
    // Therefore ridiculous ambiguities like
    //   a ( b );  // declare 'a' of type 'b' -or- call 'a' with 'b' as a parameter
    // can't always be resolved here.
    //

    ASSERT(m_fStatementCouldBe != 0);
    ASSERT(IsStatementCouldBeDeclaration() || IsStatementCouldBeExpression());

    // Collect statement attributes (supported for both declations and expressions)
    nullable<CAttributes*> npAttributes =
        derive(CAttributes, GetTokenizer()->PeekNextToken(this));
    if (!npAttributes.IsNull())
    {
        // Skip Attributes token
        GetTokenizer()->ReadNextToken(this);
    }

    // Always DECLARATION if outside of executable code
    nullable<CStatement*> npResultingStatement;
    if (!IsStatementCouldBeExpression())
    {
        ASSERT(IsStatementCouldBeDeclaration());

        npResultingStatement = new CDeclaration;
    }
    else
    if (!IsStatementCouldBeDeclaration())
    {
        ASSERT(IsStatementCouldBeExpression());

        npResultingStatement = new CExecutable;
    }
    else
    {
        // Save rollback pointer
        CToken* pFirstStatementToken = GetTokenizer()->PeekNextToken(this);

        // Analyze token pattern
        if (IsReadingAheadLooksLikeDeclaration())
            npResultingStatement = new CDeclaration;

        // Put tokenizer back at the beginning of the statement
        GetTokenizer()->RollbackToToken(pFirstStatementToken);
    }

    // Anything not recognized as a declaration at this point
    //   is destined to be an expression
    if (npResultingStatement.IsNull())
    {
        npResultingStatement = new CExecutable;
    }

    // Save attributes for statement
    if not_null(CStatement* pResultingStatement = npResultingStatement)
    {
        pResultingStatement->SetAttributes(npAttributes);
        return pResultingStatement;
    }
    else
    {
        ASSERTM(false,
                "npResultingStatement left undefined");
        return this;
    }
}

bool CStatementCategorizer::IsReadingAheadLooksLikeDeclaration()
{
    // fuzzy logic declaration detection: test if statement can be a declaration

    // see remarks for CStatementCategorizer::OnTryExtend() (above)
    // Note: this function leaves tokenizer at an undefined position ahead (speed!)

    // THE RULES:
    // For Cta a statement is a DECLARATION if one of the following is true:
    //    - Currently the parser is outside of function body
    //        (global or class scope, argument list)
    //    - Statement starts with a decl modifier
    //        (e.g.: const, class, __declspec(), namespace, etc.)
    //    - First statement tokens follow one of the patterns
    //        qID        qID ...      (int k = 0;  void f() {};  etc.)
    //        qID     *  qID ...      (int* p = 0; CData* f();  etc.)
    //        qID     &  qID ...      (int& r = k; CData& f();  etc.)
    //        qID  (  *  qID ) ( ...  (int (*pfn)();  etc.)
    //        qID  (  &  qID ) [ ...  (int (&rArray)[];  etc.)
    //        qID  ( Class:: * qID ) ( ...  (void (CUiPanelGp::*pfnRefreshMethod)())
    //        qID  ( Class:: & qID ) [ ...  
    // Note:
    //        qID  (     qID ) ... is always an EXPRESSION.
    //
    // (here qID - is a potentially complex qualified name: ::Scope<templ>::Id)

    // Decl modifier always signals a DECLARATION
    CToken* pFirstStatementToken = GetTokenizer()->PeekNextToken(this);
    nullable<CDeclModifier*> npDeclModifier =
        derive(CDeclModifier, pFirstStatementToken);
    if (!npDeclModifier.IsNull())
    {
        return true;
    }
    else
    {
        // Check for a declaration pattern

        // Check for qualified name which can be the typename in a declation
        nullable<CQualifiedName*> npPotentialTypeQualifiedName =
            derive(CQualifiedName, pFirstStatementToken);
        if (!npPotentialTypeQualifiedName.IsNull())
        {
            // Check the token following the PotentialTypeQualifiedName
            //   for signs of declaration
            GetTokenizer()->ReadNextToken(this); // skip typename
            CToken* pTokenAfterPotentialType =
                GetTokenizer()->PeekNextToken(this);

            // Skip opening paren if present (for function pointers, etc.)
            if (iskindof(CCharLeftParen, pTokenAfterPotentialType))
            {
                GetTokenizer()->ReadNextToken(this); // skip '('
                pTokenAfterPotentialType =
                    GetTokenizer()->PeekNextToken(this);

                // we can have a class prefix for a pointer: 
                //   void ( CUiPanelGp :: * pfnRefreshMethod )()
                if not_null(
                    CQualifiedName* pClassPrefix = 
                        derive(CQualifiedName, pTokenAfterPotentialType))
                {
                    //str s1 = pClassPrefix->GetRootUnqualifiedNameStr();
                    //str s2 = pClassPrefix->GetFullQualifiedNameAsStr();

                    if (pClassPrefix->IsPointerToMember())
                    {
                        // "type (Class::*Name ..." is a declaration
                        return true;
                    }
                    else
                    {
                        // "type (Name ..." is not a declaration
                        return false;
                    }
                }

                // Only '*' or '&' after '(' can make a declaration
                if (iskindof(CCharStar, pTokenAfterPotentialType) ||
                    iskindof(CCharAnd, pTokenAfterPotentialType))
                {
                    // But we have to avoid making "f ( a )" case a declaration,
                    //  so we look for function argument or array spec

                    // Skip '*' or '&'
                    GetTokenizer()->ReadNextToken(this);

                    //
                    // '.... * name ) [ ...'
                    // '.... & name ) ( ...'
                    //

                    CToken* pTokenMustBeName =
                        GetTokenizer()->PeekNextToken(this);
                    if (iskindof(CQualifiedName, pTokenMustBeName))
                    {
                        // Skip name
                        GetTokenizer()->ReadNextToken(this);

                        // ')' ?
                        CToken* pTokenMustRightParen =
                            GetTokenizer()->PeekNextToken(this);
                        if (iskindof(CCharRightParen, pTokenMustRightParen))
                        {
                            // Skip ')'
                            GetTokenizer()->ReadNextToken(this);

                            // '(' or '[' ?
                            CToken* pTokenMustLeftOpening =
                                GetTokenizer()->PeekNextToken(this);
                            if (iskindof(CCharLeftParen, pTokenMustLeftOpening) ||
                                iskindof(CCharLeftBracket, pTokenMustLeftOpening))
                            {
                                return true;

[...]