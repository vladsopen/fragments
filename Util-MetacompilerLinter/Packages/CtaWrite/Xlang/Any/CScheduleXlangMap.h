// CScheduleXlangMap.h

class CXlang;

//
// CScheduleXlangMap - 
//
//   cta_::AddOutputXlang(Lang); Queue metaprocessing runs for other output languages
//

class CScheduleXlangMap : public CCommon
{
public:
    GENERIC(CScheduleXlangMap, CCommon);
    CScheduleXlangMap();

// Const

// Attributes

// Operations

    // Schedule another run if not done yet
    void AddNextOutputXlang(
            str sLangName,
            CXlang* pAddXlang,
            CToken* pSourceToken);

    // Called after each MetaprocessFile() loop to check if there is more to write
    nullable<CXlang*> NextScheduledXlang();

protected:
private:

    map<str, CXlang*> _m_mapScheduleXlang;
};

