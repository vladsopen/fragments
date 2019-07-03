// CFilterIfaceGp.h
#pragma once

class CUbroRefineIfaceGp;
class CBasicNodeIfaceGp;

//
// CFilterIfaceGp -
//
//   Node filters for building UI organizer folders
//
ASSUME_IMPL_FOR_IFACE(CFilterIface, CFilterIfaceGp)
ASSUME_IMPL_FOR_IFACE(CFilterImpl, CFilterIface)

class CFilterIfaceGp : public object
{
public:
    CFilterIfaceGp();
    //~CFilterIfaceGp();
    //NEW_LOCAL(CFilterImpl, CFilterIfaceGp)
    NEW_GP(CloudSync, CFilterImpl, CFilterIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Primary key
    key GetFilterKey()
            vhook;

    // Automatic or user entered or "All Nodes"
    str GetFilterName()
            vhook;
    void SetCustomFilterName(
            str value)
            vhook;

    // Count
    int GetFilterNodeCount(
            ref<CUbroRefineIfaceGp> rUbroRefine)
            vhook;

    // Main unfiltered organizer?
    bool IsAllNodesFilter()
            vhook;

    // Any conditions defined?
    bool IsNullFilter()
            vhook;

// Operations

    // Init
    void MakeFilterMatchByProperty(
            key keyProperty,
            str sMatch,
            str sLike)
            vhook;

    // Override default (DbTable class based) column set
    //   unidefSort[keyColumn] = C_sSqlAsc|C_sSqlDesc
    //   unidefVisible[keyColumn] = ""|"-"(hidden)
    void BuildColumnsFromUnidef(
            unidef unidefSort,
            unidef unidefVisible)
            vhook;

    // Iterate nodes in filter
    bool IterateFilterNodes(
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode,
            ref<CUbroRefineIfaceGp> rUbroRefine)
            vhook;

    // Query multiple selection
    //   cannot: transient selection state is only in Ubro
    //bool IterateFilterSelectedNodes(
    //        out iter& out_i,
    //        out ref<CBasicNodeIfaceGp>& out_rBasicNode,
    //        ref<CUbroSessionIfaceGp> rUbroSession)
    //        vhook;

    // Get a specific Node
    bool LookupBasicNodeByKeyUnfiltered(
            key keyNode,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            vhook;

    // New empty Node associated with the organizer.
    //   A store may support this only for AllNodes organizer.
    //void HandleUbroCreateButton(
    //        ref<UiContainer> rUiAttachMenuTo)
    //        vhook;

// UI

protected:
    SEE_ALSO(CFilterImpl) // F12-lookup

    virtual key OnGetFilterKey()
            v1pure;
    virtual str OnGetFilterName()
            v1pure;
    virtual void OnSetCustomFilterName(
            str value)
            v1pure;
    virtual int OnGetFilterNodeCount(
            ref<CUbroRefineIfaceGp> rUbroRefine)
            v1pure;
    virtual bool OnIsAllNodesFilter()
            v1pure;
    virtual bool OnIsNullFilter()
            v1pure;
    virtual void OnMakeFilterMatchByProperty(
            key keyProperty,
            str sMatch,
            str sLike)
            v1pure;
    virtual void OnBuildColumnsFromUnidef(
            unidef unidefSort,
            unidef unidefVisible)
            v1pure;
    virtual bool OnIterateFilterNodes(
            out iter& out_i,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode,
            ref<CUbroRefineIfaceGp> rUbroRefine)
            v1pure;
    virtual bool OnLookupBasicNodeByKeyUnfiltered(
            key keyNode,
            out ref<CBasicNodeIfaceGp>& out_rBasicNode)
            v1pure;

private:
    //bool _m_bOneTimeInitFilterIfaceGpOk = false;
    //bool _m_bFilterIfaceGpOpened = false;

    void _init_AttachToFilterIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
