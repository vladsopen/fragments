// CHookWriteMap.h
#pragma once

class CHookWriteIface;

//
// CHookWriteMap - 
//
//   A collection of writer hooks
//

class CHookWriteMap : public CCommon
{
public:
    GENERIC(CHookWriteMap, CCommon);
    CHookWriteMap();

// Constants

// Attributes

    int GetHookCount();

// Operations

    void AddHook(
            CHookWriteIface* pHookWrite);

    void RemoveHook(
            CHookWriteIface* pHookWrite);

    bool IterateHooks(
            out iter& out_i,
            out CHookWriteIface*& out_pHook);

    // for iteration safety we have to use a temp array with map
    void FillTempArrayForWriterEvent(    
            out array<CHookWriteIface*>& out_apHookWriteCache);

// UI

protected:
private:
    map<CHookWriteIface*, unused> _m_mapHookWrite;

    void DoReFillChangedTempArrayForWriterEvent(    
            out array<CHookWriteIface*>& out_apHookWriteCache);

    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
