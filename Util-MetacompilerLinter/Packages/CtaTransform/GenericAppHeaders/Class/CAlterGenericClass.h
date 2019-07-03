// CAlterGenericClass.h
// (see "CAlterGenericClass.cpp")

#include "CAlterDeclaration.h"

class CAlterNameMention;

//
// CAlterGenericClass - VirtOs generic class transformation support
//

class CAlterGenericClass : public CAlterDeclaration
{
public:
    GENERIC(CAlterGenericClass, CAlterDeclaration);
    CAlterGenericClass();

// Constants 

    #define C_szGenericClassRootBase "object"
    #define C_szStopCheckCtaAllMembersAtClass "object"
    #define C_szGenericDbClassRootBase "dbobject"
    #define C_szGenericDbClassForMm "mmdbobject"

// Attributes

    CQualifiedName* GetClassQualifiedName();
    str GetClassQualifiedNameStr();
    bool IsNested();
    nullable<CAlterGenericClass*> GetBaseClassHeader();
    str GetBaseClassName();
    bool IsGenericUiContainer();
    bool IsImplementedInThisFile();
    bool IsNonExposableLegacyMembers();
    bool IsNonRefThisPointer();
    bool IsDbClass();

// Members

    nullable<CAlterMember*> LookupMember(
            str sName);
    nullable<CAlterMember*> FindPrevMemberInstanceInBaseClasses(
            str sMemberName,
            out str& out_sGlueimplClassFoundEarlier);
    nullable<CAlterMember*> FindMemberRecursive(
            str sMemberName);
    bool IterateMembers(
            out iter& i,
            out CAlterMember*& out_pMember);

// Procedure

    void HandleTransformCompleteForClass(
            CCompilationUnit* pCompilationUnit);

// Deferred Member Transformation Support

    void RegisterMember(CAlterMember* pAlterMember);
    void RegisterMemberVariableByBaseName(CAlterMember* pAlterMember);
    void CheckNotDeclaredOrInBaseClassByBaseName(
            str sMemberBaseName, 
            CAlterMember* pAlterMember);
    str GetMemberHeaderText(str sQualifiedClass);
    str GetMemberImplText(str sQualifiedClass);

    // member typedef registration (for 'super')
    void RegisterTypedefInClass(
            str sTypedefName, 
            CAlterMember* pAlterMember);
    nullable<CAlterMember*> LookupTypedefInClass(
            str sTypedefName);

// EnumInfo Support

    void RequestEnumInfoImpl(CAlterEnumInfo* pTransformEnumInfo);
    str GetEnumInfoImplText(str sQualifiedClass);

// ALL_MEMBERS support

    void CheckCtaAllMembersForClassAndParents(
            CAlterNameMention* pDirectiveAlterNameMention,
            const map<str, CAlterNameMention*>& mapNameMentionedInMethod,
            CDeclaration* pReportErrorFor,
            bool bCheckingBaseClasses);

// Implementation Helpers

    str GenerateClassContentExposition();
    str GenerateClassMetaExposition();
    void GenerateGenericImplementations(
            CStatementList* pStatementList);
    void GenerateAllConstructorInitializers(
            CFunctionBody* pConstructorFunctionBody);
    void ApplyClassBodyModifications();
    str GetOuterScopeImplText(
            CAlterGenericClass* pGenericClass,
            str sQualifiedClass);

    nullable<CDeclaration*> LookupScopeRegisteredName(
            str sName);

// Name Mention Support

    void NoteNameMentionedInClassMethod(
            CAlterNameMention* pNameMention);

protected:
    virtual void OnTransform();

private:
    map<str, CAlterMember*> m_mapMembers;
    map<str, CAlterMember*> m_mapMembersByBaseName;
    map<str, CAlterMember*> m_mapTypedefs;
    nullable<CAlterGenericClass*> m_npBaseClassHeader;
    array<CAlterEnumInfo*> m_aTransformEnumInfo;
    map<str, CAlterNameMention*> m_mapNameMentionedInMethods;
    bool m_bImplementedInThisFile;
    bool m_bDbClass;

    struct DGenericFlags
    {
        DGenericFlags();
        bool m_bGenericLooking;
        bool m_bGenericUi;
        bool m_bGenericUiContainer;
        bool m_bHasBaseClass;
        bool m_bGenericLookingBaseClass;
        bool m_bGenericForced;
        bool m_bGenericSuppressed;
        bool m_bClassInfoSuppressed;
        bool m_bOnExposeContentPresent;
        bool m_bOnExposeContentGeneratedPresent;
        bool m_bNewObjectPresent;
        bool m_bNewOperatorScalarPresent;
        bool m_bDeleteOperatorScalarPresent;
        bool m_bStaticClassAllocSuppressed;
        bool m_bNonExposableLegacyMembers;
        bool m_bNonRefThisPointer;
        //bool m_bFinalizePresent;
        str m_sClassName;
        str m_sBaseClassName;
    };
    DGenericFlags m_dataGenericFlags;

    bool IsClassNameLooksGeneric(str sClassName);
    void DetectGenericLooks();
    void FilterThroughExplicitGenericFlags();
    void WarnOfGenericityDetectionProblems();
    nullable<CDeclaration*> FindConstructorWithoutArgs();
    nullable<CDeclaration*> FindDestructor();
    void GenerateImplWithoutConstructor(
            CCompilationUnit* pCompilationUnit);
    //void VerifyClassImplementationConditions();
    void CheckGenericLocation();

    void CheckDbClass(
            str sClassName);

};