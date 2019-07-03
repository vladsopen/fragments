// ParseDeclaration.cpp
// (see "CToken.h")
// (see "CProcessing.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CWriter.h"
#include "CParsedArgument.h"
#include "CAlterCtaMacro.h"

bool CDeclaration::IsVoidReturn()
{
    // Scan function type tokens
    // (no more built-in types!)

    str sType = x_pDeclareTypeName->GetUnqialifiedTypeNameStr();

    if (sType == "void")
    {
        // now they say "void*" is not the same as "void"... ok,
        //   maybe we should check the declarator modifier as well

        // we MUST have the first declarator if it is a function header
        if not_null(
            CDeclarator* pDeclarator = 
                x_pDeclaratorList->x_aDeclarator.GetAtOrNull(0))
        {
            // do we have any "*" or "&" in the declarator?
            if (pDeclarator->x_pDeclareObject->
                    FindPointerOrReferenceDeclPos() >= 0)
            {
                // "void*" detected
                return false;
            }
            else
            {
                // seems like just "void" 
                //   (still some in-depth complex case checks maybe
                //   missing, but we DONT want to support *complex* cases 
                //   in generic zone anyway)
                return true;
            }
        }
    }

    return false;
}

bool CDeclaration::IsBracedBodyTerminatesDeclaraton()
{
    if (IsDeclarationForFunction() &&
        x_pFunctionBody->x_bPresent)
        return true;

    if (!x_pClassBody->x_bPresent)
        return false;

    // Scan for modifier signaling absence of ';'
    repeat(iModifier, x_pDeclModifierList->GetDeclModifierCount())
    {
        if (x_pDeclModifierList->GetDeclModifier(iModifier)->
                IsSignalForBracedBodyToTerminatesDeclaraton())
        {
            return true;
        }
    }

    return false;
}

