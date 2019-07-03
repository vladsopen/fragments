// CtaProjectVersion.h

// Version Log
#define C_szCtaProjectVersion \
    "A510.oo"
#define C_nCtaProjectCompatibility \
    20189999011
//    2018
//  
//    2018-12-26
//    - " '-> " + pUp->GetObjectClassName()
//    2018-06-13
//    - reg-by-pi
//    2018-12-26
//    - CheckForValidStatementStart(pToken);
//    2018-08-03
//    - CTA_SKIP_EXPOSE for bot
//    2018-08-03
//    - C_sPropertyAutoNameAsMagicStr
//  
//    2017
//  
//    2017-12-18
//    - VS 2017 support
//    2017-12-01
//    - "CC_EXE_PATH"
//    2017-05-13
//    - sTypename == "static_assert"
//  
//    2016
//    
//    2016-01-29 
//    - GCC/Clang support
//    2016-03-09 
//    - see 2LINUX-TODO
//    2016-02-08
//    - GTestAbsoluters();
//    2016-02-01
//    - "VirtOs_stdcall",
//    - "VirtOs_cdecl",
//  
//    2015
//  
//    2015-03-14
//    - GReportFatal()
//    2015-05-11
//    - QC: FindProjectNameForOriginalSourceFile()
//    2015-06-20
//    - spellcheck / sync:_bad_string_deny_
//    2015-05-02
//    - m_sXmetas
//        if (sExtensionKeyword == "cta_meta")
//    2015-07-15
//    - ALL_MEMBERS_LIKE(CClass functions|data plus|sans super [matching preg ""])
//    2015-12-07
//    - cannot handle subexpression case:
//        // Qx
//        //    (qxsortFromFilter),
//        //    fromNode->x_utcNodeLastChange 
//        //        ORDER(E_Order_Descending);
//  
//    2014
//  
//    2014-08-03 U3
//        - CheckForValidSourcePath(charALPHAen charDIGITS "\\/:._")
//    2014-09-27 U3
//        - MM: C_szGenericDbClassForMm
//    2014-06-08 -U2 
//        - if (m_sPureName == "count")
//    2014-08-04 U3
//        - if (sPureName == "none")
//    2014-10-08 U4
//        - "tryfrom"
//    2014-09-23 U3
//        - substitute some built-in VirtOs function<-->column mapping
//    2014-05-24 -U2 
//        - a fix for "xxx_Get()" instead of "xxx_Get"
//  
//    2013
//  
//    2013-04-29 
//    - bad includes: FindPos("#include <c:")
//    2013-02-05 
//    - PHP transpiler 
//    2013-02-10 
//    - m_nLineIndent
//    2013-02-22 
//    - Attach a new token properly ReplaceToken()
//    2013-02-24 
//    - no more built-in types
//    2013-05-03 
//    - call complete-hooks
//    2013-05-03 
//    - append cta_macro() as function modifier
//    2013-02-06 
//    - ParseFunctionArgs()
//    2013-02-24 
//    - we can have a class prefix for a pointer
//    2013-03-09 
//    - BEFORE cannot go AFTER!
//    2013-02-24 
//    - we mistake int :7; for a label ("int:") + executable ("7")
//    2013-05-17 
//    - typedef super
//    2013-04-09 
//    - "ALL_MEMBERS"
//    2013-05-03 
//    - "NameHook" // SEMIPUBLIC()
//    2013-01-06 
//    - fails in parallel build - SkipExclusiveReadWriteAsserts().
//
//  20121201001 // A509.dli
                        // - dblist and proper x-prop preinit
//  20121130001 // A508.sinarg
                        // - $(ThisFunctionArg:N) for _Set _Get AUTO-macros
//  20120905001 // A507.from
                        // - testing Udb support
                        // - PassNameToConstructor
                        // - "type name(param)" as declaration
//  20120605001 // A506.cat
                        // - catenate multiple strings in cta_macro
//  20120205001 // A505.gas
                        // - "GAssertBreak"
//  20110725001 // A504.noen
                        // - cta_flag_DisableEnumInfo
//  20110328001 // A503.creative
                        // - CreateChildOf(this) removed and automated
//  20110320001 // A502.gr
                        // - global replace str, sys, GNewObject
//  20110313001 // A501.exp
                        // - ref<CExpose>
                        // - AssertMemberPresent removed
//  20110223001 // A500.indep
                        // - no longer integrated into VC
//  20110213001 // A466.chain
                        // - $(ThisClassName) works in a class
                        // - cta_macro() statements OK in a class decl zone
                        // - Xxxx() mentioned in OnXxxx() continue OK
//  20110201001 // A465.macro
                        // - cta_macro()
//  20101129001 // A464.nsync
                        // - lock .i file for parallel build again
//  20101125001 // A463.ilock
                        // - lock .i file for parallel build
