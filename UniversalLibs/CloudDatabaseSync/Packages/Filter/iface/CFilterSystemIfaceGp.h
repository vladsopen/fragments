// CFilterSystemIfaceGp.h
#pragma once

class CFilterIfaceGp;

//
// CFilterSystemIfaceGp -
//
//   Universal Node filter manager for Ubro
//
ASSUME_IMPL_FOR_IFACE(CFilterSystemIface, CFilterSystemIfaceGp)
ASSUME_IMPL_FOR_IFACE(CFilterSystemImpl, CFilterSystemIface)

class CFilterSystemIfaceGp : public object
{
public:
    CFilterSystemIfaceGp();
    //~CFilterSystemIfaceGp();
    //NEW_LOCAL(CFilterSystemImpl, CFilterSystemIfaceGp)
    NEW_GP(CloudSync, CFilterSystemImpl, CFilterSystemIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Count
    int GetFilterCount()
            vhook;

    // "All Nodes" filter
    ref<CFilterIfaceGp> GetAllNodeFilter()
            vhook;

// Operations

    // One-time opener
    //void InitFilterSystem();

    // Cleanup
    //void CloseFilterSystem();

    // Iterate
    bool IterateFilters(
            out iter& out_i,
            out ref<CFilterIfaceGp>& out_rFilter)
            vhook;

    // Lookup
    bool LookupFilter(
            str sOrganizerKey,
            out ref<CFilterIfaceGp>& out_rFilter)
            vhook;

    // Create a filter
    ref<CFilterIfaceGp> AddNewFilter(
            key keyOptional)
            vhook;

// UI

protected:
    SEE_ALSO(CFilterSystemImpl) // F12-lookup

    virtual int OnGetFilterCount()
            v1pure;
    virtual ref<CFilterIfaceGp> OnGetAllNodeFilter()
            v1pure;
    virtual bool OnIterateFilters(
            out iter& out_i,
            out ref<CFilterIfaceGp>& out_rFilter)
            v1pure;
    virtual bool OnLookupFilter(
            str sOrganizerKey,
            out ref<CFilterIfaceGp>& out_rFilter)
            v1pure;
    virtual ref<CFilterIfaceGp> OnAddNewFilter(
            key keyOptional)
            v1pure;

private:
    //bool _m_bOneTimeInitFilterSystemIfaceGpOk = false;
    //bool _m_bFilterSystemIfaceGpOpened = false;

    void _init_AttachToFilterSystemIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
