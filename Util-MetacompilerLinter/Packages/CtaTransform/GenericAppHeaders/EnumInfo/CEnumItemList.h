// CEnumItemList.h
// (see "CEnumItemList.cpp")

// CEnumItemList - Generic class enum constant list

class CEnumItemList : public CCommon
{
public:
    GENERIC(CEnumItemList, CCommon);
    CEnumItemList();

// Init

    void InitItemList(
            CDeclaratorList* pConstDeclaratorList,
            CAlterEnumInfo* pAlterEnumInfo);

// Item Collection

    // Get by absolute index
    CEnumItem* GetItem(
            int iItem);

    // Get filtering only iterable items
    CEnumItem* GetFirstIterableItem();
    CEnumItem* GetLastIterableItem();

    // the number of explicitly declared names
    int GetTotalItemCount();

    // the numbers of NORMAL names to report for client enumeration
    //  (excludes reserved constants)
    int GetDefinedItemCount();
    int GetUniqueItemCount();

    // Transform support
    str GenerateBodyForToString();
    str GenerateBodyForFromString();
    str GenerateBodyForIncrement();
    str GenerateBodyForDecrement();

private:
    array<CEnumItem*> m_aEnumItem;
    nullable<CAlterEnumInfo*> m_npAlterEnumInfo;
};