//  20101121001 // A462.exti
                        // - tick64 in Deblsot and shared Load()
//  20100919001 // A461.dash
                        // - GCC compatible arg dash accepted
//  20100913001 // A460.ndel
                        // - new/delete hook for classalloc
//  20100605001 // A459.dbir
                        // - dbref property auto init
                        // - db property longer names
//  20090516001 // A458.ontimer
                        // - ontimer handler support without args
//  20090225001 // A457.scope
                        // - scope resulution reenabled
//  20090211001 // A456.post
                        // - onpost<> declarator support
//  20081217001 // A455.vprop
                        // - virtual x-property without auto() is OK in super class
//  20081030001 // A454.event
                        // - E_Onui_Xxxx
//  20080928001 // A453.dbf
                        // - Db class checks fix
//  20080924001 // A452.dbc
                        // - Db class checks
//  20080907001 // A451.db1
                        // - OnMirrorDb handling
//  20080824001 // A450.almeba
                        // - ALL_MEMBERS includes base classes
//  20080820001 // A449.allme
                        // - ALL_MEMBERS
//  20080801001 // A448.lima2
                        // - CTA_STR_PRECALC fixes
//  20080722003 // A447.lima1
                        // - CTA_STR_PRECALC, DefaultProjectLicense
//  20080414001 // A446.uig
                        // - ref<CUiPanelGp> rUi = this; support
//  20080227001 // A445.cui
                        // - ref<CUi> support
//  20080214001 // A444.glo
                        // - E_RemainingName_CheckForPrdNameCapitalThenLower for ->GFUNC()
//  20080117001 // A443.dq
                        // - double-? check
//  20071212005 // A442.cover
                        // - Code Coverage
//  20071210001 // A441.astr
                        // - x-property assert() got prettier:
                        //      m_sPropertyAssertExpressionOriginal
//  20071115001 // A440.uiface
                        // - _internal_InitGetUiAndVerifyClass(classinfo)
//  20071104001 // A439.xass
                        // - x_xXxxx x= ... support
//  20071025001 // A438.strex
                        // - CTA_SZ "only following string affected"
//  20070923002 // A437.hook
                        // - cta_::CounterInFile(Test)
//  20070920002 // A436.count
                        // - cta_::CounterInFile(Test)
//  20070831001 // A435.bigcrc
                        // - Cypherchar encyption changed with BIG_HILO() fix
//  20070827001 // A434.ontest
                        // - OnTestClass in meta data
//  20070707001 // A433.figlue
                        // - gluedimpl: support for _m_pFinalGluedImpl
//  20070703001 // A432.uici
                        // - Ui classinfo is passed instead of string control IDs
//  20070609001 // A431.gnull
                        // - gluedimpl NULL check "!pImpl? null() : "
//  20070603001 // A430.glued
                        // - gluedimpl auto
//  20070522001 // A429.enber
                        // - _m_e member is OK when not exposed
//  20070418001 // A428.crypt2
                        // - MagicStr encryption
//  20070416002 // A427.crypt
                        // - MagicStr encryption
//  20070411001 // A426.elduo
                        // - //ulog::GGetGlobalUlog().LogEvent("Say", sText.c());
//  20070401001 // A425.clean
                        // - //ulog::GGetGlobalUlog().LogEvent("Say", sText.c());
//  20070307001 // A424.void
                        // - void* return for new virtual
//  20070301001 // A423.warn
                        // - /analyzer warnings
//  20061226001 // A422.less
                        // - uninclude: cta_ignore_granted.h - GetFileNameWithExt()
//  20061224001 // A421.arg
                        // - ref<> --> ref_arg<> in GTransformRefToRefArg()
//  20061218001 // A420.defi
                        // - pre-#define cta_UIxxx removed modernizing UiContainer
                        // - Allow redefinition of abstract properties with Impl members
//  20061126001 // A419.unocode
                        // - unicode check - CheckForValidTextEncoding()
//  20060922001 // A418.isin
                        // - rExpose->AssertMemberPresent(sKey_" + sKeyName + ", this);"
//  20060916001 // A417.cend
                        // - ParseCommandLineArgs() terminating-zero fix
//  20060914001 // A416.terse
                        // - less words for a single file
                        // - PFN_GNewInstancePtr rename
//  20060805001 // A415.jamming
                        // - Jam adaptations for parsing args outside response file
                        // - also strip bare "cl"
//  20060716001 // A414.enline
                        // - new line check in Enru
//  20060709001 // A413.init
                        // - local initializer check
//  20060622001 // A411.ruen
                        // - warning: a russian letter follows english letter
                        // - ACTION(abstract /* .NET */, CAbstractKeyword) 
//  20060608001 // A410.char
                        // - tabs and bad char detector
                        // - more CTranslateChecker tests for skeleton