CToken* CDeclaration::OnTryExtend()
{
    //
    // Try all statements in the following order - some will respond to
    //   incoming tokens and form declaration elements
    //
    //   modifier  CTypeName  : CBase  { }  * (*  Declarator)()[]  : CRelated()  { } ;
    //   [      ]  <       >  [     ]  [ ]  [  ]  <        > [  ]  [          ]  [ ] []
    //
    //   class     CDemo      : CParent { };
    //   const     int                            nValue = 123;
    //   template<>                               CDemo::CDemo()   : CBase(123)  { }
    //                                            E_Option_Yes = 1,

    // 'class' 'const' etc.
    x_pDeclModifierList->TryParse(this);

    // 'int' 'CData' etc.
    bool bExpectTypename = true;
    /*
    if not_null(LookupBuiltinTypeAmongModifiers())
    {
        // already has builtin type
        bExpectTypename = false;
    }
    else
    */
    {
        // Possible user-defined type

        // Is it a special method without a return value?
        if not_null(CQualifiedName* pNextQualifiedName =
            derive(CQualifiedName, GetTokenizer()->PeekNextToken(this)))
        {
            if (pNextQualifiedName->IsConstructorName())
            {
                x_bDeclarationForConstructor = true;
                x_bDeclarationForFunction = true;
                bExpectTypename = false;
            }
            else
            if (pNextQualifiedName->IsDestructorName())
            {
                x_bDeclarationForConstructor = false;
                x_bDeclarationForFunction = true;
                bExpectTypename = false;
            }
            else
            if (pNextQualifiedName->IsPointerToMember())
            {
                bExpectTypename = false;
            }
            else
            if not_null(derive(COperatorSpec,
                pNextQualifiedName->GetRootUnqualifiedName()))
            {
                // 'operator' user defined type conversion function may
                //  miss return type
                bExpectTypename = false;
            }

            // leave this name to be parsed as declarator
        }
    }

    // 'enum' constants never have a typename
    if (bExpectTypename)
    {
        // Is in a enum body?
        if not_null(CDeclaration* pContainingDeclaration =
            FindFirstParentDeclaration())
        {
            if not_null(pContainingDeclaration->LookupDeclModifierKind(new CEnumKeyword))
                bExpectTypename = false;
        }
    }

    // Parse the Type Name
    if (bExpectTypename)
    {
        // user-defined type
        x_pDeclareTypeName->TryParse(this);
    }

    // ':' 'CBaseClass'  -or-  ':' after label
    CToken* pToken = GetTokenizer()->PeekNextToken(this);
    if (iskindof(CCharColon, pToken))
    {
        // Skip ':'
        pToken = GetTokenizer()->ReadNextToken(this);
        ASSERT(iskindof(CCharColon, pToken));

        // It can be a label or an access specifier if there was aggregate spec
        if (x_pDeclModifierList->GetDeclModifierCount() != 0)
        {
            // ':' 'CBase'
            x_pBaseClassList->TryParse(this); // (bit fields aren't recognized)
        }
        else
        {
            // Assume this is a label
            x_bDeclarationForLabel = true;

            // Label ends with ':'
            return this;

[...]


CToken* CDeclareObject::OnTryExtend()
{
    //
    // It either
    //   - starts with 'ID' or '(' or '='
    //        int ID;    int ID()[];    int (*(*const((*)[])[][])))();
    //        int x[2] = { 4, -8 }, (*y(int));
    //        int  = 0
    //        int <<nothing at all>>
    //
    // From bad to worse:
    // int     i;
    // int     i[];
    // int *   ();
    // int *   (int);
    // int     (*) (int);
    // int     (*(*)[])(int);
    //
    // General Case:
    //  *spec &spec ((*spec (&spec ID)) [][])()(decl) (init) = init const throw() try
    //
    /*
    int             Value                                                ;
    int         *** Pointer                                              ;
    int           & Reference                                            = ***Pointer;
    int         * & ReferenceToPointer                                   = **Pointer;
    int             Array                                  [1][1][1]     ;
    int         *** ArrayOfPointers                        [1][1][1]     = { (int***) 0 };
    int             FunctionReturningValue                 ()            ;
    int         *** FunctionReturningPointer               ()            ;
    int           & FunctionReturningReference             ()            ;
    int        (*** PointerToFunctionReturningValue)       ()            ;
    int    *** (*** PointerToFunctionReturningPointer)     ()            ;
    int      & (*** PointerToFunctionReturningReference)   ()            ;
    int          (& ReferenceToFunctionReturningValue)     ()            ;
    int    ***   (& ReferenceToFunctionReturningPointer)   ()            ;
    int      &   (& ReferenceToFunctionReturningReference) ()            ;

    // Unsupported:
    int (*(*PointerToFunctionReturningArrayOfPointers)(int))[2];
    */

    int nParenCount = 0;
    bool bErrorEncountered = false;
    bool bHaveCompleteDeclarator = false;
    //bool bConstructor = false;
    bool bDestructor = false;

    // Destructor '~' follows?
    if not_null(
        //CCharTilde* pTilde =
            derive(CCharTilde, GetTokenizer()->PeekNextToken(this)))
    {
        // Remember to mark the following name
        bDestructor = true;

        // Skip '~'
        GetTokenizer()->ReadNextToken(this);
    }

    // Match tokens until nParenCount == 0 or a termination token encountered
    nullable<CQualifiedName*> npLastQualifiedNameFound;
    while (true)
    {
        CToken* pToken = GetTokenizer()->PeekNextToken(this);

        // Open Parened level
        if (iskindof(CCharLeftParen, pToken))
        {
            if (bHaveCompleteDeclarator &&
                nParenCount == 0)
            {
                // We have already seen something like declarator, and now
                //   outer-most paren is opening (again?) meaning function arguments
                //  -- int  f  ();
                //  -- int (*) ();
                //             ^
                break;
            }

            nParenCount++;
            bHaveCompleteDeclarator = true;
        }
        else

        // Close Parened level
        if (iskindof(CCharRightParen, pToken))
        {
            nParenCount--;
            if (nParenCount <= 0)
            {
                bool bBreakDeclarator = true;
                if (nParenCount == 0)
                {
                    // This is our last paren
                    AddToken(pToken);
                    GetTokenizer()->ReadNextToken(this);

                    // Surprisingly an array specified was discovered in VS8:
                    //    char (& _String)[_Size];


[...]


                    errn(0xA21E),
                    pToken,
                    "inside " + Str(nParenCount) + " open parens in declarator (See below)",
                    this);

                GetTokenizer()->ReportTransformClientError(
                    errn(0xA21F),
                    "the declarator for previous error",
                    this);
            }

            // Error recovery: stop if token looks like terminating
            if (IsEndOfSequence(pToken))
                break;
        }

        // Eat token
        AddToken(pToken);
        GetTokenizer()->ReadNextToken(this);
    }

    return this;
}

