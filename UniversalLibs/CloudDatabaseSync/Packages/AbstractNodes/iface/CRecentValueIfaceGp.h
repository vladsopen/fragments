// CRecentValueIfaceGp.h
#pragma once

//
// CRecentValueIfaceGp -
//
//   Last assigned Trait value for a Node
//
//ASSUME_IMPL_FOR_IFACE(CRecentValueIface, CRecentValueIfaceGp)
//ASSUME_IMPL_FOR_IFACE(CRecentValueImpl, CRecentValueIface)

class CRecentValueIfaceGp : public object
{
public:
    CRecentValueIfaceGp();
    //~CRecentValueIfaceGp();
    //NEW_LOCAL(CRecentValueImpl, CRecentValueIfaceGp)
    NEW_GP(CloudSync, CRecentValueImpl, CRecentValueIfaceGp)
    virtual void OnExposeContent(
            ref<CExpose> rExpose);

// Constants

// Attributes

    // parent
    //ptr<> _x_p
    //        xauto(Get, Set);

    // Object the property assigned to
    key x_keyValueForNode
            xabstract;
    key x_keyValueForNode_Get()
            vhook;

    // Property assigned
    key x_keyValueForTrait
            xabstract;
    key x_keyValueForTrait_Get()
            vhook;

    // Tie
    key x_keyValueForTie
            xabstract;
    key x_keyValueForTie_Get()
            vhook;

    // The abstact value itself
    str x_sValueScalar
            xabstract;
    str x_sValueScalar_Get()
            vhook;

    // The abstact value as node key
    key x_keyValueNonscalar
            xabstract;
    key x_keyValueNonscalar_Get()
            vhook;

    // Used internally by NetIn to check if download action is applied
    utc x_utcNodeTraitUpdate
            xabstract;
    utc x_utcNodeTraitUpdate_Get()
            vhook;

// Operations

    // One-time opener
    //void InitRecentValue();

    // Cleanup
    //void CloseRecentValue();

// UI

protected:
    SEE_ALSO(CRecentValueImpl) // F12-lookup

    virtual key Onx_keyValueForNode_Get()
            v1pure;
    virtual key Onx_keyValueForTrait_Get()
            v1pure;
    virtual key Onx_keyValueForTie_Get()
            v1pure;
    virtual str Onx_sValueScalar_Get()
            v1pure;
    virtual key Onx_keyValueNonscalar_Get()
            v1pure;
    virtual utc Onx_utcNodeTraitUpdate_Get()
            v1pure;

private:
    //bool _m_bOneTimeInitRecentValueIfaceGpOk = false;
    //bool _m_bRecentValueIfaceGpOpened = false;

    void _init_AttachToRecentValueIfaceGp();
    //void cta_BeforeMethod(const DBeforeMethod& dataBeforeMethod);
    //virtual void OnTestClass();
};