//  20060528001 // A409.esym
                        // - more CTranslateChecker tests,
                        // - method parameter name matching
                        // - fix: prefix check <out str& sErrors> in body delaration
//  20060527001 // A408.enru
                        // - TRANSLATE macro support
//  20060418001 // A407.do
                        // - 'do' void function body inserter, 'out xxx&' check
//  20060328001 // A406.meta
                        // - expanded metadata
//  20060325001 // A405.cide
                        // - "__cta_hide" for @define ASSERT(..., __cta_hide__FILE__)
//  20060309001 // A404.const
                        // - 'static const C_' check, CTA_AUTO_NEW_AS, 'out' check
//  20060303001 // A403.eout
                        // - EOut for pure functions support for 'out' spec
//  20051216001 // A402.uic
                        // - cta_ignore(..., "virtual method 'Ui' declared 'continue'")
//  20051123001 // A401.vs8
                        // - VS2005 support
//  20051111001 // A325.magex
                        // - pRightBrace->Regenerate(pWriter); in CCompound::OnRegenerate()
//  20051103001 // A325.magex
                        // - MagicStr for Expose
//  20051029000 // A324.magic
                        // - MagicStr cache support
//  20050923000 // A323.stats
                        // - CWriteForStats to optimize IsMethodCodeComplexEnough()
//  20050811000 // A322.cinfo
                        // - static DClassAttr* s_pdataClassAttr = NULL; fix for WPO
//  20050622000 // A321.stack
                        // - CTA_BEFORE_HAIRY_METHOD;
//  20050612000 // A320.fever
                        // - cta_flag_NonExposableLegacyMembers() for Forever.lib
//  20050608000 // A319.dest
                        // - destructor generator
//  20050531000 // A318.this
                        // - CTA_GENERIC_THIS,
                        // - CTA_BEFORE_DESTRUCTOR, CTA_BEFORE_CONSTRUCTOR
//  20041216000 // A317.xall
                        // - all 'x_typeSomething' transformed as x-property
//  20040928000 // A316.abs
                        // - cta_BeforeMethod() hook
                        // - abstract x-property,
                        // - loose assert() requirement,
                        // - "= --" replaced with CTA_LEAVE_UNINITIALIZED
                        // - "[DE01]", "depreciated enum member detected"
//  20040808000 // A215.sp    - command line space handling fix
//  20040118000 // A214.null  - universal_null() renamed to null()
//  20031219000 // A213.asget - assert() check on xauto(Get)
//  20031119000 // A212.oper  - pure 'operator ' () member generation support
//  20031107000 // A211.cint  - static int init is left for backend optimizer
//  20031028000 // A209.denum - enum "_default" no longer requited
//  20031016000 // A208.ever  - forever, MFC, PCH
//  20031011000 // A207.ds    - debug lib changes
//  20031004000 // A206.brace - { ref<> rUi } bracing check
//  20031003000 // A205.uiref - ref<> rUi support
//  20030930000 // A204.popup - OnPopup 'return' removed.
//  20030927000 // A203.once  - erroneous ui retransform fix.
//  20030925000 // A202.temp  - .ctatemp stays (for debugger).
//  20030919000 // A201.ui    - Ui transform fixes
//  20030915200 // A200.ui    - UV Api3 Ui Support, 'object'-derived Generic detection,
//                              ctatemp deletion, auto constructor generation
//  20030728119:   A119.ign   - cta_ignore()
//  20030715118:   A118.ifdeb - __if_exists, Cta.ini.debug.ini
//  20030709117:   A117.xprop - support for DevPartner instrumentation
//                              simple x_ property support, new enum info, ctatemp src
//  20030626116:   A116.std   - support for std includes
//  20030625115:   A115.stic  - static member init in class body
//  20030619114:   A114.con1  - virtual 'continue 1st'
//  20030509002:   A113.cont  - CCccc* GNewInstancePtr, virtual 'continue'
//         1003:   A111.newv  - "vhooked", filter similar msg, class prototypes (rejt)
//         1002:   A110.eninc - enum _Increment syntax, fixes
//         1001:   A100.* - first transforms

// Name resolution is not fully supported, but requires some resources if enabled
#define SUPPORT_NAME_SCOPE
#if defined(SUPPORT_NAME_SCOPE)
#endif

// Parameter definition for cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod)
#ifndef DBeforeMethod_DEFINED
#define DBeforeMethod_DEFINED
struct DBeforeMethod
{
    const char* m_szBeforeMethod; // reserved for __FUNCTION__ or something similar
    // more parameters may be added in the future,
    //   use only CTA_BEFOREMETHOD_INIT to properly initialize the struct!
    // e.g.: DBeforeMethod dataBeforeMethod = CTA_BEFOREMETHOD_INIT;
};
#endif

// (macros moved to lib__CtaMacrosForApp.h)