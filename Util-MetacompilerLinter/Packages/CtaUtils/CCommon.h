// CCommon.h
// (see "CCommon.cpp")

// Version
#include "CtaProjectVersion.h"

// Main Classes
class CCommon;
class CSession;
class COptions;
class CConsole;
class CDebug;
class CTokenizer;
class CTransformContext;
class CCompilerOptions;

// Low level services
#include "type_information.h"
#include "nullable_pointer_template.h"

// Common base class
class CCommon : public object
{
public:
    CCommon();

// Type Information

    GENERIC_GetStaticClassId();
    //static CCommon* TryCast(CCommon*);
    virtual CCommon* TryCastToClass(CClassId);
    virtual CCommon* NewOfTheKind() const;
    virtual void abstract_CCommon() = 0;

    #define ReplicateToken(x) (x)

// Global Services

    static COptions* GetOptions();
    static CConsole* GetConsole();
    static CDebug* GetDebug();
    static CTokenizer* GetTokenizer();
    static CTransformContext* GetTransformContext();
};

// Cta Phases
enum ETranslationMode
{
    E_TranslationMode_Extend,
    E_TranslationMode_Transform,
    E_TranslationMode_Regenerate,
};

// #line sync mode
enum ELineNumbering
{
    E_LineNumbering_SyncWithToken,
    E_LineNumbering_SuppressSync,
};

// Main Classes
#include "CDebug.h"
#include "COptions.h"
#include "CConsole.h"
