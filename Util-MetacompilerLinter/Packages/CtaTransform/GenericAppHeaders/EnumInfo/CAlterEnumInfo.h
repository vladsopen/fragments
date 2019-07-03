// CAlterEnumInfo.h
// (see "CAlterEnumInfo.cpp")

// CAlterEnumInfo - generic 'enum' info

class CAlterEnumInfo : public CAlterDeclaration
{
public:
    GENERIC(CAlterEnumInfo, CAlterDeclaration);
    CAlterEnumInfo();

// Const

    //#define C_szDepreciatedEnumInfoPrefix "_depreciated_cta_enuminfo_"

    static bool IsEnumTypeNameGeneric(
            str sEnumTypeName);

// Attributes

    str GetUnqualifiedEnumNameStr();
    str GetPrefixStr();
    bool x_bHasDefault;

// Transformation

    str GetOuterScopeImplText(
            CAlterGenericClass* pGenericClass,
            str sQualifiedClass);

protected:
    virtual void OnTransform();

private:
    CEnumItemList* m_pEnumItemList;

    CClassBody* GetEnumBody();
    CDeclaratorList* GetEnumConstDeclaratorList();

    bool IsApplicable();
    void ApplyClassBodyTransform();

    bool IsLooksLikeApplicable();
    bool IsNameLooksGeneric();

    void WriteToContainingClassBody(CClassBody* pClassBody);
};