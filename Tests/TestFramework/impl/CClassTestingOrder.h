// CClassTestingOrder.h
#pragma once

#include "VirtOs_text.h"

//
// CClassTestingOrder - 
//
//   Orders class tests by a control list file
//
//ASSUME_IMPL_FOR_IFACE(CClassTestingOrderImpl, CClassTestingOrder)

class CClassTestingOrder : public object
{
public:
    CClassTestingOrder();
    //~CClassTestingOrder();
    //NEW_LOCAL(CClassTestingOrderImpl, CClassTestingOrder)
    //NEW_GP(Debux, CClassTestingOrderImpl, CClassTestingOrder)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

    static const path GGetTestOrderFile()
            return
                loci::GGetWorkRoot().
                    GetAppendedPath("common").
                    GetAppendedPath("VirtOs").
                    GetAppendedPath("ClassTestOrder.txt");

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);
    //ref<> Get ()
    //        return
    //            _x_p .Get().Valid();

// Operations

    // One-time opener
    //void InitClassTestingOrder();

    // Cleanup
    //void CloseClassTestingOrder();

    // Ordered iterator used a map prepared on the first iteration
    //   No need to check for new additions,
    //   but the newly registered classes won't show until reiterated.
    bool IterateAllActiveRegisteredClassesInControlledOrder(
            out iter& out_i,
            out classinfo& out_classinfoIter);

// UI

protected:
private:
    //bool _m_bOneTimeInitClassTestingOrderOk = false;
    //bool _m_bClassTestingOrderOpened = false;

    str m_sLoadedOrder;
    text _m_textLoadedOrder;
    bool m_bOrderFileFailed = false;
    map<str, classinfo> _m_mapRegisteredOrdered;
    int m_nUpdatedForRegisteredCount = 0;

    void UpdateOrderMap();

    void EnsureOrderFileLoaded();

    void AppendOrderFile(
            str sNewClassedToAppend);

    path GetOrderFilePath();

    void _init_AttachToClassTestingOrder();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
