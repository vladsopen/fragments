// RegisterAllTransforms.cpp
// (see "CToken.h")

#include "Precompiled.h"
#include "AllParserIncludes.h"
#include "AllSpecificTokenClasses.h"
#include "CKeywordMapping.h"
#include "CTransformContext.h"
#include "CAlterDeclaration.h"
#include "CAlterQualifiedName.h"
#include "CAlterCtaMode.h"
#include "CAlterCtaIgnore.h"
#include "CAlterCtaMacro.h"
#include "CAlterAssert.h"
#include "CAlterGenericClass.h"
#include "CAlterGenericClassImpl.h"
#include "CAlterMember.h"
#include "CAlterEnumInfo.h"
#include "CAlterEnumInfoKeyword.h"
#include "CAlterNameMention.h"
#include "CAlterPropertyName.h"
#include "CAlterGenericThis.h"
#include "CAlterCtaSz.h"
#include "CAlterCtaMidZero.h"
#include "CAlterTranslate.h"
#include "CAlterTranslatePlainSelector.h"
#include "CAlterLocalDeclaration.h"
#include "CAlterFunctionMacro.h"
#include "CAlterCtaStrPrecalc.h"
#include "CAlterGenericSuper.h"
#include "CAlterQx.h"
#include "CAlterPassNameToConstructor.h"
#include "CAlterDollarName.h"
#include "CXlang.h"
#include "CAlterDeclareEnumInfo.h"
#include "CAlterDslot.h"

void CTokenizer::InitTransformers()
{
    // Cta Internal Control
    GetKeywordMapping()->MapKeyword(new CAlterCtaMode);
    GetKeywordMapping()->MapKeyword(new CAlterCtaIgnoreOnce);
    GetKeywordMapping()->MapKeyword(new CAlterCtaIgnoreInFile);
    GetKeywordMapping()->MapKeyword(new CAlterCtaSz);
    GetKeywordMapping()->MapKeyword(new CAlterCtaMidZero);
    GetKeywordMapping()->MapKeyword(new CAlterCtaStrPrecalc);
    GetKeywordMapping()->MapKeyword(new CAlterFunctionMacro);
    GetKeywordMapping()->MapKeyword(new CAlterCtaMacroXcode);

    // Assert Macros
    GetKeywordMapping()->MapKeyword(new CAlterAssertm);
    GetKeywordMapping()->MapKeyword(new CAlterAssert);

    // Translate Macros
    GetKeywordMapping()->MapKeyword(new CAlterTranslate);

    // Debug support for IfaceGp
    GetKeywordMapping()->MapKeyword(new CAlterDslot);

    // SQL
    GetKeywordMapping()->MapKeyword(new CAlterQx);

    // Generic support keywords
    GetKeywordMapping()->MapKeyword(new CAlterEnumInfoKeyword);
    GetKeywordMapping()->MapKeyword(new CAlterDeclareEnumInfo);

    // Remap C++ keywords
    GetKeywordMapping()->
        MapKeyword(
            new CAlterGenericThis,
            CKeywordMapping::E_AddKeyword_RemapExisting);

    /*
    GetKeywordMapping()->
        MapKeyword(
            new CAlterGenericSuper,
            CKeywordMapping::E_AddKeyword_RemapExisting);
    */

    // Generic Classes
    GetTransformContext()->RegisterDeclarationTransformerKind(
        new CAlterGenericClass());
    GetTransformContext()->RegisterDeclarationTransformerKind(
        new CAlterGenericClassImpl());
    GetTransformContext()->RegisterDeclarationTransformerKind(
        new CAlterMember());
    GetTransformContext()->RegisterDeclarationTransformerKind(
        new CAlterEnumInfo());
    GetTransformContext()->RegisterQualifiedNameTransformerKind(
        new CAlterNameMention());
    GetTransformContext()->RegisterQualifiedNameTransformerKind(
        new CAlterPropertyName());
    GetTransformContext()->RegisterDeclarationTransformerKind(
        new CAlterLocalDeclaration());
    GetTransformContext()->RegisterDeclarationTransformerKind(
        new CAlterPassNameToConstructor());

    // PHP: $dollar name transform
    GetTransformContext()->RegisterQualifiedNameTransformerKind(
        new CAlterDollarName());

    // language dependent
    GetTransformContext()->
        GetXlang()->
            InitTransformersForXlang();

    // Property Auto Method Templates
    {
        str sName = CTransformContext::C_sPropertyAutoName;
        str sNameAsMagicStr = CTransformContext::C_sPropertyAutoNameAsMagicStr;
        str sTypeData = CTransformContext::C_sPropertyAutoTypeData;
        str sTypeArg = CTransformContext::C_sPropertyAutoTypeArg;
        str sTypeRet = CTransformContext::C_sPropertyAutoTypeRet;
        str sClass = CTransformContext::C_sPropertyAutoClass;
        str sAssert = CTransformContext::C_sPropertyAutoAssert;
        str sPreGetInit = CTransformContext::C_sPropertyAutoPreGetInit;
        str sPostInit = CTransformContext::C_sPropertyAutoPostInit;

        str sAssertStatement =
            "    " + sAssert + " "
                "; // (ASSERT expression comes from 'assert(value.....)' "
                "modifier in x-property declaration)" nl;

        GetTransformContext()->RegisterPropertyAutoMethod(
            "Get",
            sTypeRet + " " + sClass + "::" + sName + "_Get()",
            ""
                "    " C_szCtaBeforeMethodHookCode "" nl
                "    " + sPreGetInit + nl
                "    " + sTypeData + " value = " + sName + "__itl;" nl +
                sAssertStatement +
                "    " + sPostInit + nl
                "    return value;" nl);

        GetTransformContext()->RegisterPropertyAutoMethod(
            "Set",
            "void " + sClass + "::" + sName + "_Set(" + 
                //sync:_x_r = null();
                sTypeData + 
                //sTypeArg + 
                " value)",
            ""
                "    " C_szCtaBeforeMethodHookCode "" nl +
                sAssertStatement +
                "    " C_szCtaBeforeSetProperty "(" + sNameAsMagicStr + ", value);" nl
                "    " + sName + "__itl = value;" nl +
                "    " + sPostInit + nl
                );
    }

    RegisterTemplateNameStr(
        "function"); // std::function

}
