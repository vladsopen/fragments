// AllParserIncludes.h
// (see also: "AllSpecificTokenClasses.h")

//
// Token Classes
//
class CToken;

    //
    // Basic Statements
    //
    class CCompilationUnit;
    class CStatement;
        class CExecutable;
        class CDeclaration;
    class CStatementCategorizer;
    class CStatementList;
    class CExpression;
    class CExpressionList;
    class CTemplateArgList;
    class CAttributes;
    //class CUndefinedToken;

    //
    // Declaration Elements
    //
    class CDeclModifierList;
    class CDeclareTypeName;
    class CBaseClassList;
    class CClassBody;
    class CDeclaratorList;
        class CDeclarator;
            class CDeclareObject;
            //class CDeclaratorInitByConstructor;
            class CDeclareFunctionArgs;
            class CDeclareFunctionModifiers;
            class CDeclaratorInitAsAssign;
    class CDeclareRelatedConstructors;
    class CFunctionBody;

    //
    // Qualified/Templated Identifier
    //
    //class CTemplateNameArgs;
    class CUnqualifiedName;
    class CQualifier;
    class CQualifiedName;
    class CRegisteredName;
        class CRegisteredObjectName;
        class CRegisteredTypeName;

    //
    // Terminal Tokens
    //
    class CCharToken;
        //class CCharXxxx; // punctuation
        class CCharNumber;
        class CCharWord;

    //
    // Declaration Related Keywords
    //
    class CThrowOpOrModifier; // can be either modifier or operator
    class CTryKeyword; // can be either modifier or operator
    class CDeclModifier;
        class CConstKeyword;
        class CUnsignedKeyword;
        class CConstKeyword;
        class CTemplateKeywordAndArg;    // 'template' '<' ... '>'
        class CAggregateKeyword;             // :
            class CClassStructUnionKeyword;  // :
                class CClassStructKeyword;   // :
                    class CClassKeyword;     // 'class'
                    class CStructKeyword;    // 'struct'
                class CUnionKeyword;         // 'union'
            class CEnumKeyword;              // 'enum'
        class CDeclspecModifier;             // '__declspec'
        class CTypedefKeyword;               // 'typedef'
        class CExternKeywordAndText;         // 'extern' ["..."]
            //class CExternStatement;          // 'extern' ["..."] '{' ... '}'
        class CNamespaceKeyword;             // 'namespace' ... '{' ... '}'
        class CUsingStatement;               // 'using' ... ';'
        class CThrowModifier;
        //class CBuiltinType;
    class CAccessStatement;
    class CAccessKeyword;

    //
    // Executable Keywords
    //
    class CFlowControl;
        class CThrowOp;
        class CTryOp;

    // PHP (PHP)
    class CFunctionKeyword;

//
// Auxiliary Parser Classes
//
class CProcessing;
class CTokenizer;
class CSourceInput;
#if defined(SUPPORT_NAME_SCOPE)
class CScope;
#endif //defined(SUPPORT_NAME_SCOPE)
class CErrorSuppressor;
class CSourceLines;
class CKeywordMapping;
class CWriter;
class CWriterOptions;
class CWriteRegeneratedText;
class CWriteTree;
class CInsertCode;
class CParsedArgument;

//////////////////////////////////////////////////////////////////////////////////////
// Includes

#include "CCommon.h"
#include "CToken.h"
#include "CScope.h"
#include "CTokenizer.h"
#include "CProcessing.h"
