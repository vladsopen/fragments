// CKeywordMapping.cpp
// (see "CKeywordMapping.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CKeywordMapping.h"

CKeywordMapping::CKeywordMapping()
{
    m_mapKeywordToToken;
}

void CKeywordMapping::InitCppKeywords()
{
    ASSERT(m_mapKeywordToToken.GetCount() == 0);

    // Function-like
    MapKeyword(new CSizeofOp); // sizeof
    MapKeyword(new CDeleteOp); // delete
    MapKeyword(new CNewOp); // new
    MapKeyword(new CAlignOfOp); // __alignof
    MapKeyword(new CTypeidOp); // typeid
    MapKeyword(new CUuidofOp); // __uuidof
    //__hook); // (function)
    //__unhook); // (function)

    // Template-like
    MapKeyword(new CConstCast); // const_cast
    MapKeyword(new CDynamicCast); // dynamic_cast
    MapKeyword(new CReinterpretCast); // reinterpret_cast
    MapKeyword(new CStaticCast); // static_cast
    MapKeyword(new CTryCast); // __try_cast); // (#)

    // Constant-like
    MapKeyword(new CThisPointer); // this
    //MapKeyword(new CTrueConstant); // true
    //MapKeyword(new CFalseConstant); // false
    //MapKeyword(new CSuperClassSpec); // super

    // Single-word flow control
    MapKeyword(new CTryOp); // try
    MapKeyword(new CTryCstyleOp); // __try
    MapKeyword(new CBreakOp); // break
    MapKeyword(new CContinueOp); // vcontinue

    // Flow control with 1 argument
    MapKeyword(new CAssumeOp); // __assume
    MapKeyword(new CLeaveCstyleOp); // __leave
    MapKeyword(new CRaiseCstyleOp); // __raise
    MapKeyword(new CThrowOpOrModifier); // throw
    MapKeyword(new CGotoOp); // goto
    MapKeyword(new CReturnOp); // return

    // Flow control with a statement-argument
    MapKeyword(new CIfOp); // if
    MapKeyword(new CElseOp); // else
    MapKeyword(new CDoOp); // do
    MapKeyword(new CWhileOp); // while
    MapKeyword(new CForOp); // for
    MapKeyword(new CSwitchOp); // switch
    MapKeyword(new CCatchOp); // catch
    MapKeyword(new CExceptCstyleOp); // __except
    MapKeyword(new CFinallyCstyleOp); // __finally
    MapKeyword(new CIfExistsOp); // __if_exists
    MapKeyword(new CIfNotExistsOp); // __if_not_exists

    // Access specifiers
    MapKeyword(new CPrivateAccess); // private
    MapKeyword(new CProtectedAccess); // protected
    MapKeyword(new CPublicAccess); // public
    MapKeyword(new CDefaultLabel); // default
    MapKeyword(new CCaseLabel); // case

    // Bizarre syntax keywords
    MapKeyword(new CIdentifierMaskKeyword); // __identifier
    MapKeyword(new CNoop); // __noop
    MapKeyword(new COperatorSpec); // operator
    MapKeyword(new CUsingOp); // using
    MapKeyword(new CAsmUnn); // __asm
    MapKeyword(new CAsmStandard); // asm

    /*
    // Builtin Types (CIntType, etc)
    #define ACTION(keyword, CClass) \
        MapKeyword(new CClass);
    DECLARE_BUILTIN_TYPES(ACTION)
    #undef ACTION
    */

    // Declarations
    MapKeyword(new CClassKeyword); // class
    MapKeyword(new CStructKeyword); // struct
    MapKeyword(new CUnionKeyword); // union
    MapKeyword(new CEnumKeyword); // enum
    MapKeyword(new CInterfaceKeyword); // interface
    MapKeyword(new CNamespaceKeyword); // namespace
    MapKeyword(new CTemplateKeywordAndArg); // template
    MapKeyword(new CTypenameKeyword); // typename
    MapKeyword(new CTypedefKeyword); // typedef
    MapKeyword(new CExternKeywordAndText); // extern
    MapKeyword(new CDeclspecModifier); // __declspec
    MapKeyword(new CConstKeyword); // const
    MapKeyword(new CVirtualKeyword); // virtual
    MapKeyword(new CStaticKeyword); // static

    // PHP 
    MapKeyword(new CFunctionKeyword); // function
[...]