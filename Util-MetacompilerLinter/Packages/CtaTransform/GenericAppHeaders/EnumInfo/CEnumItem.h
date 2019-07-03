// CEnumItem.h
// (see "CEnumItem.cpp")

// CEnumItem - Generic class enum constant metadata

class CEnumItem : public CCommon
{
public:
    GENERIC(CEnumItem, CCommon);
    CEnumItem();

// Init

    bool InitEnumItem(
            CDeclarator* pDeclarator,
            CAlterEnumInfo* pAlterEnumInfo);

// Attributes

    str GetPureName();
    str GetFullName();
    bool IsUndefined();
    bool IsIterable();
    bool IsUniqueValue();
    bool IsShouldBeAfterNormal();

private:
    str m_sPureName;
    str m_sFullName;
    nullable<CDeclarator*> m_npConstDeclarator;
    nullable<CAlterEnumInfo*> m_npAlterEnumInfo;
};

