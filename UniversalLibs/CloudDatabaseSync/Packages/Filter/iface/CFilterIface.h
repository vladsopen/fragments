// CFilterIface.h
#pragma once

#include "CFilterIfaceGp.h"

class CUdbConnectionIfaceGp;

//
// CFilterIface - see CFilterIfaceGp
//
//ASSUME_IMPL_FOR_IFACE(CFilterImpl, CFilterIface)

class CFilterIface : public CFilterIfaceGp
{
public:
    CFilterIface();
    //~CFilterIface();
    NEW_LOCAL(CFilterImpl, CFilterIface)
    //NEW_GP(CloudSync, CFilterImpl, CFilterIface)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

// Operations

    // One-time opener
    //void InitFilter();

    // Cleanup
    //void CloseFilter();

    // SQL helper for QueryBasicNodesInFilter()
    qxbool PrepareFilterMatchingQx(
            qxkey qxkeyTrait,
            qxstring qxstringMatchScalar,
            qxkey qxkeyMatchNonscalar)
            vhook;

    // from<> preparation
    rp<dbobject> NewDbTableResult()
            vhook;

    // Insert JOINs necessary for filtered query
    void AddFilterJoins(
            out from<dbobject>& out_fromResult,
            ref<CQueryIfaceGp> rQuery,
            ptr<CUdbConnectionIfaceGp> pUdbConnection,
            qxbool qxboolOn)
            vhook;

    // Grid builder
    bool IterateFilterColumnVisibility(
            out iter& out_i,
            out key& out_keyColumnTrait,
            bool bOnlyVisible,
            out bool& out_bVisible)
            vhook;

    // Configures and verified(!) SORT expression list
    str GetValidFilterSortSql()
            vhook;

    // Caption button click handler
    void ChangeColumnOrderForFilter(
            key keyFieldClicked)
            vhook;

    // Column visibility
    bool IsFilterColumnVisible(
            key keyField)
            vhook;
    void ShowFilterColumn(
            key keyField,
            bool bShow)
            vhook;
    void MoveFilterColumn(
            key keyField,
            int nDelta)
            vhook;

// UI

protected:
    SEE_ALSO(CFilterImpl) // F12-lookup

    virtual qxbool OnPrepareFilterMatchingQx(
            qxkey qxkeyTrait,
            qxstring qxstringMatchScalar,
            qxkey qxkeyMatchNonscalar)
            v1pure;
    virtual rp<dbobject> OnNewDbTableResult()
            v1pure;
    virtual void OnAddFilterJoins(
            out from<dbobject>& out_fromResult,
            ref<CQueryIfaceGp> rQuery,
            ptr<CUdbConnectionIfaceGp> pUdbConnection,
            qxbool qxboolOn)
            v1pure;
    virtual bool OnIterateFilterColumnVisibility(
            out iter& out_i,
            out key& out_keyColumnTrait,
            bool bOnlyVisible,
            out bool& out_bVisible)
            v1pure;
    virtual str OnGetValidFilterSortSql()
            v1pure;
    virtual void OnChangeColumnOrderForFilter(
            key keyFieldClicked)
            v1pure;
    virtual bool OnIsFilterColumnVisible(
            key keyField)
            v1pure;
    virtual void OnShowFilterColumn(
            key keyField,
            bool bShow)
            v1pure;
    virtual void OnMoveFilterColumn(
            key keyField,
            int nDelta)
            v1pure;

private:
    //bool _m_bOneTimeInitFilterIfaceOk = false;
    //bool _m_bFilterIfaceOpened = false;

    void _init_AttachToFilterIface();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
