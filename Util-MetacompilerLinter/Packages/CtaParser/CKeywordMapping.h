// CKeywordMapping.h
// (see "CKeywordMapping.cpp")

// CKeywordMapping - C++ keyword registration fro CTokenizer

class CKeywordMapping : public CCommon
{
public:
    GENERIC(CKeywordMapping, CCommon);
    CKeywordMapping();

// Initialization

    void InitCppKeywords();

    // Add/readd a keyword to mappings
    enum EAddKeyword { E_AddKeyword_NewUnique, E_AddKeyword_RemapExisting };
    void MapKeyword(
            CToken* pTokenKind,
            EAddKeyword eAddKeyword = E_AddKeyword_NewUnique);

// Lookup

    nullable<CToken*> FindKeywordKind(CCharWord* pCharWord);

private:
    map<str, CToken*> m_mapKeywordToToken; //pref: forces CCharWord to str

    void VerifyAllKeywordsMapped();
    void ListKnownKeywords(out array<str>& out_asKnownKeywords);
};

