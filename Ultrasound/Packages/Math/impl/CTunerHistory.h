// CTunerHistory.h

class CTunerProfileList;

//
// CTunerHistory - a record of Tuner Profile changes during flow calculation
//

class CTunerHistory : public object
{
public:
    CTunerHistory();
      
// Constants

// Attributes

    int x_iViewOffset = -1
            xauto(Get)
            xassert(value >= -1 && value < m_aTunerProfileList.GetCount());
    void x_iViewOffset_Set(int value);

// array<> Publishing

    int GetCount()
        { return m_aTunerProfileList.GetCount(); }
    // void SetCount(int nNewSize) 
    //     { return m_aTunerProfileList.SetCount(nNewSize); }
    ptr<CTunerProfileList> LookupAt(int iAt);
    //     { return m_aTunerProfileList.GetAt(iAt); }
    // ref<CTunerProfileList> GetLast(int nCountMinus = -1) 
    //     { return m_aTunerProfileList.GetLast(nCountMinus); }
    // void SetAt(int iAt, ref<CTunerProfileList> value) 
    //     { m_aTunerProfileList.SetAt(iAt, value); }
    void Add(ref<CTunerProfileList> value);
    // void InsertAt(int iAt, ref<CTunerProfileList> value) 
    //     { m_aTunerProfileList.InsertAt(iAt, value); }
    // void RemoveAt(int iAt, int nCount = 1) 
    //     { m_aTunerProfileList.RemoveAt(iAt, nCount); }
    void RemoveAll() 
        { m_aTunerProfileList.RemoveAll(); }
    bool Iterate(out iter& out_i, out ref<CTunerProfileList>& out_value)
        { return m_aTunerProfileList.Iterate(out out_i, out out_value); } 
    // bool IterateBackwards(out iter& out_i, out ref<CTunerProfileList>& out_value)
    //     { return m_aTunerProfileList.IterateBackwards(out out_i, out out_value); } 

// Operations

// UI

    ref<UiContainer> NewTunerHistoryUi();
    ptr<CUiPanelGp> _m_pUiTunerHistory;

protected:
private:
    array< ref<CTunerProfileList> > m_aTunerProfileList;
};
    
// (UiTunerHistory.cpp)
class UiTunerHistory : public UiContainer
{
public:
    virtual void Ui();

    ref<CTunerHistory> m_rTunerHistory;
};
